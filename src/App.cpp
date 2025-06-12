#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "State.hpp"
#include "State/GG.hpp"

void App::Start() {
    LOG_TRACE("Start");
    if (!m_Menu) {
        AddMenu();
        BGM = std::make_shared<Util::BGM>(GA_RESOURCE_DIR "/BGM/stageBGM.wav");
        BGM->SetVolume(50);
    }
    if (m_GameState != GameState::TITLE && m_GameState != GameState::GG)
        m_Menu->TimeCount(true);
    if (m_GameState == GameState::STAGE2A && m_Character && m_Character->GetDeadFlag()) {
        m_Character = nullptr;
        m_SceneA = nullptr;
        m_SceneB = nullptr;
    }
    switch(m_GameState){
        case GameState::TITLE:
            m_Scene = std::make_unique<Title>();
            break;
        case GameState::STAGE0:
            m_Scene = std::make_unique<Stage0>();
            break;
        case GameState::STAGE1:
            m_Scene = std::make_shared<Stage1>();
            break;
        case GameState::STAGE2A:
            if (!m_SceneA)
                m_SceneA = std::make_shared<Stage2a>();
            m_Scene = m_SceneA;
            break;
        case GameState::STAGE2B:
            if (!m_SceneB)
                m_SceneB = std::make_shared<Stage2b>();
            m_Scene = m_SceneB;
            break;
        case GameState::STAGE3:
            m_Scene = std::make_unique<Stage3>();
            break;
        case GameState::GG:
            m_Scene = std::make_unique<GG>();
            break;
    }
    if(!m_Character && m_GameState != GameState::TITLE) {
        BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/stageBGM.wav");
        BGM->Play();
    }
    m_Scene->Start(this);
    m_Scene->m_stateState = Scene::StateState::UPDATE;
    m_AppState = AppState::UPDATE;
}

void App::Update() {
    if (m_GameState != GameState::TITLE && m_GameState != GameState::GG)
        m_Menu->TimeCount();
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

void App::AddMenu() {
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

void App::RemoveMenu() {
    m_Root.RemoveChild(m_Menu);
    m_Root.RemoveChild(m_Menu->background);
    for (auto &&num : m_Menu->numberImage){
        for (auto &&letter : num) m_Root.RemoveChild(letter);
    }
    for (auto &&txt : m_Menu->textImage){
        m_Root.RemoveChild(txt);
    }
    for (auto &&healthbar : m_Menu->health){
        for (auto &&h : healthbar) m_Root.RemoveChild(h);
    }
    m_Menu = nullptr;
}