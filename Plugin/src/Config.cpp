#include "Config.h"

void Config::Load() noexcept
{
	auto* instance = Config::GetSingleton();

	for (auto i = 0; i < 5; ++i) {
		instance->_config.Bind(instance->FmtColor[i], "ORANGE");
	}

	instance->_config.Load();
}
