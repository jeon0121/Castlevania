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
private:
    std::vector<std::string> idleImages;
    std::vector<std::string> walkImages;
    std::vector<std::string> jumpImages;

    std::string state = "idle";
};

#endif