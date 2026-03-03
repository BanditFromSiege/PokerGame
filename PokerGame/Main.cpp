#include "Game_Logic/Tests.h"
#include <chrono>

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();

    //Test_poker_game_in_console(8, Player_difficulty::Hard);
    Poker_stability_test(8, Player_difficulty::Hard, 25);

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " seconds\n";
    
    return 0;
}