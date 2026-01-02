#include "Card_Logic/Tests.h"
#include <chrono>

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    Test_distribution_of_seven_comb_cards<std::execution::parallel_policy>(100'000);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " s\n";

    std::cout << '\n';

    for (std::uint8_t i = 0; i < 10; ++i) {
        auto t3 = std::chrono::high_resolution_clock::now();

        auto result = monte_carlo_evaluation<std::execution::parallel_policy>(
            std::array{ *Card::ch("AS"), *Card::ch("AD") },
            std::array<Card, 0>{},
            std::array<std::pair<Card, Card>, 0>{},
            2,
            10000
        );

        if (result) {
            std::cout << i + 1 << ": ";
            for (auto d : *result) {
                std::cout << std::fixed << std::setprecision(1) << (d * 100) << "% ";
            }
        } else {
            std::cout << "Error ";
        }

        auto t4 = std::chrono::high_resolution_clock::now();

        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() << " ms\n";
    }
    
    return 0;
}