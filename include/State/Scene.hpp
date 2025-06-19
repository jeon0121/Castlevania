#ifndef SCENE_HPP
#define SCENE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Object/ImageItems.hpp"
#include "Object/Character.hpp"
#include "Object/Block.hpp"
#include "Object/Stair.hpp"
#include "Object/HitableBlock.hpp"
#include "Object/Torch.hpp"
#include "Object/Loot.hpp"
#include "Object/LootType/Loot.hpp"
#include "Object/EnemiesManager.hpp"
#include "Object/ObjectData.hpp"
#include "App.hpp"

class Scene {
public:
    enum class StateState {
        START,
        UPDATE,
        END,
    };

    virtual void Start(App* app) = 0;
    virtual void Update(App* app) = 0;
    virtual void End(App* app) = 0;

    void UpdateTorch(App* app);

    void UpdateScroll(int mapWidth, float offset = 0.0f);

    void UpdateSubWeapon(App* app);

    void SetSubweapon(App* app);

    std::shared_ptr<EnemiesManager> GetEnemiesManager();

    void Blink();

    void UpdateHitableBlock(App* app);

    void EndAnimation(App* app, std::string imagePath, glm::vec2 scale, std::shared_ptr<AnimatedItems> door_1, std::shared_ptr<AnimatedItems> door_2, std::shared_ptr<Util::SFX> doorSound);

    void SceneReset(App* app);

    // for debug mode
    bool GetSwitchStageFlag() const;

    void SetSwitchStageFlag(bool ifSwitch);

    StateState m_stateState = StateState::START;
    int screenWidth = 1024;
    int screenHeight = 768;

protected:
    std::shared_ptr<ImageItems> m_Background;
    std::shared_ptr<AnimatedItems> m_Blink;
    std::shared_ptr<Character> m_Character;
    std::vector<std::shared_ptr<Util::GameObject>> m_All;
    std::vector<std::shared_ptr<Util::GameObject>> m_Loots;
    std::vector<std::shared_ptr<Block>> m_Blocks;
    std::vector<std::shared_ptr<HitableBlock>> m_HitableBlocks;
    std::vector<std::shared_ptr<Stair>> m_Stairs;
    std::vector<std::shared_ptr<Torch>> m_Torches;
    std::shared_ptr<Loot> asLoot;
    std::shared_ptr<Loot> specialBag;
    std::shared_ptr<EnemiesManager> m_EnemiesManager;
    std::shared_ptr<Enemy> m_Boss;
    glm::vec2 backgroundPos;
    float offsetX = 0.0f;

    Uint64 blinkStartTime = 0;

    bool whipDropped = false; // to ensure that dropped whip dont exceed 1
    bool reachBoss = false;
    bool isTimeOut = false;
    bool switchStage = false;
};

#endif