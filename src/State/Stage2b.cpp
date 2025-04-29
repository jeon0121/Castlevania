#include "State/Stage2.hpp"

void Stage2b::Start(App *app) {
    if (m_Blocks.empty()) {
        // background
        m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-2/crypt-b.png");
        m_Background->m_Transform.scale = glm::vec2(1.025, 0.90);
        m_Background->SetPosition({525, -35});
        backgroundPos = m_Background->GetPosition();
        m_All.push_back(m_Background);

        // block
        std::vector<BlockData> blocks = {
            { { -65,  52   }, { 1.08, 0.68 } },
            { { -65,  -64  }, { 7.5,  0.68 } },
            { { 590,  -64  }, { 1.08, 0.68 } },
            { { 1115, -64  }, { 5.4,  0.68 } },
            { { 1375, -296 }, { 1.08, 0.68 } },
            { { 1510, -180 }, { 1.08, 0.68 } },

            { { -545,  65 }, { 0.4,  4 } },
            { { 1565,  50  }, { 0.4,  5 } },

            { { -325,  240 }, { 0.5, 0.15 } }//changescene block
         };
        for (auto& b : blocks) {
            auto block = std::make_shared<Block>(b.pos, b.scale);
            m_Blocks.push_back(block);
            m_All.push_back(block);
        }

        //stair
        std::vector<StairData> stairs = {
            { { -325,  280  }, { -133,  100 } }
         };
        for (auto& s : stairs) {
            auto stair = Stair::CreateStair(s.pos1, s.pos2);
            m_Stairs.insert(m_Stairs.end(), stair.begin(), stair.end());
            m_All.insert(m_All.end(), stair.begin(), stair.end());
        }
    }

    //character
    this->m_Character = app->m_Character;
    m_Character->SetPosition({-324.2, 315.85});
    m_Character->SetOffStairs();
    std::shared_ptr<Stair> stair = m_Character->CollideStair(m_Stairs);
    m_Character->Descending(stair);
    app->m_Root.AddChild(m_Character->m_Behavior);

    app->AddAllChildren(m_All);
    m_stateState = StateState::UPDATE;
}

void Stage2b::Update(App *app) {
    m_Character->Keys(m_Blocks, m_Stairs);
    // UpdateTorch(app);
    // UpdateSubWeapon(app);
    UpdateScroll(mapWidth);
    if (m_Character->GetPosition().y > 300  && m_Character->GetDirection() == "left") {
        app->m_Character = this->m_Character;
        app->m_AppState = App::AppState::START;
        app->m_GameState = App::GameState::STAGE2A;
        app->RemoveAllChildren(m_All);
        app->m_Root.RemoveChild(m_Character->m_Behavior);
    }
}

void Stage2b::End(App *app) {
   
}
