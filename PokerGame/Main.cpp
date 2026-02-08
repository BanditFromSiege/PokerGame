#include "Game_Logic/Poker_game_manager.h"
#include "Card_Logic/Tests.h"
#include <chrono>

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    Test_distribution_of_seven_comb_cards(100'000);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " s\n";

    std::cout << '\n';

    //Poker_game_manager q(4, 1000);
    //q.Run();

    return 0;
}