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
    CharacterValue charactervalue;
    charactervalue.position = glm::vec2(-315, -265.35);
    charactervalue.direction = "right";
    charactervalue.beIndex = 2;
    m_Character = std::make_shared<Character>(charactervalue);
    m_All.push_back(m_Character->m_Behavior);

    //torch
    // std::shared_ptr<Torch> torch_1 = std::make_shared<Torch>(glm::vec2(-130, -242), glm::vec2(1, 0.9), LootType::HeartBig, 2);
    // std::shared_ptr<Torch> torch_2 = std::make_shared<Torch>(glm::vec2(395, -242), glm::vec2(1, 0.9), LootType::Whip, 2);
    // std::shared_ptr<Torch> torch_3 = std::make_shared<Torch>(glm::vec2(920, -242), glm::vec2(1, 0.9), LootType::Whip, 2);
    // std::shared_ptr<Torch> torch_4 = std::make_shared<Torch>(glm::vec2(1445, -242), glm::vec2(1, 0.9), LootType::HeartBig, 2);
    // std::shared_ptr<Torch> torch_5 = std::make_shared<Torch>(glm::vec2(1965, -242), glm::vec2(1, 0.9), LootType::Dagger, 2);
    // m_Torches.push_back(torch_1);
    // m_Torches.push_back(torch_2);
    // m_Torches.push_back(torch_3);
    // m_Torches.push_back(torch_4);
    // m_Torches.push_back(torch_5);
    // m_All.push_back(torch_1);
    // m_All.push_back(torch_2);
    // m_All.push_back(torch_3);
    // m_All.push_back(torch_4);
    // m_All.push_back(torch_5);

    //block
    std::shared_ptr<Block> block_1 = std::make_shared<Block>(glm::vec2(0, -352), glm::vec2(100, 0.7));
    std::shared_ptr<Block> block_2 = std::make_shared<Block>(glm::vec2(-545, -50), glm::vec2(0.4, 7));
    std::shared_ptr<Block> block_3 = std::make_shared<Block>(glm::vec2(2395, -122), glm::vec2(1.61, 0.66));
    std::shared_ptr<Block> block_4 = std::make_shared<Block>(glm::vec2(2888, -5), glm::vec2(5.32, 0.66));
    std::shared_ptr<Block> block_5 = std::make_shared<Block>(glm::vec2(3478, -122), glm::vec2(3.22, 0.66));
    std::shared_ptr<Block> block_6 = std::make_shared<Block>(glm::vec2(5670, -50), glm::vec2(0.4, 7));
    m_Blocks.push_back(block_1);
    m_Blocks.push_back(block_2);
    m_Blocks.push_back(block_3);
    m_Blocks.push_back(block_4);
    m_Blocks.push_back(block_5);
    m_Blocks.push_back(block_6);
    m_All.push_back(block_1);
    m_All.push_back(block_2);
    m_All.push_back(block_3);
    m_All.push_back(block_4);
    m_All.push_back(block_5);
    m_All.push_back(block_6);


    app->AddAllChildren(m_All);
    m_stateState = StateState::UPDATE;
}

void Stage1::Update(App* app){
    m_Character->Keys();
    m_Character->CollideBoundary(m_Blocks);
    // UpdateTorch(app);
    UpdateSubWeapon(app);
    UpdateScroll(mapWidth);
}

void Stage1::End(App* app){

}
