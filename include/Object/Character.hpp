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
#include "Object/Block.hpp"

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

    const glm::vec2& GetLastPosition() const;

    const glm::vec2& GetPosition() const;

    const glm::vec2& GetSize() const;

    void UpdatePosition();

    void Keys();

    float OffsetValues(std::string typeName);

    void HandleFallDuck(const std::string& direction);

    void SubWeapon();

    void Whip();

    void Duck(std::string direction);

    void Jump();

    void Fall();

    void Move(std::string direction);

    void Idle();

    void Flip();

    void CollideBoundary(const std::vector<std::shared_ptr<Block>>& m_Blocks);

    bool IfWhip() const;

    int GetWhipLevel() const;

    void LevelUpWhip();

    std::string GetDirection() const;

    void SetLevelUpWhip(bool ifLevelUp); // check if it's still at level-up animation

    std::shared_ptr<AnimatedItems> m_Behavior;

private:
    std::vector<std::vector<std::string>> behaviorVector;
    std::vector<std::vector<std::vector<std::string>>> whipVector;
    std::string m_direction = "left";
    std::string lastVecPos = "be";

    glm::vec2 lastPos;
    glm::vec2 m_pos;  // Actual character position (ignores whip size)
    glm::vec2 m_size; // Character size without whip
    glm::vec2 is_collide = {0, 0};

    int m_life = 16;
    int m_whip_level = 1;
    float x_vel = 0;
    float y_vel = 0;
    float jumph = 0;
    float landh = 0;
    float height = 0;
    float prevLandPosition = -299.35;
    float landPosition = -299.35;

    int currentBeIndex = 2;
    int currentFrame = -1;
    int jumptype = 0;
    int countTime = 0;
    
    // some flag
    bool is_whip = false;
    bool is_duck = false;
    bool is_jump = false;
    bool is_subweapon = false;
    bool change_land = false;
    bool is_levelUpWhip = false;
};

#endif