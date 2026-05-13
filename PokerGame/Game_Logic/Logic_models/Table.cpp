#include "Table.h"

Table::Table(std::size_t money, std::size_t count_of_big_blinds) noexcept {
	count_of_big_blinds = std::clamp(count_of_big_blinds, std::size_t(5), money);

	initial_big_blind = money / count_of_big_blinds;
	big_blind = initial_big_blind;
}

void Table::add_card(Card c1) noexcept {
	table_cards.push_back(c1);
}

void Table::add_to_sum_of_bets_on_current_stage(std::size_t bet) noexcept {
	sum_of_bets_on_current_stage += bet;
}

std::size_t Table::get_current_big_blind() const noexcept {
	return big_blind;
}

std::size_t Table::get_current_small_blind() const noexcept {
	return big_blind / 2;
}

std::size_t Table::get_current_bet() const noexcept {
	return current_bet;
}

std::size_t Table::get_last_bet_diff() const noexcept {
	return last_bet_diff;
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

const std::vector<Pot>& Table::get_const_pots() const noexcept {
	return pots;
}

std::size_t Table::get_sum_of_bets_on_current_stage() const noexcept {
	return sum_of_bets_on_current_stage;
}

void Table::set_current_bet(std::size_t bet) noexcept {
	if (bet > current_bet) {
		last_bet_diff = bet - current_bet;
	}
	else {
		last_bet_diff = 0;
	}
	
	current_bet = bet;
}

void Table::raise_blinds(double factor) noexcept {
	big_blind *= factor;
}

void Table::reset_big_blinds() noexcept {
	big_blind = initial_big_blind;
}

void Table::reset_sum_of_bets_on_current_stage() noexcept {
	sum_of_bets_on_current_stage = 0;
}

void Table::clear() noexcept {
	table_cards.clear();
	pots.clear();

	current_bet = 0;
	last_bet_diff = 0;
	sum_of_bets_on_current_stage = 0;
}