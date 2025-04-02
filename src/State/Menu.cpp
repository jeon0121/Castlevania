#include "State/Menu.hpp"

Menu::Menu(const MenuValue& values)
    : GameObject(std::make_unique<Util::Image>(GA_RESOURCE_DIR "/transparent.png"), 0),
    m_value(values)
{
    background = std::make_shared<ImageItems>(GA_RESOURCE_DIR "/title/key/title-key-1.png", glm::vec2(1, 1.5));
    background->SetZIndex(30);
    background->SetPosition({0, 380});

    // score
    addText("score", {-390, 350});
    addNumber(formatScore(m_value.score), {-275, 350});

    // time
    addText("time", {0, 350});
    addNumber(formatTime(m_value.time), {100, 350});

    // stage
    addText("stage", {330, 350});
    addNumber(formatTwoDigits(m_value.currentStage), {450, 350});

    // heart and ammo
    addText("heart-n-life", {215, 302});
    addNumber(formatTwoDigits(m_value.playerAmmo), {265, 317});
    addNumber(formatTwoDigits(m_value.playerLife), {265, 285});

    // player health
    addText("player", {-390, 317});
    addHealth(m_value.playerHeart, {-260, 314}, "player");

    // enemy health
    addText("enemy", {-405, 285});
    addHealth(m_value.enemyHeart, {-260, 280}, "enemy");

    // weapon
    addText("weapon-frame", {90, 287});
    addWeapon(m_value.weapon, {95, 285});
}

std::string Menu::formatTime(int time) {
    int minutes = time / 100;
    int seconds = time % 100;
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

void Menu::modifyNumber(std::string text, int type){
    std::vector<std::shared_ptr<ImageItems>>& numberbar = numberImage[type];
    int length = text.length();
    for (int i = 0; i < length; i++)
        numberbar[i]->SetImage(GA_RESOURCE_DIR "/fonts/number/" + std::string(1, text[i]) + ".png");
}

void Menu::addHealth(int heart, const glm::vec2& position, std::string type){
    std::vector<std::shared_ptr<ImageItems>> healthbar;
    float x = position.x, y = position.y;
    for (int i = 0; i < heart; i++) {
        std::string imagePath = GA_RESOURCE_DIR "/fonts/menu/" + type + "-health-bar.png";
        auto characterImage = std::make_shared<ImageItems>(imagePath);
        characterImage->SetPosition({x, y});
        characterImage->SetZIndex(50);
        characterImage->m_Transform.scale = {0.95, 0.88};
        healthbar.push_back(characterImage);
        x += characterImage->GetScaledSize().x + 5;
    }
    health.push_back(healthbar);
}

void Menu::modifyHealth(int heart, std::string type) {
    std::vector<std::shared_ptr<ImageItems>>& healthbar = health[type == "player" ? 0 : 1];
    for (auto &&h : healthbar)
        h->SetVisible(false);
    int i = 0;
    for (auto &&h : healthbar){
        if (i++ != heart)
            h->SetVisible(true);
    }
}

std::string SetWeaponImagePath(WeaponType weapon){
    std::string imagePath = GA_RESOURCE_DIR "/items/weapon/";
    switch (weapon) {
        case WeaponType::None:
            imagePath = GA_RESOURCE_DIR "/transparent.png";
            break;
        case WeaponType::Axe:
            imagePath += "axe.png";
            break;
        case WeaponType::HolyWater:
            imagePath += "holywater.png";
            break;
        case WeaponType::Dagger:
            imagePath += "dagger.png";
            break;
        case WeaponType::Stopwatch:
            imagePath += "stopwatch.png";
            break;
    }
    return imagePath;
}

void Menu::addWeapon(WeaponType weapon, const glm::vec2 &position){
    float x = position.x, y = position.y;
    std::string imagePath = SetWeaponImagePath(weapon);
    auto characterImage = std::make_shared<ImageItems>(imagePath);
    characterImage->SetPosition({x, y});
    characterImage->SetZIndex(50);
    textImage.push_back(characterImage);
}

void Menu::modifyWeapon(WeaponType weapon) {
    std::string imagePath = SetWeaponImagePath(weapon);
    textImage[7]->SetImage(imagePath);
}

void Menu::SetMenuVisibility(const bool visible) {
    for (auto &&num : numberImage){
        for (auto &&letter : num)
            letter->SetVisible(visible);
    }
    for (auto &&txt : textImage){
        txt->SetVisible(visible);
    }
    for (auto &&healthbar : health){
        for (auto &&h : healthbar)
            h->SetVisible(visible);
    }
    background->SetVisible(visible);
}