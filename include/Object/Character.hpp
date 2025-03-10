#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <iostream>
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Time.hpp"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Object/AnimatedItems.hpp"
#include "Object/ImageItems.hpp"
#include <Object/Block.hpp>

struct CharacterValue {
    glm::vec2 position;
    int beIndex;
    std::string direction = "left";
    float scale = 1.0f;
};

class Character : public Util::GameObject{
public:
    explicit Character(const CharacterValue& value);

    void ChangeBehavior(int BehaviorIndex, bool if_whip=false);

    void SetPosition(const glm::vec2& Position);

    const glm::vec2& GetPosition() const;

    void Keys();

    void Whip();

    void Jump();

    void Move(std::string direction);

    void Flip();

    void CollideBoundary(const std::vector<std::shared_ptr<Block>>& m_Blocks);

    std::shared_ptr<AnimatedItems> m_Behavior;

private:
    std::vector<std::vector<std::string>> behaviorVector;
    std::vector<std::vector<std::vector<std::string>>> whipVector;
    std::string m_direction = "left";
    int m_life = 16;
    int m_whip_level = 0;
    float x_vel = 0;
    float y_vel = 0;

    // some flag
    bool is_whip = false;
    bool is_duck = false;
    bool is_left = false;
    bool is_right = false;
};

#endif