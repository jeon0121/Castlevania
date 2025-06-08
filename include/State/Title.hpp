#ifndef TITLE_HPP
#define TITLE_HPP

#include "Scene.hpp"
#include "App.hpp"
#include "Object/AnimatedItems.hpp"

class Title : public Scene{
public:
    void Start(App* app) override;
    void Update(App* app) override;
    void End(App* app) override;

private:
    std::shared_ptr<AnimatedItems> m_Bat;
    std::shared_ptr<AnimatedItems> m_Key;
    std::shared_ptr<AnimatedItems> m_BatScene_1;
    std::shared_ptr<AnimatedItems> m_BatScene_2;
    std::shared_ptr<ImageItems> m_Boat;
    std::shared_ptr<Util::BGM> titleBGM;
};
#endif

