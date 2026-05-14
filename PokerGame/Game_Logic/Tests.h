#pragma once
#include "Poker_game_manager.h"

template <typename T = std::execution::sequenced_policy>
requires (std::is_same_v<T, std::execution::sequenced_policy> || std::is_same_v<T, std::execution::parallel_policy>)
void Poker_stability_test(
    std::uint8_t number_of_players,
    Player_difficulty d,
    std::size_t number_of_iterations,
    std::size_t initial_money,
    bool out_in_console = false
) {
    std::mt19937_64 rng{ std::random_device{}() };

    Probability_evaluator eval;

    number_of_players = std::clamp(
        number_of_players,
        Probability_evaluator::MIN_PLAYERS,
        Probability_evaluator::MAX_PLAYERS
    );

    if (initial_money != 500 && initial_money != 1000 && initial_money != 2000) {
        initial_money = 1000;
    }
    
    for (std::size_t i = 0; i < number_of_iterations; ++i) {
        std::cout << i + 1 << " test\n";

        std::vector<Player> players;
        players.reserve(number_of_players);

        for (std::uint8_t j = 0; j < number_of_players; ++j) {
            std::string name = "Bot_";
            name += std::to_string(j);

            players.emplace_back(Player{
                rng, std::move(name), j, initial_money, d
            });
        }

        Poker_game_manager manager(rng, players, eval);

        Logger l(manager);

        if constexpr (std::is_same_v<T, std::execution::parallel_policy>) {
            manager.set_evaluator_parallel_policy();
        }

        while (manager.is_game_still_run()) {
            manager.call_next_move();

            if (out_in_console) {
                std::cout << l.get_message();
            }
        }

        std::cout << "Rounds: " << manager.get_number_of_rounds() << '\n';

        for (const Player& p : players) {
            std::cout << "Player: " << p.get_name() << " | Difficulty: " << p.get_difficulty()
                << " | Money: " << p.get_money() << " | Status: " << p.get_status() << '\n';

            if (p.get_status() == Player_status::Active && p.get_money() != players.size() * initial_money) {
                throw std::runtime_error("Error");
            }
        }

        std::cout << '\n';
    }
}