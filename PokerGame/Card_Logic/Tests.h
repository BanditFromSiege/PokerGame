#pragma once
#include "Poker_combination.h"
#include <random>

consteval void TESTS() noexcept {
	auto run_test = [](
		std::initializer_list<Card> player,
		std::initializer_list<Card> opponent,
		Result outcome
	) consteval -> bool
	{
		auto c1 = *Poker_combination::create_combination_by_cards(std::vector(player));
		auto c2 = *Poker_combination::create_combination_by_cards(std::vector(opponent));
		return compare_combinations(c1, c2) == outcome;
	};

	static_assert(run_test(
		{*Card::ch("2H"), *Card::ch("3H"), *Card::ch("4H"), *Card::ch("5H"), *Card::ch("6H")},
		{*Card::ch("KS"), *Card::ch("AS"), *Card::ch("TS"), *Card::ch("QS"), *Card::ch("JS")},
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("2H"), *Card::ch("3H"), *Card::ch("4H"), *Card::ch("5H"), *Card::ch("6H") },
		{ *Card::ch("AS"), *Card::ch("AD"), *Card::ch("AC"), *Card::ch("AH"), *Card::ch("JD") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("AS"), *Card::ch("AH"), *Card::ch("2H"), *Card::ch("AD"), *Card::ch("AC") },
		{ *Card::ch("JS"), *Card::ch("JD"), *Card::ch("JC"), *Card::ch("JH"), *Card::ch("3D") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("AH"), *Card::ch("2H"), *Card::ch("AS"), *Card::ch("AC") },
		{ *Card::ch("JS"), *Card::ch("JD"), *Card::ch("JC"), *Card::ch("JH"), *Card::ch("3D") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("AH"), *Card::ch("2H"), *Card::ch("AS"), *Card::ch("AC") },
		{ *Card::ch("2H"), *Card::ch("3H"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("7H") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("AS"), *Card::ch("3S"), *Card::ch("4S"), *Card::ch("8S"), *Card::ch("2S") },
		{ *Card::ch("2H"), *Card::ch("3H"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("7H") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("2H"), *Card::ch("3H"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("7H") },
		{ *Card::ch("2S"), *Card::ch("3H"), *Card::ch("4H"), *Card::ch("5S"), *Card::ch("6C") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("3H"), *Card::ch("4H"), *Card::ch("5S"), *Card::ch("6C") },
		{ *Card::ch("3D"), *Card::ch("4C"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("2S") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("3H"), *Card::ch("4H"), *Card::ch("5S"), *Card::ch("6C") },
		{ *Card::ch("AH"), *Card::ch("AC"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("AS") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("3H"), *Card::ch("4H"), *Card::ch("5S"), *Card::ch("AC") },
		{ *Card::ch("AH"), *Card::ch("AC"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("AS") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("2H"), *Card::ch("4H"), *Card::ch("5S"), *Card::ch("4C") },
		{ *Card::ch("AH"), *Card::ch("AC"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("AS") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("2H"), *Card::ch("4H"), *Card::ch("5S"), *Card::ch("4C") },
		{ *Card::ch("AH"), *Card::ch("AC"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("7S") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("6S"), *Card::ch("AD"), *Card::ch("7H"), *Card::ch("4S"), *Card::ch("AS") },
		{ *Card::ch("AH"), *Card::ch("AC"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("7S") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("AH"), *Card::ch("4H"), *Card::ch("5S"), *Card::ch("KC") },
		{ *Card::ch("AH"), *Card::ch("AC"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("7S") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("3H"), *Card::ch("6H"), *Card::ch("7S"), *Card::ch("9C") },
		{ *Card::ch("7H"), *Card::ch("3C"), *Card::ch("TH"), *Card::ch("6H"), *Card::ch("9S") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("4S"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("TS"), *Card::ch("AC") },
		{ *Card::ch("3S"), *Card::ch("4C"), *Card::ch("6H"), *Card::ch("TS"), *Card::ch("AC") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("2S"), *Card::ch("AH"), *Card::ch("4H"), *Card::ch("5S"), *Card::ch("6C") },
		{ *Card::ch("AD"), *Card::ch("4C"), *Card::ch("5H"), *Card::ch("6H"), *Card::ch("2C") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("KD"), *Card::ch("KC"), *Card::ch("KH"), *Card::ch("QS"), *Card::ch("QD") },
		{ *Card::ch("KS"), *Card::ch("KC"), *Card::ch("KH"), *Card::ch("QC"), *Card::ch("QH") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("AS"), *Card::ch("5H"), *Card::ch("4S"), *Card::ch("3C"), *Card::ch("2D") },
		{ *Card::ch("AS"), *Card::ch("2S"), *Card::ch("3S"), *Card::ch("QS"), *Card::ch("KS") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("AD"), *Card::ch("2D"), *Card::ch("JD"), *Card::ch("QD"), *Card::ch("KD") },
		{ *Card::ch("AC"), *Card::ch("TC"), *Card::ch("JC"), *Card::ch("QC"), *Card::ch("KC") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("AS"), *Card::ch("TS"), *Card::ch("JS"), *Card::ch("QS"), *Card::ch("KS") },
		{ *Card::ch("AD"), *Card::ch("TD"), *Card::ch("JD"), *Card::ch("QD"), *Card::ch("KD") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("AH"), *Card::ch("2S"), *Card::ch("3S"), *Card::ch("4H"), *Card::ch("KS") },
		{ *Card::ch("9D"), *Card::ch("TC"), *Card::ch("JC"), *Card::ch("QD"), *Card::ch("KD") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("AH"), *Card::ch("AS"), *Card::ch("2D"), *Card::ch("7S"), *Card::ch("9S") },
		{ *Card::ch("AD"), *Card::ch("AC"), *Card::ch("2H"), *Card::ch("7C"), *Card::ch("9C") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("AH"), *Card::ch("TD"), *Card::ch("9D"), *Card::ch("AS"), *Card::ch("9H") },
		{ *Card::ch("AC"), *Card::ch("KD"), *Card::ch("5D"), *Card::ch("AD"), *Card::ch("5H") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("AH"), *Card::ch("9D"), *Card::ch("9H"), *Card::ch("AS"), *Card::ch("9C") },
		{ *Card::ch("AC"), *Card::ch("5D"), *Card::ch("5H"), *Card::ch("AD"), *Card::ch("5C") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("8S"), *Card::ch("3S"), *Card::ch("TC"), *Card::ch("7H"), *Card::ch("AH"), *Card::ch("KC") },
		{ *Card::ch("8S"), *Card::ch("3S"), *Card::ch("TC"), *Card::ch("7H"), *Card::ch("QC"), *Card::ch("TH") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("KS"), *Card::ch("QH"), *Card::ch("AC"), *Card::ch("QS"), *Card::ch("TD"), *Card::ch("QD") },
		{ *Card::ch("KS"), *Card::ch("QH"), *Card::ch("AC"), *Card::ch("QS"), *Card::ch("QC"), *Card::ch("9H") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("5S"), *Card::ch("9D"), *Card::ch("AC"), *Card::ch("KH"), *Card::ch("QS"), *Card::ch("9C") },
		{ *Card::ch("5S"), *Card::ch("9D"), *Card::ch("AC"), *Card::ch("KH"), *Card::ch("9H"), *Card::ch("TH") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("5D"), *Card::ch("8S"), *Card::ch("TD"), *Card::ch("7H"), *Card::ch("KS"), *Card::ch("2S") },
		{ *Card::ch("5D"), *Card::ch("8S"), *Card::ch("TD"), *Card::ch("7H"), *Card::ch("2H"), *Card::ch("JC") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("9D"), *Card::ch("9H"), *Card::ch("9S"), *Card::ch("9C"), *Card::ch("AS"), *Card::ch("2H") },
		{ *Card::ch("9D"), *Card::ch("9H"), *Card::ch("9S"), *Card::ch("9C"), *Card::ch("JH"), *Card::ch("JC") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("KD"), *Card::ch("QH"), *Card::ch("TS"), *Card::ch("3C"), *Card::ch("JS"), *Card::ch("7H") },
		{ *Card::ch("KD"), *Card::ch("QH"), *Card::ch("TS"), *Card::ch("3C"), *Card::ch("JD"), *Card::ch("9C") },
	Result::Loss), "Error");

	static_assert(run_test(
		{ *Card::ch("KD"), *Card::ch("QH"), *Card::ch("TS"), *Card::ch("JC"), *Card::ch("AH"), *Card::ch("4H") },
		{ *Card::ch("KD"), *Card::ch("QH"), *Card::ch("TS"), *Card::ch("JC"), *Card::ch("AD"), *Card::ch("6H") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("4C"), *Card::ch("2D"), *Card::ch("7S"), *Card::ch("7D"), *Card::ch("9S"), *Card::ch("7C") },
		{ *Card::ch("4C"), *Card::ch("2D"), *Card::ch("7S"), *Card::ch("7D"), *Card::ch("7H"), *Card::ch("6D") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("8S"), *Card::ch("3D"), *Card::ch("4H"), *Card::ch("7C"), *Card::ch("QH"), *Card::ch("TH") },
		{ *Card::ch("8S"), *Card::ch("3D"), *Card::ch("4H"), *Card::ch("7C"), *Card::ch("JC"), *Card::ch("9D") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("6S"), *Card::ch("3S"), *Card::ch("AC"), *Card::ch("KH"), *Card::ch("QS"), *Card::ch("7D") },
		{ *Card::ch("6S"), *Card::ch("3S"), *Card::ch("AC"), *Card::ch("KH"), *Card::ch("JH"), *Card::ch("TC") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("8S"), *Card::ch("3S"), *Card::ch("TC"), *Card::ch("7H"), *Card::ch("AH"), *Card::ch("KS"), *Card::ch("TD") },
		{ *Card::ch("8S"), *Card::ch("3S"), *Card::ch("TC"), *Card::ch("7H"), *Card::ch("AH"), *Card::ch("QC"), *Card::ch("TH") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("KS"), *Card::ch("QH"), *Card::ch("AC"), *Card::ch("QS"), *Card::ch("5S"), *Card::ch("TD"), *Card::ch("QD") },
		{ *Card::ch("KS"), *Card::ch("QH"), *Card::ch("AC"), *Card::ch("QS"), *Card::ch("5S"), *Card::ch("QC"), *Card::ch("9H") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("5S"), *Card::ch("9D"), *Card::ch("AC"), *Card::ch("KH"), *Card::ch("4S"), *Card::ch("QS"), *Card::ch("9C") },
		{ *Card::ch("5S"), *Card::ch("9D"), *Card::ch("AC"), *Card::ch("KH"), *Card::ch("4S"), *Card::ch("9H"), *Card::ch("TH") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("5D"), *Card::ch("8S"), *Card::ch("TD"), *Card::ch("7H"), *Card::ch("2C"), *Card::ch("KS"), *Card::ch("2S") },
		{ *Card::ch("5D"), *Card::ch("8S"), *Card::ch("TD"), *Card::ch("7H"), *Card::ch("2C"), *Card::ch("2H"), *Card::ch("JC") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("9D"), *Card::ch("9H"), *Card::ch("9S"), *Card::ch("9C"), *Card::ch("3H"), *Card::ch("AS"), *Card::ch("2H") },
		{ *Card::ch("9D"), *Card::ch("9H"), *Card::ch("9S"), *Card::ch("9C"), *Card::ch("3H"), *Card::ch("JH"), *Card::ch("JC") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("KD"), *Card::ch("QH"), *Card::ch("TS"), *Card::ch("3C"), *Card::ch("AH"), *Card::ch("JS"), *Card::ch("7H") },
		{ *Card::ch("KD"), *Card::ch("QH"), *Card::ch("TS"), *Card::ch("3C"), *Card::ch("AH"), *Card::ch("JD"), *Card::ch("9C") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("KD"), *Card::ch("QH"), *Card::ch("TS"), *Card::ch("JC"), *Card::ch("AH"), *Card::ch("4S"), *Card::ch("4H") },
		{ *Card::ch("KD"), *Card::ch("QH"), *Card::ch("TS"), *Card::ch("JC"), *Card::ch("AH"), *Card::ch("6S"), *Card::ch("6H") },
	Result::Draw), "Error");

	static_assert(run_test(
		{ *Card::ch("4C"), *Card::ch("2D"), *Card::ch("7S"), *Card::ch("7D"), *Card::ch("JC"), *Card::ch("9S"), *Card::ch("7C") },
		{ *Card::ch("4C"), *Card::ch("2D"), *Card::ch("7S"), *Card::ch("7D"), *Card::ch("JC"), *Card::ch("7H"), *Card::ch("6D") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("8S"), *Card::ch("3D"), *Card::ch("4H"), *Card::ch("7C"), *Card::ch("2S"), *Card::ch("QH"), *Card::ch("TH") },
		{ *Card::ch("8S"), *Card::ch("3D"), *Card::ch("4H"), *Card::ch("7C"), *Card::ch("2S"), *Card::ch("JC"), *Card::ch("9D") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("6S"), *Card::ch("3S"), *Card::ch("AC"), *Card::ch("KH"), *Card::ch("9H"), *Card::ch("QS"), *Card::ch("7D") },
		{ *Card::ch("6S"), *Card::ch("3S"), *Card::ch("AC"), *Card::ch("KH"), *Card::ch("9H"), *Card::ch("JH"), *Card::ch("TC") },
	Result::Win), "Error");

	static_assert(run_test(
		{ *Card::ch("AH"), *Card::ch("AD"), *Card::ch("AC"), *Card::ch("AS"), *Card::ch("8D"), *Card::ch("9H"), *Card::ch("7C") },
		{ *Card::ch("AH"), *Card::ch("AD"), *Card::ch("AC"), *Card::ch("AS"), *Card::ch("8D"), *Card::ch("7H"), *Card::ch("6D") },
	Result::Win), "Error");
}

void Test_distribution_of_seven_comb_cards(std::size_t number_of_ñard_shuffles, std::optional<Combination> opt = std::nullopt) {
	std::array<Card, Card::COUNT_OF_ALL_CARDS> cards;
	
	for (std::uint8_t i = 0; Card& c : cards) {
		c = *Card::create_by_index(i++);
	}

	static std::random_device g;

	auto ptr_to_rand_engine = std::make_unique<std::mt19937>(g());

	std::array<Card, Card::COUNT_OF_CARDS_ON_RIVER> cards_of_combinations;

	std::array<std::size_t, 10> map_of_combinations = { 0 };

	for (std::size_t i = 0; i < number_of_ñard_shuffles; ++i) {
		std::sample(
			cards.begin(), cards.end(), cards_of_combinations.begin(), Card::COUNT_OF_CARDS_ON_RIVER, *ptr_to_rand_engine
		);

		auto hand = *Poker_combination::create_combination_by_cards(cards_of_combinations);
		++map_of_combinations[static_cast<std::uint8_t>(hand.get_power())];
		
		if (opt && hand.get_power() >= *opt) {
			std::cout << "Table cards: ";
			for (std::uint8_t i = 0; i < 5; ++i) {
				std::cout << cards_of_combinations[i] << ' ';
			}
			std::cout << '\n' << "Hand cards: ";
			for (std::uint8_t i = 5; i < 7; ++i) {
				std::cout << cards_of_combinations[i] << ' ';
			}
			std::cout << '\n' << "Kickers: ";
			for (auto c : hand.get_kickers()) {
				if (c) {
					std::cout << *c << ' ';
				}
			}
			std::cout << '\n';
			hand.show_combination();
			std::cout << '\n';
		}
	}

	for (std::uint8_t i = 0; i < map_of_combinations.size(); ++i) {
		auto count = map_of_combinations[i];
		if (count != 0) {
			std::cout << static_cast<Combination>(i) << ": " << count << '\n';
		}
	}

	std::cout << '\n';
}