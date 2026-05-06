#pragma once
#include "Poker_game_manager.h"

template <typename T = std::execution::sequenced_policy>
requires (std::is_same_v<T, std::execution::sequenced_policy> || std::is_same_v<T, std::execution::parallel_policy>)
void Poker_stability_test(std::uint8_t number_of_players, Player_difficulty d, std::size_t number_of_iterations) {
    std::mt19937_64 rng{ std::random_device{}() };

    Probability_evaluator<std::execution::sequenced_policy> eval_seq;
    Probability_evaluator<std::execution::parallel_policy> eval_par;

    number_of_players = std::clamp(
        number_of_players,
        Probability_evaluator<>::MIN_PLAYERS,
        Probability_evaluator<>::MAX_PLAYERS
    );
    
    for (std::size_t i = 0; i < number_of_iterations; ++i) {
        std::cout << i + 1 << " test\n";

        std::vector<Player> players;
        players.reserve(number_of_players);

        for (std::uint8_t j = 0; j < number_of_players; ++j) {
            std::string name = "Bot_";
            name += std::to_string(j);

            players.emplace_back(Player{
                std::move(name), j, 1000, d
            });
        }

        Poker_game_manager manager(rng, players, eval_seq, eval_par);

        if constexpr (std::is_same_v<T, std::execution::parallel_policy>) {
            manager.set_evaluator_parallel_policy();
        }

        while (manager.is_game_still_run()) {
            manager.call_next_move();
        }

        for (const Player& p : players) {
            std::cout << "Player: " << p.get_name() << " | Money: " << p.get_money() << " | Status: " << p.get_status() << '\n';
            if (p.get_status() == Player_status::Active && p.get_money() != players.size() * 1000) {
                throw std::runtime_error("Error");
            }
        }

        std::cout << '\n';
    }
}