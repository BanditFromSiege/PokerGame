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

		std::array<std::atomic<double>, MAX_PLAYERS + 1> probabilities{};

		Poker_deck deck = *opt_poker_deck;

		auto temp_range = std::views::iota(std::size_t{ 0 }, number_of_iterations);

		std::atomic<bool> failed = false;

		auto main_work = [
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

			const double val = 1.0 / static_cast<double>(count);

			for (std::uint8_t i = 0; i < number_of_players; ++i) {
				if (*res_it == combinations[i]) {
					probabilities[i].fetch_add(val, std::memory_order_relaxed);
				}
			}

			if (*res_it == *Poker_combination::create_combination_by_cards(table_cards)) {
				probabilities.back().fetch_add(1, std::memory_order_relaxed);
			}
		};

		if (is_sequnced_policy) {
			std::for_each(
				std::execution::seq,
				temp_range.begin(),
				temp_range.end(),
				main_work
			);
		}
		else {
			std::for_each(
				std::execution::par,
				temp_range.begin(),
				temp_range.end(),
				main_work
			);
		}
		
		if (failed.load(std::memory_order_relaxed)) {
			return std::nullopt;
		}

		std::array<double, MAX_PLAYERS + 1> result = {0};

		for (std::uint8_t i = 0; i < number_of_players; ++i) {
			double val = probabilities[i].load(std::memory_order_relaxed);
			result[i] = val / static_cast<double>(number_of_iterations);
		}

		result.back()
			= probabilities.back().load(std::memory_order_relaxed) / static_cast<double>(number_of_iterations);

		return std::pair{ result, number_of_players };
	}

public:
	Probability_evaluator() noexcept = default;

	void set_sequnced_execution_policy(bool sequnced_policy) noexcept {
		is_sequnced_policy = sequnced_policy;
	}

	std::optional<std::pair<double, double>> get_relative_probability(
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
			auto [arr, size] = *opt;

			if (initial_table_cards.empty()) {
				return std::pair{ arr[0], 0.0 };
			}
			else {
				return std::pair{ arr[0], arr.back() };
			}
		}

		return std::nullopt;
	}

	std::optional<std::pair<std::array<double, MAX_PLAYERS>, std::size_t>> get_absolute_probability(
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
			std::array<double, MAX_PLAYERS> result;

			auto [arr, size] = *opt;

			for (std::size_t i = 0; i < size; ++i) {
				result[i] = arr[i];
			}

			return std::pair{ result, size };
		}

		return std::nullopt;
	}

	void generate_and_show_preflop_table(std::uint8_t number_of_players) noexcept {
		number_of_players = std::clamp(
			number_of_players,
			MIN_PLAYERS,
			MAX_PLAYERS
		);

		std::vector<std::pair<double, std::tuple<std::uint8_t, std::uint8_t, bool>>> preflop_cards;
		preflop_cards.reserve(Card::COUNT_OF_CARD_VALUES * Card::COUNT_OF_CARD_VALUES);

		for (std::uint8_t i = 0; i < Card::COUNT_OF_CARD_VALUES; ++i) {
			for (std::uint8_t j = i; j < Card::COUNT_OF_CARD_VALUES; ++j) {
				if (i == j) {
					Card c1(static_cast<Card_value>(i), Card_suit::Hearts);
					Card c2(static_cast<Card_value>(j), Card_suit::Spades);

					auto [probablity, _] = *get_relative_probability(std::array{ c1, c2 }, {}, number_of_players);

					preflop_cards.push_back({ probablity, {i, j, false} });
				}
				else {
					Card c1(static_cast<Card_value>(i), Card_suit::Hearts);
					Card c2(static_cast<Card_value>(j), Card_suit::Hearts);

					auto [probablity, _] = *get_relative_probability(std::array{ c1, c2 }, {}, number_of_players);

					preflop_cards.push_back({ probablity, {i, j, true} });

					c1 = Card(static_cast<Card_value>(i), Card_suit::Hearts);
					c2 = Card(static_cast<Card_value>(j), Card_suit::Diamonds);

					std::tie(probablity, _) = *get_relative_probability(std::array{ c1, c2 }, {}, number_of_players);

					preflop_cards.push_back({ probablity, {i, j, false} });
				}
			}
		}
		
		std::sort(preflop_cards.begin(), preflop_cards.end());

		for (auto [probability, tuple] : preflop_cards) {
			auto [rank_card1, rank_card2, suited] = tuple;
			std::cout << static_cast<int>(number_of_players) << " players"
				<< '\t' << static_cast<Card_value>(rank_card1) << static_cast<Card_value>(rank_card2)
				<< '\t' << (suited ? "suited\t" : "outsuited")
				<< '\t' << probability << '\n';
		}
	}
};