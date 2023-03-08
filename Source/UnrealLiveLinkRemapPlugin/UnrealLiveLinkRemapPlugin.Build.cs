// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class UnrealLiveLinkRemapPlugin : ModuleRules
{
	public UnrealLiveLinkRemapPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = true;
		bEnableExceptions = true;

		PublicIncludePaths.AddRange(
			new string[]
			{
				//Path.Combine(ModuleDirectory, "RadicalLiveLibraryPublic")
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(ModuleDirectory, "Private/include")
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"LiveLinkInterface",
				"LiveLink",
				"LiveLinkComponents",
				"LiveLinkAnimationCore",
				"RadicalSocketIOLib", "Json",
				"LevelEditor",
				"EditorStyle",
				"EditorSubsystem",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"Blutility",
				"UMG",
				"UMGEditor",
				"Projects"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"LiveLink",
				"LiveLinkComponents",
				"RadicalLiveClient",
				"UMGEditor",
				"Blutility",
				"UMG",
				"UnrealEd",
			}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}