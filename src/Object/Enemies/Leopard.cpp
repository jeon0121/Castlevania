#include "Object/Enemies/Leopard.hpp"

Leopard::Leopard(glm::vec2 position, std::string direction) : Enemy(position, direction, {}, 300) {
    idleImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-1.png");
    jumpImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-2.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-3.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-4.png");
    SetAnimationFrames(idleImages, 0);
    m_Transform.scale = glm::vec2(1.2f, 1.0f);
    if (direction == "right")
        Flip();
    SetPlaying();
    SetLooping(true);
}

void Leopard::MoveBehav(std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks) {
    glm::vec2 pos = GetPosition();
    // if character near leopard 3 blocks, leopard will move to character
    if (character->GetPosition().x > pos.x - 3 * character->GetSize().x)
        state = "walk";
    for (auto &block : blocks) {
        if (block->GetPosition().x - block->GetScaledSize().x * 0.5f < pos.x && pos.x < block->GetPosition().x + block->GetScaledSize().x * 0.5f) {
            if ((pos.x < block->GetPosition().x - block->GetScaledSize().x * 0.5f + 10) ||
                (pos.x > block->GetPosition().x + block->GetScaledSize().x * 0.5f - 10))
                state = "jump";
                break;
        }
    }
}

void Leopard::SetReset() {
    is_dead = false;
    is_hidden = false;
    hasEnteredWindow = false;
    SetAnimationFrames(idleImages, 300);
    SetPlaying();
    SetLooping(true);
}

void Leopard::SetDirection(std::string direction) {
    if (direction != this->direction)
        Flip();
    this->direction = direction;
}