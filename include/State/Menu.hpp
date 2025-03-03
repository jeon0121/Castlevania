#ifndef MENU_HPP
#define MENU_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Object/ImageItems.hpp"

class Menu : public Util::GameObject {
public:
    Menu(int time, int score, int playerHeart, int enemyHeart, int playerAmmo, int playerLife, int currentStage);
    
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
    std::vector<std::vector<std::shared_ptr<ImageItems>>> health;

    std::string formatTime(int time);

    std::string formatTwoDigits(int num);

    std::string formatScore(int score);

    void addText(std::string text, const glm::vec2& position);

    void addNumber(std::string text, const glm::vec2& position);

    void addHealth(int heart, const glm::vec2& position, std::string type);

    void SetMenuVisibility(const bool visible);
};

#endif