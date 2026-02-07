#include "Table.h"

Table::Table() noexcept = default;

void Table::add_card(Card c1) noexcept {
	table_cards.push_back(c1);
}

void Table::add_to_sum_of_bets(std::size_t bet) noexcept {
	sum_of_bets += bet;
}

std::size_t Table::get_current_bet() const noexcept {
	return current_bet;
}

const std::vector<Card>& Table::get_cards() const noexcept {
	return table_cards;
}

std::size_t Table::get_bank_for_player(const Player& p) const noexcept {
	std::size_t bank = 0;

	for (auto& pot : pots) {
		if (pot.player_is_in_pot(p)) {
			bank += pot.get_bank();
		}
	}

	return bank;
}

std::vector<Pot>& Table::get_pots() noexcept {
	return pots;
}

std::size_t Table::get_sum_of_bets() const noexcept {
	return sum_of_bets;
}

void Table::set_current_bet(std::size_t bet) noexcept {
	current_bet = bet;
}

void Table::reset_sum_of_bets() noexcept {
	sum_of_bets = 0;
}

void Table::clear() noexcept {
	table_cards.clear();
	pots.clear();

	current_bet = 0;
	sum_of_bets = 0;
}