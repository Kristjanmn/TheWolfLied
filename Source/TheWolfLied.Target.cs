// Made by Kristjanmn

using UnrealBuildTool;
using System.Collections.Generic;

public class TheWolfLiedTarget : TargetRules
{
	public TheWolfLiedTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.Add("TheWolfLied");
	}
}
