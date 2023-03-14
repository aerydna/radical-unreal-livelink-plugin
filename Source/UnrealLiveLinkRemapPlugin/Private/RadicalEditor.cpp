// Fill out your copyright notice in the Description page of Project Settings.
#include "RadicalEditor.h"
#include <memory>
#include <array>
#include <vector>
#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Editor/Blutility/Public/IBlutilityModule.h"
#include "LevelEditor.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Interfaces/IPluginManager.h"
#include "UObject/UObjectGlobals.h"
#include "SIOConnectionManagerAdapter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstanceProxy.h"
#include "AnimationRuntime.h"

#include "RadicalLiveClient.h"
#include "Async/Async.h"

using namespace rad::live_client;

#define LOCTEXT_NAMESPACE "A3Nodes"
FText UAnimGraphNode_Radical::GetControllerDescription() const
{
	return LOCTEXT("RadicalLive", "Radical Live");
}

FText UAnimGraphNode_Radical::GetNodeTitle(ENodeTitleType::Type titleType) const
{
	return GetControllerDescription();
}

FText UAnimGraphNode_Radical::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_RadicalLive_Tooltip", "stream radical live data");
}

void UAnimGraphNode_Radical::Draw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* previewSkelMeshComp) const
{

}
#undef LOCTEXT_NAMESPACE

DECLARE_CYCLE_STAT(TEXT("Radical Live joint"), STAT_Radical_Eval, STATGROUP_Anim);
FAnimNode_Radical::FAnimNode_Radical() {}

void FAnimNode_Radical::GatherDebugData(FNodeDebugData& debugData) {
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(GatherDebugData)

	FString DebugLine = debugData.GetNodeName(this);

	DebugLine += "(";
	AddDebugNodeData(DebugLine);
	DebugLine += FString::Printf(TEXT(")"));
	debugData.AddDebugItem(DebugLine);

	ComponentPose.GatherDebugData(debugData);

}

void FAnimNode_Radical::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& output, TArray<FBoneTransform>& outBoneTransforms) {
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateSkeletalControl_AnyThread)
	SCOPE_CYCLE_COUNTER(STAT_Radical_Eval);

	// get last pose for selected ID
	// fill outBoneTransforms with that pose
	UE_LOG(LogTemp, Warning, TEXT("Tick"));
	if (playerID.IsEmpty())
		return;

	URadicalEditor* radSub = GEditor->GetEditorSubsystem<URadicalEditor>();
	if (!radSub)
		return;
	auto m = radSub->getLastPose(playerID);
	if (!m.isValid())
		return;

	static std::map<std::string, std::string> parent{
		{ "root_r", ""},
		{ "LeftUpLeg_r", "root_r" },
		{ "LeftLeg_r", "LeftUpLeg_r" },
		{ "LeftFoot_r", "LeftLeg_r" },
		{ "RightUpLeg_r", "root_r"},
		{ "RightLeg_r", "RightUpLeg_r" },
		{ "RightFoot_r", "RightLeg_r" },
		{ "Spine_r", "root_r" },
		{ "Spine1_r", "Spine_r" },
		{ "Spine2_r", "Spine1_r" },
		{ "Neck_r", "Spine2_r" },
		{ "Head_r", "Neck_r" },
		{ "RightShoulder_r", "Spine2_r" },
		{ "RightArm_r", "RightShoulder_r" },
		{ "RightForeArm_r", "RightArm_r" },
		{ "RightHand_r", "RightForeArm_r" },
		{ "LeftShoulder_r", "Spine2_r" },
		{ "LeftArm_r", "LeftShoulder_r" },
		{ "LeftForeArm_r", "LeftArm_r" },
		{ "LeftHand_r", "LeftForeArm_r" }
	};

	//quaternion flip axis
	auto xMod = quatOptions.flipX ? -1 : 1;
	auto yMod = quatOptions.flipY ? -1 : 1;
	auto zMod = quatOptions.flipZ ? -1 : 1;
	auto wMod = quatOptions.flipW ? -1 : 1;
	auto rotShift = FQuat::MakeFromEuler(FVector(90, 0, 0));

	if (boneNameToId.Contains(boneMapping.root)) {
		//applies root transform
		auto rootQ = m.get().root_r;
		auto rootT = m.get().root_t;
		auto rootUnrealT = FVector(100 * rootT[0], 100 * rootT[2], 100 * rootT[1]);
		auto rootUnrealQ = FQuat(xMod * rootQ.x, yMod * rootQ.y, zMod * rootQ.z, wMod * rootQ.w);
		auto rootId = FCompactPoseBoneIndex(boneNameToId[boneMapping.root]);
		auto rootCSTransform = output.Pose.GetComponentSpaceTransform(rootId);
		output.Pose.SetComponentSpaceTransform(rootId, FTransform(rotShift * rootUnrealQ, rootUnrealT));
	}

	//applies per bones transform
	for (auto& jname : boneOrder) {
		if (!boneNameToId.Contains(jname))
			continue;
		const auto& joints = m.get().joints;
		const auto& jId = boneNameToId[jname];
		//auto streamedJoint = joints.begin();
		if (joints.find(boneToRadName[jname]) == joints.end())
			continue;

		if (jname == "root_t" || jname == "root_r" || jId < 0)
			continue;
		
		const auto& jPose = joints.at(boneToRadName[jname]).pose;
		auto id = FCompactPoseBoneIndex(jId);
		
		//auto local = output.Pose.GetLocalSpaceTransform(FCompactPoseBoneIndex(id));		
		//auto parentT = output.Pose.GetComponentSpaceTransform(FCompactPoseBoneIndex(boneId[parent[jname]]));
		auto csTransform = output.Pose.GetComponentSpaceTransform(id);
		auto radTransform = FTransform(rotShift * FQuat(xMod * jPose.x, yMod * jPose.y, zMod * jPose.z, wMod * jPose.w));
		radTransform = debugTransformPrePost ? debugTransform * radTransform : radTransform * debugTransform;
		output.Pose.SetComponentSpaceTransform(id, radTransform * csTransform);
	}
}

bool FAnimNode_Radical::IsValidToEvaluate(const USkeleton* skeleton, const FBoneContainer& requiredBones) {
	return true;
}

void FAnimNode_Radical::InitializeBoneReferences(const FBoneContainer& requiredBones) {
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(InitializeBoneReferences)

	const FReferenceSkeleton ReferenceSkeleton = requiredBones.GetReferenceSkeleton();
	for (int16 boneInd = 0; boneInd < ReferenceSkeleton.GetNum(); boneInd++)
	{
		auto name = ReferenceSkeleton.GetBoneName(boneInd);
		
		boneNameToId.Add(name, requiredBones.GetPoseBoneIndexForBoneName(name));
	}

	boneToRadName = {
		{ boneMapping.root, "root_r" },
		{ boneMapping.LeftUpLeg, "LeftUpLeg_r"},
		{ boneMapping.LeftLeg, "LeftLeg_r" },
		{ boneMapping.LeftFoot, "LeftFoot_r"},
		{ boneMapping.RightUpLeg, "RightUpLeg_r"},
		{ boneMapping.RightLeg, "RightLeg_r" },
		{ boneMapping.RightFoot, "RightFoot_r" },
		{ boneMapping.Spine, "Spine_r" },
		{ boneMapping.Spine1, "Spine1_r" },
		{ boneMapping.Spine2, "Spine2_r" },
		{ boneMapping.Neck, "Neck_r" },
		{ boneMapping.Head, "Head_r" },
		{ boneMapping.RightShoulder, "RightShoulder_r" },
		{ boneMapping.RightArm, "RightArm_r" },
		{ boneMapping.RightForeArm, "RightForeArm_r" },
		{ boneMapping.RightHand, "RightHand_r" },
		{ boneMapping.LeftShoulder, "LeftShoulder_r" },
		{ boneMapping.LeftArm, "LeftArm_r" },
		{ boneMapping.LeftForeArm, "LeftForeArm_r" },
		{ boneMapping.LeftHand, "LeftHand_r" }
	};

	boneOrder = {
		boneMapping.root,
		boneMapping.LeftUpLeg,
		boneMapping.LeftLeg,
		boneMapping.LeftFoot,
		boneMapping.RightUpLeg,
		boneMapping.RightLeg,
		boneMapping.RightFoot,
		boneMapping.Spine,
		boneMapping.Spine1,
		boneMapping.Spine2,
		boneMapping.Neck,
		boneMapping.Head,
		boneMapping.RightShoulder,
		boneMapping.RightArm,
		boneMapping.RightForeArm,
		boneMapping.RightHand,
		boneMapping.LeftShoulder,
		boneMapping.LeftArm,
		boneMapping.LeftForeArm,
		boneMapping.LeftHand
	};
}

UWidgetFinder::UWidgetFinder() {
	//ConstructorHelpers::FObjectFinder<UEditorUtilityWidgetBlueprint> Ob(TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/UnrealLiveLinkRemapPlugin/Widgets/RadicalEditorWidget.RadicalEditorWidget'"));
	/*if (Ob.Succeeded())
		UI = Ob.Object;*/
}

void URadicalEditor::StartWidget()
{
	auto UI = NewObject<UWidgetFinder>()->UI;
	if (!UI || !UI->GeneratedClass->IsChildOf(UEditorUtilityWidget::StaticClass()))
		return;

	auto EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	EditorUtilitySubsystem->SpawnAndRegisterTab(UI);
	return;

	/*FName ModuleName("LevelEditor");
	const UEditorUtilityWidget* CDO = (UEditorUtilityWidget*)UI->GeneratedClass->GetDefaultObject();
	if (CDO->ShouldAutoRunDefaultAction())
	{
		// This is an instant-run blueprint, just execute it
		UEditorUtilityWidget* Instance = NewObject<UEditorUtilityWidget>((UObject*)GetTransientPackage());
		Instance->ExecuteDefaultAction();
	}

	auto RegistrationName = FName(*(UI->GetPathName() + TEXT("_ActiveTab")));
	auto DisplayName = FText::FromString(UI->GetName());
	auto& ModuleManager = FModuleManager::Get();
	checkf(ModuleManager.IsModuleLoaded(ModuleName), TEXT("Tried to get module interface for unloaded module: '%s'"), *(ModuleName.ToString()));
	auto& LevelEditorModule = static_cast<FLevelEditorModule&>(*ModuleManager.GetModule(ModuleName));
	auto LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
	auto NewDockTab = LevelEditorTabManager->TryInvokeTab(RegistrationName);*/
}

template <typename K, typename V>
bool contains(const std::map<K, V>& map, const K& key) {
	return map.find(key) != map.end();
}

bool parseMessage(sio::message::ptr message, Message& out )
{
	if (message->get_flag() != sio::message::flag_object)
		return false;
	auto map = message->get_map();
	
	if (map.find("attendeeId") != map.end()) {
		out.name = map["attendeeId"]->get_string();
	}
	else {
		return false;
	}
	
	if (!contains(map, std::string("result")))
		return false; 
	
	const auto& v = map["result"]->get_vector();
	if(!v.size() || !contains(v[0]->get_map(), std::string("frame_data")))
		return false;

	map = map["result"]->get_vector()[0]->get_map()["frame_data"]->get_map();

	if (map.find("root_r") != map.end()) {
		auto vec = map["root_r"]->get_vector();
		if (vec.size() < 4)
			return false;
		out.root_r.x = vec[1]->get_double();
		out.root_r.y = vec[2]->get_double();
		out.root_r.z = vec[3]->get_double();
		out.root_r.w = vec[0]->get_double();
	}
	else {
		return false;
	}

	if (map.find("root_t") != map.end()) {
		auto vec = map["root_t"]->get_vector();
		if (vec.size() < 3)
			return false;
		out.root_t[0] = vec[0]->get_double();
		out.root_t[1] = vec[1]->get_double();
		out.root_t[2] = vec[2]->get_double();
	}
	else {
		return false;
	}

	for (auto& element : map) {

		const auto& key = element.first;
		const auto& value = element.second;

		if (key == "root_t" || key == "root_r" || key == "attendeeId" || key == "timestamp")
			continue;

		joint j;
		auto vec = value->get_vector();
		j.name = key;
		j.pose.x = vec[1]->get_double();
		j.pose.y = vec[2]->get_double();
		j.pose.z = vec[3]->get_double();
		j.pose.w = vec[0]->get_double();
		/*
		for (auto const& elVec : message->get_vector()) {
		Non capisco a cosa serva questo for loop che hai messo tu, immagino sia per valorizzare i quaternioni, ma non posso ciclare su uno struct, vero?
		}
		*/
		out.joints[j.name] = j;
	}
	return true;
}

void URadicalEditor::disconnect() {
	if (!connected || !audience) {
		return;
	}

	audience->DisconnectSync();
	connected = false;
	players.clear();
}

Optional<Message> URadicalEditor::getLastPose(FString playerid) {
	Optional<Message> ret;
	mut.lock();
	if (lastPose.find(playerid) != lastPose.end()) {
		ret.set(lastPose[playerid]);
	}
	mut.unlock();
	return ret;
}

void URadicalEditor::connect(const FString& _uriString, const FString& _userAuthToken, const FString& _roomId, const FString& _clientLabel)
{
	if (_uriString.IsEmpty() || _userAuthToken.IsEmpty() || _roomId.IsEmpty())
		return;

	if (connected) {
		return;
	}

	audience = std::make_unique<Audience>(
		TCHAR_TO_ANSI(*_uriString),
		TCHAR_TO_ANSI(*_userAuthToken),
		TCHAR_TO_ANSI(*_roomId),
		"unity", std::shared_ptr<IConnectionManager>(new SIOConnectionManagerAdapter));

	audience->OnConnected(
		[this]()
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
				connected = true;
				ConnectDelegate.Broadcast();
				UE_LOG(LogTemp, Warning, TEXT("Connected"));
			});
		});

	audience->OnDisconnected(
		[this]()
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
				connected = false;
				DisconnectDelegate.Broadcast();
				UE_LOG(LogTemp, Warning, TEXT("Disconnected"));
			});
		});

	audience->OnError(
		[this](sio::message::ptr message)
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
				ErrorDelegate.Broadcast();
				UE_LOG(LogTemp, Warning, TEXT("Error"));
			});
		});

	audience->OnData(
		[this](sio::message::ptr data)
		{
			Message m;
			if (parseMessage(data, m)) {
				
				mut.lock();
				lastPose[m.name.c_str()] = m;
				mut.unlock();
				// if there's new player
				if (std::find(players.begin(), players.end(), m.name) == players.end()) {
					players.push_back(m.name);
					if (NewPlayerDelegate.IsBound()) {
						AsyncTask(ENamedThreads::GameThread, [this, m]() {
							NewPlayerDelegate.Broadcast(m.name.c_str());
						});
					}
						
				}
			}
		});

	UE_LOG(LogTemp, Warning, TEXT("Attempting connection"));
	audience->Connect();
}
