#include "State/Stage2.hpp"

void Stage2b::Start(App *app) {
    if (m_Blocks.empty()) {
        // background
        m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-2/crypt-b.png");
        m_Background->m_Transform.scale = glm::vec2(1.025, 0.90);
        m_Background->SetPosition({525, -35});
        backgroundPos = m_Background->GetPosition();
        m_All.push_back(m_Background);

        //torch
        std::vector<TorchData> torchs = {
            { { -393, 163  }, {1, 0.9}, LootType::WhiteBag,   1},
            { { -131, 163  }, {1, 0.9}, LootType::RedBag,     1},
            { { 131,  163  }, {1, 0.9}, LootType::RedBag,     1},
            { { 393,  163  }, {1, 0.9}, LootType::WhiteBag,   1},
            { { 656,  163  }, {1, 0.9}, LootType::Stopwatch,  1},
            { { 915,  163  }, {1, 0.9}, LootType::HeartSmall, 1},
            { { 1177, 163  }, {1, 0.9}, LootType::HeartSmall, 1},
            { { 1505, 163  }, {1, 0.9}, LootType::HeartBig,   1}
        };
        for (auto& t : torchs) {
            auto torch = std::make_shared<Torch>(t.pos, t.scale, t.loot, t.type);
            m_Torches.push_back(torch);
            m_All.push_back(torch);
        }

        // block
        std::vector<BlockData> blocks = {
            { { -65,  54   }, { 1.08, 0.68 } },
            { { -65,  -62  }, { 7.5,  0.68 } },
            { { 590,  -62  }, { 1.08, 0.68 } },
            { { 1077, -62  }, { 4.75, 0.68 } },
            { { 1375, -296 }, { 1.08, 0.68 } },
            { { 1510, -180 }, { 1.08, 0.68 } },

            { { -545,  65  }, { 0.4,  4    } },
            { { 1565,  50  }, { 0.4,  5    } },
         };
        for (auto& b : blocks) {
            auto block = std::make_shared<Block>(b.pos, b.scale);
            m_Blocks.push_back(block);
            // m_All.push_back(block);
        }
        // hitable block
        std::vector<std::vector<HitableBlockData>> hitableBlocks = {
            { 
                { { 1402, -64 }, LootType::None, GA_RESOURCE_DIR"/background/block/block-3.png" },
            },
        };
        for (auto& hb : hitableBlocks) {
            auto hitableBlock = std::make_shared<HitableBlock>(LootType::SpecialBag);
            for (auto& b : hb) {
                auto block = std::make_shared<Block>(b.pos, glm::vec2(1.025, 0.90), b.path);
                m_Blocks.push_back(block);
                m_All.push_back(block);
                hitableBlock->AddBlock(block, b.loot, m_All);
            }
            m_HitableBlocks.push_back(hitableBlock);
        }
        //stair
        std::vector<StairData> stairs = {
            { { -325, 280  }, { -133, 107 } },
            { { 986,  280  }, { 1303, -7  } }
        };
        for (auto& s : stairs) {
            auto stair = Stair::CreateStair(s.pos1, s.pos2);
            m_Stairs.insert(m_Stairs.end(), stair.begin(), stair.end());
            // m_All.insert(m_All.end(), stair.begin(), stair.end());
        }

        // EnemiesManager
        std::vector<PossibleLootData> possibleLoots = {
            {LootType::HeartSmall, 0.5, -1},
            {LootType::HeartBig,   0.2,  1},
            {LootType::Stopwatch,  0.2,  1},
            {LootType::None,       1.0, -1},
         };
        m_EnemiesManager = std::make_shared<EnemiesManager>(possibleLoots);
    }

    //character
    if (this->m_Character == nullptr) {
        if (app->stairNum[0] == 1) {
            this->m_Character = app->m_Character;
            UpdateScroll(mapWidth, 1050);
        }
    }else {
        if (app->stairNum[1] == 0 && app->stairNum[0] == 1)
            UpdateScroll(mapWidth, 1050);
        else if (app->stairNum[1] == 1 && app->stairNum[0] == 0)
            UpdateScroll(mapWidth, m_Torches[0]->GetPosition().x + 393);
        else if (app->stairNum[1] == 1 && app->stairNum[0] == 1)
            UpdateScroll(mapWidth, m_Torches[5]->GetPosition().x + 135);
    }
    this->m_Character = app->m_Character;
    m_Character->SetPosition({((m_Character->GetPosition().x < 0) ? -324.2 : -60.5), 315.85});//-285.7 -4.1
    m_Character->SetOffStairs();
    std::shared_ptr<Stair> stair = m_Character->CollideStair(m_Stairs);
    m_Character->Descending(stair);
    app->m_Root.AddChild(m_Character->m_Behavior);

    // Fishman
    std::vector<FishmanData> fishmans = {
        {{-395, -250}, "right"},
        {{185,  -250}, "left"}
    };
    for (auto& f : fishmans) {
        m_EnemiesManager->AddFishman(f.pos, f.direction, app);
    }

    app->AddAllChildren(m_All);
    m_EnemiesManager->AddAllChild(app);
    m_stateState = StateState::UPDATE;
}

void Stage2b::Update(App *app) {
    m_Character->Keys(m_Blocks, m_Stairs);
    UpdateTorch(app);
    m_EnemiesManager->Update(offsetX, screenWidth, m_Character, m_Blocks, app);
    UpdateSubWeapon(app);
    UpdateScroll(mapWidth);
    UpdateHitableBlock(app);
    if (m_Character->GetPosition().y > 300  && m_Character->GetDirection() == "left") {
        app->stairNum[1] = (m_Character->GetPosition().x < -100) ? 0 : 1;
        app->m_AppState = App::AppState::START;
        app->m_GameState = App::GameState::STAGE2A;
        app->RemoveAllChildren(m_All);
        app->m_Root.RemoveChild(m_Character->m_Behavior);
        m_EnemiesManager->RemoveAllChild(app);
    }else if (m_Character->GetDeadFlag()) {
        app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/deadBGM.wav");
        app->BGM->Play(1);
    }
    if (m_Character->GetEndSceneFlag()) {
        m_Character->m_Behavior->SetLooping(false);
        m_stateState = StateState::END;
    }
}

void Stage2b::End(App *app) {
    // dead and reset
    if (m_Character->GetEndSceneFlag()) {
        app->m_Menu->SetMenuVisibility(false);
        m_EnemiesManager->RemoveAllChild(app);
        app->m_Menu->modifyWeapon(WeaponType::None);
        app->m_Menu->modifyNumber(app->m_Menu->formatTwoDigits(5), 3);
        if (app->m_Menu->m_value.playerLife == 0)
            app->m_GameState = App::GameState::GG;
        else {
            app->m_Menu->modifyNumber(app->m_Menu->formatTwoDigits(--(app->m_Menu->m_value.playerLife)), 4);
            app->m_GameState = App::GameState::STAGE2A;
        }
        app->RemoveAllChildren(m_All);
        app->m_Root.RemoveChild(m_Character->m_Behavior);
        app->m_AppState = App::AppState::START;
    }
}
