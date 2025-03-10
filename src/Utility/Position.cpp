#include "Utility/Position.hpp"

void Position::PrintCursorCoordinate(){
   std::cout << Util::Input::GetCursorPosition().x << ", " << Util::Input::GetCursorPosition().y << std::endl;
}

void Position::LogCursorCoordinate(){
   std::ostringstream logMessage;
   logMessage << Util::Input::GetCursorPosition().x << ", " << Util::Input::GetCursorPosition().y;
   LOG_DEBUG(logMessage.str());
}