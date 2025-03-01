#ifndef TITLE_HPP
#define TITLE_HPP
#include <Scene.hpp>

#include <AnimatedItems.hpp>

class Title : public Scene{
public:
    void start() override;
    void update();
    void end();

private:
    std::shared_ptr<AnimatedItems> m_Bat;
};
#endif

