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
        background->SetPosition({0, 0});
        addText(formatTime(time), {0, 0});
        // addText(formatScore(score), {0, 30});
        // addText(formatTwoDigits(playerAmmo), {0, 60});
        // addText(formatTwoDigits(playerLife), {0, 90});
        // addText(formatTwoDigits(currentStage), {0, 120});
    }
    
    int time;         // mm:dd
    int score;        // 000000
    int playerHeart;  // [][][][][][][][][][][][][][][][][]
    int enemyHeart;   // [][][][][][][][][][][][][][][][][]
    int playerAmmo;   // 00
    int playerLife;   // 00
    int currentStage; // 00

    std::shared_ptr<ImageItems> background;
    std::vector<std::vector<std::shared_ptr<ImageItems>>> textImage;
    std::vector<std::shared_ptr<ImageItems>> health;

    std::string formatTime(int time) {
        int minutes = time / 60;
        int seconds = time % 60;
        return formatTwoDigits(minutes) + ":" + formatTwoDigits(seconds);
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
        std::vector<std::shared_ptr<ImageItems>> textRow;
        float x = position.x, y = position.y;

        for (char c : text) {
            std::string imagePath = GA_RESOURCE_DIR "/fonts/number/" + std::string(1, c) + ".png";
            auto characterImage = std::make_shared<ImageItems>(imagePath);
            characterImage->SetPosition({x, y});

            textRow.push_back(characterImage);
            x += characterImage->GetScaledSize().x + 10;
        }

        textImage.push_back(textRow);
    }
};

#endif