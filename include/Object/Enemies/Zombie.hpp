#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Object/Enemy.hpp"

class Zombie : public Enemy {
public:
    Zombie(glm::vec2 position, std::string direction);
    void MoveBehav() override;
    void SetReset() override;
private:
    std::vector<std::string> zomImages;
    glm::vec2 spawnRange;
};

class ZombieHorde {
public:
    ZombieHorde(glm::vec2 range, int numZombie, glm::vec2 pos, std::string direction);
    void SetSpawnRange(glm::vec2 range);
    glm::vec2 GetSpawnRange() const;
    int GetNumZombie() const;
    std::vector<std::shared_ptr<Zombie>> zombies;
private:
    glm::vec2 spawnRange;
    int numZombie;
    glm::vec2 pos;
    std::string direction;
};

#endif