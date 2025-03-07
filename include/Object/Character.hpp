#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Object/AnimatedItems.hpp"
#include "Object/ImageItems.hpp"

class Character : public Util::GameObject{
public:
    explicit Character(const glm::vec2 &position, int beIndex, float scale=1.0);

    void ChangeBehavior(int BehaviorIndex, std::vector<std::shared_ptr<Util::GameObject>>* m_All);

    void SetPosition(const glm::vec2& Position);

    const glm::vec2& GetPosition() const;

    void Flip();

    std::shared_ptr<AnimatedItems> m_Behavior;

    std::vector<std::vector<std::string>> behaviorVector;
};

#endif