#pragma once
#include "Poker_deck.h"

class Poker_combination final {
private:
	Combination power = Combination::High_card;

	Card_value high_card = Card_value::Two;

	std::array<std::optional<Card_value>, 4> kickers
		= { std::nullopt, std::nullopt, std::nullopt, std::nullopt };

	static constexpr std::optional<Poker_combination> find_best_six_cards(std::span<const Card> span_of_cards) noexcept {
		std::array<Card, Card::COUNT_OF_CARDS_ON_FLOP> combination_1 =
		{ span_of_cards[0], span_of_cards[1], span_of_cards[2], span_of_cards[3], span_of_cards[4] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_FLOP> combination_2 =
		{ span_of_cards[0], span_of_cards[1], span_of_cards[2], span_of_cards[3], span_of_cards[5] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_FLOP> combination_3 =
		{ span_of_cards[0], span_of_cards[1], span_of_cards[2], span_of_cards[5], span_of_cards[4] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_FLOP> combination_4 =
		{ span_of_cards[0], span_of_cards[1], span_of_cards[5], span_of_cards[3], span_of_cards[4] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_FLOP> combination_5 =
		{ span_of_cards[0], span_of_cards[5], span_of_cards[2], span_of_cards[3], span_of_cards[4] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_FLOP> combination_6 =
		{ span_of_cards[5], span_of_cards[1], span_of_cards[2], span_of_cards[3], span_of_cards[4] };

		std::array<std::array<Card, Card::COUNT_OF_CARDS_ON_FLOP>, Card::COUNT_OF_CARDS_ON_TURN> combinations = {
			combination_1, combination_2, combination_3, combination_4, combination_5, combination_6
		};

		std::array<Poker_combination, Card::COUNT_OF_CARDS_ON_TURN> poker_combinations;

		for (std::uint8_t i = 0; i < poker_combinations.size(); ++i) {
			auto c = create_combination_by_cards(combinations[i]);
			if (c) {
				poker_combinations[i] = *c;
			} else {
				return std::nullopt;
			}
		}

		auto res_it = std::max_element(poker_combinations.begin(), poker_combinations.end());

		return *res_it;
	}

	static constexpr std::optional<Poker_combination> find_best_seven_cards(std::span<const Card> span_of_cards) noexcept {
		std::array<Card, Card::COUNT_OF_CARDS_ON_TURN> combination_1 =
		{ span_of_cards[0], span_of_cards[1], span_of_cards[2], span_of_cards[3], span_of_cards[4], span_of_cards[5] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_TURN> combination_2 =
		{ span_of_cards[0], span_of_cards[1], span_of_cards[2], span_of_cards[3], span_of_cards[4], span_of_cards[6] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_TURN> combination_3 =
		{ span_of_cards[0], span_of_cards[1], span_of_cards[2], span_of_cards[3], span_of_cards[5], span_of_cards[6] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_TURN> combination_4 =
		{ span_of_cards[0], span_of_cards[1], span_of_cards[2], span_of_cards[4], span_of_cards[5], span_of_cards[6] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_TURN> combination_5 =
		{ span_of_cards[0], span_of_cards[1], span_of_cards[3], span_of_cards[4], span_of_cards[5], span_of_cards[6] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_TURN> combination_6 =
		{ span_of_cards[0], span_of_cards[2], span_of_cards[3], span_of_cards[4], span_of_cards[5], span_of_cards[6] };

		std::array<Card, Card::COUNT_OF_CARDS_ON_TURN> combination_7 =
		{ span_of_cards[1], span_of_cards[2], span_of_cards[3], span_of_cards[4], span_of_cards[5], span_of_cards[6] };

		std::array<std::array<Card, Card::COUNT_OF_CARDS_ON_TURN>, Card::COUNT_OF_CARDS_ON_RIVER> combinations = {
			combination_1, combination_2, combination_3, combination_4, combination_5, combination_6, combination_7
		};

		std::array<Poker_combination, Card::COUNT_OF_CARDS_ON_RIVER> poker_combinations;

		for (std::uint8_t i = 0; i < poker_combinations.size(); ++i) {
			auto c = find_best_six_cards(combinations[i]);
			if (c) {
				poker_combinations[i] = *c;
			} else {
				return std::nullopt;
			}
		}

		auto res_it = std::max_element(poker_combinations.begin(), poker_combinations.end());

		return *res_it;
	}

public:
	constexpr Combination get_power() const noexcept {
		return power;
	}

	constexpr Card_value get_high_card() const noexcept {
		return high_card;
	}

	constexpr std::array<std::optional<Card_value>, 4> get_kickers() const noexcept {
		return kickers;
	}

	constexpr Poker_combination() noexcept = default;

	constexpr Poker_combination(Combination c, Card_value v, std::array<std::optional<Card_value>, 4> k) noexcept
		: power(c), high_card(v), kickers(k) {}

	static constexpr std::optional<Poker_combination> create_combination_by_cards(std::span<const Card> span_of_cards) noexcept {
		if (span_of_cards.size() == Card::COUNT_OF_CARDS_ON_RIVER) {
			return find_best_seven_cards(span_of_cards);
		}
		else if (span_of_cards.size() == Card::COUNT_OF_CARDS_ON_TURN) {
			return find_best_six_cards(span_of_cards);
		}
		else if (span_of_cards.size() != Card::COUNT_OF_CARDS_ON_FLOP) {
			return std::nullopt;
		}

		if (!Poker_deck::check_cards_for_unique(span_of_cards)) {
			return std::nullopt;
		}

		std::array<std::uint8_t, 13> values = { 0 };
		std::array<std::uint8_t, 4> suits = { 0 };

		for (Card c : span_of_cards) {
			++values[static_cast<std::uint8_t>(c.get_rank())];
			++suits[static_cast<std::uint8_t>(c.get_suit())];
		}

		std::array<std::pair<Card_value, std::uint8_t>, Card::COUNT_OF_CARDS_ON_FLOP> player_cards;
		std::uint8_t player_cards_size = 0;

		for (std::uint8_t i = 0; i < values.size(); ++i) {
			if (values[i] != 0) {
				player_cards[player_cards_size++] = { static_cast<Card_value>(i), values[i] };
			}
		}

		if (player_cards_size < Card::COUNT_OF_CARDS_ON_PREFLOP
			|| player_cards_size > Card::COUNT_OF_CARDS_ON_FLOP)
		{
			return std::nullopt;
		}

		std::uint8_t royal_flush = 0;
		std::uint8_t straight_flush = 0;
		std::uint8_t care = 0;
		std::uint8_t full_house = 0;
		std::uint8_t flush = 0;
		std::uint8_t straight = 0;
		std::uint8_t set = 0;
		std::uint8_t two_pair = 0;
		std::uint8_t pair = 0;

		std::uint8_t flush_series = 0;

		for (auto suit : suits) {
			if (suit != 0) {
				++flush_series;
			}
		}

		if (flush_series == 1) {
			++flush;
		}

		//Straight check
		std::uint8_t straight_series = 0;
		for (std::uint8_t i = 1; i < player_cards_size; ++i) {
			if (Card::get_poker_card_value(player_cards[i].first)
				- Card::get_poker_card_value(player_cards[i - 1].first) == 1)
			{
				++straight_series;
			}
		}

		bool ace_low = false;
		if (player_cards[player_cards_size - 1].first == Card_value::Ace
			&& player_cards[0].first == Card_value::Two
			&& player_cards[player_cards_size - 2].first != Card_value::King)
		{
			++straight_series;
			ace_low = true;
		}

		if (straight_series == 4) {
			++straight;
		}

		if (straight == 1
			&& flush == 1
			&& player_cards[player_cards_size - 1].first == Card_value::Ace
			&& player_cards[0].first == Card_value::Ten)
		{
			++royal_flush;
		}

		else if (straight == 1 && flush == 1) {
			++straight_flush;
		}

		for (std::uint8_t i = 0; i < player_cards_size; ++i) {
			auto [_, count] = player_cards[i];

			if (count == 2) {
				++pair;
			} else if (count == 3) {
				++set;
			} else if (count == 4) {
				++care;
			}
		}

		if (set == 1 && pair == 1) {
			++full_house;
		} else if (pair == 2) {
			++two_pair;
		}

		// Power initialization
		std::array<std::uint8_t, 9> res = {
			royal_flush, straight_flush, care, full_house, flush, straight, set, two_pair, pair
		};

		Combination power = Combination::High_card;

		for (std::uint8_t i = 0, potential_power = 9; i < res.size(); ++i, --potential_power) {
			if (res[i] == 1) {
				power = static_cast<Combination>(potential_power);
				break;
			}
		}

		// High_Card initialization
		Card_value high_card;
		std::optional<Card_value> second_high_card;

		if (power == Combination::High_card || power == Combination::Flush
			|| power == Combination::Royal_flush)
		{
			high_card = player_cards[player_cards_size - 1].first;
		}
		else if (power == Combination::Pair) {
			for (std::uint8_t i = 0; i < player_cards_size; ++i) {
				auto [card, count] = player_cards[i];

				if (count == 2) {
					high_card = card;
					break;
				}
			}
		}
		else if (power == Combination::Two_pair) {
			bool high_card_checker = true;
			for (std::uint8_t i = player_cards_size; i-- > 0;) {
				auto [card, count] = player_cards[i];

				if (count == 2 && high_card_checker) {
					high_card = card;
					high_card_checker = false;
				}
				else if (count == 2 && !high_card_checker) {
					second_high_card = card;
				}
			}
		}
		else if (power == Combination::Set) {
			for (std::uint8_t i = 0; i < player_cards_size; ++i) {
				auto [card, count] = player_cards[i];

				if (count == 3) {
					high_card = card;
					break;
				}
			}
		}
		else if (power == Combination::Full_house) {
			for (std::uint8_t i = 0; i < player_cards_size; ++i) {
				auto [card, count] = player_cards[i];

				if (count == 3) {
					high_card = card;
				}
				else if (count == 2) {
					second_high_card = card;
				}
			}
		}
		else if (power == Combination::Care) {
			for (std::uint8_t i = 0; i < player_cards_size; ++i) {
				auto [card, count] = player_cards[i];

				if (count == 4) {
					high_card = card;
					break;
				}
			}
		}
		else if (power == Combination::Straight || power == Combination::Straight_flush) {
			high_card = player_cards[player_cards_size - 1].first;
			if (ace_low) {
				high_card = player_cards[player_cards_size - 2].first;
			}
		}

		//kickers initialization
		std::uint8_t count_of_kickers = 0;
		std::array<std::optional<Card_value>, 4> kickers
			= { std::nullopt, std::nullopt, std::nullopt, std::nullopt };

		for (std::uint8_t i = 0; i < player_cards_size; ++i) {
			auto [card, count] = player_cards[i];

			if (card != high_card && card != second_high_card) {
				kickers[count_of_kickers++] = card;
			}
		}

		if (second_high_card) {
			kickers[count_of_kickers++] = second_high_card;
		}

		if (!ace_low) {
			std::reverse(kickers.begin(), kickers.end());
		} else {
			std::reverse(kickers.begin(), std::prev(kickers.end()));
		}

		return Poker_combination{ power, high_card, kickers };
	}

	void show_combination() const noexcept {
		std::cout << power << ": High cart - " << high_card << '\n';
	}
};

constexpr bool operator<(const Poker_combination& comb1, const Poker_combination& comb2) noexcept {
	if (comb1.get_power() == comb2.get_power()) {
		if (comb1.get_high_card() == comb2.get_high_card()) {
			auto kickers1 = comb1.get_kickers();
			auto kickers2 = comb2.get_kickers();

			for (std::uint8_t i = 0; i < kickers1.size(); ++i) {
				if ((kickers1[i] && kickers2[i])
					&& (Card::get_poker_card_value(*kickers1[i]) != Card::get_poker_card_value(*kickers2[i])))
				{
					return Card::get_poker_card_value(*kickers1[i]) < Card::get_poker_card_value(*kickers2[i]);
				}
			}
			return false;
		}
		return Card::get_poker_card_value(comb1.get_high_card()) < Card::get_poker_card_value(comb2.get_high_card());
	}
	return comb1.get_power() < comb2.get_power();
}

constexpr bool operator>(const Poker_combination& comb1, const Poker_combination& comb2) noexcept {
	return comb2 < comb1;
}

constexpr bool operator==(const Poker_combination& comb1, const Poker_combination& comb2) noexcept {
	return !(comb1 < comb2) && !(comb2 < comb1);
}

constexpr bool operator!=(const Poker_combination& comb1, const Poker_combination& comb2) noexcept {
	return !(comb1 == comb2);
}

constexpr bool operator<=(const Poker_combination& comb1, const Poker_combination& comb2) noexcept {
	return !(comb2 < comb1);
}

constexpr bool operator>=(const Poker_combination& comb1, const Poker_combination& comb2) noexcept {
	return comb2 <= comb1;
}

constexpr Result compare_combinations(const Poker_combination& comb1, const Poker_combination& comb2) noexcept {
	if (comb1 < comb2) {
		return Result::Loss;
	}
	else if (comb1 > comb2) {
		return Result::Win;
	}
	else {
		return Result::Draw;
	}
}