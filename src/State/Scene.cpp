#include "State/Scene.hpp"
#include "Util/Logger.hpp"

void Scene::UpdateTorch(App* app) {
    for (auto torch : m_Torches) 
        torch->Update(app, m_Character, m_Menu, m_Blocks, torch);
}

void Scene::UpdateScroll(int mapWidth) {
    glm::vec2 pos = m_Character->GetPosition();
    glm::vec2 lastPos = m_Character->GetLastPosition();
    float dx = pos.x - lastPos.x;
    if (((pos.x >= 4.5) && (dx > 0) && (offsetX < mapWidth - screenWidth)) ||
        ((pos.x <= -4.5) && (dx < 0) && (offsetX > 0))) {
        offsetX += dx;
        m_Background->SetPosition(glm::vec2(1050, -60) - glm::vec2(offsetX, 0.0f));
        m_Character->SetPosition(m_Character->GetPosition() - glm::vec2(dx, 0.0f));
        for (auto& block : m_Blocks) {
            block->SetPosition(block->GetPosition() - glm::vec2(dx, 0.0f));
        }
        for (auto& torch : m_Torches) {
            torch->SetPosition(torch->GetPosition() - glm::vec2(dx, 0.0f));
            if (torch->loot)
                torch->loot->SetPosition(torch->loot->GetPosition() - glm::vec2(dx, 0.0f));
        }
    }
}