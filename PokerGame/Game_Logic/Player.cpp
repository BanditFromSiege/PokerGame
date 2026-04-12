#include "Player.h"

Player::Player() noexcept = default;

Player::Player(std::string name, std::uint8_t id, std::size_t money, Player_difficulty d)
	: name(std::move(name))
	, id(id)
	, initial_money(money)
	, money(money)
	, current_big_blind(money / count_of_big_blinds)
	, difficulty(d) 
{}

std::string Player::get_name() const noexcept {
	return name;
}

std::optional<Poker_combination> Player::get_combination() const noexcept {
	return combination;
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

std::size_t Player::get_current_big_blind() const noexcept {
	return current_big_blind;
}

std::size_t Player::get_bet_difference() const noexcept {
	return bet_difference;
}

std::array<Card, Card::COUNT_OF_CARDS_IN_HAND> Player::get_cards() const noexcept {
	return cards;
}

std::pair<Card, Card> Player::get_pair_of_cards() const noexcept {
	return { cards[0], cards[1] };
}

std::optional<Player_action> Player::get_last_move() const noexcept {
	return last_move;
}

std::uint8_t Player::get_id() const noexcept {
	return id;
}

Player_status Player::get_status() const noexcept {
	return status;
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

void Player::set_combination(const std::vector<Card>& table_cards) noexcept {
	std::vector<Card> new_span(table_cards.begin(), table_cards.end());
	new_span.push_back(cards[0]);
	new_span.push_back(cards[1]);

	combination = Poker_combination::create_combination_by_cards(new_span);
}

void Player::set_current_bet(std::size_t bet) noexcept {
	current_bet = bet;
}

void Player::set_sum_of_bets(std::size_t bet) noexcept {
	sum_of_bets = bet;
}

void Player::set_cards(Card c1, Card c2) noexcept {
	combination = std::nullopt;
	cards[0] = c1;
	cards[1] = c2;
	status = Player_status::Active;
}

void Player::set_id(std::uint8_t index) noexcept {
	id = index;
}

void Player::set_status(Player_status new_status) noexcept {
	status = new_status;
}

void Player::set_last_move(std::optional<Player_action> new_move) noexcept {
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
	last_move = std::nullopt;
}

void Player::reset_for_new_game() noexcept {
	reset_for_new_hand();
	money = initial_money;
}

std::size_t Player::make_bet_or_check(std::size_t bet) noexcept {
	std::size_t diff = bet - current_bet;

	if (diff >= money) {
		diff = money;
		status = Player_status::All_in;
		current_bet += diff;
	} else {
		current_bet = bet;
	}

	money -= diff;
	sum_of_bets += diff;

	bet_difference = diff;
	
	return diff;
}