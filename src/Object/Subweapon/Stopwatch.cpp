#include "Object/Subweapon/Stopwatch.hpp"

namespace Subweapon {
    Stopwatch::Stopwatch(glm::vec2 position, std::string type) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/stopwatch.png"}, 0, LootType::Stopwatch), m_type(type) {}

    void Stopwatch::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        menu->modifyWeapon(WeaponType::Stopwatch);
        character->SetSubWeaponType(WeaponType::Stopwatch);
        (void) app;
        is_endResult = true;
    }

    void Stopwatch::Use(const std::vector<std::shared_ptr<Block>>& m_Blocks) {
        
    }
}