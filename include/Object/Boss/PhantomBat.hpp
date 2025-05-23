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
    void MoveBehav(std::shared_ptr<Character> &character) override;
    void SetReset() override {}
    void Fly(std::shared_ptr<Character> &character);
    void Dive(std::shared_ptr<Character> &character);
    void SetDivePosition(std::shared_ptr<Character> &character);
private:
    std::string m_state;
    std::vector<std::string> idleImages;
    std::vector<std::string> flyImages;
    std::vector<std::string> deathImages;

    Uint64 idleTime = 0;
};

#endif