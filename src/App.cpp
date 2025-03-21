#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "State.hpp"

void App::Start() {
    LOG_TRACE("Start");
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
        case GameState::STAGE2:
            m_Scene = std::make_unique<Stage2>();
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

    m_Scene->Update(this);
    if(m_Scene->m_stateState == Scene::StateState::END) {
        m_Scene->End(this);
    }
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
