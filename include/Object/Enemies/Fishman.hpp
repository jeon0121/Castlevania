#ifndef FISHMAN_HPP
#define FISHMAN_HPP

#include "Object/Enemy.hpp"

class Fishman : public Enemy {
public:
    Fishman(glm::vec2 position, std::string direction);
    void MoveBehav() override;
    void SetReset() override;
private:
    std::vector<std::string> idleImages;
    std::vector<std::string> walkImages;
    std::vector<std::string> shootImages;
    float y_vel = 0.0f;
};

#endif