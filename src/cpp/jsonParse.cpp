#include "../header/jsonParse.h"
#include"../header/session.h"


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
			else if (command == "lButton") {
				systemController_.SendMouseClick(MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
			}
			else if (command == "rButton") {
				systemController_.SendMouseClick(MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
			}
			else if (command == "Scroll") {
				int value = static_cast<int>(obj.at("value").as_double());
				systemController_.SendMouseWheel(value,1);
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
