#include "Card_Logic/Tests.h"
#include <chrono>

int main() {
    TESTS();
    
    auto t1 = std::chrono::high_resolution_clock::now();
    Test_distribution_of_seven_comb_cards(10000);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " s\n";

    return 0;
}