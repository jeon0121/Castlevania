#ifndef SCENE_HPP
#define SCENE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class BackgroundImage : public Util::GameObject {
public:
    BackgroundImage(const std::string& path) : GameObject(std::make_unique<Util::Image>(path)) {}
};

class Scene {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void End() = 0;

protected:
    std::shared_ptr<BackgroundImage> m_Background;
    State m_currentState = State::START;
};

#endif

//Scene.hpp - start update end
//title - \ - stage2 - stage3
//start ptr 
// start game - AnimatedItems
// background - image

// ptr- memory 
// ~ptr 
