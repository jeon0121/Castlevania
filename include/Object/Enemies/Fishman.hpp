#ifndef FISHMAN_HPP
#define FISHMAN_HPP

#include "Object/Enemy.hpp"

class Fishman : public Enemy {
public:
    Fishman(glm::vec2 position, std::string direction, App* app);
    void MoveBehav(std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks) override;
    void SetReset() override;
    void Spawn(std::shared_ptr<Character> &character);
    void Bubble(glm::vec2 fishpos);
    void Walk(std::shared_ptr<Character> &character);
    void CollideBlock(std::vector<std::shared_ptr<Block>> &blocks);
    bool GetFireFlag();
    bool CheckReset() override;
private:
    std::vector<std::string> idleImages;
    std::vector<std::string> walkImages;
    std::vector<std::string> shootImages;
    std::vector<std::shared_ptr<ImageItems>> bubbles;
    bool isFire = false;
    bool isBubble = false;
    bool isOnBlock = false;
    float y_vel = 27.5f;
    std::vector<float> bbl_vel = {0, 12.0f, 0};
    Uint64 startWalkTime = 0;
    std::string state = "Spawn";
};

#endif