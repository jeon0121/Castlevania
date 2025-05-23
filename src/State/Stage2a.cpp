#include "State/Stage2.hpp"
#include "Utility/Position.hpp"

void Stage2a::Start(App *app) {
   if (m_Blocks.empty()) {
      // menu
      app->m_Menu->SetMenuVisibility(true);

      // background
      m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-2/crypt-a.png");
      m_Background->m_Transform.scale = glm::vec2(1.025, 0.90);
      m_Background->SetPosition({525, -35});
      backgroundPos = m_Background->GetPosition();
      m_All.push_back(m_Background);

      // blink
      std::vector<std::string> blinkImages = {GA_RESOURCE_DIR"/background/stage-2/crypt-a-blink.png", GA_RESOURCE_DIR"/background/stage-2/crypt-a.png"};
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
      app->m_Root.AddChild(m_Character->m_Behavior);

      // EnemiesManager
      std::vector<PossibleLootData> possibleLoots = {
         {LootType::HeartSmall, 0.5, -1},
         {LootType::Stopwatch,  0.2,  1},
         {LootType::None,       1.0, -1},
      };
      m_EnemiesManager = std::make_shared<EnemiesManager>(possibleLoots);

      // bats
      m_EnemiesManager->AddBat({screenWidth * 0.5, m_Character->GetPosition().y}, "left", app);

      //torch
      std::vector<TorchData> torchs = {
         { { -131, 106  }, {1, 0.9}, LootType::HeartBig,   1},
         { { 130,  167  }, {1, 0.9}, LootType::HeartSmall, 1},
         { { -393,-240  }, {1, 0.9}, LootType::WhiteBag,   1},
         { { 394, -125  }, {1, 0.9}, LootType::RedBag,     1},
         { { 721,  163  }, {1, 0.9}, LootType::HeartBig,   1},
         { { 919,  -240 }, {1, 0.9}, LootType::RedBag,     1},
         { { 1180, 220  }, {1, 0.9}, LootType::Rosary,     1},
         { { 1443, 105  }, {1, 0.9}, LootType::RedBag,     1}
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

         { { -191, -450 }, { 0.5,  0.15 } }//changescene block
      };
      for (auto& b : blocks) {
         auto block = std::make_shared<Block>(b.pos, b.scale);
         m_Blocks.push_back(block);
         // m_All.push_back(block);
      }
      // hitable block
      std::vector<std::vector<HitableBlockData>> hitableBlocks = {
         { 
            { { 558, -296 }, LootType::Chicken, GA_RESOURCE_DIR"/background/block/block-2.png" },
            { { 558, -237 }, LootType::None   , GA_RESOURCE_DIR"/background/block/block-1.png" },
         },
      };
      for (auto& hb : hitableBlocks) {
         auto hitableBlock = std::make_shared<HitableBlock>(LootType::Chicken);
         for (auto& b : hb) {
            auto block = std::make_shared<Block>(b.pos, glm::vec2(1.025, 0.90), b.path);
            m_Blocks.push_back(block);
            m_All.push_back(block);
            hitableBlock->AddBlock(block, b.loot);
         }
         m_HitableBlocks.push_back(hitableBlock);
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
         // m_All.insert(m_All.end(), stair.begin(), stair.end());
      }
      m_stateState = StateState::UPDATE;
   }else {
      if (app->stairNum[0] == 1 && app->stairNum[1] == 0)
         UpdateScroll(mapWidth, -1050);
      else if (app->stairNum[0] == 0 && app->stairNum[1] == 1)
         UpdateScroll(mapWidth, 1050);
      m_Character->SetPosition({((m_Character->GetPosition().x < -100) ? -189.9 : 70.1), -378.65});
      m_Character->SetOffStairs();
      std::shared_ptr<Stair> stair = m_Character->CollideStair(m_Stairs);
      m_Character->Ascending(stair);
      app->m_Root.AddChild(m_Character->m_Behavior);
   }
   app->AddAllChildren(m_All);
   m_stateState = StateState::UPDATE;
}

void Stage2a::Update(App *app) {
   if (blinkStartTime != 0)
      Blink();
   m_Character->Keys(m_Blocks, m_Stairs);
   UpdateTorch(app);
   m_EnemiesManager->Update(offsetX, screenWidth, m_Character, m_Blocks, app);
   UpdateSubWeapon(app);
   UpdateScroll(mapWidth);
   UpdateHitableBlock(app);
   // Position::PrintObjectCoordinate(m_Character, offsetX);
   if (m_Character->GetPosition().y < -380 && m_Character->GetDirection() == "right") {
      app->stairNum[0] = (m_Character->GetPosition().x < -100) ? 0 : 1;
      app->m_AppState = App::AppState::START;
      app->m_GameState = App::GameState::STAGE2B;
      app->RemoveAllChildren(m_All);
      app->m_Root.RemoveChild(m_Character->m_Behavior);
      m_EnemiesManager->RemoveAllEnemies(app);
   }

}

void Stage2a::End(App *app) {
   
}
