#include "Card_Logic/Probability_evaluator.h"
#include "Card_Logic/Tests.h"
#include <chrono>

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    Test_distribution_of_seven_comb_cards<std::execution::parallel_policy>(100'000);
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count() << " s\n";

    std::cout << '\n';

    Probability_evaluator<std::execution::parallel_policy> ev(10000);

    auto cards = *Poker_deck::create_poker_deck();

    std::mt19937_64 rng(std::random_device{}());
    cards.shuffle(rng);

    for (std::uint8_t i = 0; i < 10; ++i) {
        auto t3 = std::chrono::high_resolution_clock::now();

        std::array<Card, 2> player_cards{ *cards.get_card(), *cards.get_card() };
        std::array<Card, 0> table_cards{ };
        
        auto result = ev.get_relative_probability(
            player_cards,
            table_cards,
            2
        );

        if (result) {
            std::cout << i + 1 << ": ";
            for (Card c : player_cards) {
                std::cout << c << ' ';
            }
            std::cout << ": ";
            
            std::cout << std::fixed << std::setprecision(1) << (*result * 100) << "% ";
        } else {
            std::cout << "Error ";
        }

        auto t4 = std::chrono::high_resolution_clock::now();

        cards.shuffle(rng);

        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() << " ms\n";
    }

    return 0;
}