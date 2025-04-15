#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Object/Enemy.hpp"

class Zombie : public Enemy {
public:
    Zombie(glm::vec2 position, std::string direction);
    void MoveBehav() override;
    void SetReset() override;
    void SetDirection(std::string direction);
private:
    std::vector<std::string> zomImages;
};

#endif