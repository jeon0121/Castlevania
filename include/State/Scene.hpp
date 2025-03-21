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

    void TorchBehavior(App* app) {
        for (auto torch : m_Torches) {
            torch->CollideDetection(m_Character);
            if (torch->is_destroyed && !torch->IfCollected()) {
                torch->Destroy();
            }
            if (torch->loot && !torch->loot->IfCollected()) {
                app->m_Root.AddChild(torch->loot);
                torch->loot->Fall(m_Blocks);
                torch->loot->IsCollected(m_Character, m_Menu);
                torch->SetCollected();
            }
        }
    }

    StateState m_stateState = StateState::START;

protected:
    std::shared_ptr<ImageItems> m_Background;
    std::shared_ptr<Character> m_Character;
    std::shared_ptr<Menu> m_Menu;
    std::vector<std::shared_ptr<Util::GameObject>> m_All;
    std::vector<std::shared_ptr<Block>> m_Blocks;
    std::vector<std::shared_ptr<Torch>> m_Torches;
};

#endif

//Scene.hpp - start update end
//title - \ - stage2 - stage3
//start ptr 
// start game - AnimatedItems
// background - image

// ptr- memory 
// ~ptr 
