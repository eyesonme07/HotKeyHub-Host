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

class SystemController {
public:
	//меняет громкость звука
	HRESULT SetSystemVolume(float volume);
	//получаем громкость звука
	HRESULT GetSystemVolume(float* volume);
	//нажатие на кнопки
	void SendMediaKey(WORD vkMedia);
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