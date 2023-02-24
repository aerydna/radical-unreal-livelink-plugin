// Copyright RADiCAL Solutions, LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Commands/UIAction.h"
#include "Editor/EditorEngine.h"
#include "EditorUtilityWidget.h"

class FToolBarBuilder;
class FMenuBuilder;
class FReply;
class SWidget;

class FUnrealLiveLinkRemapPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void ButtonCreateWidget();
	void TestAction();

	void AddToolbarButton(FToolBarBuilder& Builder);
	void AddMenuEntry(FMenuBuilder& MenuBuilder);
	void FillSubmenu(FMenuBuilder& MenuBuilder);
	TSharedRef<SWidget> FillComboButton(TSharedPtr<class FUICommandList> Commands);

	TSharedPtr<FUICommandList> PluginCommands;
	
};
