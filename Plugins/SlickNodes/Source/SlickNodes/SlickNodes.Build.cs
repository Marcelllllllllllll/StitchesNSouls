// Copyright N.A. Tools 2025. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class SlickNodes : ModuleRules 
{
	public SlickNodes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] { });
		PrivateIncludePaths.AddRange(new string[] { });
        
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine",
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
			"Slate",
			"SlateCore",
			"UnrealEd",
			"GraphEditor",
			"BlueprintGraph",
			"Kismet",
			"Projects",
			"DeveloperSettings",
		});
		
		if (Target.Version.MinorVersion >= 3)
		{
			PrivateDependencyModuleNames.Add("ToolWidgets");
		}
	}
}