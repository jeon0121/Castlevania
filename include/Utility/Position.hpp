#ifndef POSITION_HPP
#define POSITION_HPP

#include <iostream>
#include "Util/Input.hpp"

class Position {
public:
   /**
    * @brief Prints the current cursor position to the standard output.
    *
    * Useful for determining coordinates for placing objects (e.g., blocks, player, etc.).
    */
   void PrintCursorCoordinate();
};

#endif