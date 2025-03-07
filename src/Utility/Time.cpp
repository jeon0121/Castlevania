#include "Utility/Time.hpp"
#include <SDL.h>

Util::ms_t Time::GetRunTimeMs(Util::sdl_count_t s_Initial) {
   return static_cast<float>(SDL_GetPerformanceCounter() - s_Initial)/
          static_cast<float>(SDL_GetPerformanceFrequency()) * 1000.0F;
}
