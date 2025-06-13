#include "State/Stage0.hpp"
#include "Util/Logger.hpp"
#include "Utility/Time.hpp"

void Stage0::Start(App* app){
    // menu
    app->m_Menu->SetStageNum(1);
    app->m_Menu->SetMenuVisibility(true);

    // background
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-0/castle-entrance.png");
    m_Background->m_Transform.scale = glm::vec2(1.025, 0.92);
    m_Background->SetPosition({1050, -45});
    backgroundPos = m_Background->GetPosition();
    m_All.push_back(m_Background);

    m_End = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-0/end.png");
    m_End->m_Transform.scale = glm::vec2(1.026, 0.92);
    m_End->SetPosition({441.2, -45.0});
    m_End->SetZIndex(11);
    m_End->SetVisible(false);
    m_All.push_back(m_End);

    //character
    if (!app->m_Character) {
        CharacterValue charactervalue;
        charactervalue.position = glm::vec2(-315, -220.35);
        charactervalue.direction = "right";
        charactervalue.beIndex = 2;
        app->m_Character = std::make_shared<Character>(charactervalue);
    }else {
        app->m_Character->SetPosition({-315, -220.35});
        app->m_Character->m_Behavior->m_Transform.scale = glm::vec2(1.0f, 1.0f);
        app->m_Character->SetDirection("right");
        app->m_Character->Flip();
        app->m_Character->SetZIndex(2);
    }
    app->m_Menu->modifyHealth(app->m_Character->GetHeart(), "player");
    this->m_Character = app->m_Character;
    m_All.push_back(m_Character->m_Behavior);

    // torch
    std::vector<TorchData> torchs = {
        { {-130, -227}, {1, 0.9}, LootType::HeartBig, 2 },
        { {395,  -227}, {1, 0.9}, LootType::Whip,     2 },
        { {920,  -227}, {1, 0.9}, LootType::Whip,     2 },
        { {1445, -227}, {1, 0.9}, LootType::HeartBig, 2 },
        { {1965, -227}, {1, 0.9}, LootType::Dagger,   2 },
    };
    for (auto& t : torchs) {
        auto torch = std::make_shared<Torch>(t.pos, t.scale, t.loot, t.type);
        m_Torches.push_back(torch);
        m_All.push_back(torch);
    }

    // block
    std::vector<BlockData> blocks = {
        { {0,    -307}, {100, 0.7} },
        { {-545, -50 }, {0.4, 7  } },
        { {2600, -50 }, {0.4, 7  } }
    };
    for (auto& b : blocks) {
        auto block = std::make_shared<Block>(b.pos, b.scale);
        m_Blocks.push_back(block);
    }

    // block for hidden item
    auto block = std::make_shared<Block>(glm::vec2(2067, -340.5), glm::vec2(1.025, 0.90), GA_RESOURCE_DIR"/background/block/block-4.png");
    block->SetZIndex(8);
    m_Blocks.push_back(block);
    m_All.push_back(block);

    app->AddAllChildren(m_All);
    m_stateState = StateState::UPDATE;
}

void Stage0::Update(App* app){
    m_Character->Keys(m_Blocks, m_Stairs, app->m_Menu->m_value.time);
    UpdateTorch(app);
    UpdateSubWeapon(app);
    UpdateScroll(mapWidth);
    HiddenItem(app);

    if (m_Character->GetPosition().x >= 200 && m_Character->GetPosition().x <= 320 && m_Character->GetPosition().y <= -220.35) {
        if (m_Character->GetDirection() == "left") {
            m_Character->Flip();
            m_Character->SetDirection(m_Character->GetDirection() == "right" ? "left" : "right");
        }
        m_Background->SetZIndex(8);
        m_Character->ChangeBehavior(0);
        m_Character->m_Behavior->SetPlaying();
        m_Character->m_Behavior->SetLooping(true);
        m_End->SetVisible(true);
        m_stateState = StateState::END;
    }
    if (app->m_Menu->GetTime() == 0 && !isTimeOut) {
        app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/deadBGM.wav");
        app->BGM->Play(1);
        isTimeOut = true;
    }
    if (app->SwitchStage() || m_Character->GetEndSceneFlag()) {
        m_Character->m_Behavior->SetLooping(false);
        m_stateState = StateState::END;
    }
}

void Stage0::End(App* app){
    if (specialBag)
        app->m_Root.RemoveChild(specialBag->score);
    // dead and reset
    if (switchStage || m_Character->GetEndSceneFlag()) {
        SceneReset(app);
        app->m_Character = nullptr;
    }
    else {
        glm::vec2 pos = m_Character->GetPosition();
        m_Character-> SetPosition(glm::vec2(pos.x+1.5f, pos.y));
        if (!is_endSound && m_Character->GetPosition().x >= 280) {
            std::shared_ptr<Util::SFX> endSound = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/25.wav");
            endSound->SetVolume(60);
            endSound->Play();
            is_endSound = true;
        }
        if (m_Character->GetPosition().x >= 500) {
            app->RemoveAllChildren(m_All);
            app->m_AppState = App::AppState::START;
            app->m_GameState = App::GameState::STAGE1;
        }
    }
}

void Stage0::HiddenItem(App *app) {
    if (m_Character->GetPosition().x >= 320 && m_Character->GetPosition().y <= -220.35) {
        if (hiddenLootTime == 0) {
            hiddenLootTime = SDL_GetPerformanceCounter();
            specialBag = Loot::CreateLoot(LootType::SpecialBag, glm::vec2(2067 - offsetX, -300));
            specialBag->SetZIndex(7);
            specialBag->SetFallFlag(false);
            std::shared_ptr<Util::SFX> specialBagAppear = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/34.wav");
            specialBagAppear->SetVolume(50);
            specialBagAppear->Play();
            app->m_Root.AddChild(specialBag);
            m_All.push_back(specialBag);
        }
    }
    if (specialBag) {
        if ((hiddenLootTime != 0 && Time::GetRunTimeMs(hiddenLootTime) > 6000.0f) || specialBag->IfEnded()) {
            app->m_Root.RemoveChild(specialBag);
            specialBag = nullptr;
            return;
        }
        if (hiddenLootTime != 0) {
            if (specialBag->GetPosition().y < -258) {
                specialBag->SetPosition({specialBag->GetPosition().x, specialBag->GetPosition().y + 0.5f});
            }
        }
        if (!specialBag->IfCollected()) {
            specialBag->IsCollected(m_Character);
        }else if (specialBag->IfCollected() && !specialBag->IfEnded()){
            specialBag->Result(app, m_Character, app->m_Menu);
        }
    }
}