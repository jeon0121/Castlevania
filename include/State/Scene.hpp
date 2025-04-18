#ifndef SCENE_HPP
#define SCENE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Object/ImageItems.hpp"
#include "Object/Character.hpp"
#include "Object/Block.hpp"
#include "Object/Torch.hpp"
#include "Object/Loot.hpp"
#include "Object/LootType/Loot.hpp"
#include "Object/EnemiesManager.hpp"
#include "App.hpp"

class Scene {
public:
    enum class StateState {
        START,
        UPDATE,
        END,
    };
    struct TorchData {
        glm::vec2 pos;
        glm::vec2 scale;
        LootType loot;
        int type;
    };
    struct BlockData {
        glm::vec2 pos;
        glm::vec2 scale;
    };
    struct StairData {
        glm::vec2 pos1;
        glm::vec2 pos2;
    };

    virtual void Start(App* app) = 0;
    virtual void Update(App* app) = 0;
    virtual void End(App* app) = 0;

    void UpdateTorch(App* app);

    void UpdateScroll(int mapWidth);

    void UpdateSubWeapon(App* app);

    void SetSubweapon(App* app);

    StateState m_stateState = StateState::START;
    int screenWidth = 1024;
    int screenHeight = 768;

protected:
    std::shared_ptr<ImageItems> m_Background;
    std::shared_ptr<Character> m_Character;
    std::vector<std::shared_ptr<Util::GameObject>> m_All;
    std::vector<std::shared_ptr<Block>> m_Blocks;
    std::vector<std::shared_ptr<Stair>> m_Stairs;
    std::vector<std::shared_ptr<Torch>> m_Torches;
    std::shared_ptr<Loot> asLoot;
    std::shared_ptr<EnemiesManager> m_EnemiesManager;
    float offsetX = 0.0f;
    glm::vec2 backgroundPos;
};

#endif

//Scene.hpp - start update end
//title - \ - stage2 - stage3
//start ptr 
// start game - AnimatedItems
// background - image

// ptr- memory 
// ~ptr 
