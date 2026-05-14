#pragma once
#include "../Enums.h"
#include "../../Card_Logic/Probability_evaluator.h"

class Player final {
private:
	std::string name;

	std::optional<Poker_combination> combination = std::nullopt;

	std::optional<double> relative_probability = std::nullopt;
	std::optional<double> absolute_probability = std::nullopt;

	double tightness = 0.0;
	double aggression = 0.0;

	const std::size_t initial_money = 0;

	std::size_t money = 0;
	std::size_t current_player_bet = 0;
	std::size_t sum_of_bets = 0;

	std::array<Card, Card::COUNT_OF_CARDS_IN_HAND> cards = {};

	std::optional<Player_action> last_move = std::nullopt;
	std::uint8_t id = 0;

	Player_difficulty difficulty = Player_difficulty::Easy;
	Player_status status = Player_status::Active;
	Player_type type = Player_type::TAG;

public:
	Player() noexcept;

	Player(
		std::mt19937_64& rng,
		std::string name,
		std::uint8_t id,
		std::size_t money,
		Player_difficulty d
	) noexcept;

	std::string get_name() const noexcept;
	std::optional<Poker_combination> get_combination() const noexcept;

	std::optional<double> get_relative_probability() const noexcept;
	std::optional<double> get_absolute_probability() const noexcept;

	std::size_t get_initial_money() const noexcept;

	std::size_t get_money() const noexcept;
	std::size_t get_current_player_bet() const noexcept;
	std::size_t get_sum_of_bets() const noexcept;

	std::array<Card, Card::COUNT_OF_CARDS_IN_HAND> get_cards() const noexcept;
	std::pair<Card, Card> get_pair_of_cards() const noexcept;

	std::optional<Player_action> get_last_move() const noexcept;
	std::uint8_t get_id() const noexcept;

	Player_difficulty get_difficulty() const noexcept;
	Player_status get_status() const noexcept;
	Player_type get_type() const noexcept;

	bool is_active() const noexcept;
	bool is_in_game() const noexcept;
	bool is_all_in() const noexcept;

	void check_money_enough() noexcept;

	void set_combination(const std::vector<Card>& table_cards) noexcept;

	void set_relative_probability(std::optional<double> probability) noexcept;
	void set_absolute_probability(std::optional<double> probability) noexcept;

	void set_current_player_bet(std::size_t bet) noexcept;
	void set_sum_of_bets(std::size_t bet) noexcept;
	void set_cards(Card c1, Card c2) noexcept;
	void set_last_move(std::optional<Player_action> new_move) noexcept;
	void set_id(std::uint8_t index) noexcept;
	void set_status(Player_status new_status) noexcept;

	void make_fold() noexcept;
	void get_win(std::size_t share) noexcept;

	void reset_for_new_round() noexcept;
	void reset_for_new_game() noexcept;

	std::size_t make_bet_or_check(std::size_t bet) noexcept;

	std::pair<Player_action, std::size_t> make_decision(
		std::mt19937_64& rng,
		Probability_evaluator& evaluator,
		std::span<const Card> table_cards,
		std::span<const Player> players,
		std::size_t bank_with_all_current_bets_on_street,
		std::size_t table_current_bet,
		std::size_t table_current_big_blind,
		std::size_t table_last_bet_diff
	) noexcept;
};