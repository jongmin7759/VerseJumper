// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class SteamAPI : ModuleRules
{
	public SteamAPI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
				
		// Steamworks API
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Steam", "lib", "steam_api64.lib"));
		
		try
		{
			string SteamDLLFileName = "steam_api64.dll";
			string SteamDLLPath = Path.Combine(ModuleDirectory, "Steam", "lib", SteamDLLFileName);
			string ProjectPath = Directory.GetParent(ModuleDirectory).Parent.ToString();
			string BinariesDir = Path.Combine(ProjectPath, "Binaries", Target.Platform.ToString());

			if (!Directory.Exists(BinariesDir))
				Directory.CreateDirectory(BinariesDir);

			File.Copy(SteamDLLPath, Path.Combine(BinariesDir, SteamDLLFileName), true);
		} catch { }
	}
}
