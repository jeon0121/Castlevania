#ifndef MENU_HPP
#define MENU_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Object/ImageItems.hpp"

class Menu : public Util::GameObject {
public:
    Menu(int time, int score, int playerHeart, int enemyHeart, int playerAmmo, int playerLife, int currentStage)
        : GameObject(std::make_unique<Util::Image>(GA_RESOURCE_DIR "/transparent.png"), 0),
            time(time), score(score), playerHeart(playerHeart), enemyHeart(enemyHeart),
            playerAmmo(playerAmmo), playerLife(playerLife), currentStage(currentStage)
    {
        background = std::make_shared<ImageItems>(GA_RESOURCE_DIR "/title/key/title-key-1.png");
        background->SetZIndex(50);
        background->SetPosition({0, 0});

        addText("time", {0, 420});
        addNumber(formatTime(time), {100, 420});

        addText("score", {-400, 420});
        addNumber(formatScore(score), {-280, 420});

        addText("stage", {350, 420});
        addNumber(formatTwoDigits(currentStage), {450, 420});

        addText("heart-n-life", {210, 380});
        addNumber(formatTwoDigits(playerAmmo), {250, 392});
        addNumber(formatTwoDigits(playerLife), {250, 368});
    }
    
    int time;         // mm:dd
    int score;        // 000000
    int playerHeart;  // [][][][][][][][][][][][][][][][][]
    int enemyHeart;   // [][][][][][][][][][][][][][][][][]
    int playerAmmo;   // 00
    int playerLife;   // 00
    int currentStage; // 00

    std::shared_ptr<ImageItems> background;
    std::vector<std::vector<std::shared_ptr<ImageItems>>> numberImage;
    std::vector<std::shared_ptr<ImageItems>> textImage;
    std::vector<std::shared_ptr<ImageItems>> health;

    std::string formatTime(int time) {
        int minutes = time / 60;
        int seconds = time % 60;
        return formatTwoDigits(minutes) + formatTwoDigits(seconds);
    }

    std::string formatTwoDigits(int num) {
        return (num < 10 ? "0" : "") + std::to_string(num);
    }

    std::string formatScore(int score) {
        std::string scoreStr = std::to_string(score);
        int zero = 6 - scoreStr.length();
        return std::string(zero, '0') + scoreStr;
    }

    void addText(std::string text, const glm::vec2& position){
        float x = position.x, y = position.y;

        std::string imagePath = GA_RESOURCE_DIR "/fonts/menu/" + text + ".png";
        auto characterImage = std::make_shared<ImageItems>(imagePath);
        characterImage->SetPosition({x, y});
        characterImage->SetZIndex(50);
        characterImage->m_Transform.scale = glm::vec2(0.95, 0.8);
        textImage.push_back(characterImage);
    }

    void addNumber(std::string text, const glm::vec2& position){
        std::vector<std::shared_ptr<ImageItems>> textRow;
        float x = position.x, y = position.y;

        for (char c : text) {
            std::string imagePath = GA_RESOURCE_DIR "/fonts/number/" + std::string(1, c) + ".png";
            auto characterImage = std::make_shared<ImageItems>(imagePath);
            characterImage->SetPosition({x, y});
            characterImage->SetZIndex(50);
            characterImage->m_Transform.scale = glm::vec2(0.95, 0.8);
            textRow.push_back(characterImage);
            x += characterImage->GetScaledSize().x + 5;
        }

        numberImage.push_back(textRow);
    }

    void SetMenuVisibility(const bool visible) {
        for (auto &&num : numberImage){
            for (auto &&letter : num) letter->SetVisible(visible);
        }
        for (auto &&txt : textImage){
            txt->SetVisible(visible);
        }
    }
};

#endif