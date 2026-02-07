#include "Game_Logic/Poker_game_manager.h"
#include "Card_Logic/Tests.h"
#include <chrono>

int main() {
    Poker_game_manager q(4, 1000);
    q.Run();

    return 0;
}