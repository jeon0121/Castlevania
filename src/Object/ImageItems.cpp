#include "Object/ImageItems.hpp"

std::string ImageItems::ConvertImageIfNeeded(const std::string& path){
    SDL_Surface* loaded = IMG_Load(path.c_str());
    if (!loaded) { // fallback or record error
        return path;
    }
    if (loaded->format->format == SDL_PIXELFORMAT_INDEX8) {
        SDL_Surface* converted = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(loaded);

        std::string tempPath = "/tmp/converted_image.png";
        IMG_SavePNG(converted, tempPath.c_str());
        SDL_FreeSurface(converted);
        return tempPath;
    }
    SDL_FreeSurface(loaded);
    return path;
}