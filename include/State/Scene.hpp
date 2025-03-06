#ifndef SCENE_HPP
#define SCENE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Object/ImageItems.hpp"
#include "Object/Character.hpp"
#include "State/Menu.hpp"

class App;

class Scene {
public:
    enum class StateState {
        START,
        UPDATE,
        END,
    };
    virtual void Start(App* app) = 0;
    virtual void Update() = 0;
    virtual void End(App* app) = 0;
    StateState m_stateState = StateState::START;

protected:
    std::shared_ptr<ImageItems> m_Background;
    std::shared_ptr<Character> m_Character;
    std::shared_ptr<Menu> m_Menu;
};

#endif

//Scene.hpp - start update end
//title - \ - stage2 - stage3
//start ptr 
// start game - AnimatedItems
// background - image

// ptr- memory 
// ~ptr 
