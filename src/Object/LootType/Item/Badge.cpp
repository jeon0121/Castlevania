#include "Object/LootType/Item/Badge.hpp"
#include "Utility/Time.hpp"
#include "State/Menu.hpp"
#include "App.hpp"

namespace LootItem {
Badge::Badge(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/Sound Effects/24.wav"}, {GA_RESOURCE_DIR"/items/item/badge.png"}, 0, LootType::Badge) {}

void Badge::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
    (void) app;
    (void) character;
    if (startTime == 0) {
        startTime = SDL_GetPerformanceCounter();
    } else if (Time::GetRunTimeMs(startTime) > 1000.0f) {
        menu->modifyBadge(true);
        is_endResult = true;
    } else {
        int timeCount = static_cast<int>(Time::GetRunTimeMs(startTime));
        menu->modifyBadge((timeCount / 100) % 2 == 0);
    }
}
}