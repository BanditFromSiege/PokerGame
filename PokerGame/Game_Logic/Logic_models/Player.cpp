#include "Player.h"

Player::Player() noexcept = default;

Player::Player(
	std::mt19937_64& rng,
	std::string name,
	std::uint8_t id,
	std::size_t money,
	Player_difficulty d
) noexcept
	: name(std::move(name))
	, id(id)
	, initial_money(money)
	, money(money)
	, difficulty(d) 
{
	std::uniform_int_distribution<> random_type(0, 3);

	std::uniform_real_distribution<> aggresive(0.0, 0.1);
	std::uniform_real_distribution<> passive(-0.1, 0.0);
	std::uniform_real_distribution<> taight_parrametr(0.0, 0.1);
	std::uniform_real_distribution<> loose_parrametr(-0.1, 0.0);

	auto index = random_type(rng);

	if (index == 0) {
		type = Player_type::TAG;
		tightness = taight_parrametr(rng);
		aggression = aggresive(rng);
	}
	else if (index == 1) {
		type = Player_type::TP;
		tightness = taight_parrametr(rng);
		aggression = passive(rng);
	}
	else if (index == 2) {
		type = Player_type::LAG;
		tightness = loose_parrametr(rng);
		aggression = aggresive(rng);
	}
	else if (index == 3) {
		type = Player_type::LP;
		tightness = loose_parrametr(rng);
		aggression = passive(rng);
	}
}

std::string Player::get_name() const noexcept {
	return name;
}

std::optional<Poker_combination> Player::get_combination() const noexcept {
	return combination;
}

std::optional<double> Player::get_relative_probability() const noexcept {
	return relative_probability;
}

std::optional<double> Player::get_absolute_probability() const noexcept {
	return absolute_probability;
}

std::size_t Player::get_money() const noexcept {
	return money;
}

std::size_t Player::get_current_player_bet() const noexcept {
	return current_player_bet;
}

std::size_t Player::get_sum_of_bets() const noexcept {
	return sum_of_bets;
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

Player_difficulty Player::get_difficulty() const noexcept {
	return difficulty;
}

Player_status Player::get_status() const noexcept {
	return status;
}

Player_type Player::get_type() const noexcept {
	return type;
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

void Player::set_relative_probability(std::optional<double> probability) noexcept {
	relative_probability = probability;
}

void Player::set_absolute_probability(std::optional<double> probability) noexcept {
	absolute_probability = probability;
}

void Player::set_current_player_bet(std::size_t bet) noexcept {
	current_player_bet = bet;
}

void Player::set_sum_of_bets(std::size_t bet) noexcept {
	sum_of_bets = bet;
}

void Player::set_cards(Card c1, Card c2) noexcept {
	combination = std::nullopt;

	absolute_probability = std::nullopt;
	relative_probability = std::nullopt;

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
	current_player_bet = 0;
	absolute_probability = std::nullopt;
}

void Player::get_win(std::size_t share) noexcept {
	money += share;
}

void Player::reset_for_new_round() noexcept {
	current_player_bet = 0;
	sum_of_bets = 0;

	combination = std::nullopt;
	absolute_probability = std::nullopt;
	relative_probability = std::nullopt;
	last_move = std::nullopt;
}

void Player::reset_for_new_game() noexcept {
	reset_for_new_round();
	money = initial_money;
}

std::size_t Player::make_bet_or_check(std::size_t bet) noexcept {
	std::size_t diff = bet - current_player_bet;

	if (diff >= money) {
		diff = money;
		status = Player_status::All_in;
		current_player_bet += diff;
	}
	else {
		current_player_bet = bet;
	}

	money -= diff;
	sum_of_bets += diff;
	
	return diff;
}