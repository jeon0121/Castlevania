#include "State/Menu.hpp"

Menu::Menu(int time, int score, int playerHeart, int enemyHeart, int playerAmmo, int playerLife, int currentStage)
    : GameObject(std::make_unique<Util::Image>(GA_RESOURCE_DIR "/transparent.png"), 0),
      time(time), score(score), playerHeart(playerHeart), enemyHeart(enemyHeart),
      playerAmmo(playerAmmo), playerLife(playerLife), currentStage(currentStage)
{
    background = std::make_shared<ImageItems>(GA_RESOURCE_DIR "/title/key/title-key-1.png");
    background->SetZIndex(50);
    background->SetPosition({0, 0});

    // score
    addText("score", {-390, 420});
    addNumber(formatScore(score), {-275, 420});

    // time
    addText("time", {0, 420});
    addNumber(formatTime(time), {100, 420});

    // stage
    addText("stage", {330, 420});
    addNumber(formatTwoDigits(currentStage), {450, 420});

    // heart and ammo
    addText("heart-n-life", {215, 372});
    addNumber(formatTwoDigits(playerAmmo), {265, 387});
    addNumber(formatTwoDigits(playerLife), {265, 355});

    // player health
    addText("player", {-390, 387});
    addHealth(playerHeart, {-260, 387}, "player");

    // enemy health
    addText("enemy", {-405, 355});
    addHealth(enemyHeart, {-260, 355}, "enemy");
}

std::string Menu::formatTime(int time) {
    int minutes = time / 60;
    int seconds = time % 60;
    return formatTwoDigits(minutes) + formatTwoDigits(seconds);
}

std::string Menu::formatTwoDigits(int num) {
    return (num < 10 ? "0" : "") + std::to_string(num);
}

std::string Menu::formatScore(int score) {
    std::string scoreStr = std::to_string(score);
    int zero = 6 - scoreStr.length();
    return std::string(zero, '0') + scoreStr;
}

void Menu::addText(std::string text, const glm::vec2& position){
    float x = position.x, y = position.y;
    std::string imagePath = GA_RESOURCE_DIR "/fonts/menu/" + text + ".png";
    auto characterImage = std::make_shared<ImageItems>(imagePath);
    characterImage->SetPosition({x, y});
    characterImage->SetZIndex(50);
    textImage.push_back(characterImage);
}

void Menu::addNumber(std::string text, const glm::vec2& position){
    std::vector<std::shared_ptr<ImageItems>> textRow;
    float x = position.x, y = position.y;
    for (char c : text) {
        std::string imagePath = GA_RESOURCE_DIR "/fonts/number/" + std::string(1, c) + ".png";
        auto characterImage = std::make_shared<ImageItems>(imagePath);
        characterImage->SetPosition({x, y});
        characterImage->SetZIndex(50);
        textRow.push_back(characterImage);
        x += characterImage->GetScaledSize().x + 5;
    }
    numberImage.push_back(textRow);
}

void Menu::addHealth(int heart, const glm::vec2& position, std::string type){
    std::vector<std::shared_ptr<ImageItems>> healthbar;
    float x = position.x, y = position.y;
    for (int i = 0; i < heart; i++) {
        std::string imagePath = GA_RESOURCE_DIR "/fonts/menu/" + type + "-health-bar.png";
        auto characterImage = std::make_shared<ImageItems>(imagePath);
        characterImage->SetPosition({x, y});
        characterImage->SetZIndex(50);
        characterImage->m_Transform.scale = {0.95, 1.0};
        healthbar.push_back(characterImage);
        x += characterImage->GetScaledSize().x + 5;
    }
    health.push_back(healthbar);
}

void Menu::SetMenuVisibility(const bool visible) {
    for (auto &&num : numberImage){
        for (auto &&letter : num) letter->SetVisible(visible);
    }
    for (auto &&txt : textImage){
        txt->SetVisible(visible);
    }
    for (auto &&healthbar : health){
        for (auto &&h : healthbar) h->SetVisible(visible);
    }
}