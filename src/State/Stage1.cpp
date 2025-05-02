#include "State/Stage1.hpp"
#include "Util/Logger.hpp"

void Stage1::Start(App* app){
    // menu
    app->m_Menu->SetMenuVisibility(true);

    // background
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-1/entrance-hall.png");
    m_Background->m_Transform.scale = glm::vec2(1.025, 0.90);
    m_Background->SetPosition({2625, -35});
    backgroundPos = m_Background->GetPosition();
    m_All.push_back(m_Background);

    // blink
    std::vector<std::string> blinkImages = {GA_RESOURCE_DIR"/background/stage-1/entrance-hall-blink.png", GA_RESOURCE_DIR"/background/stage-1/entrance-hall.png"};
    m_Blink = std::make_shared<AnimatedItems>(blinkImages, 40, m_Background->m_Transform.scale);
    m_Blink->SetZIndex(1);
    m_Blink->SetPosition(m_Background->GetPosition());
    m_Blink->SetVisible(false);
    m_All.push_back(m_Blink);

    //character
    if (!app->m_Character) {
        CharacterValue charactervalue;
        charactervalue.position = glm::vec2(-315, -265.35);
        charactervalue.direction = "right";
        charactervalue.beIndex = 2;
        app->m_Character = std::make_shared<Character>(charactervalue);
    }
    else
        app->m_Character->SetPosition({-315, -265.35});
    this->m_Character = app->m_Character;
    m_All.push_back(m_Character->m_Behavior);

    // EnemiesManager
    std::vector<PossibleLootData> possibleLoots = {
        {LootType::HeartSmall, 0.5, -1}, // infinite drop
        {LootType::HolyWater,  0.2,  1}, // max drop is 1 on stage 1
        {LootType::Stopwatch,  0.2,  1}, // max drop is 1 on stage 1
        {LootType::None,       1.0, -1}, // infinite drop
    };
    m_EnemiesManager = std::make_shared<EnemiesManager>(possibleLoots);

    // zombies
    std::vector<ZombieData> zombies = {
        { { -500, 1215 }, 3, -265.35f },
        { { 4030, 6144 }, 3, -265.35f }
    };
    for (auto& z : zombies) {
        m_EnemiesManager->AddZombie(z.range, z.numZombie, {screenWidth * 0.5, z.yPos}, "left", app);
    }

    //leopards
    std::vector<LeopardData> leopards = {
        { {2355, -69} },
        { {3150, 50 } },
        { {3450, -69} }
    };
    for (auto& l : leopards) {
        m_EnemiesManager->AddLeopard(l.pos, "left", app);
    }
    
    //torch
    std::vector<TorchData> torchs = {
        { { -392, -240 }, {1, 0.9}, LootType::PurpleBag,  1},
        { { -130, -127 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 132,  -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 394,  -127 }, {1, 0.9}, LootType::RedBag,     1},
        { { 656,  -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 918,  -127 }, {1, 0.9}, LootType::RedBag,     1},
        { { 1180, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 1442, -127 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 1704, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 1964, -125 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 2227, 105  }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 2490, -240 }, {1, 0.9}, LootType::WhiteBag,   1},
        { { 2753, 210  }, {1, 0.9}, LootType::HolyWater,  1},
        { { 3015, 105  }, {1, 0.9}, LootType::HeartBig,   1},
        { { 3277, -240 }, {1, 0.9}, LootType::WhiteBag,   1},
        { { 3539, 105  }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 3802, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 4064, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 4327, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 4589, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 4851, -240 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 5114, -125 }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 5376, 105  }, {1, 0.9}, LootType::HeartSmall, 1},
        { { 5639, 222  }, {1, 0.9}, LootType::Rosary,     1}
    };
    for (auto& t : torchs) {
        auto torch = std::make_shared<Torch>(t.pos, t.scale, t.loot, t.type);
        m_Torches.push_back(torch);
        m_All.push_back(torch);
    }

    // block
    std::vector<BlockData> blocks = {
        { { 0,    -352 }, { 100,  0.7  } },
        { { -545, -50  }, { 0.4,  7    } },
        { { 2395, -122 }, { 1.61, 0.68 } },
        { { 2888, -5   }, { 5.32, 0.68 } },
        { { 3478, -122 }, { 3.22, 0.68 } },
        { { 5484, -5   }, { 4.82, 0.68 } },
        { { 5764, -50  }, { 0.4,  7    } }
    };
    for (auto& b : blocks) {
        auto block = std::make_shared<Block>(b.pos, b.scale);
        m_Blocks.push_back(block);
        // m_All.push_back(block);
    }

    // stair
    std::vector<StairData> stairs = {
        { { 2035, -297 }, { 2296, -69 } },
        { { 2430, -67  }, { 2560, 45  } },
        { { 3220, 45   }, { 3350, -67 } },
        { { 4790, -297 }, { 5180, 50  } }
    };
    for (auto& s : stairs) {
        auto stair = Stair::CreateStair(s.pos1, s.pos2);
        m_Stairs.insert(m_Stairs.end(), stair.begin(), stair.end());
        // m_All.insert(m_All.end(), stair.begin(), stair.end());
    }

    //door
    std::vector<std::string> door1Imgs, door2Imgs;
    for (int i = 1; i <= 2; ++i) {
        door1Imgs.push_back(GA_RESOURCE_DIR "/items/door/door-" + std::to_string(i) + ".png");
        door2Imgs.push_back(GA_RESOURCE_DIR "/items/door/door-" + std::to_string(3 - i) + ".png");
    }
    door_1 = std::make_shared<AnimatedItems>(door1Imgs, 200, glm::vec2{1.0f, 0.9f});
    door_2 = std::make_shared<AnimatedItems>(door2Imgs, 200, glm::vec2{1.0f, 0.9f});
    for (auto& door : {door_1, door_2}) {
        door->SetPosition({15.0f, 110.0f});
        door->SetVisible(false);
        door->SetZIndex(9);
        m_All.push_back(door);
    }
    
    app->AddAllChildren(m_All);
    m_stateState = StateState::UPDATE;
}

void Stage1::Update(App* app){
    if (blinkStartTime != 0)
        Blink();
    m_Character->Keys(m_Blocks, m_Stairs);
    UpdateTorch(app);
    m_EnemiesManager->Update(offsetX, screenWidth, m_Character, m_Blocks, app);
    UpdateSubWeapon(app);
    UpdateScroll(mapWidth);
    if (m_Character->GetEndSceneFlag() || (m_Character->GetPosition().x >= 422 && m_Character->GetPosition().y > 80.75 && m_Character->GetPosition().y < 80.77)) {
        m_Character->m_Behavior->SetLooping(false);
        m_stateState = StateState::END;
    }
}

void Stage1::End(App* app){
    // dead and reset
    if (m_Character->GetEndSceneFlag()) {
        app->m_Menu->SetMenuVisibility(false);
        m_EnemiesManager->RemoveAllEnemies(app);
        app->m_Menu->modifyWeapon(WeaponType::None);
        app->m_Menu->modifyNumber(app->m_Menu->formatTwoDigits(5), 3);
        app->m_Character = nullptr;
        app->RemoveAllChildren(m_All);
        app->m_AppState = App::AppState::START;
        app->m_GameState = App::GameState::STAGE1;
    }
    // end scene animation
    else {
        if (m_Background->m_Transform.scale.x > 1.0f) {
            m_Character->ChangeBehavior(2);
            m_Background->SetDrawable(std::make_unique<Util::Image>(GA_RESOURCE_DIR"/background/stage-1/end.png"));
            m_Background->m_Transform.scale = glm::vec2(0.528, 0.465);
            m_Background->SetPosition({522.5, -36.2});
            m_Background->SetZIndex(8);
        }else if (m_Background->GetPosition().x > 0) {
            m_Background->SetPosition({m_Background->GetPosition().x - 4.0f, m_Background->GetPosition().y});
            m_Character->SetPosition({m_Character->GetPosition().x - 4.0f, m_Character->GetPosition().y });
        }else if (m_Background->GetPosition().x < 0 && !door_2->IsLooping()) {
            if (!door_1->IsPlaying() && !m_Character->m_Behavior->IsLooping()) {
                door_1->SetVisible(true);
                door_1->SetPlaying();
            }
            if (door_1->IfAnimationEnds() && !m_Character->m_Behavior->IsLooping()) {
                m_Character->ChangeBehavior(0);
                m_Character->m_Behavior->SetPlaying();
                m_Character->m_Behavior->SetLooping(true);
            }
            if (m_Character->GetPosition().x < 200 && m_Character->m_Behavior->IsLooping())
                m_Character->SetPosition({m_Character->GetPosition().x + 4.0f, m_Character->GetPosition().y });
            else if (m_Character->GetPosition().x > 200) {
                m_Character->ChangeBehavior(2);
                door_1->SetVisible(false);
                door_1->SetPaused();
                if (!door_2->IsPlaying()) {
                    door_2->SetVisible(true);
                    door_2->SetPlaying();
                }
                if (door_2->IfAnimationEnds()) {
                    door_2->SetLooping(true);
                }
            }
        } else {
            if (door_1->IsPlaying() && m_Background->GetPosition().x > -527) {
                m_Background->SetPosition({m_Background->GetPosition().x - 4.0f, m_Background->GetPosition().y});
                m_Character->SetPosition({m_Character->GetPosition().x - 4.0f, m_Character->GetPosition().y });
            } else if (!door_1->IsPlaying() && !door_2->IsPlaying()) {
                door_2->SetVisible(false);
                door_1->SetPlaying();
            } else if (m_Background->GetPosition().x < -527) {
                app->m_Menu->SetMenuVisibility(false);
                m_EnemiesManager->RemoveAllEnemies(app);
                app->RemoveAllChildren(m_All);
                app->m_AppState = App::AppState::START;
                app->m_GameState = App::GameState::STAGE2A;
            }
        }
    }
}