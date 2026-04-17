#include"../header/systemController.h"
#include"../header/session.h"

//установка громкости
HRESULT SystemController::SetSystemVolume(float volume)
{
    HRESULT hrCo = CoInitialize(nullptr);
    bool bCoInitialized = SUCCEEDED(hrCo);

    if (FAILED(hrCo) && hrCo != RPC_E_CHANGED_MODE) {
        std::cout << "CoInitialize failed: " << std::hex << hrCo << std::endl;
        return hrCo;
    }

    HRESULT hr = S_OK;
    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioEndpointVolume* pEndpointVolume = nullptr;

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);

    if (SUCCEEDED(hr)) {
        hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    }
    if (SUCCEEDED(hr)) {
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&pEndpointVolume);
    }
    if (SUCCEEDED(hr)) {
        hr = pEndpointVolume->SetMasterVolumeLevelScalar(volume, nullptr);
    }

    if (pEndpointVolume) pEndpointVolume->Release();
    if (pDevice) pDevice->Release();
    if (pEnumerator) pEnumerator->Release();

    if (bCoInitialized) CoUninitialize();

    return hr;
}

// Получение текущей громкости
HRESULT SystemController::GetSystemVolume(float* volume)
{
    HRESULT hr;
    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioEndpointVolume* pEndpointVolume = nullptr;

    hr = CoInitialize(nullptr);
    bool comWasInitializedByUs = SUCCEEDED(hr);

    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) return hr;

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);

    if (SUCCEEDED(hr)) {
        hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
        if (SUCCEEDED(hr)) {
            hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL,
                nullptr, (void**)&pEndpointVolume);
            if (SUCCEEDED(hr)) {
                hr = pEndpointVolume->GetMasterVolumeLevelScalar(volume);
            }
        }
    }

    if (pEndpointVolume) pEndpointVolume->Release();
    if (pDevice) pDevice->Release();
    if (pEnumerator) pEnumerator->Release();

    if (comWasInitializedByUs) CoUninitialize();

    return hr;
}

//нажатие на клавиши 
void SystemController::SendMediaKey(WORD vkMedia)
{
    INPUT ip = { 0 };
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = vkMedia;
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));
    
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

void SystemController::SendMouseClick(DWORD buttonDown, DWORD buttonUp)
{
    INPUT ip = { 0 };
    ip.type = INPUT_MOUSE;
    ip.mi.dwFlags = buttonDown;
    SendInput(1, &ip, sizeof(INPUT));

    ip.mi.dwFlags = buttonUp;
    SendInput(1, &ip, sizeof(INPUT));
}
//скролл мышки
void SystemController::SendMouseWheel(int delta, int times = 1)
{
    for (int i = 0; i < times; i++) {
        INPUT ip = { 0 };
        ip.type = INPUT_MOUSE;
        ip.mi.dwFlags = MOUSEEVENTF_WHEEL;
        ip.mi.mouseData = delta;
        SendInput(1, &ip, sizeof(INPUT));
    }
}

//получаем название музыки которя сейчас играет
boost::json::value SystemController::GetMedia()
{
	boost::json::object mediaInfo;

	winrt::init_apartment();

    auto manager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
    
    auto currentSession = manager.GetCurrentSession();
    
    // helper: wchar_t* -> UTF-8 std::string
    auto to_utf8 = [](std::wstring const& w) -> std::string {
        if (w.empty()) return {};
        int size = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string s;
        s.resize(size > 0 ? size - 1 : 0);
        if (!s.empty())
            WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, &s[0], size, nullptr, nullptr);
        return s;
    };

    if (currentSession)
    {
        auto appId = currentSession.SourceAppUserModelId();
        mediaInfo["Programm"] = to_utf8(std::wstring(appId.c_str()));

        auto props = currentSession.TryGetMediaPropertiesAsync().get();

        mediaInfo["Name"] = to_utf8(std::wstring(props.Title().c_str()));
        mediaInfo["Artist"] = to_utf8(std::wstring(props.Artist().c_str()));

        auto playbackInfo = currentSession.GetPlaybackInfo();
        std::string status;
        switch (playbackInfo.PlaybackStatus())
        {
        case GlobalSystemMediaTransportControlsSessionPlaybackStatus::Playing: status = "Playing"; break;
        case GlobalSystemMediaTransportControlsSessionPlaybackStatus::Paused:  status = "Paused"; break;
        default: status = "Other"; break;
        }
        mediaInfo["Status"] = status;
    }
    else
    {
        mediaInfo["Programm"] = "";
        mediaInfo["Name"] = "";
        mediaInfo["Artist"] = "";
        mediaInfo["Status"] = "NothingPlaying";
    }

    return mediaInfo;
}
//получаем название ПК
std::string SystemController::Get_PC_Name()
{
    char name[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = sizeof(name);

    if (GetComputerName(name, &size)) {
		return std::string(name);
    }

    return std::string();
}
