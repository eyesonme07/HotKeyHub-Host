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
					SetCursorPos(cursorPos.x - x, cursorPos.y - y);
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
			else if(command == "cButton"){
				systemController_.SendMouseClick(MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP);
			}
			/////////////////////////////////////////////////////////////////////////////////////
			else if (command == "Scroll") {
				int value = static_cast<int>(obj.at("value").as_double());
				systemController_.SendMouseWheel(value,1);
			}
			/////////////////////////////////////////////////////////////////////////////////////
			//нажатие не клавиши
			else if (command == "clickKey") {
				if (obj.contains("keys")) {
					int keysCount = std::stoi(obj.at("keys").as_string().c_str());

					//меняем под размер клавиш
					keys.resize(keysCount);

					for (int i = 0; i < keysCount; ++i) {
						std::string keyName = obj.at(std::to_string(i)).as_string().c_str();
						
						//получаем код клавиши, если это спец клавиша то отправляем ее, если нет то отправляем символ
						WORD vk = getVirtualKey(keyName);
						if (vk == 0) {
							wchar_t ch = utf8ToWchar(keyName);//переводим utf-8 в wchar_t
							systemController_.SendUnicodeChar(ch, true);
							keys[i] = ch;
						}
						else {
							systemController_.SendMediaKey(vk, true);
							keys[i] = vk;
						}
						
					}
					//отпускание клавиш
					for (auto key : keys) {
						if (std::holds_alternative<WORD>(key)) {
							systemController_.SendMediaKey(std::get<WORD>(key), false);
						} else {
							systemController_.SendUnicodeChar(std::get<wchar_t>(key), false);
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

//получаем нужную клавишу из строки
WORD JsonParse::getVirtualKey(const std::string& keyName)
{
	std::string lower = keyName;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

	auto it = specialKeys.find(lower);
	if (it != specialKeys.end()) {
		return it->second;
	}
	
	//неизвестная клавиша
	return 0;
}
