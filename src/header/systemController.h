#ifndef SYSTEM_CONTROLLER
#define SYSTEM_CONTROLLER

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>

#pragma comment(lib, "ole32.lib")

class SystemController {
public:
	//меняет громкость звука
	HRESULT SetSystemVolume(float volume);
	//получаем громкость звука
	HRESULT GetSystemVolume(float* volume);

	//нажатие на кнопки
	void SendMediaKey(WORD vkMedia);
};

#endif //SYSTEM_CONTROLLER