#pragma once
#include "Poker_game_manager.h"
#include <string>

template <typename T = std::execution::sequenced_policy>
requires (std::is_same_v<T, std::execution::sequenced_policy> || std::is_same_v<T, std::execution::parallel_policy>)
void Test_poker_game_in_console(std::uint8_t number_of_players, Player_difficulty d) noexcept {
	number_of_players = std::clamp(
		number_of_players,
		Probability_evaluator<>::MIN_PLAYERS,
		Probability_evaluator<>::MAX_PLAYERS
	);

    std::mt19937_64 rng{ std::random_device{}() };

    std::vector<Player> players;
    players.reserve(number_of_players);

    for (std::uint8_t i = 0; i < number_of_players; ++i) {
        std::string name = "Bot_";
        name += std::to_string(i);

        players.emplace_back(Player{
            name.c_str(), i, 1000, d
        });
    }

    Poker_game_manager manager(rng, players);

    if constexpr (std::is_same_v<T, std::execution::parallel_policy>) {
        manager.set_evaluator_parallel_policy();
    }

    while (manager.is_game_still_run()) {
        const auto stage = manager.get_current_stage();
        auto table = manager.get_table();

        if (stage == Poker_stage::Preparation_preflop) {
            std::cout << "New round\n";
        }

        const Player* p = manager.get_current_player();

        if (p) {
            if (p->is_all_in()) {
                std::cout << "Player: " << p->get_name() << " | Money: " << p->get_money()
                    << " | Cards: " << p->get_cards()[0] << ' ' << p->get_cards()[1]
                    << " | Status: " << Player_status::All_in << '\n';
            }
            else {
                std::cout << "Player: " << p->get_name() << " | Money: " << p->get_money()
                    << " | Cards: " << p->get_cards()[0] << ' ' << p->get_cards()[1];

                auto p_move = p->get_last_move();

                std::cout << " | Status: " << p_move;

                if (p_move == Player_action::Call || p_move == Player_action::Raise) {
                    std::cout << " | Bet: " << p->get_last_bet() << " | Diff: " << p->get_bet_difference()
                        << " | Sum_of_bets " << table.get_sum_of_bets() << '\n';
                }
                else if (p_move == Player_action::Fold) {
                    std::cout << " | Bet: " << 0 << ' ' << " | Sum_of_bets " << table.get_sum_of_bets() << '\n';
                }
                else if (p_move == Player_action::Check) {
                    std::cout << " | Bet: " << 0 << ' ' << " | Sum_of_bets " << table.get_sum_of_bets() << '\n';
                }
            }
        }

        if (stage == Poker_stage::After_showdown) {
            std::cout << "\nShowdown\n";

            const auto& vec = table.get_pots();

            for (std::uint8_t i = 0; const auto& pot : vec) {
                std::cout << pot.get_bank() << " Players: ";

                for (std::uint8_t id : pot.get_players_id()) {
                    std::cout << int(id) << ' ';
                }

                std::cout << '\n';
            }

            const auto& winners_and_rewards = manager.get_winners_and_rewards();

            for (const auto& pair : winners_and_rewards) {
                std::cout << "WIN: ";

                for (std::uint8_t id : pair.second) {
                    std::cout << players[id].get_name() << ' ';
                }

                std::cout << " Reward: " << pair.first << '\n';
            }

            std::cout << '\n';
        }

        manager.call_next_move();

        table = manager.get_table();

        if (stage == Poker_stage::Preparation_flop || stage == Poker_stage::Preparation_turn || stage == Poker_stage::Preparation_river) {
            std::cout << '\n';
            table.show_table();
        }
    }
}