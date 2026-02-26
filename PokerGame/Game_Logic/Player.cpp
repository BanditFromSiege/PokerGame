#include "Player.h"

Player::Player() noexcept = default;

Player::Player(const char* name, std::uint8_t id, std::size_t money, Player_difficulty d)
	: name(name), id(id), money(money), current_big_blind(money / count_of_big_blinds), difficulty(d) {
}

std::string Player::get_name() const noexcept {
	return name;
}

std::array<Card, Card::COUNT_OF_CARDS_IN_HAND> Player::get_cards() const noexcept {
	return cards;
}

std::pair<Card, Card> Player::get_pair_of_cards() const noexcept {
	return { cards[0], cards[1] };
}

std::size_t Player::get_money() const noexcept {
	return money;
}

std::size_t Player::get_current_bet() const noexcept {
	return current_bet;
}

std::size_t Player::get_sum_of_bets() const noexcept {
	return sum_of_bets;
}

Player_status Player::get_player_status() const noexcept {
	return status;
}

std::uint8_t Player::get_id() const noexcept {
	return id;
}

std::size_t Player::get_current_big_blind() const noexcept {
	return current_big_blind;
}

std::size_t Player::get_last_bet() const noexcept {
	return last_bet;
}

std::size_t Player::get_bet_difference() const noexcept {
	return bet_difference;
}

Player_action Player::get_last_move() const noexcept {
	return last_move;
}

bool Player::is_active() const noexcept {
	return status == Player_status::Active;
}

bool Player::is_in_game() const noexcept {
	return status != Player_status::Out_game;
}

bool Player::is_all_in() const noexcept {
	return status == Player_status::All_in;
}

void Player::check_money_enough() noexcept {
	status = (money <= 0) ? Player_status::Out_game : status;
}

void Player::set_current_bet(std::size_t bet) noexcept {
	current_bet = bet;
}

void Player::set_sum_of_bets(std::size_t bet) noexcept {
	sum_of_bets = bet;
}

void Player::set_cards(Card c1, Card c2) noexcept {
	cards[0] = c1;
	cards[1] = c2;
	status = Player_status::Active;
}

void Player::set_id(std::uint8_t index) noexcept {
	id = index;
}

void Player::set_action(Player_action new_move) noexcept {
	last_move = new_move;
}

void Player::make_fold() noexcept {
	status = Player_status::Folded;
	current_bet = 0;
}

void Player::get_win(std::size_t share) noexcept {
	money += share;
}

void Player::reset_for_new_hand() noexcept {
	current_bet = 0;
	sum_of_bets = 0;
	bet_difference = 0;
	last_bet = 0;
}

std::size_t Player::make_bet_or_check(std::size_t bet) noexcept {
	std::size_t diff = std::min<std::int64_t>(bet - current_bet, money);

	if (diff == money) {
		status = Player_status::All_in;
		current_bet = money;
	} else {
		current_bet = bet;
	}

	money -= diff;
	sum_of_bets += diff;

	bet_difference = diff;
	last_bet = bet;
	
	return diff;
}