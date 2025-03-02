#ifndef SCENE_HPP
#define SCENE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Object/ImageItems.hpp"

class App;

// class BackgroundImage : public Util::GameObject {
// public:
//     BackgroundImage(const std::string& path) : GameObject(std::make_unique<Util::Image>(path), 0) {}

//     void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }
// };

class Menu : public Util::GameObject {
public:
    Menu(int time, int score, int playerHeart, int enemyHeart, int playerAmmo, int playerLife)
        : time(time), score(score), playerHeart(playerHeart), enemyHeart(enemyHeart),
          playerAmmo(playerAmmo), playerLife(playerLife), currentStage(currentStage),
          background(std::make_shared<ImageItems>(GA_RESOURCE_DIR "/title/key/title-key-1.png")) {}
    
    std::shared_ptr<ImageItems> background;
    int time;
    int score;
    int playerHeart;
    int enemyHeart;
    int playerAmmo;
    int playerLife;
    int currentStage;
};

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
};

#endif

//Scene.hpp - start update end
//title - \ - stage2 - stage3
//start ptr 
// start game - AnimatedItems
// background - image

// ptr- memory 
// ~ptr 
