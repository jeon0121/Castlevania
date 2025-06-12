#include "Object/Enemies/Zombie.hpp"

Zombie::Zombie(glm::vec2 position, std::string direction): Enemy(position, direction, {}, 250, 100) {
    for (int i = 0; i < 2; ++i) {
        zomImages.emplace_back(GA_RESOURCE_DIR"/enemies/zombie/zombie-" + std::to_string(i + 1) + ".png");
    }
    SetAnimationFrames(zomImages, 250);
    m_Transform.scale = glm::vec2(1, 0.9f);
    if (direction == "right")
        Flip();
    SetPlaying();
    SetLooping(true);
    countHurt = 2;
}

void Zombie::MoveBehav(std::vector<std::shared_ptr<Block>> &blocks) {
    glm::vec2 pos = GetPosition();
    glm::vec2 size = GetScaledSize();
    bool isOnGround = false;
    for (auto& block : blocks) {
        glm::vec2 blockPos = block->GetPosition();
        glm::vec2 blockSize = block->GetScaledSize();
        bool horizontallyAligned = pos.x > blockPos.x - blockSize.x * 0.5f
                                && pos.x < blockPos.x + blockSize.x * 0.5f;
        bool verticallyClose = std::abs((pos.y - size.y * 0.5) - (blockPos.y + blockSize.y * 0.5)) < 12.0f;
        if (horizontallyAligned && verticallyClose) {
            SetPosition({pos.x, blockPos.y + blockSize.y * 0.5f + size.y * 0.5f});
            isOnGround = true;
            break;
        }
    }
    pos = GetPosition();
    if (!isOnGround) {
        y_vel -= 0.5f;
        if (y_vel < -12.0f)
            y_vel = -12.0f;
        SetPosition({pos.x, pos.y+=y_vel});
    }
    else {
        y_vel = 0;
        SetPosition({((direction == "right") ? pos.x+=3 : pos.x-=3), pos.y});
    }
}

void Zombie::SetReset() {
    is_dead = false;
    is_hidden = false;
    hasEnteredWindow = false;
    SetAnimationFrames(zomImages, 250);
    SetPlaying();
    SetLooping(true);
}

ZombieHorde::ZombieHorde(glm::vec2 range, int numZombie, glm::vec2 pos, std::string direction) : 
    spawnRange(range), numZombie(numZombie), pos(pos), direction(direction) {
    for (int i = 0; i < numZombie; ++i) {
        float spawnX = (std::rand() % 60 + 120) * i;
        std::shared_ptr<Zombie> zombie = std::make_shared<Zombie>(glm::vec2(pos.x + spawnX, pos.y), direction);
        zombies.push_back(zombie);
    }
}

void ZombieHorde::SetSpawnRange(glm::vec2 range) {
    spawnRange = range;
}

glm::vec2 ZombieHorde::GetSpawnRange() const {
    return spawnRange;
}

int ZombieHorde::GetNumZombie() const {
    return numZombie;
}

glm::vec2 ZombieHorde::GetPosition() const {
    return pos;
}