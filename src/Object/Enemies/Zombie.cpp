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

void Zombie::MoveBehav() {
    glm::vec2 pos = GetPosition();
    SetPosition({((direction == "right") ? pos.x+=3 : pos.x-=3), pos.y});
}

void Zombie::SetReset() {
    is_dead = false;
    is_hidden = false;
    hasEnteredWindow = false;
    SetAnimationFrames(zomImages, 250);
    SetPlaying();
    SetLooping(true);
}

void Zombie::SetDirection(std::string direction) {
    if (direction != this->direction)
        Flip();
    this->direction = direction;
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