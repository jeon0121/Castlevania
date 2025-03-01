#include "App.hpp"

#include "Core/Context.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;

    while (!context->GetExit()) {
        switch (app.GetAppState()) {
            case App::AppState::START:
                app.Start();
                break;

            case App::AppState::UPDATE:
                app.Update();
                break;

            case App::AppState::END:
                app.End();
                context->SetExit(true);
                break;
        }
        context->Update();
    }
    return 0;
}
