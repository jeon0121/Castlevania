#include "Object/Subweapon/Dagger.hpp"

namespace Subweapon {
    Dagger::Dagger(glm::vec2 position, std::string type) : Loot(position, {GA_RESOURCE_DIR "/Sound Effects/24.wav"}, {GA_RESOURCE_DIR "/items/weapon/dagger.png"}, 0, LootType::Dagger), m_type(type) {
        usingSound = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/09.wav");
        usingSound->SetVolume(50);
    }

    void Dagger::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        menu->modifyWeapon(WeaponType::Dagger);
        character->SetSubWeaponType(WeaponType::Dagger);
        (void) app;
        is_endResult = true;
    }

    void Dagger::Use(const std::vector<std::shared_ptr<Block>>& m_Blocks) {
        (void) m_Blocks;
        if (!is_playSound) {
            usingSound->Play();
            is_playSound = true;
        }
        glm::vec2 pos = GetPosition();
        SetPosition({((GetDirection() == "right") ? pos.x+=15 : pos.x-=15), pos.y});
        if (if_collide)
            if_destroyed = true;
    }
}