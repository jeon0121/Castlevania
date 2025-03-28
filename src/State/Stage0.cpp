#include "State/Stage0.hpp"
#include "Util/Logger.hpp"

void Stage0::Start(App* app){
    // menu
    MenuValue menuvalue;
    m_Menu = std::make_shared<Menu>(menuvalue);
    m_All.push_back(m_Menu);
    m_All.push_back(m_Menu->background);
    for (auto &&num : m_Menu->numberImage){
        for (auto &&letter : num) app->m_Root.AddChild(letter);
    }
    for (auto &&txt : m_Menu->textImage){
        app->m_Root.AddChild(txt);
    }
    for (auto &&healthbar : m_Menu->health){
        for (auto &&h : healthbar) app->m_Root.AddChild(h);
    }
    // background
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-0/castle-entrance.png");
    m_Background->m_Transform.scale = glm::vec2(1.025, 0.92);
    m_Background->SetPosition({1050, -60});
    m_All.push_back(m_Background);

    //character
    CharacterValue charactervalue;
    charactervalue.position = glm::vec2(-315, -235.35);
    charactervalue.direction = "right";
    charactervalue.beIndex = 2;
    m_Character = std::make_shared<Character>(charactervalue);
    m_All.push_back(m_Character->m_Behavior);

    //torch
    std::shared_ptr<Torch> torch_1 = std::make_shared<Torch>(glm::vec2(395, -242), glm::vec2(1, 0.9), LootType::Whip, 2);
    std::shared_ptr<Torch> torch_2 = std::make_shared<Torch>(glm::vec2(255, -242), glm::vec2(1, 0.9), LootType::Whip, 2);
    std::shared_ptr<Torch> torch_3 = std::make_shared<Torch>(glm::vec2(-100, -242), glm::vec2(1, 0.9), LootType::Axe, 2);
    m_Torches.push_back(torch_1);
    m_Torches.push_back(torch_2);
    m_Torches.push_back(torch_3);
    m_All.push_back(torch_1);
    m_All.push_back(torch_2);
    m_All.push_back(torch_3);

    //block
    std::shared_ptr<Block> block_1 = std::make_shared<Block>(glm::vec2(0, -322), glm::vec2(100, 0.7));
    std::shared_ptr<Block> block_2 = std::make_shared<Block>(glm::vec2(-545, -50), glm::vec2(0.4, 7));
    // std::shared_ptr<Block> block_3 = std::make_shared<Block>(glm::vec2(0, -203), glm::vec2(1, 0.7));
    // std::shared_ptr<Block> block_4 = std::make_shared<Block>(glm::vec2(190, -80), glm::vec2(1, 0.7));
    m_Blocks.push_back(block_1);
    m_Blocks.push_back(block_2);
    // m_Blocks.push_back(block_3);
    // m_Blocks.push_back(block_4);
    // m_All.push_back(block_1);
    // m_All.push_back(block_2);
    // m_All.push_back(block_3);
    // m_All.push_back(block_4);


    app->AddAllChildren(m_All);
    m_stateState = StateState::UPDATE;
}

void Stage0::Update(App* app){
    m_Character->Keys();
    m_Character->CollideBoundary(m_Blocks);
    UpdateTorch(app);
    // UpdateSubWeapon(app);
    UpdateScroll(mapWidth);
}

void Stage0::End(App* app){
    (void) app;

    // //End Stage0
    // if (m_Character->m_Behavior->IfPlayingTime(8)) {
    //     app->m_Root.RemoveAllChildren();
    //     app->m_AppState = App::AppState::START;
    //     app->m_GameState = App::GameState::STAGE1;
    // }
}
