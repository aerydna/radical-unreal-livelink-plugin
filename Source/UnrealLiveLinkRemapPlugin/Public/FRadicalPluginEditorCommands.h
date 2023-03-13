// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "EditorStyle/Public/EditorStyleSet.h"
#include "SlateBasics.h"

class FRadicalPluginEditorCommands : public TCommands<FRadicalPluginEditorCommands>
{
public:
	FRadicalPluginEditorCommands()
		: TCommands<FRadicalPluginEditorCommands>
		(
			TEXT("RadicalPluginEditor"),
			NSLOCTEXT("Contexts", "RadicalPluginEditor", "RadicalPluginEditor Plugin"),
			NAME_None,
			FEditorStyle::GetStyleSetName()
		) {}

	virtual void RegisterCommands() override;

public:

	TSharedPtr<FUICommandInfo> TestCommand;
	TSharedPtr<FUICommandInfo> MenuTestCommand;
	TSharedPtr<FUICommandInfo> RunRADiCALLiveWidget;
	TSharedPtr<FUICommandInfo> SubmenuTestCommand02;
	TSharedPtr<FUICommandInfo> SubmenuTestCommand03;
	TSharedPtr<FUICommandInfo> UserGuide;
};