#pragma once
#include "Pot.h"

class Table final {
private:
	std::vector<Card> table_cards = {};
	std::vector<Pot> pots = {};

	std::size_t current_bet = 0;
	std::size_t sum_of_bets = 0;

public:
	Table() noexcept;

	void add_card(Card c1) noexcept;
	void add_to_sum_of_bets(std::size_t bet) noexcept;

	std::size_t get_current_bet() const noexcept;
	const std::vector<Card>& get_cards() const noexcept;
	std::size_t get_bank_for_player(const Player& p) const noexcept;

	std::vector<Pot>& get_pots() noexcept;
	std::size_t get_sum_of_bets() const noexcept;

	void set_current_bet(std::size_t bet) noexcept;
	void reset_sum_of_bets() noexcept;
	void clear() noexcept;
};