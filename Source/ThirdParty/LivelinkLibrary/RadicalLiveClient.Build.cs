// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class RadicalLiveClient : ModuleRules
{
	public RadicalLiveClient(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			bUseRTTI = true;
			bEnableExceptions = true;

            
			PublicAdditionalLibraries.AddRange(new string[]{
				Path.Combine(ModuleDirectory, "./RadicalLiveClient.lib")
			});

			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));
		}
	}
}
