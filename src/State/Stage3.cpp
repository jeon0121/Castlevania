#include "State/Stage3.hpp"
#include "Utility/Position.hpp"

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
    // std::vector<PossibleLootData> possibleLoots = {
    //     {LootType::HeartSmall, 0.5, -1}, // infinite drop
    // };
    // m_EnemiesManager = std::make_shared<EnemiesManager>(possibleLoots);

    // boss
    m_Boss = std::make_shared<PhantomBat>(glm::vec2(2020, 190));
    m_Boss->SetZIndex(7);
    m_All.push_back(m_Boss);

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

        { { 2610, -240 }, { 0.3,  2    } },
        { { 2610,  160 }, { 0.3,  2    } },
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

    app->AddAllChildren(m_All);
    m_stateState = StateState::UPDATE;
}

void Stage3::Update(App* app){
    Position::PrintObjectCoordinate(m_Character, offsetX);
    m_Character->Keys(m_Blocks, m_Stairs);
    UpdateTorch(app);
    UpdateSubWeapon(app);
    UpdateHitableBlock(app);
    // once reach the boss, stop scrolling
    if (m_Character->GetPosition().x <= 20 && !reachBoss) {
        UpdateScroll(mapWidth);
    }
    else if (m_Character->GetPosition().x > 20 && !reachBoss) {
        reachBoss = true;
        app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/bossBGM.wav");
        app->BGM->Play();
    }
    // boss
    else if (reachBoss) {
        whipDropped = true;
        m_Boss->MoveBehav(m_Character);
    }
}

void Stage3::End(App* app){

}