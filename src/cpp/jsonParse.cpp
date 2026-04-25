#include "../header/jsonParse.h"
#include"../header/session.h"
#include<string>

void JsonParse::parse(const boost::json::value& jsonValue)
{
	//если это объект
	if (jsonValue.is_object())
	{
		const boost::json::object& obj = jsonValue.as_object();
		if (obj.contains("command"))
		{
			std::string command = obj.at("command").as_string().c_str();
			///////////////////////////////////////////////////////////////////////////////////
			if (command == "play_pause")
			{
				systemController_.SendMediaKey(VK_MEDIA_PLAY_PAUSE);
			}
			////////////////////////////////////////////////////////////////////////////////////
			else if (command == "next")
			{
				systemController_.SendMediaKey(VK_MEDIA_NEXT_TRACK);
			}
			////////////////////////////////////////////////////////////////////////////////////
			else if (command == "previous")
			{
				systemController_.SendMediaKey(VK_MEDIA_PREV_TRACK);
			}
			////////////////////////////////////////////////////////////////////////////////////
			else if (command == "volume")
			{
				if (obj.contains("value")) {

					double command_val = obj.at("value").as_double();
					systemController_.SetSystemVolume(static_cast<float>(command_val));
				}
			}
			////////////////////////////////////////////////////////////////////////////////////
			else if (command == "moveMouse") {
				if (obj.contains("x") && obj.contains("y")) {
					double x = obj.at("x").as_double();
					double y = obj.at("y").as_double();
					POINT cursorPos;
					GetCursorPos(&cursorPos);
					SetCursorPos(cursorPos.x + x, cursorPos.y + y);
				}
			}
			////////////////////////////////////////////////////////////////////////////////////
			else if (command == "lButton") {
				systemController_.SendMouseClick(MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
			}
			////////////////////////////////////////////////////////////////////////////////////
			else if (command == "rButton") {
				systemController_.SendMouseClick(MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
			}
			/////////////////////////////////////////////////////////////////////////////////////
			else if (command == "Scroll") {
				int value = static_cast<int>(obj.at("value").as_double());
				systemController_.SendMouseWheel(value,1);
			}
			/////////////////////////////////////////////////////////////////////////////////////
			//нажатие не клавиши
			else if (command == "pressKey" || command == "releaseKey") {
				bool isPress = (command == "pressKey");

				if (obj.contains("keys")) {
					int keysCount = std::stoi(obj.at("keys").as_string().c_str());

					for (int i = 0; i < keysCount; ++i) {
						std::string keyName = obj.at(std::to_string(i)).as_string().c_str();

						// получаем виртуальный код клавиши
						WORD vkKey = getVirtualKey(keyName);

						if (isPress) {
							std::lock_guard<std::mutex> lock(keysMutex_);

							// не запускаем второй поток если клавиша уже нажата
							if (activeKeys_.find(keyName) != activeKeys_.end()) continue;

							// запускаем поток и кладём в map
							activeKeys_[keyName] = std::thread(
								&JsonParse::pressKeyFunc, this, keyName, vkKey
							);

						}
						else { // releaseKey
							std::lock_guard<std::mutex> lock(keysMutex_);

							auto it = activeKeys_.find(keyName);
							if (it != activeKeys_.end()) {
								// Убираем из map поток сам увидит и выйдет из цикла
								it->second.detach(); // отцепляем поток
								activeKeys_.erase(it);
							}
						}
					}
				}
			}
		}
		else
		{
			std::cout << "command не найден!" << std::endl;
		}
	}
	else
	{
		std::cout << "Полученные данные не являются JSON объектом." << std::endl;
	}
}

void JsonParse::pressKeyFunc(const std::string& keyName, WORD vkKey)
{
	//первоначальное нажатие
	systemController_.SendMediaKey(vkKey, true);

	//пауза потому что так надо (как в системе)
	std::this_thread::sleep_for(std::chrono::milliseconds(250));

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(70));

		std::lock_guard<std::mutex> lock(keysMutex_);
		// Если клавишу убрали из map то выходим
		if (activeKeys_.find(keyName) == activeKeys_.end()) {
			break;
		}
		// иначе продолжаем удерживать клавишу
		systemController_.SendMediaKey(vkKey, true);
	}

	// финальный отпуск
	systemController_.SendMediaKey(vkKey, false);
}

//получаем нужную клавишу из строки
WORD JsonParse::getVirtualKey(const std::string& keyName)
{
	static const std::unordered_map<std::string, WORD> specialKeys = {
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
				{"pause",     VK_PAUSE},
				{"scroll",    VK_SCROLL},
	};

	std::string lower = keyName;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

	auto it = specialKeys.find(lower);
	if (it != specialKeys.end()) {
		return it->second;
	}

	if (keyName.size() >= 1) {
		return VkKeyScanA(keyName[0]) & 0xFF;
	}
	
	//неизвестная клавиша
	return 0;
}
