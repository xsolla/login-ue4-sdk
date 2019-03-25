// Copyright 2019 Vladimir Alyamkin. All Rights Reserved.

using UnrealBuildTool;

public class XsollaLogin : ModuleRules
{
	public XsollaLogin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"HTTP",
				"Json",
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			}
			);

		PublicDefinitions.Add("WITH_XSOLLA_LOGIN=1");
	}
}
