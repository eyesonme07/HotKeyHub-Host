#ifndef SYSTEM_CONTROLLER
#define SYSTEM_CONTROLLER

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Storage.Streams.h>
#include <boost/json.hpp>
#include <iostream>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "windowsapp.lib")

using namespace winrt;
using namespace Windows::Media::Control;
using namespace Windows::Foundation;

const std::unordered_map<std::string, WORD> specialKeys = {
				{"ctrl",      VK_CONTROL},
				{"shift",     VK_SHIFT},
				{"alt",       VK_MENU},
				{"win",       VK_LWIN},
				{"enter",     VK_RETURN},
				{"tab",       VK_TAB},
				{"backspace", VK_BACK},
				{"space",     VK_SPACE},
				{"escape",    VK_ESCAPE},
				{"up",        VK_UP},
				{"down",      VK_DOWN},
				{"left",      VK_LEFT},
				{"right",     VK_RIGHT},
				{"f1",        VK_F1},
				{"f2",        VK_F2},
				{"f3",        VK_F3},
				{"f4",        VK_F4},
				{"f5",        VK_F5},
				{"f6",        VK_F6},
				{"f7",        VK_F7},
				{"f8",        VK_F8},
				{"f9",        VK_F9},
				{"f10",       VK_F10},
				{"f11",       VK_F11},
				{"f12",       VK_F12},
				{"pageup",    VK_PRIOR},
				{"pagedown",  VK_NEXT},
				{"home",      VK_HOME},
				{"end",       VK_END},
				{"insert",    VK_INSERT},
				{"delete",    VK_DELETE},
				{"numlock",   VK_NUMLOCK},
				{"capslock",  VK_CAPITAL},
				{"printscr",  VK_SNAPSHOT},
				{"pause",     VK_PAUSE}
};

wchar_t utf8ToWchar(const std::string& str);

class SystemController {
public:
	//меняет громкость звука
	HRESULT SetSystemVolume(float volume);
	//получаем громкость звука
	HRESULT GetSystemVolume(float* volume);
	//нажатие на кнопки
	void SendMediaKey(WORD vkMedia, bool keyDown = true);
	//нажатие на клавиши (для символов)
	void SendUnicodeChar(wchar_t ch, bool keyDown);
	//нажатие на мышку
	void SendMouseClick(DWORD buttonDown, DWORD buttonUp);
	//скролл мышки
	void SendMouseWheel(int delta, int times);
	//получаем название музыки которя сейчас играет
	boost::json::value GetMedia();
	//получаем название ПК
	std::string Get_PC_Name();
};

#endif //SYSTEM_CONTROLLER