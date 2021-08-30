class CfgPatches
{
	class LostCoastBasicMap
	{
		name = "Lost Coast BasicMap Markers";
		author = "aflyingcougar";
		url = "github.com/aflyingcougar";

		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"BasicMap", "DZ_Scripts"};
	};
};
class CfgMods
{
	class LostCoastBasicMap
	{
		type = "Mod";
		
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"LostCoastBasicMap/scripts/3_Game"};
			};
		};
	};
};