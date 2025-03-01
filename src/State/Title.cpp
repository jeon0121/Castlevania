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
    m_Bat->SetZIndex(5);
    app->m_Root.AddChild(m_Bat);
    m_stateState = StateState::UPDATE;
}

void Title::Update(){
    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        m_stateState = StateState::END;
    }
}

void Title::End(App* app){
    app->m_AppState = App::AppState::START;
    app->m_GameState = App::GameState::STAGE0;
}
