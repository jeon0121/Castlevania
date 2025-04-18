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
#include "Object/Subweapon/IUseSubweapon.hpp"
#include "Object/ImageItems.hpp"
#include "Object/Block.hpp"
#include "Object/Stair.hpp"

struct CharacterValue {
    glm::vec2 position;
    int beIndex;
    std::string direction = "left";
    float scale = 1.0f;
};

enum class WeaponType {
    None,
    Axe,
    HolyWater,
    Dagger,
    Stopwatch
};

class Character : public Util::GameObject{
public:
    explicit Character(const CharacterValue& value);

    void SetPosition(const glm::vec2& Position);
    void SetDirection(const std::string& Direction);
    void SetSubWeaponType(WeaponType type);
    void SetUseWeaponFlag(bool ifuse);
    void SetHurtFlag(bool ifhurt, bool ifNeedSlip);
    const WeaponType& GetSubWeaponType() const;
    const bool& GetUseWeaponFlag() const;
    bool GetHurtFlag();
    bool GetEndSceneFlag() const;
    const glm::vec2& GetPosition() const;
    const glm::vec2& GetLastPosition() const;
    const glm::vec2& GetSize() const;
    std::string GetDirection() const;
    bool IfWhip() const;
    int GetWhipLevel() const;

    void SetAmmo(int ammo);
    int GetAmmo() const;
    void SetHeart(int heart);
    int GetHeart() const;
    void SetLevelUpWhip(bool ifLevelUp); // check if it's still at level-up animation
    bool GetLevelUpWhipFlag() const;
    void LevelUpWhip();

    float OffsetValues(std::string typeName);
    void ChangeBehavior(int BehaviorIndex, bool if_whip = false);

    void UpdatePosition();
    void Keys(const std::vector<std::shared_ptr<Block>>& m_Blocks, const std::vector<std::shared_ptr<Stair>>& m_Stairs);

    void HandleFallDuck(const std::string& direction);
    void Ascending(std::shared_ptr<Stair>& stair);
    void Descending(std::shared_ptr<Stair>& stair);
    void SubWeapon();
    void Whip();
    void Duck(std::string direction);
    void Jump();
    void Fall();
    void Move(std::string direction);
    void Idle();
    void Flip();
    void Hurt();
    void Dead();

    void CollideBoundary(const std::vector<std::shared_ptr<Block>>& m_Blocks);
    std::shared_ptr<Stair> CollideStair(const std::vector<std::shared_ptr<Stair>>& m_Stairs);

    std::shared_ptr<AnimatedItems> m_Behavior;

    std::shared_ptr<Subweapon::IUseSubweapon> m_SubWeapon;
    
private:
    std::vector<std::vector<std::string>> behaviorVector;
    std::vector<std::vector<std::vector<std::string>>> whipVector;
    std::string m_direction = "left";
    std::string lastVecPos = "be";

    glm::vec2 lastPos;
    glm::vec2 m_pos;  // Actual character position (ignores whip size)
    glm::vec2 m_size; // Character size without whip
    glm::vec2 nextStairPos;
    glm::vec2 is_collide = {0, 0};

    std::shared_ptr<Stair> currentStair;
    WeaponType m_subweapon = WeaponType::None;
    int m_life = 16;
    int m_ammo = 5;
    int m_whip_level = 1;
    float x_vel = 0;
    float y_vel = 0;
    float jumph = 0;
    float landh = 0;
    float height = 0;
    float prevLandPosition;
    float landPosition;
    Uint64 startHurtTime = 0;
    Uint64 startDuckTime = 0;
    Uint64 startDeadTime = 0;

    int currentBeIndex = 2;
    int currentFrame = -1;
    int jumptype = 0;
    int prevStairState = -1; // -1: none, 0: ascending, 1: descending
    
    // some flag
    bool is_whip = false;
    bool is_duck = false;
    bool is_jump = false;
    bool is_subweapon = false; //if character animation finish
    bool is_useweapon = false; //if subweapon finish attack
    bool is_ascending = false;
    bool is_descending = false;
    bool is_onStair = false;
    bool is_levelUpWhip = false;
    bool change_land = false;
    bool is_hurt = false;
    bool is_dead = false;
    bool ifNeedSlip = false;//if char need slip when collide with enemy
};

#endif