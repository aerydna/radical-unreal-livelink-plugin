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
			PublicAdditionalLibraries.AddRange(new string[]{
				Path.Combine(ModuleDirectory, "./RadicalLiveClient.lib"),
				Path.Combine(ModuleDirectory, "./sioclient.lib"),
				Path.Combine(ModuleDirectory, "./sioclient_tls.lib")
		  });
		PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "./include") });
		}
	}
}
