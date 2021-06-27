// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UEPythonTest : ModuleRules
{
	public UEPythonTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "InputCore"});

		PrivateDependencyModuleNames.AddRange(new string[] {"PythonScriptPlugin"});
	}
}
