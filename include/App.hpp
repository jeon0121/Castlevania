#ifndef APP_HPP
#define APP_HPP
#include "Util/Renderer.hpp"
#include "pch.hpp" // IWYU pragma: export
#include "State/Menu.hpp"

class Scene;

class App {
public:
    enum class AppState {
        START,
        UPDATE,
        END,
    };

    enum class GameState {
        TITLE,
        STAGE0,
        STAGE1,
        STAGE2,
        STAGE3,
    };

    AppState GetAppState() const { return m_AppState; }
    
    GameState GetGameState() const { return m_GameState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void AddAllChildren(std::vector<std::shared_ptr<Util::GameObject>> m_All);

    void RemoveAllChildren(std::vector<std::shared_ptr<Util::GameObject>> m_All);

    Util::Renderer m_Root;
    AppState m_AppState = AppState::START;
    GameState m_GameState = GameState::STAGE1;
    std::shared_ptr<Scene> m_Scene;
    std::shared_ptr<Menu> m_Menu;
    std::shared_ptr<Character> m_Character;
};

#endif
