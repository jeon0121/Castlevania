#ifndef GG_HPP
#define GG_HPP

#include "Scene.hpp"
#include "App.hpp"

class GG : public Scene{
public:
    void Start(App* app) override;
    void Update(App* app) override;
    void End(App* app) override;

private:
    int choice = 1;
    std::shared_ptr<ImageItems> selectHeart;
    std::shared_ptr<Util::SFX> choiceSound;
};
#endif