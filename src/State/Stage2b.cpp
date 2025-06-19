#include "State/Stage2.hpp"
#include "Utility/Position.hpp"
#include "Utility/Time.hpp"

void Stage2b::Start(App *app) {
    if (m_Blocks.empty()) {
        // menu
        app->m_Menu->SetStageNum(2);
        app->m_Menu->SetMenuVisibility(true);

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
            { { 1505, 163  }, {1, 0.9}, LootType::HeartBig,   1},

            // for stair
            { { -393, -100 }, {1, 0.9}, LootType::None,       1},
            { { 915,  -100 }, {1, 0.9}, LootType::None,       1}
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
                { { 1410.5, -64 }, LootType::None, GA_RESOURCE_DIR"/background/block/block-3.png" },
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

        // block for hidden item
        auto block = std::make_shared<Block>(glm::vec2(950, -64), glm::vec2(1.025, 0.90), GA_RESOURCE_DIR"/background/block/block-3.png");
        block->SetZIndex(8);
        m_Blocks.push_back(block);
        m_All.push_back(block);

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

        // character bubbles
        std::string bubbleImage = GA_RESOURCE_DIR"/enemies/fishman/bubble.png";
        for (int i = 0; i < 3; i++) {
            bubblesVector.push_back(std::make_shared<ImageItems>(bubbleImage, glm::vec2(0.8, 0.8)));
            bubblesVector[i]->SetVisible(false);
            bubblesVector[i]->SetZIndex(7.5);
            m_All.push_back(bubblesVector[i]);
        }
    }

    //character
    if (!app->m_Character) {
        CharacterValue charactervalue;
        charactervalue.position = glm::vec2(-324.2, 315.85);
        charactervalue.direction = "right";
        charactervalue.beIndex = 2;
        app->m_Character = std::make_shared<Character>(charactervalue);
    }
    std::vector<std::shared_ptr<Torch>> pTorches;
    for (auto& t : m_Torches) {
        if (t->GetLootType() == LootType::None) {
            t->SetVisible(false);
            pTorches.push_back(t);
        }
    }
    if (this->m_Character == nullptr) {
        if (app->stairNum[0] == 1) {
            this->m_Character = app->m_Character;
            UpdateScroll(mapWidth, 1050);
        }
    } else {
        if (app->stairNum[1] == 0 && app->stairNum[0] == 1)
            UpdateScroll(mapWidth, 1050);
        else if (app->stairNum[1] == 1 && app->stairNum[0] == 0)
            UpdateScroll(mapWidth, pTorches[0]->GetPosition().x + 393);
        else if (app->stairNum[1] == 1 && app->stairNum[0] == 1)
            UpdateScroll(mapWidth, pTorches[1]->GetPosition().x + 135);
    }
    app->m_Menu->modifyHealth(app->m_Character->GetHeart(), "player");
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
    Position::PrintObjectCoordinate(m_Character, offsetX);
    m_Character->Keys(m_Blocks, m_Stairs, app->m_Menu->m_value.time);
    UpdateTorch(app);
    m_EnemiesManager->Update(offsetX, screenWidth, m_Character, m_Blocks, app, m_Loots);
    UpdateSubWeapon(app);
    UpdateScroll(mapWidth);
    UpdateHitableBlock(app);
    HiddenItem(app);
    if (m_Character->GetPosition().y > 300  && m_Character->GetDirection() == "left") {
        app->stairNum[1] = (m_Character->GetPosition().x < -100) ? 0 : 1;
        app->m_AppState = App::AppState::START;
        app->m_GameState = App::GameState::STAGE2A;
        app->RemoveAllChildren(m_All, m_Loots);
        app->m_Root.RemoveChild(m_Character->m_Behavior);
        m_EnemiesManager->RemoveAllChild(app);
    }else if (m_Character->GetStartDeadFlag() || ((app->m_Menu->GetTime() == 0 || m_Character->GetPosition().y < -240) && !isTimeOut)) {
        app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/deadBGM.wav");
        app->BGM->Play(1);
        isTimeOut = true;
    }
    if (app->SwitchStage() || m_Character->GetEndSceneFlag()) {
        m_Character->m_Behavior->SetLooping(false);
        m_stateState = StateState::END;
    }else if (m_Character->GetPosition().y < -240) {
        m_Character->m_Behavior->SetVisible(false);
        m_Character->SetDeadFlag(true);
        if (!Bubble()) {
            m_Character->m_Behavior->SetLooping(false);
            m_stateState = StateState::END;
        }
    }
}

void Stage2b::End(App *app) {
    // dead and reset
    m_EnemiesManager->RemoveAllChild(app);
    SceneReset(app);
    app->m_Root.RemoveChild(m_Character->m_Behavior);
    if (!switchStage)
        app->m_GameState = App::GameState::STAGE2A;
}

bool Stage2b::Bubble() {
    bool isBubble = true;
    bool check = false;
    if (bbl_vel[0] == 0) {
        glm::vec2 pos = m_Character->GetPosition();
        for (int i = 0; i < 3; i++) {
            bubblesVector[i]->SetPosition({pos.x, -220});
            bubblesVector[i]->SetVisible(true);
        }
        std::shared_ptr<Util::SFX> soundEft= std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/15.wav");
        soundEft->Play();
        soundEft->SetVolume(30);
    }
    for (int i = 0; i < 3; i++) {
        glm::vec2 pos = bubblesVector[i]->GetPosition();
        float x = (i == 0) ? -1.8 :((i == 1) ? 1.0 : 1.8);
        bbl_vel[i] = std::max(bbl_vel[i] - ((bbl_vel[i] >= 0) ? 1.2f : 1.8f), -5.0f);
        bubblesVector[i]->SetPosition({pos.x + x, pos.y + bbl_vel[i]});
        if (pos.y > -800)
            check = true;
    }
    if (!check) {
        bbl_vel = {0, 12.0f, 0};
        isBubble = false;
    }
    return isBubble;
}

void Stage2b::HiddenItem(App *app) {
    if (m_HitableBlocks[0]->IsDestroyed() && m_Character->GetPosition().y < -230 && m_Character->IfDuck()) {
        if (hiddenLootTime == 0) {
            hiddenLootTime = SDL_GetPerformanceCounter();
            m_HitableBlocks[0]->loot = Loot::CreateLoot(LootType::SpecialBag, glm::vec2(950 - offsetX, -64));
            m_HitableBlocks[0]->loot->SetZIndex(7);
            m_HitableBlocks[0]->loot->SetFallFlag(false);
            std::shared_ptr<Util::SFX> specialBagAppear = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/34.wav");
            specialBagAppear->SetVolume(50);
            specialBagAppear->Play();
            app->m_Root.AddChild(m_HitableBlocks[0]->loot);
            m_All.push_back(m_HitableBlocks[0]->loot);
        }
    }
    if (hiddenLootTime != 0 && Time::GetRunTimeMs(hiddenLootTime) > 6000.0f && m_HitableBlocks[0]->loot) {
        m_HitableBlocks[0]->loot->SetVisible(false);
        m_HitableBlocks[0]->loot->SetPosition({0, -300});
    } else if (hiddenLootTime != 0) {
        if (m_HitableBlocks[0]->loot->GetPosition().y < -10) {
            m_HitableBlocks[0]->loot->SetPosition({m_HitableBlocks[0]->loot->GetPosition().x, m_HitableBlocks[0]->loot->GetPosition().y + 0.5f});
        }
    }
}