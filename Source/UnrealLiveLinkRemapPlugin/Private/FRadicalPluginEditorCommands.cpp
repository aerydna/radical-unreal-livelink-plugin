
#include "FRadicalPluginEditorCommands.h"
#include "UnrealLiveLinkRemapPlugin.h"


#define LOCTEXT_NAMESPACE "FRadicalPluginEditorModule"

void FRadicalPluginEditorCommands::RegisterCommands()
{
	UI_COMMAND(TestCommand, "TestCommand", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(MenuTestCommand, "MenuTestCommand", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SubmenuTestCommand01, "SubmenuTestCommand01", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SubmenuTestCommand02, "SubmenuTestCommand02", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(SubmenuTestCommand03, "SubmenuTestCommand03", "This is test command", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE