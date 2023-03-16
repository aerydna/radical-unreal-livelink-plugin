// Copyright RADiCAL Solutions, LLC. All Rights Reserved.

#include "UnrealLiveLinkRemapPlugin.h"
#include "FRadicalPluginEditorCommands.h"
#include "LevelEditor.h"
#include "Framework/Commands/UICommandList.h"
#include "Editor/EditorEngine.h"
#include "EditorUtilityWidget.h"



#define LOCTEXT_NAMESPACE "FUnrealLiveLinkRemapPluginModule"

void FUnrealLiveLinkRemapPluginModule::StartupModule()
{

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FRadicalPluginEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FRadicalPluginEditorCommands::Get().TestCommand,
		FExecuteAction::CreateRaw(this, &FUnrealLiveLinkRemapPluginModule::ButtonCreateWidget)
	);
	PluginCommands->MapAction(
		FRadicalPluginEditorCommands::Get().MenuTestCommand,
		FExecuteAction::CreateRaw(this, &FUnrealLiveLinkRemapPluginModule::TestAction)
	);
	PluginCommands->MapAction(
		FRadicalPluginEditorCommands::Get().RunRADiCALLiveWidget,
		FExecuteAction::CreateLambda([]() {UE_LOG(LogTemp, Warning, TEXT("radical command")); })
	);
	PluginCommands->MapAction(
		FRadicalPluginEditorCommands::Get().SubmenuTestCommand02,
		FExecuteAction::CreateLambda([]() {UE_LOG(LogTemp, Warning, TEXT("radical command")); })
	);
	PluginCommands->MapAction(
		FRadicalPluginEditorCommands::Get().SubmenuTestCommand03,
		FExecuteAction::CreateLambda([]() {FPlatformProcess::LaunchURL(TEXT("www.radical.com"), NULL, NULL); })
	);
	PluginCommands->MapAction(
		FRadicalPluginEditorCommands::Get().UserGuide,
		FExecuteAction::CreateLambda([]() {FPlatformProcess::LaunchURL(TEXT("radicalmotion.com/ue5-user-guide"), NULL, NULL); })
	);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedPtr<FExtender> NewToolbarExtender = MakeShareable(new FExtender);
	NewToolbarExtender->AddToolBarExtension(
		"Content",
		EExtensionHook::After,
		PluginCommands,
		FToolBarExtensionDelegate::CreateRaw(this, &FUnrealLiveLinkRemapPluginModule::AddToolbarButton)
	);

	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(NewToolbarExtender);

	TSharedPtr<FExtender> NewMenuExtender = MakeShareable(new FExtender);
	NewMenuExtender->AddMenuExtension(
		"LevelEditor",
		EExtensionHook::After,
		PluginCommands,
		FMenuExtensionDelegate::CreateRaw(this, &FUnrealLiveLinkRemapPluginModule::AddMenuEntry)
	);

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(NewMenuExtender);
		
}

void FUnrealLiveLinkRemapPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FRadicalPluginEditorCommands::Unregister();
	
}

void FUnrealLiveLinkRemapPluginModule::ButtonCreateWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("Test Button Create Widget"));
	/*
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Test Button Create Widget Post !World"));
	
	// load UMG Of BP
	UBlueprint* UMGBP = LoadObject<UBlueprint>(nullptr, L"/Script/Blutility.EditorUtilityWidgetBlueprint'/Game/Widgets/RadicalEditorWidget.RadicalEditorWidget'"); //Reference to your EditorUtilityWidget
	UClass* BlueprintClass = UMGBP->GeneratedClass;
	TSubclassOf<UEditorUtilityWidget> WidgetClass = BlueprintClass;
	CreateWidget<UEditorUtilityWidget>(World, WidgetClass)->Run();
	*/
}

void FUnrealLiveLinkRemapPluginModule::TestAction()
{
	
}

void FUnrealLiveLinkRemapPluginModule::AddToolbarButton(FToolBarBuilder& Builder)
{	
	Builder.AddToolBarButton(FRadicalPluginEditorCommands::Get().TestCommand);

	FUIAction TempCompileOptionsCommand;

	Builder.AddComboButton(
		TempCompileOptionsCommand,
		FOnGetContent::CreateRaw(this, &FUnrealLiveLinkRemapPluginModule::FillComboButton, PluginCommands),
		LOCTEXT("ExampleComboButton", "Example combo button"),
		LOCTEXT("ExampleComboButtonTootlip", "Example combo button tootltip"),
		TAttribute<FSlateIcon>(),
		true
	);
}

void FUnrealLiveLinkRemapPluginModule::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("CustomMenu", TAttribute<FText>(FText::FromString("TestMenu")));
	MenuBuilder.AddMenuEntry(FRadicalPluginEditorCommands::Get().MenuTestCommand);
	MenuBuilder.AddSubMenu(
		FText::FromString("Radical Live"),
		FText::FromString("Radical Live Submenu"),
		FNewMenuDelegate::CreateRaw(this, &FUnrealLiveLinkRemapPluginModule::FillSubmenu)
	);

	MenuBuilder.EndSection();
}

void FUnrealLiveLinkRemapPluginModule::FillSubmenu(FMenuBuilder& MenuBuilder)
{
	
	MenuBuilder.AddMenuEntry(FRadicalPluginEditorCommands::Get().RunRADiCALLiveWidget);
	MenuBuilder.AddMenuEntry(FRadicalPluginEditorCommands::Get().SubmenuTestCommand02);
	MenuBuilder.AddMenuEntry(FRadicalPluginEditorCommands::Get().SubmenuTestCommand03);
	MenuBuilder.AddMenuEntry(FRadicalPluginEditorCommands::Get().UserGuide);
}


TSharedRef<SWidget> FUnrealLiveLinkRemapPluginModule::FillComboButton(TSharedPtr<class FUICommandList> Commands)
{

	FMenuBuilder MenuBuilder(true, Commands);
	
	MenuBuilder.AddMenuSeparator(FName(TEXT("RADICAL LIVE")));
	MenuBuilder.AddMenuEntry(FRadicalPluginEditorCommands::Get().RunRADiCALLiveWidget);
	MenuBuilder.AddMenuSeparator(FName(TEXT("ABOUT & USER GUIDE")));
	MenuBuilder.AddMenuEntry(FRadicalPluginEditorCommands::Get().SubmenuTestCommand02);
	MenuBuilder.AddMenuEntry(FRadicalPluginEditorCommands::Get().SubmenuTestCommand03);
	MenuBuilder.AddMenuEntry(FRadicalPluginEditorCommands::Get().UserGuide);

	return MenuBuilder.MakeWidget();
}
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealLiveLinkRemapPluginModule, UnrealLiveLinkRemapPlugin)