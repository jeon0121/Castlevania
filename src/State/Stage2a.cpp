#include "State/Stage2.hpp"
#include "Utility/Position.hpp"

void Stage2a::Start(App *app) {
   // menu
   app->m_Menu->SetStageNum(2);
   app->m_Menu->SetMenuVisibility(true);

   if (m_Blocks.empty()) {
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
            hitableBlock->AddBlock(block, b.loot, m_All);
         }
         m_HitableBlocks.push_back(hitableBlock);
      }
      // stair
      std::vector<StairData> stairs = {
         { { 193,  -67  }, { 459,  -297 } },
         { { 1111, -67  }, { 1377, -297 } },

         { { -327, -297 }, { -191, -450 } },
         { { 984,  -297 }, { 1120, -420 } },
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
   app->m_Menu->modifyHealth(app->m_Character->GetHeart(), "player");
   app->AddAllChildren(m_All);
   m_stateState = StateState::UPDATE;
}

void Stage2a::Update(App *app) {
   if (m_EnemiesManager->m_Enemies.size() == 0 && m_Character->GetPosition().y >= -266) {
      m_EnemiesManager->AddBat({screenWidth * 0.5, m_Character->GetPosition().y}, "left", app);
      m_EnemiesManager->AddAllChild(app);
   }
   if (blinkStartTime != 0)
      Blink();
   m_Character->Keys(m_Blocks, m_Stairs, app->m_Menu->m_value.time);
   UpdateTorch(app);
   m_EnemiesManager->Update(offsetX, screenWidth, m_Character, m_Blocks, app, m_Loots);
   UpdateSubWeapon(app);
   UpdateScroll(mapWidth);
   UpdateHitableBlock(app);
   // Position::PrintObjectCoordinate(m_Character, offsetX);

   //change to stage2B
   if (m_Character->GetPosition().y < -380 && m_Character->GetDirection() == "right") {
      app->stairNum[0] = (m_Character->GetPosition().x < -100) ? 0 : 1;
      app->m_AppState = App::AppState::START;
      app->m_GameState = App::GameState::STAGE2B;
      app->RemoveAllChildren(m_All, m_Loots);
      app->m_Root.RemoveChild(m_Character->m_Behavior);
      m_EnemiesManager->RemoveAllChild(app);
   }
   //character dead bgm
   else if (m_Character->GetStartDeadFlag() || (app->m_Menu->GetTime() == 0 && !isTimeOut)) {
      app->BGM->LoadMedia(GA_RESOURCE_DIR "/BGM/deadBGM.wav");
      app->BGM->Play(1);
      isTimeOut = true;
   }
   if (app->SwitchStage() || m_Character->GetEndSceneFlag() || (m_Character->GetPosition().x >= 422 && m_Character->GetPosition().y > 80.75 && m_Character->GetPosition().y < 80.77)) {
      m_Character->m_Behavior->SetLooping(false);
      m_stateState = StateState::END;
   }
}

void Stage2a::End(App *app) {
    // dead and reset
    if (switchStage || m_Character->GetEndSceneFlag()) {
        m_EnemiesManager->RemoveAllChild(app);
        SceneReset(app);
        app->m_Root.RemoveChild(m_Character->m_Behavior);
    }
    // end scene animation
    else {
        std::shared_ptr<Util::SFX> doorSound = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/29.wav");
        doorSound->SetVolume(50);
        EndAnimation(app, GA_RESOURCE_DIR"/background/stage-2/end.png", glm::vec2(0.537, 0.473), door_1, door_2, doorSound);
        if (app->m_AppState == App::AppState::START)
            app->m_Root.RemoveChild(m_Character->m_Behavior);
        app->m_GameState = App::GameState::STAGE3;
    }
}
