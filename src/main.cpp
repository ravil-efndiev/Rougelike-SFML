#include "Game.hpp"
#include "MapEditor.hpp"

i32 main(i32 argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "edit") {
        MapEditor editor ("../assets/maps/test_set.tls");
    }
    else {
        Game* game = Game::create();
        game->run();
        delete game;
    }
    return 0;
}
