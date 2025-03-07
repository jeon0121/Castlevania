#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Object/AnimatedItems.hpp"
#include "Object/ImageItems.hpp"

class Character : public Util::GameObject{
public:
    explicit Character(const glm::vec2 &position);

    void LoadBehavior(int imIndex, int beIndex);

    void UpdatePosition();

    void SetPosition(const glm::vec2& Position);

    const glm::vec2& GetPosition() const;

    std::shared_ptr<AnimatedItems> m_Behavior;

    std::shared_ptr<ImageItems> m_Image;

private:
    glm::vec2 pos;
};

#endif