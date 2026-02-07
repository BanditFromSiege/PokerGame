#pragma once
#include "Enums.h"
#include <optional>

class Card final {
private:
	Card_value rank = Card_value::Two;
	Card_suit suit = Card_suit::Hearts;

public:
	static constexpr std::uint8_t COUNT_OF_CARDS_ON_PREFLOP = 2;
	static constexpr std::uint8_t COUNT_OF_CARDS_ON_FLOP = 5;
	static constexpr std::uint8_t COUNT_OF_CARDS_ON_TURN = 6;
	static constexpr std::uint8_t COUNT_OF_CARDS_ON_RIVER = 7;
	static constexpr std::uint8_t COUNT_OF_ALL_CARDS = 52;

	static constexpr std::uint8_t COUNT_OF_CARDS_IN_HAND = 2;
	static constexpr std::uint8_t COUNT_OF_TABLE_CARDS_ON_FLOP = 3;
	static constexpr std::uint8_t COUNT_OF_TABLE_CARDS_ON_TURN = 4;
	static constexpr std::uint8_t COUNT_OF_TABLE_CARDS_ON_RIVER = 5;

	constexpr Card() noexcept = default;

	constexpr Card(Card_value rank, Card_suit suit) noexcept : suit(suit), rank(rank) {}

	constexpr static std::optional<Card> ch(const char* str) noexcept {
		std::uint8_t sz = 0;
		auto copy = str;

		while (copy[sz] != '\0') {
			++sz;
		}
		
		if (sz != 2) {
			return std::nullopt;
		}
		
		return create_by_chars(str[0], str[1]);
	}

	constexpr static std::optional<Card> create_by_chars(char value, char suit) noexcept {
		Card_value v;
		switch (value) {
		case '2' : v = Card_value::Two;
			break;
		case '3' : v = Card_value::Three;
			break;
		case '4' : v = Card_value::Four;
			break;
		case '5' : v = Card_value::Five;
			break;
		case '6' : v = Card_value::Six;
			break;
		case '7' : v = Card_value::Seven;
			break;
		case '8' : v = Card_value::Eight;
			break;
		case '9' : v = Card_value::Nine;
			break;
		case 'T' : v = Card_value::Ten;
			break;
		case 'J' : v = Card_value::Jack;
			break;
		case 'Q' : v = Card_value::Queen;
			break;
		case 'K' : v = Card_value::King;
			break;
		case 'A' : v = Card_value::Ace;
			break;
		default : return std::nullopt;
		}

		Card_suit s;
		switch (suit) {
		case 'H' : s = Card_suit::Hearts;
			break;
		case 'D' : s = Card_suit::Diamonds;
			break;
		case 'C' : s = Card_suit::Clubs;
			break;
		case 'S' : s = Card_suit::Spades;
			break;
		default : return std::nullopt;
		}

		return Card{v, s};
	}

	constexpr static std::optional<Card> create_by_index(std::uint8_t index) noexcept {
		if (index > 51) {
			return std::nullopt;
		}

		const auto suit = static_cast<Card_suit>(index / 13);
		const auto rank = static_cast<Card_value>(index % 13);

		return Card{ rank, suit };
	}

	constexpr static std::uint8_t get_poker_card_value(Card_value val) noexcept {
		return static_cast<std::uint8_t>(val) + 2;
	}

	constexpr Card_suit get_suit() const noexcept {
		return suit;
	}

	constexpr Card_value get_rank() const noexcept {
		return rank;
	}

	constexpr std::uint8_t get_card_index() const noexcept {
		return 13 * static_cast<std::uint8_t>(suit) + static_cast<std::uint8_t>(rank);
	}
};

inline std::ostream& operator<<(std::ostream& out, Card c) {
	out << c.get_rank() << c.get_suit();
	return out;
}