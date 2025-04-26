#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "State.hpp"

void App::Start() {
    LOG_TRACE("Start");
    if (!m_Menu) {
        MenuValue menuvalue;
        m_Menu = std::make_shared<Menu>(menuvalue);
        m_Menu->SetMenuVisibility(false);
        m_Root.AddChild(m_Menu);
        m_Root.AddChild(m_Menu->background);
        for (auto &&num : m_Menu->numberImage){
            for (auto &&letter : num) m_Root.AddChild(letter);
        }
        for (auto &&txt : m_Menu->textImage){
            m_Root.AddChild(txt);
        }
        for (auto &&healthbar : m_Menu->health){
            for (auto &&h : healthbar) m_Root.AddChild(h);
        }
    }
    switch(m_GameState){
        case GameState::TITLE:
            m_Scene = std::make_unique<Title>();
            break;
        case GameState::STAGE0:
            m_Scene = std::make_unique<Stage0>();
            break;
        case GameState::STAGE1:
            m_Scene = std::make_unique<Stage1>();
            break;
        case GameState::STAGE2A:
            m_Scene = std::make_unique<Stage2a>();
            break;
        case GameState::STAGE2B:
            m_Scene = std::make_unique<Stage2b>();
            break;
        case GameState::STAGE3:
            m_Scene = std::make_unique<Stage3>();
            break;
    }
    m_Scene->Start(this);
    m_Scene->m_stateState = Scene::StateState::UPDATE;
    m_AppState = AppState::UPDATE;
}

void App::Update() {

    if(m_Scene->m_stateState == Scene::StateState::END)
        m_Scene->End(this);
    else
        m_Scene->Update(this);
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    m_Root.Update();
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_AppState = AppState::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

void App::AddAllChildren(std::vector<std::shared_ptr<Util::GameObject>> m_All) {
    for (auto i : m_All) {
        m_Root.AddChild(i);
    }
}

void App::RemoveAllChildren(std::vector<std::shared_ptr<Util::GameObject>> m_All) {
    for (auto i : m_All) {
        m_Root.RemoveChild(i);
    }
}
