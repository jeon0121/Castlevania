#include "Object/Enemies/Fishman.hpp"

Fishman::Fishman(glm::vec2 position, std::string direction): Enemy(position, direction, {}, 250, 100) {
    idleImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-1.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-1.png");
    walkImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-2.png");
    shootImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-3.png");
    shootImages.emplace_back(GA_RESOURCE_DIR"/enemies/fishman/fishman-4.png");
    SetAnimationFrames(idleImages, 0);
    m_Transform.scale = glm::vec2(1, 0.9f);
    if (direction == "right")
        Flip();
    SetPlaying();
    SetLooping(true);
    countHurt = 2;
}

void Fishman::MoveBehav() {
    
}

void Fishman::SetReset() {
    is_dead = false;
    is_hidden = false;
    hasEnteredWindow = false;
    SetAnimationFrames(idleImages, 0);
    SetPlaying();
    SetLooping(true);
}