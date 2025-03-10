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
   void PrintCursorCoordinate();

   /**
    * @brief Logs the current cursor position to the log console.
    *
    * Useful for determining coordinates for placing objects (e.g., blocks, player, etc.).
    */
   void LogCursorCoordinate();
};

#endif