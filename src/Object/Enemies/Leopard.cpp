#include "Object/Enemies/Leopard.hpp"

Leopard::Leopard(glm::vec2 position, std::string direction) : Enemy(position, direction, {}, 300), initialPos(position) {
    idleImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-1.png");
    jumpImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-2.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-3.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-4.png");
    SetAnimationFrames(idleImages, 0);
    m_Transform.scale = glm::vec2(1.2f, 1.0f);
    if (direction == "right")
        Flip();
}

void Leopard::MoveBehav(std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks) {
    SetPlaying();
    SetLooping(true);
    glm::vec2 pos = GetPosition();
    glm::vec2 size = GetScaledSize();
    // if character near leopard 3 blocks, leopard will move to character
    if (character->GetPosition().x > pos.x - size.x - 3 * character->GetSize().x && state == "idle")
        state = "walk";
    if (state == "idle") return;

    bool isOnGround = false;
    for (auto& block : blocks) {
        glm::vec2 blockPos = block->GetPosition();
        glm::vec2 blockSize = block->GetScaledSize();
        bool horizontallyAligned = pos.x + size.x * 0.5f > blockPos.x && pos.x - size.x * 0.5f < blockPos.x + blockSize.x;
        bool verticallyClose = ((pos.y - size.y * 0.5) - (blockPos.y + blockSize.y * 0.5)) < 5.0f;
        if (horizontallyAligned && verticallyClose) { 
            SetPosition({pos.x, blockPos.y + blockSize.y * 0.5f + size.y * 0.5f});
            isOnGround = true;
            break;
        }
    }
    if (!isOnGround) 
        state = "jump";
    else 
        state = "walk";
    if (!is_jump && state == "jump") {
        y_vel = 7.5f;
        SetAnimationFrames(jumpImages, 0);
        is_jump = true;
        is_walk = false;
    } else if (!is_walk && state == "walk") {
        if (character->GetPosition().x > pos.x) 
            SetDirection("right");
        else 
            SetDirection("left");
        SetAnimationFrames(walkImages, 100);
        is_jump = false;
        is_walk = true;
    }
    if (is_jump) 
        Jump();
    else if (is_walk)
        Walk();
}

void Leopard::SetReset() {
    is_dead = false;
    is_hidden = false;
    hasEnteredWindow = false;
    SetAnimationFrames(idleImages, 300);
    SetPosition(initialPos);
}

void Leopard::SetDirection(std::string direction) {
    if (direction != this->direction)
        Flip();
    this->direction = direction;
}

void Leopard::Walk() {
    glm::vec2 pos = GetPosition();
    if (direction == "right") {
        SetPosition({pos.x + 5.0f, pos.y});
    } else {
        SetPosition({pos.x - 5.0f, pos.y});
    }
}

void Leopard::Jump() {
    glm::vec2 pos = GetPosition();
    if (direction == "right") {
        SetPosition({pos.x + 7.5f, pos.y + y_vel});
    } else {
        SetPosition({pos.x - 7.5f, pos.y + y_vel});
    }
    y_vel -= 0.5f;
}

glm::vec2 Leopard::GetInitialPos() {
    return initialPos;
}