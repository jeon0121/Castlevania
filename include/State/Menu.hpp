#ifndef MENU_HPP
#define MENU_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Object/ImageItems.hpp"
#include "Object/Character.hpp"

struct MenuValue {
    int time = 300;       // mm:dd
    int score = 0;        // 000000
    int playerHeart = 16; // [][][][][][][][][][][][][][][][][]
    int enemyHeart = 16;  // [][][][][][][][][][][][][][][][][]
    int playerAmmo = 5;   // 00
    int playerLife = 3;   // 00
    int currentStage = 1; // 00
    WeaponType weapon = WeaponType::None;
};
class Menu : public Util::GameObject {
public:
    Menu(const MenuValue& value);

    MenuValue m_value;
    std::shared_ptr<ImageItems> background;
    std::vector<std::vector<std::shared_ptr<ImageItems>>> numberImage;
    std::vector<std::shared_ptr<ImageItems>> textImage;
    std::vector<std::shared_ptr<ImageItems>> GGImage;
    std::vector<std::vector<std::shared_ptr<ImageItems>>> health;

    std::string formatTime(int time);

    std::string formatTwoDigits(int num);

    std::string formatScore(int score);

    void addText(std::string text, const glm::vec2& position, int type=0);

    void addNumber(std::string text, const glm::vec2& position);

    void modifyNumber(std::string text, int type, int number=0);

    void addHealth(int heart, const glm::vec2& position, std::string type);

    void modifyHealth(int heart, std::string type);

    void addWeapon(WeaponType weapon, const glm::vec2& position);

    void modifyWeapon(WeaponType weapon);

    void addBadge(glm::vec2 position);
    
    void modifyBadge(bool badge);

    void SetMenuVisibility(const bool visible);

    void TimeCount(bool endScene = false);

    int  GetTime();

    void SetTime(int time);

    int  GetScore();

    void SetStageNum(int stage);
private:
    Util::ms_t timer = 0;
    std::shared_ptr<Util::SFX> timeoutSound;
};

#endif