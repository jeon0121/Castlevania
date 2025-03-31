#include "Object/Subweapon/HolyWater.hpp"

namespace Subweapon {
    HolyWater::HolyWater(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/holywater.png"}, 0) {}

    void HolyWater::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        menu->modifyWeapon(WeaponType::HolyWater);
        character->SetSubWeaponType(WeaponType::HolyWater);
        (void) app;
        is_endResult = true;
    }

    void HolyWater::Use() {
        
    }
}