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
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-0/castle-entrance.png");
    m_Background->m_Transform.scale = glm::vec2(1.1, 1);
    m_Background->SetPosition({1048, -50});
    app->m_Root.AddChild(m_Background);

    //character
    m_Character = std::make_shared<Character>(glm::vec2(0, -280));
    m_Character->LoadBehavior(0, 0);
    app->m_Root.AddChild(m_Character->m_Behavior);
    app->m_Root.AddChild(m_Character->m_Image);
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
