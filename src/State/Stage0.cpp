#include "State/Stage0.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "App.hpp"
#include "Util/Logger.hpp"

void Stage0::Start(App* app){
    // menu
    MenuValue menuvalue;
    m_Menu = std::make_shared<Menu>(menuvalue);
    app->m_Root.AddChild(m_Menu);
    app->m_Root.AddChild(m_Menu->background);
    for (auto &&num : m_Menu->numberImage){
        for (auto &&letter : num) app->m_Root.AddChild(letter);
    }
    for (auto &&txt : m_Menu->textImage){
        app->m_Root.AddChild(txt);
    }
    for (auto &&healthbar : m_Menu->health){
        for (auto &&h : healthbar) app->m_Root.AddChild(h);
    }
    // background
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-0/castle-entrance.png", glm::vec2(4.1, 3.5));
    m_Background->SetPosition({1048, -50});
    m_Background->SetZIndex(0);
    app->m_Root.AddChild(m_Background);
}

void Stage0::Update(){

}

void Stage0::End(App* app){


    // //End Stage0
    // if (m_Character->m_Behavior->IfPlayingTime(8)) {
    //     app->m_Root.RemoveAllChildren();
    //     app->m_AppState = App::AppState::START;
    //     app->m_GameState = App::GameState::STAGE1;
    // }
}
