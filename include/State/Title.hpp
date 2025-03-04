#ifndef TITLE_HPP
#define TITLE_HPP

#include "Scene.hpp"
#include "State/Menu.hpp"
#include "Object/AnimatedItems.hpp"

class Title : public Scene{
public:
    void Start(App* app) override;
    void Update() override;
    void End(App* app) override;

private:
    std::shared_ptr<AnimatedItems> m_Bat;
    std::shared_ptr<AnimatedItems> m_Key;
    std::shared_ptr<AnimatedItems> m_BatScene_1;
    std::shared_ptr<AnimatedItems> m_BatScene_2;
    std::shared_ptr<ImageItems> m_Boat;
    std::shared_ptr<Menu> m_Menu;
};
#endif

