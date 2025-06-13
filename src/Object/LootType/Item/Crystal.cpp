#include "Object/LootType/Item/Crystal.hpp"

namespace LootItem {
Crystal::Crystal(glm::vec2 position) : Loot(position, {}, {}, 0, LootType::Crystal) {
    SetAnimationFrames({GA_RESOURCE_DIR "/items/item/crystal-1.png", GA_RESOURCE_DIR "/items/item/crystal-2.png"}, 50);
    SetPlaying();
    SetLooping(true);
}


void Crystal::Fall(const std::vector<std::shared_ptr<Block>>& m_Blocks){
    glm::vec2 itemPos = UpdatePosition();
    glm::vec2 itemSize = glm::abs(GetScaledSize());
    SetPosition(itemPos + glm::vec2(x_vel, y_vel));
    y_vel = std::max(y_vel - ((y_vel >= -2.0f) ? 0.5f : 1.5f), -15.0f);

    if (!is_landed){
        for (auto &&block : m_Blocks) {
            glm::vec2 blockPos = block->GetPosition();
            glm::vec2 blockSize = glm::abs(block->GetScaledSize());
            float blockTop = blockPos.y + blockSize.y * 0.5f + 8.5f;
            float blockLeft = blockPos.x - blockSize.x * 0.5f;
            float blockRight = blockPos.x + blockSize.x * 0.5f;
            bool overlapX = itemRight > blockLeft && itemLeft < blockRight;
            bool isLanding = itemBottom <= blockTop && itemTop > blockTop;

            if (overlapX && isLanding) {
                is_landed = true;
                if (itemBottom < blockTop)
                SetPosition({itemPos.x, blockTop + itemSize.y * 0.2f});
                break;
            }
        }
    }
}


void Crystal::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
    (void) app;
    character->SetHeart(16);
    menu->modifyHealth(16, "player");
    is_endResult = true;
}
}