#ifndef CASTLEVANIA_UTIL_POSITION_HPP
#define CASTLEVANIA_UTIL_POSITION_HPP

#include <sstream>
#include <iostream>
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

class Position {
public:
   /**
    * @brief Prints the current cursor position to the standard output.
    *
    * Useful for determining coordinates for placing objects (e.g., blocks, player, etc.).
    */
   static void PrintCursorCoordinate();

   /**
    * @brief Logs the current cursor position to the log console.
    *
    * Useful for determining coordinates for placing objects (e.g., blocks, player, etc.).
    */
   static void LogCursorCoordinate();

   template <typename T>
   /**
    * @brief Used to print object coordinate with offset
    * 
    * (Make sure that the object have GetPosition() method)
    */
   static void PrintObjectCoordinate(const std::shared_ptr<T>& object, int offsetX) {
      if (object) {
         std::cout << object->GetPosition().x + offsetX << ", " << object->GetPosition().y << std::endl;
      }
   }
};

#endif