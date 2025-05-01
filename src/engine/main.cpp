#include "movegenerator.h"
#include "position.h"
#include <iostream>
#include <string>

int main() {
    std::string fenString;

    std::getline(std::cin, fenString);

    Engine::Position position(fenString);

    position.display();
    std::cout << "FEN : " << position.to_fen() << std::endl;
    position.display_bitboards();

    Engine::initialize_magic_bitboards();

    return 0;
}
