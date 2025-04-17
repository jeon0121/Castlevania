#include "Object/Enemies/Leopard.hpp"

Leopard::Leopard(glm::vec2 position, std::string direction) : Enemy(position, direction, {}, 300, 200), initialPos(position) {
    idleImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-1.png");
    jumpImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-2.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-3.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/leopard/leopard-4.png");
    SetAnimationFrames(idleImages, 0);
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    if (direction == "right")
        Flip();
}

void Leopard::MoveBehav(std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks) {
    SetPlaying();
    SetLooping(true);
    glm::vec2 pos = GetPosition();
    glm::vec2 size = GetScaledSize();
    // if character near leopard 3 blocks, leopard will move to character
    if (((character->GetPosition().x > pos.x - size.x - 3 * character->GetSize().x && character->GetPosition().x < pos.x) ||
         (character->GetPosition().x < pos.x - size.x + 3 * character->GetSize().x && character->GetPosition().x > pos.x)) &&
          state == "idle")
        state = "walk";
    if (state == "idle") return;

    bool isOnGround = false;
    for (auto& block : blocks) {
        glm::vec2 blockPos = block->GetPosition();
        glm::vec2 blockSize = block->GetScaledSize();
        bool horizontallyAligned = pos.x > blockPos.x - blockSize.x * 0.5f
                                && pos.x < blockPos.x + blockSize.x * 0.5f;
        bool verticallyClose = std::abs((pos.y - size.y * 0.5) - (blockPos.y + blockSize.y * 0.5)) < 5.0f;
        if (horizontallyAligned && verticallyClose) {
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
        y_vel = 0.0f;
    }
    if (is_jump) 
        Jump();
    else if (is_walk)
        Walk();
}

void Leopard::SetReset() {
    state = "idle";
    y_vel = 0.0f;
    is_dead = false;
    is_hidden = false;
    is_walk = false;
    is_jump = false;
    hasEnteredWindow = false;
    SetAnimationFrames(idleImages, 300);
    SetVisible(true);
}

void Leopard::SetDirection(std::string direction) {
    if (direction != this->direction)
        Flip();
    this->direction = direction;
}

void Leopard::Walk() {
    glm::vec2 pos = GetPosition();
    if (direction == "right") {
        SetPosition({pos.x + 7.5f, pos.y});
    } else {
        SetPosition({pos.x - 7.5f, pos.y});
    }
}

void Leopard::Jump() {
    glm::vec2 pos = GetPosition();
    if (direction == "right") {
        SetPosition({pos.x + 7.5f, pos.y + y_vel});
    } else {
        SetPosition({pos.x - 7.5f, pos.y + y_vel});
    }
    if (y_vel < -10.0f) 
        y_vel = -10.0f;
    else
        y_vel -= 0.5f;
}

glm::vec2 Leopard::GetInitialPos() {
    return initialPos;
}

void Leopard::InWindowDetection(int screenWidth) {
    if (!is_dead) {
        glm::vec2 pos = GetPosition();
        bool outOfWindow = (pos.x < screenWidth * -0.5 || pos.x > screenWidth * 0.5) && state != "idle";
        if (is_hidden) {
            SetVisible(false);
            return;
        }
        if (outOfWindow) {
            if (hasEnteredWindow) {
                is_hidden = true;
                SetVisible(false);
            } else {
                SetVisible(true);
            }
        } else {
            hasEnteredWindow = true;
            SetVisible(true);
        }
    }
}