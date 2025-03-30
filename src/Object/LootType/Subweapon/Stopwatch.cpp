#include "Object/LootType/Subweapon/Stopwatch.hpp"

namespace LootItem {
    Stopwatch::Stopwatch(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/stopwatch.png"}, 0) {}

    void Stopwatch::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        menu->modifyWeapon(WeaponType::Stopwatch);
        character->SetSubWeaponType(WeaponType::Stopwatch);
        (void) app;
        is_endResult = true;
    }

    void Stopwatch::Use(std::string direction) {
        
    }
}