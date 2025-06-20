#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "State.hpp"

void App::Start() {
    LOG_TRACE("Start");
    if (!m_Menu) {
        AddMenu();
        BGM = std::make_shared<Util::BGM>(GA_RESOURCE_DIR "/BGM/stageBGM.wav");
        BGM->SetVolume(50);
    }
    if (m_GameState != GameState::TITLE && m_GameState != GameState::GG && debugMode == 0)
        m_Menu->TimeCount(true);
    if (m_GameState == GameState::STAGE2A && ((m_Character && m_Character->GetDeadFlag()) || (m_Scene && m_Scene->GetSwitchStageFlag()))) {
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
    SwitchMode();
    if (m_GameState != GameState::TITLE && m_GameState != GameState::GG && debugMode == 0 && !isVictory)
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

void App::RemoveAllChildren(std::vector<std::shared_ptr<Util::GameObject>> m_All, std::vector<std::shared_ptr<Util::GameObject>> m_Loots) {
    for (auto i : m_All) {
        m_Root.RemoveChild(i);
    }
    for (auto i : m_Loots) {
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

int  App::GetVictoryFlag() {
    return isVictory;
}

void App::SetVictoryFlag(bool isVictory) {
    this->isVictory = isVictory;
}

void App::SwitchMode() {
    if (Util::Input::IsKeyUp(Util::Keycode::NUM_6))
        debugMode = (debugMode + 1) % 2;
}

bool App::SwitchStage() {
    GameState prevStage = m_GameState;
    if (debugMode == 1) {
        if (m_GameState != GameState::STAGE0 && Util::Input::IsKeyPressed(Util::Keycode::NUM_0))
            m_GameState = GameState::STAGE0;
        else if (m_GameState != GameState::STAGE1 && Util::Input::IsKeyPressed(Util::Keycode::NUM_1))
            m_GameState = GameState::STAGE1;
        else if (m_GameState != GameState::STAGE2A && Util::Input::IsKeyPressed(Util::Keycode::NUM_2))
            m_GameState = GameState::STAGE2A;
        else if (m_GameState != GameState::STAGE3 && Util::Input::IsKeyPressed(Util::Keycode::NUM_3))
            m_GameState = GameState::STAGE3;
    }
    if (prevStage != m_GameState) {
        m_Scene->SetSwitchStageFlag(true);
        if (prevStage ==  GameState::STAGE2A || prevStage ==  GameState::STAGE2B) {
            m_Character = nullptr;
            m_SceneA = nullptr;
            m_SceneB = nullptr;
        }
        return true;
    }
    return false;
}

int App::GetModeState() {
    return debugMode;
}