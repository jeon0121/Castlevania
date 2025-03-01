#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "State/Title.hpp"

void App::Start() {
    LOG_TRACE("Start");
    switch(m_GameState){
        case GameState::TITLE:
            m_Scene = std::make_unique<Title>();
        case GameState::STAGE0:
            m_Scene = std::make_unique<STAGE0>();
        case GameState::STAGE1:
            m_Scene = std::make_unique<STAGE1>();
        case GameState::STAGE2:
            m_Scene = std::make_unique<STAGE2>();
        case GameState::STAGE3:
            m_Scene = std::make_unique<STAGE3>();
    }
    m_AppState = AppState::UPDATE;
    m_Scene->Start();
}

void App::Update() {

    m_Scene->Update();
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_AppState = AppState::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
