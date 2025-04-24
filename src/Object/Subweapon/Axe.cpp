#include "Object/Subweapon/Axe.hpp"

namespace Subweapon {
    Axe::Axe(glm::vec2 position, std::string type) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/axe.png"}, 0, LootType::Axe), m_type(type) {}

    void Axe::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        menu->modifyWeapon(WeaponType::Axe);
        character->SetSubWeaponType(WeaponType::Axe);
        (void) app;
        is_endResult = true;
    }

    void Axe::Use(const std::vector<std::shared_ptr<Block>>& m_Blocks) {

    }
}