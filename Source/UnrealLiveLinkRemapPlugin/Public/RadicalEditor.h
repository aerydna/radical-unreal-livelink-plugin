// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <array>
#include <mutex>
#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "RadicalLiveClient.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetBlueprint.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "UObject/ObjectMacros.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "RadicalEditor.generated.h"


struct quaternion {
	double x;
	double y;
	double z;
	double w;
};

struct joint {
	std::string name;
	quaternion pose;
	bool operator==(const joint& other) {
		return name == other.name;
	}
};

struct Message {
	std::string name;
	std::map<std::string, joint> joints;
	quaternion root_r;
	std::array<double, 3> root_t;
};

UCLASS()
class UNREALLIVELINKREMAPPLUGIN_API UWidgetFinder : public UObject {
	GENERATED_BODY()
public:
	UWidgetFinder();
	UEditorUtilityWidgetBlueprint* UI;
};

USTRUCT(BlueprintType)
struct FDebugQuatOptions {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool flipX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool flipY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool flipZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool flipW;
};

USTRUCT(BlueprintInternalUseOnly)
struct UNREALLIVELINKREMAPPLUGIN_API FAnimNode_Radical : public FAnimNode_SkeletalControlBase {
	GENERATED_USTRUCT_BODY()

	virtual void GatherDebugData(FNodeDebugData& debugData) override;
	virtual bool NeedsOnInitializeAnimInstance() const override { return true; }

	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& output, TArray<FBoneTransform>& outBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* skeleton, const FBoneContainer& requiredBones) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links, meta = (PinShownByDefault))
	FString playerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links, meta = (PinShownByDefault))
	FTransform debugTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links, meta = (PinShownByDefault))
	bool debugTransformPrePost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links, meta = (PinShownByDefault))
	FDebugQuatOptions quatOptions;

	FAnimNode_Radical();
private:
	virtual void InitializeBoneReferences(const FBoneContainer& requiredBones) override;
	std::map<FName, int16> boneNameToId;
};

UCLASS(MinimalAPI)
class UAnimGraphNode_Radical : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()

public:

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface

protected:

	// UAnimGraphNode_SkeletalControlBase interface
	virtual FText GetControllerDescription() const override;
	virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }
	virtual void Draw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* previewSkelMeshComp) const override;
	// End of UAnimGraphNode_SkeletalControlBase interface

private:

	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_Radical Node;

};

template <typename T>
class Optional {
	bool valid{false};
	T value;
public:
	void set(T val) { value = val; valid = true; }
	bool isValid() { return valid; }
	T get() { return value; }
};

/**
 * 
 */
UCLASS()
class UNREALLIVELINKREMAPPLUGIN_API URadicalEditor : public UEditorSubsystem
{
	GENERATED_BODY()

	std::unique_ptr<rad::live_client::Audience> audience;
	std::map<FString, Message> lastPose;
	mutable std::mutex mut;
	std::vector<std::string> players;

	UPROPERTY()
	TMap<FString, AActor*> rigs;
public:
	void StartWidget();
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnect);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisconnect);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnError);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewPlayer, const FString, playerId);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRemoved, const FString, playerId);

	
	
	Optional<Message> getLastPose(FString playerid);

	UPROPERTY(BlueprintReadOnly, Category = "RadicalSubsystem")
	bool connected{false};
	
	UPROPERTY(BlueprintAssignable, Category = "RadicalSubsystem")
	FOnConnect ConnectDelegate;

	UPROPERTY(BlueprintAssignable, Category = "RadicalSubsystem")
	FOnDisconnect DisconnectDelegate;

	UPROPERTY(BlueprintAssignable, Category = "RadicalSubsystem")
	FOnError ErrorDelegate;

	UPROPERTY(BlueprintAssignable, Category = "RadicalSubsystem")
	FOnNewPlayer NewPlayerDelegate;

	UPROPERTY(BlueprintAssignable, Category = "RadicalSubsystem")
	FOnPlayerRemoved PlayerRemovedDelegate;

	UFUNCTION(BlueprintCallable, Category = "RadicalSubsystem")
	TArray<FString> getAvailablePlayers() {
		auto ret = TArray<FString>();
		for (const auto& p : players) {
			ret.Push(p.c_str());
		}
		return ret;
	}

	UFUNCTION(BlueprintCallable, Category = "RadicalSubsystem")
	void RegisterRig(FString playerID, AActor* actor) {
		rigs[playerID] = actor;
	}

	UFUNCTION(BlueprintCallable, Category = "RadicalSubsystem")
	TMap<FString, AActor*> GetRigs() {
		return rigs;
	}

	UFUNCTION(BlueprintCallable, Category = "RadicalSubsystem")
	void connect(
		const FString& _uriString,
		const FString& _userAuthToken,
		const FString& _roomId,
		const FString& _clientLabel = "");
	
	UFUNCTION(BlueprintCallable, Category = "RadicalSubsystem")
	void disconnect();
};


