#include "Object/Subweapon/HolyWater.hpp"

namespace Subweapon {
    HolyWater::HolyWater(glm::vec2 position, std::string type) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/holywater.png"}, 0, LootType::HolyWater), m_type(type) {
        m_throw.emplace_back(GA_RESOURCE_DIR "/main character/subweapon/holy-water/holy-water.png");
        for (int i = 0; i < 10; i++) 
            m_flame.emplace_back(GA_RESOURCE_DIR "/main character/subweapon/fire-bomb/fire-bomb-" + std::to_string(i % 5 + 1) + ".png");
        if (m_type == "subweapon") {
            is_landed = false;
            SetAnimationFrames(m_throw, 0);
            y_vel = 5;
        }
    }

    void HolyWater::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        menu->modifyWeapon(WeaponType::HolyWater);
        character->SetSubWeaponType(WeaponType::HolyWater);
        (void) app;
        is_endResult = true;
    }

    void HolyWater::Use(const std::vector<std::shared_ptr<Block>>& m_Blocks) {
        glm::vec2 pos = UpdatePosition();
        SetPosition(pos + glm::vec2(x_vel, y_vel));
        x_vel = m_direction == "left" ? -6
              : m_direction == "right" ? 6 : 0;
        y_vel = std::max(y_vel - ((y_vel >= -2.0f) ? 0.5f : 1.5f), -15.0f);
        
        if (!is_landed){
            for (auto &&block : m_Blocks) {
                glm::vec2 blockPos = block->GetPosition();
                glm::vec2 blockSize = glm::abs(block->GetScaledSize());
                float blockTop = blockPos.y + blockSize.y * 0.5f + 8.5f;
                float blockLeft = blockPos.x - blockSize.x * 0.5f;
                float blockRight = blockPos.x + blockSize.x * 0.5f;
                bool overlapX = itemRight > blockLeft && itemLeft < blockRight;
                bool isLanding = itemBottom <= blockTop;
                if (overlapX && isLanding) {
                    is_landed = true;
                    SetAnimationFrames(m_flame, 100);
                    break;
                }
            }
        }
        else {
            SetPlaying();
            if (IfAnimationEnds())
                if_destroyed = true;
            x_vel = y_vel = 0.0f;
        }
    }
}