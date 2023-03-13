
#include "FRadicalPluginEditorCommands.h"
#include "UnrealLiveLinkRemapPlugin.h"


#define LOCTEXT_NAMESPACE "FRadicalPluginEditorModule"

void FRadicalPluginEditorCommands::RegisterCommands()
{
	UI_COMMAND(TestCommand, "TestCommand", "Opens the RADiCAL editor widget", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(MenuTestCommand, "MenuTestCommand", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(RunRADiCALLiveWidget, "Run RADiCAL Live", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SubmenuTestCommand02, "Radical Live for Unreal engine V.1  |  1 March 2023", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SubmenuTestCommand03, "www.radicalmotion.com", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(UserGuide, "User Guide", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE