
#include "FRadicalPluginEditorCommands.h"
#include "UnrealLiveLinkRemapPlugin.h"


#define LOCTEXT_NAMESPACE "FRadicalPluginEditorModule"

void FRadicalPluginEditorCommands::RegisterCommands()
{
	UI_COMMAND(TestCommand, "Open Radical Live", "Opens the RADiCAL editor widget", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(MenuTestCommand, "Radical Live", "", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(RunRADiCALLiveWidget, "Run RADiCAL Live", "", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SubmenuTestCommand02, "Radical Live for Unreal engine V.1  |  1 March 2023", "", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SubmenuTestCommand03, "www.radicalmotion.com", "", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(UserGuide, "User Guide", "", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE