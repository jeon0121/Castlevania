#include "Utility/Position.hpp"

void Position::PrintCursorCoordinate(){
   std::cout << Util::Input::GetCursorPosition().x << ',' << Util::Input::GetCursorPosition().y << std::endl;
}