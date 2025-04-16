#include "Object/Subweapon/Dagger.hpp"

namespace Subweapon {
    Dagger::Dagger(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/dagger.png"}, 0) {}

    void Dagger::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        menu->modifyWeapon(WeaponType::Dagger);
        character->SetSubWeaponType(WeaponType::Dagger);
        (void) app;
        is_endResult = true;
    }

    void Dagger::Use() {
        glm::vec2 pos = GetPosition();
        SetPosition({((GetDirection() == "right") ? pos.x+=15 : pos.x-=15), pos.y});
    }

    bool Dagger::IsDestroyed() {
        if (if_destroyed)
            return true;
        return false;
    }
}