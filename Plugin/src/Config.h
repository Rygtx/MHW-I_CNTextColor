#pragma once

#include "DKUtil/Config.hpp"

using namespace DKUtil::Alias;


class Config : 
    public dku::model::Singleton<Config>
{
public:
	enum class ColorFmt
	{
		RED,
		GREEN,
		BLUE,
		PURPLE,
		YELLOW,
		ORANGE,
		LIGHTBLUE,
		LIGHTGREEN,
		LIGHTYELLOW,
		SLGREEN,
	};

	String FmtColor[5]{
		String{ "SkillProc" },
		String{ "EquipmentReady" },
		String{ "FieldWeaponReady" },
		String{ "HHMelodyPlayed" },
		String{ "HHMelodyGranted" },
	};

    static void Load() noexcept;

private:
	TomlConfig _config = COMPILE_PROXY("nativePC\\Plugins\\DKTextColor.toml"sv);
};
