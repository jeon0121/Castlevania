#include "State/Title.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "App.hpp"
#include "Util/Logger.hpp"

void Title::Start(App* app){
    // menu
    MenuValue menuvalue;
    m_Menu = std::make_shared<Menu>(menuvalue);
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
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/title/title-screen/title-screen.png", glm::vec2(1, 0.9));
    m_Background->SetZIndex(0);
    app->m_Root.AddChild(m_Background);

    // bat animation
    std::vector<std::string> batImages;
    for (int i = 0; i < 41; ++i) {
        batImages.emplace_back(GA_RESOURCE_DIR"/title/bat/bat-" + std::to_string(i + 1) + ".png");
    }
    m_Bat = std::make_shared<AnimatedItems>(batImages, 65, glm::vec2(1, 0.9));
    m_Bat->SetPosition({368, -43});
    m_Bat->SetZIndex(5);
    m_Bat->SetPlaying();
    app->m_Root.AddChild(m_Bat);

    // key animation
    std::vector<std::string> keyImages;
    for (int i = 0; i < 2; ++i) {
        keyImages.emplace_back(GA_RESOURCE_DIR"/title/key/title-key-" + std::to_string(i + 1) + ".png");
    }
    m_Key = std::make_shared<AnimatedItems>(keyImages, 120, glm::vec2(0.1662, 0.1652));
    m_Key->SetPosition({0, -70});
    m_Key->SetVisible(false);
    m_Key->SetZIndex(6);
    app->m_Root.AddChild(m_Key);

    //character
    m_Character = std::make_shared<Character>(glm::vec2(480, -280));
    m_Character->LoadBehavior(4, 0);
    app->m_Root.AddChild(m_Character->m_Behavior);
    app->m_Root.AddChild(m_Character->m_Image);

    //boat
    std::string boatImage = GA_RESOURCE_DIR"/cutscene/boat/boat.png";
    m_Boat = std::make_shared<ImageItems>(boatImage, glm::vec2(0.1, 0.1));
    m_Boat->SetPosition({480, 125});
    m_Boat->SetVisible(false);
    m_Boat->SetZIndex(6);
    app->m_Root.AddChild(m_Boat);

    //bat in cutscene
    std::vector<std::string> batScene;
    for (int i = 0; i < 2; ++i) {
        batScene.emplace_back(GA_RESOURCE_DIR"/cutscene/bat/bat-" + std::to_string(i + 1) + ".png");
    }
    m_BatScene_1 = std::make_shared<AnimatedItems>(batScene, 65, glm::vec2(0.08, 0.08));
    m_BatScene_1->SetPosition({75, 150});
    m_BatScene_2 = std::make_shared<AnimatedItems>(batScene, 65, glm::vec2(0.08, 0.08));
    m_BatScene_2->SetPosition({-450, -135});
    std::vector<std::shared_ptr<AnimatedItems>> m_BatScene = {m_BatScene_1, m_BatScene_2};
    for (auto &&anim : m_BatScene) {
        anim->SetVisible(false);
        anim->SetZIndex(6);
        app->m_Root.AddChild(anim);
    }

    m_stateState = StateState::UPDATE;
}

void Title::Update(){
    if (m_Bat->IfAnimationEnds()) {
        m_Bat->SetCurrentFrame(37);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        m_Key->SetVisible(true);
        m_Key->SetPlaying();
        m_Key->SetLooping(true);
        m_Bat->SetPaused();
    }
    if (m_Key->IsPlaying() && m_Key->IfPlayingTime(1.5)) {
        m_Key->SetVisible(false);
        m_Bat->SetVisible(false);
        m_Character->m_Behavior->SetVisible(true);
        m_Boat->SetVisible(true);
        m_BatScene_1->SetVisible(true);
        m_BatScene_2->SetVisible(true);
        m_stateState = StateState::END;
    }
}

void Title::End(App* app){
    m_Background->SetDrawable(std::make_unique<Util::Image>(GA_RESOURCE_DIR"/cutscene/intro/intro.png"));
    m_Background->m_Transform.scale = glm::vec2(0.65, 0.55);
    m_Background->SetPosition({0, -100});
    m_Menu->SetMenuVisibility(true);

    //Cutscene animation
    m_Character->m_Behavior->Move(m_Character->m_Behavior, -1, 0, 2.08, 3.9);
    m_Character-> SetPosition(m_Character->m_Behavior->GetPosition());
    if (m_Character->m_Behavior->IfPlayingTime(3.9)) { //end animation and idle
        m_Character->m_Behavior->SetVisible(false);
        m_Character->UpdatePosition();
        m_Character->m_Image->SetVisible(true);
    }

    glm::vec2 pos = m_Boat->GetPosition();
    m_Boat->SetPosition({pos.x-0.4, pos.y});
    m_BatScene_1->Move(m_BatScene_1, -1, 0, 0.25, 8);
    m_BatScene_2->Move(m_BatScene_2, 1, 1, 0.6, 8);

    //End Title
    if (m_Character->m_Behavior->IfPlayingTime(8)) {
        app->m_Root.RemoveAllChildren();
        app->m_AppState = App::AppState::START;
        app->m_GameState = App::GameState::STAGE0;
    }
}
