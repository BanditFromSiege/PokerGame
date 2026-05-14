#pragma once
#include "Poker_combination.h"
#include <algorithm>
#include <execution>
#include <ranges>

class Probability_evaluator final {
public:
	static constexpr std::uint8_t MAX_PLAYERS = 8;
	static constexpr std::uint8_t MIN_PLAYERS = 2;
	static constexpr std::size_t iterations = 5000;

private:
	bool is_sequnced_policy = true;

	std::optional<std::pair<std::array<double, MAX_PLAYERS + 1>, std::size_t>> monte_carlo_evaluation(
		std::span<const Card> initial_player_cards,
		std::span<const Card> initial_table_cards,
		std::span<const std::pair<Card, Card>> initial_opponent_cards,
		std::uint8_t number_of_players,
		std::size_t number_of_iterations
	) noexcept;

public:
	Probability_evaluator() noexcept;

	void set_sequnced_execution_policy(bool sequnced_policy) noexcept;

	std::optional<std::pair<double, double>> get_relative_probability(
		std::span<const Card> initial_player_cards,
		std::span<const Card> initial_table_cards,
		std::uint8_t number_of_players
	) noexcept;

	std::optional<std::pair<std::array<double, MAX_PLAYERS>, std::size_t>> get_absolute_probability(
		std::span<const Card> initial_player_cards,
		std::span<const Card> initial_table_cards,
		std::span<const std::pair<Card, Card>> initial_opponent_cards,
		std::uint8_t number_of_players
	) noexcept;

	void generate_and_show_preflop_table(std::uint8_t number_of_players) noexcept;
};