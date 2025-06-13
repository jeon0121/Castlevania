#include "State/Stage3.hpp"
#include "Utility/Position.hpp"
#include "Utility/Time.hpp"

void Stage3::Start(App* app){
    // menu
    app->m_Menu->SetMenuVisibility(true);

    // background
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-3/phantom-bat.png");
    m_Background->m_Transform.scale = glm::vec2(1.025, 0.90);
    m_Background->SetPosition({1050, -35});
    backgroundPos = m_Background->GetPosition();
    m_All.push_back(m_Background);

    // blink
    std::vector<std::string> blinkImages = {GA_RESOURCE_DIR"/background/stage-3/phantom-bat-blink.png", GA_RESOURCE_DIR"/background/stage-3/phantom-bat.png"};
    m_Blink = std::make_shared<AnimatedItems>(blinkImages, 40, m_Background->m_Transform.scale);
    m_Blink->SetZIndex(1);
    m_Blink->SetPosition(m_Background->GetPosition());
    m_Blink->SetVisible(false);
    m_All.push_back(m_Blink);

    //character
    if (!app->m_Character) {
        CharacterValue charactervalue;
        charactervalue.position = glm::vec2(-315, 80.76);
        charactervalue.direction = "right";
        charactervalue.beIndex = 2;
        app->m_Character = std::make_shared<Character>(charactervalue);
    }
    else
        app->m_Character->SetPosition({-315, 80.76});
    this->m_Character = app->m_Character;
    m_All.push_back(m_Character->m_Behavior);

    // EnemiesManager
    std::vector<PossibleLootData> possibleLoots = {
        {LootType::HeartSmall, 0.5, -1},
        {LootType::Dagger,     0.2,  1},
        {LootType::Rosary,     0.2,  1},
        {LootType::None,       1.0, -1},
    };
    m_EnemiesManager = std::make_shared<EnemiesManager>(possibleLoots);

    // zombies
    std::vector<ZombieData> zombies = {
        { { -500, 1215 }, 2, -265.35f },
        { { -500, 10   }, 1, -36.24f  },
        { { 600,  1000 }, 1, -36.24f  },
    };
    for (auto& z : zombies) {
        m_EnemiesManager->AddZombie(z.range, z.numZombie, {screenWidth * 0.5, z.yPos}, "left", app);
    }
    
    m_EnemiesManager->SetEnemyRange({-5000, 1500});

    // boss
    m_Boss = std::make_shared<PhantomBat>(glm::vec2(2020, 190));
    m_Boss->SetZIndex(7);
    auto boss = std::dynamic_pointer_cast<PhantomBat>(m_Boss);
    m_All.push_back(boss->hurtEffect);
    m_All.push_back(boss);

    //torch
    std::vector<TorchData> torchs = {
        { { -393, -125 }, {1, 0.9}, LootType::HeartBig,   1},
        { { -131,  106 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 131,   -10 }, {1, 0.9}, LootType::RedBag,     1},
        { { 394,   106 }, {1, 0.9}, LootType::Pot,        1},
        { { 656,  -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 914,  -125 }, {1, 0.9}, LootType::Axe,        1},
        { { 1177, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 1440, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 1702, -125 }, {1, 0.9}, LootType::HeartBig,   1},
        { { 1964, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 2227, -125 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 2489, -125 }, {1, 0.9}, LootType::HeartBig,   1}
    };
    for (auto& t : torchs) {
        auto torch = std::make_shared<Torch>(t.pos, t.scale, t.loot, t.type);
        m_Torches.push_back(torch);
        m_All.push_back(torch);
    }

    // block
    std::vector<BlockData> blocks = {
        { { 0,    -352 }, { 100,  0.7  } },
        { { -547, -50  }, { 0.4,  7    } },
        { { -303, -5   }, { 3.86, 0.68 } },
        { { 360,  -122 }, { 5.85, 0.68 } },

        { { 2262, -240 }, { 0.52, 0.68 } },
        { { 2525, -122 }, { 1.62, 0.68 } },

        { { 2610, -50  }, { 0.3,  7    } }
    };
    for (auto& b : blocks) {
        auto block = std::make_shared<Block>(b.pos, b.scale);
        m_Blocks.push_back(block);
        // m_All.push_back(block);
    }
    // hitable block
    std::vector<std::vector<HitableBlockData>> hitableBlocks = {
        { 
            { { 2329, -236 }, LootType::Badge, GA_RESOURCE_DIR"/background/block/block-3.png" },
        },
    };
    for (auto& hb : hitableBlocks) {
        auto hitableBlock = std::make_shared<HitableBlock>(LootType::Badge);
        for (auto& b : hb) {
            auto block = std::make_shared<Block>(b.pos, glm::vec2(1.025, 0.90), b.path);
            m_Blocks.push_back(block);
            hitableBlock->AddBlock(block, b.loot, m_All);
        }
        m_HitableBlocks.push_back(hitableBlock);
    }
    // stair
    std::vector<StairData> stairs = {
        { { -70,   45  }, { 68,   -66 } },
        { { 716,  -66  }, { 985, -294 } },
        { { 2295, -180 }, { 2430, -66 } }
    };
    for (auto& s : stairs) {
        auto stair = Stair::CreateStair(s.pos1, s.pos2);
        m_Stairs.insert(m_Stairs.end(), stair.begin(), stair.end());
        // m_All.insert(m_All.end(), stair.begin(), stair.end());
    }

    countScore = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/05.wav");
    countScore->SetVolume(50);

    app->AddAllChildren(m_All);
    m_EnemiesManager->AddAllChild(app);
    m_stateState = StateState::UPDATE;
}

void Stage3::Update(App* app){
    // Position::PrintObjectCoordinate(m_Character, offsetX);
    if (blinkStartTime != 0)
        Blink();
    m_Character->Keys(m_Blocks, m_Stairs, app->m_Menu->m_value.time);
    UpdateTorch(app);
    m_EnemiesManager->Update(offsetX, screenWidth, m_Character, m_Blocks, app);
    UpdateSubWeapon(app);
    UpdateHitableBlock(app);
    if (m_Character->GetDeadFlag()) {
        app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/deadBGM.wav");
        app->BGM->Play(1);
    }
    if (m_Character->GetPosition().x <= 20 && !reachBoss) {
        UpdateScroll(mapWidth);
    }
    else if (m_Character->GetPosition().x > 20 && !reachBoss) {
        reachBoss = true;
        BlockData bData = { { -547, -50 }, { 0.4, 7 } };
        auto block = std::make_shared<Block>(bData.pos, bData.scale);
        m_Blocks.push_back(block);
        app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/bossBGM.wav");
        app->BGM->Play();
    }
    // boss
    else if (reachBoss) {
        whipDropped = true;
        BossFight(app);
    }
    if (m_Character->GetStartDeadFlag() || (app->m_Menu->GetTime() == 0 && !isTimeOut)) {
        app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/deadBGM.wav");
        app->BGM->Play(1);
        isTimeOut = true;
    }
    if (app->SwitchStage() || m_Character->GetEndSceneFlag()) {
        m_Character->m_Behavior->SetLooping(false);
        m_stateState = StateState::END;
    }
}

void Stage3::End(App* app){
    // dead and reset
    if (switchStage || m_Character->GetEndSceneFlag()) {
        m_EnemiesManager->RemoveAllChild(app);
        SceneReset(app);
        app->m_Menu->modifyBadge(false);
        app->m_Character = nullptr;
    }
    // end scene animation
    else {
        EndAnimation(app);
        std::cout << "Stage 3 End" << std::endl;
    }
}

void Stage3::BossFight(App *app) {
    if (!m_Boss->CollideDetection(m_Character, app->m_Menu, app->GetModeState()))
        m_Boss->MoveBehav(m_Character, screenHeight, screenWidth);
    else {
        auto boss = std::dynamic_pointer_cast<PhantomBat>(m_Boss);
        if (boss->IsDead()) {
            boss->hurtEffect->SetVisible(false);
            boss->SetVisible(false);
            DropCrystal(app);
        }
        else
            boss->Hurt();
    }
}
void Stage3::DropCrystal(App *app) {
    if (crystalTime == 0) {
        crystalTime = SDL_GetPerformanceCounter();
        m_crystal = Loot::CreateLoot(LootType::Crystal, glm::vec2(0, 0));
        m_crystal->SetVisible(false);
        app->m_Root.AddChild(m_crystal);
        m_All.push_back(m_crystal);
    } else if (Time::GetRunTimeMs(crystalTime) > 2500.0f && m_crystal) {
        m_crystal->SetVisible(true);
        m_crystal->Fall(m_Blocks);
        if (m_crystal->IsCollected(m_Character)) {
            m_crystal->Result(app, m_Character, app->m_Menu);
            crystalTime = SDL_GetPerformanceCounter();
            m_Character->ChangeBehavior(2);
            app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/victoryBGM.wav");
            app->BGM->SetVolume(50);
            app->BGM->Play(1);
            app->SetVictoryFlag(true);
            m_stateState = StateState::END;
        }
    } else if (Time::GetRunTimeMs(crystalTime) > 2000.0f && m_crystal) {
        int timeCount = static_cast<int>(Time::GetRunTimeMs(crystalTime));
        m_crystal->SetVisible((timeCount / 50) % 2 == 0);
    }
}

void Stage3::EndAnimation(App *app) {
    if (Time::GetRunTimeMs(crystalTime) < 5000.0f)
        return;
    if (m_Character->GetAmmo() == 0 && endScoreTime == 0)
        endScoreTime = SDL_GetPerformanceCounter();
    if (app->m_Menu->GetTime() > 0) {
        app->m_Menu->modifyNumber(app->m_Menu->formatTime(app->m_Menu->GetTime() - 1), 1);
        app->m_Menu->SetTime(app->m_Menu->GetTime() - 1);
        app->m_Menu->modifyNumber(app->m_Menu->formatScore(app->m_Menu->GetScore() + 10), 0, (app->m_Menu->GetScore() + 10));
        countScore->Play();
    }else if (m_Character->GetAmmo() > 0) {
        if (countHeartTime == 0)
            countHeartTime = SDL_GetPerformanceCounter();
        else if (Time::GetRunTimeMs(countHeartTime) > 250.0f) {
            app->m_Menu->modifyNumber(app->m_Menu->formatTwoDigits(m_Character->GetAmmo()-1), 3);
            m_Character->SetAmmo(m_Character->GetAmmo()-1);
            app->m_Menu->modifyNumber(app->m_Menu->formatScore(app->m_Menu->GetScore() + 100), 0, (app->m_Menu->GetScore() + 100));
            countScore->Play();
            countHeartTime = 0;
        }
    }else if (Time::GetRunTimeMs(endScoreTime) > 3000.0f) {
        m_EnemiesManager->RemoveAllChild(app);
        app->RemoveAllChildren(m_All);
        app->m_Character = nullptr;
        app->m_AppState = App::AppState::START;
        app->m_GameState = App::GameState::GG;
    }
}