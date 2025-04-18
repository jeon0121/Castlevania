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

    //EnemiesManager
    m_EnemiesManager = std::make_shared<EnemiesManager>();

    // zombies
    std::vector<std::tuple<glm::vec2, int, float>> zombies = {
        { { -500, 1215 }, 3, -265.35f },
        { { 4030, 6144 }, 3, -265.35f }
    };
    for (auto& z : zombies) {
        auto [range, numZombie, yPos] = z;
        m_EnemiesManager->AddZombie(range, numZombie, {screenWidth * 0.5, yPos}, "left", app);
    }

    //leopards
    std::vector<glm::vec2> leopards = {
        { 2355, -69 },
        { 3150, 50  },
        { 3450, -69 }
    };
    for (auto& l : leopards) {
        m_EnemiesManager->AddLeopard(l, "left", app);
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
        { { 1704, -240 }, {1, 0.9}, LootType::HeartSmall, 1}
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
        { { 5414, -5   }, { 3.73, 0.68 } }
    };
    for (auto& b : blocks) {
        auto block = std::make_shared<Block>(b.pos, b.scale);
        m_Blocks.push_back(block);
        m_All.push_back(block);
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
        m_All.insert(m_All.end(), stair.begin(), stair.end());
    }
    
    app->AddAllChildren(m_All);
    m_stateState = StateState::UPDATE;
}

void Stage1::Update(App* app){
    m_Character->Keys(m_Blocks, m_Stairs);
    m_EnemiesManager->Update(offsetX, screenWidth, m_Character, m_Blocks, app);
    UpdateTorch(app);
    UpdateSubWeapon(app);
    UpdateScroll(mapWidth);
    if (m_Character->GetEndSceneFlag())
        m_stateState = StateState::END;
}

void Stage1::End(App* app){
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
}
