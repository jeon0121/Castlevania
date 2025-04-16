#ifndef LEOPARD_HPP
#define LEOPARD_HPP

#include "Object/Enemy.hpp"
#include "Object/Character.hpp"
#include "Object/Block.hpp"

class Leopard : public Enemy {
public:
    Leopard(glm::vec2 position, std::string direction);
    void MoveBehav(std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks) override;
    void SetReset() override;
    void SetDirection(std::string direction);
    void Walk();
    void Jump();
    glm::vec2 GetInitialPos();
    void InWindowDetection(int screenWidth) override;
private:
    std::vector<std::string> idleImages;
    std::vector<std::string> walkImages;
    std::vector<std::string> jumpImages;
    glm::vec2 initialPos;

    std::string state = "idle";
    bool is_walk = false;
    bool is_jump = false;

    float y_vel = 0.0f;
};

#endif