#include "Object/LootType/Subweapon/Axe.hpp"

namespace LootItem {
    Axe::Axe(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/axe.png"}, 0) {}

    void Axe::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        menu->modifyWeapon(WeaponType::Axe);
        character->SetSubWeaponType(WeaponType::Axe);
        (void) app;
        is_endResult = true;
    }

    void Axe::Use(std::string direction) {

    }
}