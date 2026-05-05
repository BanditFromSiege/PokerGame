#pragma once
#include "Poker_combination.h"
#include <algorithm>
#include <execution>
#include <ranges>

template <typename T = std::execution::sequenced_policy>
requires (std::is_same_v<T, std::execution::sequenced_policy> || std::is_same_v<T, std::execution::parallel_policy>)
class Probability_evaluator final {
private:
	std::size_t iterations = 5000;

	std::optional<std::vector<double>> monte_carlo_evaluation(
		std::span<const Card> initial_player_cards,
		std::span<const Card> initial_table_cards,
		std::span<const std::pair<Card, Card>> initial_opponent_cards,
		std::uint8_t number_of_players,
		std::size_t number_of_iterations
	) noexcept {
		if (initial_player_cards.size() != Card::COUNT_OF_CARDS_ON_PREFLOP) {
			return std::nullopt;
		}

		if (!initial_table_cards.empty() && (initial_table_cards.size() < Card::COUNT_OF_TABLE_CARDS_ON_FLOP
			|| initial_table_cards.size() > Card::COUNT_OF_TABLE_CARDS_ON_RIVER))
		{
			return std::nullopt;
		}

		if (number_of_players > MAX_PLAYERS || number_of_players < MIN_PLAYERS) {
			return std::nullopt;
		}

		const std::uint8_t opponent_number = number_of_players - 1;

		if (!initial_opponent_cards.empty() && initial_opponent_cards.size() > opponent_number) {
			return std::nullopt;
		}

		std::array<Card, Card::COUNT_OF_ALL_CARDS> all_initial_cards;

		std::uint8_t index = 0;

		for (Card c : initial_player_cards) {
			all_initial_cards[index++] = c;
		}

		for (Card c : initial_table_cards) {
			all_initial_cards[index++] = c;
		}

		for (auto [c1, c2] : initial_opponent_cards) {
			all_initial_cards[index++] = c1;
			all_initial_cards[index++] = c2;
		}

		std::span<const Card> span_all_initial_cards(all_initial_cards.data(), index);

		auto opt_poker_deck = Poker_deck::create_poker_deck(span_all_initial_cards);

		if (!opt_poker_deck) {
			return std::nullopt;
		}

		std::array<std::atomic<double>, MAX_PLAYERS> probabilities{};

		T policy;

		Poker_deck deck = *opt_poker_deck;

		auto temp_range = std::views::iota(std::size_t{ 0 }, number_of_iterations);

		std::atomic<bool> failed = false;

		std::for_each(
			policy,
			temp_range.begin(),
			temp_range.end(),
		[
			&failed,
			&deck,
			&probabilities,
			&initial_player_cards,
			&initial_table_cards,
			&initial_opponent_cards,
			number_of_players,
			opponent_number
		](std::size_t) {
			if (failed.load(std::memory_order_relaxed)) {
				return;
			}

			auto cards = deck;

			thread_local std::mt19937_64 rng(std::random_device{}());
			cards.shuffle(rng);

			std::array<Card, Card::COUNT_OF_TABLE_CARDS_ON_RIVER> table_cards;
			std::uint8_t table_cards_counter = 0;

			std::array<Card, Card::COUNT_OF_CARDS_ON_RIVER> player_cards;
			std::uint8_t player_cards_counter = 0;

			for (std::uint8_t i = 0; i < initial_table_cards.size(); ++i) {
				table_cards[table_cards_counter++] = initial_table_cards[i];
			}

			for (; table_cards_counter < Card::COUNT_OF_TABLE_CARDS_ON_RIVER; ++table_cards_counter) {
				auto opt = cards.get_card();

				if (opt) {
					table_cards[table_cards_counter] = *opt;
				}
				else {
					failed.store(true, std::memory_order_relaxed);
					return;
				}
			}

			for (std::uint8_t i = 0; i < initial_player_cards.size(); ++i) {
				player_cards[player_cards_counter++] = initial_player_cards[i];
			}

			for (std::uint8_t i = 0; i < Card::COUNT_OF_TABLE_CARDS_ON_RIVER; ++i) {
				player_cards[player_cards_counter++] = table_cards[i];
			}

			std::array<std::array<Card, Card::COUNT_OF_CARDS_ON_RIVER>, MAX_PLAYERS> opponents_cards;

			for (std::size_t i = 0, current_opponent = 0; i < opponent_number; ++i) {
				if (current_opponent < initial_opponent_cards.size()) {
					opponents_cards[i][0] = initial_opponent_cards[current_opponent].first;
					opponents_cards[i][1] = initial_opponent_cards[current_opponent].second;

					++current_opponent;
				}
				else {
					for (std::uint8_t j = 0; j < Card::COUNT_OF_CARDS_IN_HAND; ++j) {
						auto opt = cards.get_card();

						if (opt) {
							opponents_cards[i][j] = *opt;
						}
						else {
							failed.store(true, std::memory_order_relaxed);
							return;
						}
					}
				}

				for (std::uint8_t j = 0; j < Card::COUNT_OF_TABLE_CARDS_ON_RIVER; ++j) {
					opponents_cards[i][j + Card::COUNT_OF_CARDS_IN_HAND] = table_cards[j];
				}
			}

			std::array<Poker_combination, MAX_PLAYERS> combinations;

			combinations[0] = *Poker_combination::create_combination_by_cards(player_cards);

			for (std::uint8_t i = 0; i < opponent_number; ++i) {
				auto opt = Poker_combination::create_combination_by_cards(opponents_cards[i]);

				if (opt) {
					combinations[i + 1] = *opt;
				}
				else {
					failed.store(true, std::memory_order_relaxed);
					return;
				}
			}

			auto res_it = std::max_element(combinations.begin(), combinations.begin() + number_of_players);

			std::size_t count = std::count(combinations.begin(), combinations.begin() + number_of_players, *res_it);

			for (std::uint8_t i = 0; i < number_of_players; ++i) {
				if (*res_it == combinations[i]) {
					double val = 1.0 / static_cast<double>(count);
					probabilities[i].fetch_add(val, std::memory_order_relaxed);
				}
			}
		});

		if (failed.load(std::memory_order_relaxed)) {
			return std::nullopt;
		}

		const std::size_t sz = 1 + initial_opponent_cards.size();

		std::vector<double> result(sz);

		for (std::uint8_t i = 0; i < sz; ++i) {
			double val = probabilities[i].load(std::memory_order_relaxed);
			result[i] = val / static_cast<double>(number_of_iterations);
		}

		return result;
	}

public:
	static constexpr std::uint8_t MAX_PLAYERS = 8;
	static constexpr std::uint8_t MIN_PLAYERS = 2;

	Probability_evaluator() noexcept = default;

	Probability_evaluator(std::size_t n) noexcept : iterations(n) {};

	std::optional<double> get_relative_probability(
		std::span<const Card> initial_player_cards,
		std::span<const Card> initial_table_cards,
		std::uint8_t number_of_players
	) noexcept {
		auto opt = monte_carlo_evaluation(
			initial_player_cards,
			initial_table_cards,
			{},
			number_of_players,
			iterations
		);

		if (opt) {
			return (*opt)[0];
		}

		return std::nullopt;
	}

	std::optional<std::vector<double>> get_absolute_probability(
		std::span<const Card> initial_player_cards,
		std::span<const Card> initial_table_cards,
		std::span<const std::pair<Card, Card>> initial_opponent_cards,
		std::uint8_t number_of_players
	) noexcept {
		if (initial_opponent_cards.empty()) {
			return std::nullopt;
		}

		auto opt = monte_carlo_evaluation(
			initial_player_cards,
			initial_table_cards,
			initial_opponent_cards,
			number_of_players,
			iterations
		);

		if (opt) {
			return *opt;
		}

		return std::nullopt;
	}
};