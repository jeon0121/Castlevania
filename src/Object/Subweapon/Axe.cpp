#include "Object/Subweapon/Axe.hpp"

namespace Subweapon {
    Axe::Axe(glm::vec2 position, std::string type) : Loot(position, {GA_RESOURCE_DIR "/Sound Effects/24.wav"}, {GA_RESOURCE_DIR "/items/weapon/axe.png"}, 0, LootType::Axe), m_type(type) {
        if (m_type == "subweapon")
            y_vel = 20.0f;
    }

    void Axe::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        menu->modifyWeapon(WeaponType::Axe);
        character->SetSubWeaponType(WeaponType::Axe);
        (void) app;
        is_endResult = true;
    }

    void Axe::Use(const std::vector<std::shared_ptr<Block>>& m_Blocks) {
        (void) m_Blocks;
        glm::vec2 pos = UpdatePosition();
        SetPosition(pos + glm::vec2(x_vel, y_vel));
        x_vel = m_direction == "left" ? -8
              : m_direction == "right" ? 8 : 0;
        static int delayCount = 0;
        delayCount++;
        if (delayCount >= 5) {
            m_Transform.rotation += 90;
            delayCount = 0;
            if (m_Transform.rotation >= 360)
                m_Transform.rotation = 0;
        }
        y_vel = std::max(y_vel - ((-2.0f <= y_vel && y_vel <= 2.0f) ? 0.3f : 1.0f), -15.0f);
        if (pos.y < -400.0f) 
            if_destroyed = true;
    }
}