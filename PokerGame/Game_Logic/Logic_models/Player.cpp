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

std::size_t Player::get_initial_money() const noexcept {
	return initial_money;
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

bool Player::get_is_can_show_cards() const noexcept {
	return is_can_show_cards;
}

bool Player::get_is_can_show_combination() const noexcept {
	return is_can_show_combination;
}

bool Player::get_is_can_show_relative_probability() const noexcept {
	return is_can_show_relative_probability;
}

bool Player::get_is_can_show_absolute_probability() const noexcept {
	return is_can_show_absolute_probability;
}

bool Player::get_is_can_show_type() const noexcept {
	return is_can_show_type;
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

	if (!combination) {
		is_can_show_combination = false;
	}
}

void Player::set_combination(std::optional<Poker_combination> opt_comb) noexcept {
	if (!opt_comb) {
		is_can_show_combination = false;
	}

	combination = opt_comb;
}

void Player::set_relative_probability(std::optional<double> probability) noexcept {
	if (!probability) {
		is_can_show_relative_probability = false;
	}

	relative_probability = probability;
}

void Player::set_absolute_probability(std::optional<double> probability) noexcept {
	if (!probability) {
		is_can_show_absolute_probability = false;
	}

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

	is_can_show_combination = false;
	is_can_show_absolute_probability = false;
	is_can_show_relative_probability = false;

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

void Player::set_is_can_show_cards(bool f) noexcept {
	is_can_show_cards = f;
}

void Player::set_is_can_show_combination(bool f) noexcept {
	is_can_show_combination = f;
}

void Player::set_is_can_show_relative_probability(bool f) noexcept {
	is_can_show_relative_probability = f;
}

void Player::set_is_can_show_absolute_probability(bool f) noexcept {
	is_can_show_absolute_probability = f;
}

void Player::set_is_can_show_type(bool f) noexcept {
	is_can_show_type = f;
}

void Player::set_last_move(std::optional<Player_action> new_move) noexcept {
	last_move = new_move;
}

void Player::make_fold() noexcept {
	status = Player_status::Folded;
	current_player_bet = 0;

	absolute_probability = std::nullopt;
	is_can_show_absolute_probability = false;
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

	is_can_show_cards = false;
	is_can_show_combination = false;
	is_can_show_relative_probability = false;
	is_can_show_absolute_probability = false;
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

std::pair<Player_action, std::size_t> Player::make_decision(
	std::mt19937_64& rng,
	Probability_evaluator& evaluator,
	std::span<const Card> table_cards,
	std::span<const Player> players,
	std::size_t bank_with_all_current_bets_on_street,
	std::size_t table_current_bet,
	std::size_t table_current_big_blind,
	std::size_t table_last_bet_diff
) noexcept {
	const std::size_t bet_for_call = table_current_bet - current_player_bet;

	const std::uint8_t count_of_active_players = std::count_if(players.begin(), players.end(),
		[](const Player& p) {
			return p.is_active() || p.is_all_in();
		}
	);

	std::size_t max_bet = 0;

	for (const Player& p : players) {
		if (p.get_id() != id && p.get_money() != 0 && (p.is_active() || p.is_all_in())) {
			max_bet = std::max(max_bet, p.get_money());
		}
	}

	max_bet = std::min(max_bet, money);

	auto [win_prob, table_prob] = *evaluator.get_relative_probability(
		this->get_cards(),
		table_cards,
		count_of_active_players);

	std::uniform_real_distribution<double> noise(
		-0.05 * static_cast<double>(difficulty), 0.05 * static_cast<double>(difficulty)
	);

	win_prob += noise(rng);
	win_prob -= tightness;

	win_prob = std::clamp(win_prob, 0.0, 1.0);

	relative_probability = win_prob;

	const double multiway_factor
		= static_cast<double>(count_of_active_players - 1)
		/ static_cast<double>(Probability_evaluator::MAX_PLAYERS);

	const double table_aggression_factor = 1.0 - table_prob * multiway_factor;

	const double personality_risk_factor = std::clamp(1.0 - aggression, 0.5, 1.5);

	const double final_risk_factor = table_aggression_factor * personality_risk_factor;

	std::vector<std::size_t> bets;

	double max_ev = 0.0;

	std::size_t potential_bet = bet_for_call;

	bool begin_raise_bet = true;

	if (table_last_bet_diff < table_current_big_blind) {
		table_last_bet_diff = table_current_big_blind;
	}

	do {
		const double ev =
			(win_prob * (bank_with_all_current_bets_on_street + potential_bet))
			- (potential_bet * (1 - win_prob)) * final_risk_factor;

		if (ev > max_ev) {
			max_ev = ev;
			bets.push_back(potential_bet);
		}
		else {
			break;
		}

		if (begin_raise_bet) {
			potential_bet += table_last_bet_diff;
			begin_raise_bet = false;
		}
		else {
			potential_bet += table_current_big_blind;
		}

	} while (potential_bet <= max_bet);

	std::size_t new_bet = 0;

	if (bets.size() == 1) {
		new_bet = bets.front();
	}
	else if (bets.size() > 1) {
		std::vector<std::size_t> weights;
		weights.reserve(bets.size());

		for (std::size_t i = bets.size(); i > 0; --i) {
			weights.push_back(i);
		}

		std::discrete_distribution<> random_bet_index(weights.begin(), weights.end());
		new_bet = bets[random_bet_index(rng)];
	}

	if (bet_for_call == 0) {
		if (new_bet > 0) {
			return { Player_action::Raise, new_bet + current_player_bet };
		}
		else {
			return { Player_action::Check, 0 };
		}
	}
	else {
		if (new_bet > bet_for_call) {
			return { Player_action::Raise, new_bet + current_player_bet };
		}
		else if (new_bet == bet_for_call) {
			return { Player_action::Call, table_current_bet };
		}
		else {
			return { Player_action::Fold, 0 };
		}
	}
}