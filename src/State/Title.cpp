#include "State/Title.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Object/AnimatedItems.hpp"
#include "App.hpp"

void Title::Start(App* app){
    m_Background = std::make_shared<BackgroundImage>(GA_RESOURCE_DIR"/title/title-screen/title-screen.png");
    m_Background->SetZIndex(0);
    app->m_Root.AddChild(m_Background);

    std::vector<std::string> batImages;
    for (int i = 0; i < 40; ++i) {
        batImages.emplace_back(GA_RESOURCE_DIR"/title/bat/bat-" + std::to_string(i + 1) + ".png");
    }
    m_Bat = std::make_shared<AnimatedItems>(batImages);
    m_Bat->SetPosition({368, -48});
    m_Bat->SetZIndex(5);
    app->m_Root.AddChild(m_Bat);

    std::vector<std::string> keyImages;
    for (int i = 0; i < 2; ++i) {
        keyImages.emplace_back(GA_RESOURCE_DIR"/title/title-screen/title-key-" + std::to_string(i + 1) + ".png");
    }
    m_Key = std::make_shared<AnimatedItems>(keyImages);
    m_Key->SetPosition({0, -80});
    m_Key -> m_Transform.scale = glm::vec2(0.1652, 0.1652);
    m_Key->SetVisible(false);
    m_Key->SetZIndex(6);
    app->m_Root.AddChild(m_Key);
    m_stateState = StateState::UPDATE;
}

void Title::Update(){
    m_Bat -> SetPlaying();
    m_Bat -> SetLooping(true);
    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        m_Key->SetVisible(true);
        m_Key -> SetPlaying();
        m_Key -> SetLooping(true);
        m_stateState = StateState::END;
    }
}

void Title::End(App* app){
    app->m_AppState = App::AppState::START;
    app->m_GameState = App::GameState::STAGE0;
}
