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
#include "State/Menu.hpp"
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

    void UpdateScroll(int mapWidth);

    StateState m_stateState = StateState::START;

protected:
    std::shared_ptr<ImageItems> m_Background;
    std::shared_ptr<Character> m_Character;
    std::shared_ptr<Menu> m_Menu;
    std::vector<std::shared_ptr<Util::GameObject>> m_All;
    std::vector<std::shared_ptr<Block>> m_Blocks;
    std::vector<std::shared_ptr<Torch>> m_Torches;
    float offsetX = 0.0f;
    int screenWidth = 1024;
};

#endif

//Scene.hpp - start update end
//title - \ - stage2 - stage3
//start ptr 
// start game - AnimatedItems
// background - image

// ptr- memory 
// ~ptr 
