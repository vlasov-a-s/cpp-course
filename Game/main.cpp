#include "Engine.h"

int main() {
    thirtythree::Engine engine(sf::VideoMode(1024, 600), "Game33", {2000, 2000});
    engine.StartGame();
    return 0;
}
