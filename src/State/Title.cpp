#include "State/Title.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "App.hpp"
#include "Object/AnimatedItems.hpp"

void Title::Start(){
    m_Giraffe = std::make_shared<BackgroundImage>(GA_RESOURCE_DIR"/Resources/title/title-screen/title-screen.png");
    m_Background->SetZIndex(0);
    m_Root.AddChild(m_Background);

    std::vector<std::string> batImages;
    for (int i = 0; i < 40; ++i) {
        batImages.emplace_back(GA_RESOURCE_DIR"/title/bat/bat-" + std::to_string(i + 1) + ".png");
    }

    m_Bat = std::make_shared<AnimatedItems>(batImages);
    m_Bat->SetZIndex(5);
    m_Root.AddChild(m_Bat);
    m_currentState = State::UPDATE;
}

void Title::Update(){
    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        m_AppState = AppState::START;
        m_CurrentState = State::END;
        m_GameState = GameState::STAGE0;
    }
}
