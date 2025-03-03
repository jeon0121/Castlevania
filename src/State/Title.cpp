#include "State/Title.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "App.hpp"

void Title::Start(App* app){
    // menu
    m_Menu = std::make_shared<Menu>(180, 0, 16, 16, 5, 3, 1);
    m_Menu->SetMenuVisibility(false);
    app->m_Root.AddChild(m_Menu);
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
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/title/title-screen/title-screen.png");
    m_Background->SetZIndex(0);
    app->m_Root.AddChild(m_Background);

    // bat animation
    std::vector<std::string> batImages;
    for (int i = 0; i < 41; ++i) {
        batImages.emplace_back(GA_RESOURCE_DIR"/title/bat/bat-" + std::to_string(i + 1) + ".png");
    }
    m_Bat = std::make_shared<AnimatedItems>(batImages, 70);
    m_Bat->SetPosition({368, -48});
    m_Bat->SetZIndex(5);
    app->m_Root.AddChild(m_Bat);

    // key animation
    std::vector<std::string> keyImages;
    for (int i = 0; i < 2; ++i) {
        keyImages.emplace_back(GA_RESOURCE_DIR"/title/key/title-key-" + std::to_string(i + 1) + ".png");
    }
    m_Key = std::make_shared<AnimatedItems>(keyImages, 150);
    m_Key->SetPosition({0, -80});
    m_Key -> m_Transform.scale = glm::vec2(0.1652, 0.1652);
    m_Key->SetVisible(false);
    m_Key->SetZIndex(6);
    app->m_Root.AddChild(m_Key);

    m_stateState = StateState::UPDATE;
}

void Title::Update(){
    m_Bat->SetPlaying();
    if (m_Bat->IfAnimationEnds()) {
        m_Bat->SetCurrentFrame(37);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        m_Key->SetVisible(true);
        m_Key->SetPlaying();
        m_Key->SetLooping(true);
    }
    if (m_Key->IsPlaying() && m_Key->IfPlayingTime(2)) {
        m_Key->SetVisible(false);
        m_Bat->SetVisible(false);
        m_Menu->SetVisible(true);
        m_stateState = StateState::END;
    }
}

void Title::End(App* app){
    m_Background->SetDrawable(std::make_unique<Util::Image>(GA_RESOURCE_DIR"/cutscene/intro/intro.png"));
    m_Background->m_Transform.scale = glm::vec2(0.65, 0.65);
    m_Background->SetPosition({0, -100});
    m_Menu->SetMenuVisibility(true);
    // app->m_AppState = App::AppState::START;
    // app->m_GameState = App::GameState::STAGE0;
}
