#include "Config.h"
#include "MinHook.h"


// 缤·纷·色·彩
#define MAKE_COLOR(COLOR, G)	"<STYL MOJI_" #COLOR "_DEFAULT>$" #G "</STYL>"


// 15.11.01 (build 421471) 应该不会再更 新  了   吧
// 15.20.00 (build ) 更新了.
enum class MsgFlag : std::uint32_t
{
	kDefault = static_cast<std::uint32_t>(-1)
};


using ShowGameMessage_t = std::add_pointer_t<void(void*, const char*, float, MsgFlag, const bool)>;
ShowGameMessage_t ShowGameMessage = ShowGameMessage_t(0x141A53400);
ShowGameMessage_t _OriginalFunc = nullptr;
void*             MsgManager = (void*)0x14500AC30;


std::string MakeColorFmt(std::string a_color, bool a_prefix)
{
	return fmt::format("{}<STYL MOJI_{}_DEFAULT>{}</STYL>", 
		a_prefix ? "$1" : "",
		a_color,
		a_prefix ? "$2" : "$&"
	);
}


// <regex, full_match>
std::pair<std::string, bool> RgxTbl[]{
	// 技能
	{ "\\[[^\\]]+\\]", false },
	// 衣装
	{ "^.*?(?=\\s+现在可以使用了。)", false },
	// 场地装备
	{ "(可使用)(.*?)(?=了。)", true },
	// 狩猎笛演奏
	{ "^.*?(?=的旋律效果。)", false },
	// 狩猎笛聆听
	{ "(获得)(.*?)(?=的\\s+旋律效果。)", true },
};


void Hook_ShowGameMessage(void* a_this, const char* a_msg, float a_depth = -1.f, MsgFlag a_flag = MsgFlag::kDefault, const bool a_useIceborneColor = false)
{
	std::string msg{ a_msg };

	if (msg.empty()) {
		return;
	}

	auto* config = Config::GetSingleton();
	for (auto i : std::views::iota(0, 5)) {
		auto& [rgx, fm] = RgxTbl[i];
		if (std::regex_search(msg, std::regex(rgx))) {
			msg = std::regex_replace(msg, std::regex(rgx), 
				MakeColorFmt(config->FmtColor[i].get_data(), fm));
			break;
		}
	}

	_OriginalFunc(a_this, &msg[0], a_depth, a_flag, a_useIceborneColor);
}

 
BOOL APIENTRY DllMain(HMODULE a_hModule, DWORD  a_ul_reason_for_call, LPVOID a_lpReserved)
{
	if (a_ul_reason_for_call == DLL_PROCESS_ATTACH) {
		Config::Load();
		MH_Initialize();
		MH_CreateHook(ShowGameMessage, &Hook_ShowGameMessage, reinterpret_cast<void**>(&_OriginalFunc));
		MH_EnableHook(ShowGameMessage);
	}
	
    return TRUE;
}
