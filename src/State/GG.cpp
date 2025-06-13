#include "State/GG.hpp"

void GG::Start(App* app){
    // menu
    app->m_Menu->SetMenuVisibility(true);

    // text
    app->m_Menu->addText("continue", {37, -137}, 1);
    app->m_Menu->addText("end", {37, -227}, 1);
    if (app->GetVictoryFlag())
        app->m_Menu->addText("victory", {37, 16}, 1);
    else
        app->m_Menu->addText("game-over", {37, 16}, 1);
    for (auto&& txt : app->m_Menu->GGImage)
        app->m_Root.AddChild(txt);

    // selectHeart
    selectHeart = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/fonts/menu/select-heart.png", glm::vec2(1, 0.88));
    selectHeart->SetPosition({-123, -137});
    selectHeart->SetZIndex(50);
    m_All.push_back(selectHeart);

    // background
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/title/key/title-key-1.png", glm::vec2(5, 10));
    m_Background->SetZIndex(0);
    m_All.push_back(m_Background);

    // sound
    app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/gameoverBGM.wav");
    app->BGM->SetVolume(50);
    app->BGM->Play(1);
    choiceSound = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/05.wav");
    choiceSound->SetVolume(50);

    app->AddAllChildren(m_All);
}

void GG::Update(App* app){
    if (Util::Input::IsKeyDown(Util::Keycode::RSHIFT)) {
        choice *= -1;
        glm::vec2 pos = selectHeart->GetPosition();
        selectHeart->SetPosition({pos.x, pos.y + (choice * 90)});
        choiceSound->Play();
    }else if (Util::Input::IsKeyPressed(Util::Keycode::RETURN))
        m_stateState = StateState::END;
}

void GG::End(App* app){
    for (auto&& txt : app->m_Menu->GGImage)
        app->m_Root.RemoveChild(txt);
    choiceSound->SetVolume(0);
    app->m_SceneA = nullptr;
    app->m_SceneB = nullptr;
    app->m_Character = nullptr;
    app->stairNum = {0, 0};
    app->RemoveAllChildren(m_All);
    app->RemoveMenu();
    app->SetVictoryFlag(false);
    app->m_AppState = App::AppState::START;
    if (choice == 1)
        app->m_GameState = App::GameState::STAGE0;
    else {
        app->BGM->Pause();
        app->BGM = nullptr;
        app->m_GameState = App::GameState::TITLE;
    }
}
