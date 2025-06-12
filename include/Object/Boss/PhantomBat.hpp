#ifndef PHANTOM_BAT_HPP
#define PHANTOM_BAT_HPP

#include "Object/AnimatedItems.hpp"
#include "Object/Character.hpp"
#include "Object/Block.hpp"
#include "Object/Enemy.hpp"

// boss
class PhantomBat : public Enemy {
public:
    PhantomBat(const glm::vec2& position);
    void MoveBehav(std::shared_ptr<Character> &character, int screenHeight, int screeenWidth) override;
    void SetReset() override {}
    void Fly(std::shared_ptr<Character> &character, int screenHeight, int screenWidth);
    void Dive(int screenWidth);
    void SetDivePosition(std::shared_ptr<Character> &character);
    bool CollideDetection(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu, int modeState) override;
private:
    std::string m_state;
    std::vector<std::string> idleImages;
    std::vector<std::string> flyImages;
    std::vector<std::string> deathImages;

    glm::vec2 vel;
    glm::vec2 pos; // current position
    glm::vec2 flyPos;
    glm::vec2 divePos;
    Uint64 idleTime = 0;
    Uint64 flyTime = 0;
    int health = 16;
    double yDistance = 0.0f;
    double tDive = 0.0f;
    bool hitWall = false;
};

#endif