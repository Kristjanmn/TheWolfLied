// Made by Kristjanmn

using UnrealBuildTool;
using System.Collections.Generic;

public class TheWolfLiedEditorTarget : TargetRules
{
	public TheWolfLiedEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
        ExtraModuleNames.Add("TheWolfLied");
	}
}
