#include "State/Stage2.hpp"
#include "Utility/Position.hpp"

void Stage2a::Start(App *app) {
   // menu
   app->m_Menu->SetMenuVisibility(true);

   // background
   m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-2/crypt-a.png");
   m_Background->m_Transform.scale = glm::vec2(1.025, 0.90);
   m_Background->SetPosition({525, -35});
   backgroundPos = m_Background->GetPosition();
   m_All.push_back(m_Background);

   //character
   if (!app->m_Character) {
      CharacterValue charactervalue;
      charactervalue.position = glm::vec2(-326.6, 80.76);
      charactervalue.direction = "right";
      charactervalue.beIndex = 2;
      app->m_Character = std::make_shared<Character>(charactervalue);
   }
   else
      app->m_Character->SetPosition({-326.6, 80.76});
   this->m_Character = app->m_Character;
   m_All.push_back(m_Character->m_Behavior);

   // block
   std::vector<BlockData> blocks = {
      { { 0,    -352 }, { 100,  0.7  } },
      { { -547, -50  }, { 0.4,  7    } },
      { { -515, -200 }, { 0.4,  4    } },
      { { -263, -5   }, { 4.26, 0.68 } },
      { { 128,  -122 }, { 2.12, 0.68 } },
      { { 658,  -130 }, { 2.13, 1.05 } },

      { { 689,  -240 }, { 1.62, 2.13 } },
      
      { { 722,  -67  }, { 3.18, 0.68 } },
      { { 1046, -122 }, { 2.12, 0.68 } },
      { { 1377, -5   }, { 3.19, 0.68 } },
      { { 1510, 228  }, { 1.08, 0.68 } },
      { { 1572, -50  }, { 0.4,  7    } },
   };
   for (auto& b : blocks) {
      auto block = std::make_shared<Block>(b.pos, b.scale);
      m_Blocks.push_back(block);
      m_All.push_back(block);
   }

   // stair
   std::vector<StairData> stairs = {
      { { 193,  -67  }, { 459,  -297 } },
      { { 1111, -67  }, { 1377, -297 } },

      { { -327, -297 }, { -191, -410 } },
      { { 984,  -297 }, { 1120, -410 } },
   };
   for (auto& s : stairs) {
      auto stair = Stair::CreateStair(s.pos1, s.pos2);
      m_Stairs.insert(m_Stairs.end(), stair.begin(), stair.end());
      m_All.insert(m_All.end(), stair.begin(), stair.end());
   }
   app->AddAllChildren(m_All);
   m_stateState = StateState::UPDATE;
}

void Stage2a::Update(App *app) {
   m_Character->Keys(m_Blocks, m_Stairs);
   UpdateTorch(app);
   UpdateSubWeapon(app);
   UpdateScroll(mapWidth);
   Position::PrintObjectCoordinate(m_Character, offsetX);
}

void Stage2a::End(App *app) {
   
}
