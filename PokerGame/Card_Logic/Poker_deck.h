#pragma once
#include "Card.h"
#include <array>
#include <span>
#include <random>

class Poker_deck final {
private:
	std::array<Card, Card::COUNT_OF_ALL_CARDS> cards = {};
	std::size_t size_of_deck = 0;
	std::size_t current_card = 0;

	constexpr Poker_deck(
		std::array<Card, Card::COUNT_OF_ALL_CARDS> cards,
		std::size_t size_of_deck,
		std::size_t current_card
	) noexcept : cards(cards), size_of_deck(size_of_deck), current_card(current_card) {}

public:
	constexpr Poker_deck() noexcept = default;

	static constexpr bool check_cards_for_unique(std::span<const Card> cards) noexcept {
		std::array<bool, Card::COUNT_OF_ALL_CARDS> unique_cards_checker = { false };

		for (Card c : cards) {
			std::uint8_t index = c.get_card_index();

			if (!unique_cards_checker[index]) {
				unique_cards_checker[index] = true;
			} else {
				return false;
			}
		}

		return true;
	}

	static constexpr std::optional<Poker_deck> create_poker_deck(
		std::span<const Card> excluded_cards = {}
	) noexcept {
		if (excluded_cards.empty()) {
			std::array<Card, Card::COUNT_OF_ALL_CARDS> cards = {};

			for (std::uint8_t i = 0; i < Card::COUNT_OF_ALL_CARDS; ++i) {
				cards[i] = *Card::create_by_index(i);
			}

			return Poker_deck{cards, Card::COUNT_OF_ALL_CARDS, 0};
		}
		
		std::array<bool, Card::COUNT_OF_ALL_CARDS> unique_cards_checker = { false };

		for (Card c : excluded_cards) {
			std::uint8_t index = c.get_card_index();

			if (!unique_cards_checker[index]) {
				unique_cards_checker[index] = true;
			} else {
				return std::nullopt;
			}
		}

		std::array<Card, Card::COUNT_OF_ALL_CARDS> cards = {};
		std::size_t size = 0;

		for (std::uint8_t i = 0; i < Card::COUNT_OF_ALL_CARDS; ++i) {
			if (!unique_cards_checker[i]) {
				cards[size++] = *Card::create_by_index(i);
			}
		}

		return Poker_deck{cards, size, 0};
	}

	constexpr std::optional<Card> get_card() noexcept {
		if (current_card < size_of_deck) {
			return cards[current_card++];
		}

		return std::nullopt;
	}

	void shuffle(std::mt19937_64& rng) noexcept {
		std::shuffle(cards.begin(), cards.begin() + size_of_deck, rng);
		current_card = 0;
	}
};