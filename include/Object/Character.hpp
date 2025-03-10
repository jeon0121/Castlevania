#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Object/AnimatedItems.hpp"
#include "Object/ImageItems.hpp"
#include <Object/Block.hpp>
#include <iostream>

class Character : public Util::GameObject{
public:
    explicit Character(const glm::vec2 &position, int beIndex, float scale=1.0);

    void ChangeBehavior(int BehaviorIndex);

    void SetPosition(const glm::vec2& Position);

    const glm::vec2& GetPosition() const;

    void Keys();

    void Move(std::string direction);

    void Flip();

    void CollideBoundary(const std::vector<std::shared_ptr<Block>>& m_Blocks);

    std::shared_ptr<AnimatedItems> m_Behavior;

    std::string m_direction;

private:
    std::vector<std::vector<std::string>> behaviorVector;
};

#endif