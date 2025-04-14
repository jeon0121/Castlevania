#include "State/Stage0.hpp"
#include "Util/Logger.hpp"

void Stage0::Start(App* app){
    // menu
    app->m_Menu->SetMenuVisibility(true);

    // background
    m_Background = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-0/castle-entrance.png");
    m_Background->m_Transform.scale = glm::vec2(1.025, 0.92);
    m_Background->SetPosition({1050, -45});
    backgroundPos = m_Background->GetPosition();
    m_All.push_back(m_Background);

    m_End = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/background/stage-0/end.png");
    m_End->m_Transform.scale = glm::vec2(1.025, 0.92);
    m_End->SetPosition({441.5, -44.5});
    m_End->SetZIndex(8);
    m_End->SetVisible(false);
    m_All.push_back(m_End);

    //character
    if (!app->m_Character) {
        CharacterValue charactervalue;
        charactervalue.position = glm::vec2(-315, -220.35);
        charactervalue.direction = "right";
        charactervalue.beIndex = 2;
        app->m_Character = std::make_shared<Character>(charactervalue);
    }else {
        app->m_Character->SetPosition({-315, -220.35});
        app->m_Character->m_Behavior->m_Transform.scale = glm::vec2(1.0f, 1.0f);
        app->m_Character->SetDirection("right");
        app->m_Character->Flip();
        app->m_Character->SetZIndex(2);
    }
    this->m_Character = app->m_Character;
    m_All.push_back(m_Character->m_Behavior);

    //torch
    std::shared_ptr<Torch> torch_1 = std::make_shared<Torch>(glm::vec2(-130, -227), glm::vec2(1, 0.9), LootType::HeartBig, 2);
    std::shared_ptr<Torch> torch_2 = std::make_shared<Torch>(glm::vec2(395, -227), glm::vec2(1, 0.9), LootType::Whip, 2);
    std::shared_ptr<Torch> torch_3 = std::make_shared<Torch>(glm::vec2(920, -227), glm::vec2(1, 0.9), LootType::Whip, 2);
    std::shared_ptr<Torch> torch_4 = std::make_shared<Torch>(glm::vec2(1445, -227), glm::vec2(1, 0.9), LootType::HeartBig, 2);
    std::shared_ptr<Torch> torch_5 = std::make_shared<Torch>(glm::vec2(1965, -227), glm::vec2(1, 0.9), LootType::Dagger, 2);
    m_Torches.push_back(torch_1);
    m_Torches.push_back(torch_2);
    m_Torches.push_back(torch_3);
    m_Torches.push_back(torch_4);
    m_Torches.push_back(torch_5);
    for (auto& torch : m_Torches)
        m_All.push_back(torch);

    //block
    std::shared_ptr<Block> block_1 = std::make_shared<Block>(glm::vec2(0, -307), glm::vec2(100, 0.7));
    std::shared_ptr<Block> block_2 = std::make_shared<Block>(glm::vec2(-545, -50), glm::vec2(0.4, 7));
    m_Blocks.push_back(block_1);
    m_Blocks.push_back(block_2);

    app->AddAllChildren(m_All);
    m_stateState = StateState::UPDATE;
}

void Stage0::Update(App* app){
    m_Character->Keys(m_Blocks, m_Stairs);
    UpdateTorch(app);
    UpdateSubWeapon(app);
    UpdateScroll(mapWidth);

    if (m_Character->GetPosition().x >= 200) {
        m_Character->ChangeBehavior(0);
        m_Character->m_Behavior->SetPlaying();
        m_Character->m_Behavior->SetLooping(true);
        m_End->SetVisible(true);
        m_stateState = StateState::END;
    }
}

void Stage0::End(App* app){
    glm::vec2 pos = m_Character->GetPosition();
    m_Character-> SetPosition(glm::vec2(pos.x+1.5f, pos.y));

    if (m_Character->GetPosition().x >= 500) {
        app->m_Menu->SetMenuVisibility(false);
        app->RemoveAllChildren(m_All);
        app->m_AppState = App::AppState::START;
        app->m_GameState = App::GameState::STAGE1;
    }
}
