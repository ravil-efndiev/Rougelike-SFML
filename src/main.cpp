#include "Application.hpp"

i32 main(i32 argc, char* argv[]) {
    Application* game;
#if R_DEV == true
    if (argc > 2 && std::string(argv[1]) == "edit") {
        game = Application::create(Application::editor, std::string(argv[2]));
        game->run();
    }
    else {
        game = Application::create(Application::game);
        game->run();
    }
#else
    game = Application::create(Application::game);
#endif

    delete game;
    return 0;
}
