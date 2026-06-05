#pragma once
#include "Logic_models/Table.h"

class Poker_game_manager final {
private:
	static constexpr std::uint8_t round_per_blind_increase = 10;

	Poker_deck deck;
	Table table;

	std::vector<std::uint8_t> players_index{};
	std::vector<std::pair<std::size_t, std::vector<std::uint8_t>>> winners_and_rewards{};

	std::optional<std::pair<Player_action, std::size_t>> player_move = std::nullopt;
	
	std::vector<Player>& players;
	std::mt19937_64& rng;

	std::array<bool, Probability_evaluator::MAX_PLAYERS> arr_players_ids_in_game = { false };

	std::size_t number_of_rounds = 0;

	std::optional<std::uint8_t> current_player_id = std::nullopt;

	Probability_evaluator evaluator;

	Game_mode game_mode = Game_mode::Spectator;
	Poker_stage stage = Poker_stage::Preparation_preflop;

	std::uint8_t dealer_id = 0;
	std::uint8_t small_blind_id = 0;

	std::uint8_t count_active_players = 0;
	std::uint8_t count_all_in_players = 0;
	std::uint8_t count_players_in_game = 0;

	std::uint8_t current_player_index_id = 0;
	std::uint8_t current_bank_index_id = 0;

	bool player_turn = false;
	bool is_game_run = true;

	void rotate_players(std::uint8_t new_index) noexcept;
	std::uint8_t get_next_id(std::uint8_t index) noexcept;

	void find_absolute_probabilities_for_players() noexcept;

	std::uint8_t make_blinds_for_2_players() noexcept;
	std::uint8_t make_blinds_for_3_players() noexcept;
	std::uint8_t make_blinds_for_more_then_3_players() noexcept;

	void prepare_to_Preflop() noexcept;
	void prepare_to_Flop() noexcept;
	void prepare_to_Turn() noexcept;
	void prepare_to_River() noexcept;
	void prepare_to_Showdown() noexcept;

	void prepare_players_to_next_stage() noexcept;

	void add_bets_to_pots() noexcept;

	void perform_stage() noexcept;
	void perform_player_step() noexcept;
	void perform_showdown_step() noexcept;

	bool reset_for_next_round() noexcept;

public:
	Poker_game_manager(
		std::mt19937_64& rng,
		std::vector<Player>& players,
		Game_mode mode
	) noexcept;

	const Table& get_table() const noexcept;
	const std::vector<Player>& get_players() const noexcept;

	Game_mode get_current_game_mode() const noexcept;
	Poker_stage get_current_stage() const noexcept;

	const std::optional<std::uint8_t> get_current_player_id() const noexcept;
	const std::uint8_t get_dealer_player_id() const noexcept;

	std::uint8_t get_count_active_players() const noexcept;
	std::uint8_t get_count_all_in_players() const noexcept;
	std::uint8_t get_count_players_in_game() const noexcept;

	auto get_winners_and_rewards() const noexcept
		-> const std::vector<std::pair<std::size_t, std::vector<std::uint8_t>>>&;

	std::size_t get_number_of_rounds() const noexcept;
	std::optional<std::uint8_t> get_winner_id() const noexcept;

	void set_evaluator_sequenced_policy() noexcept;
	void set_evaluator_parallel_policy() noexcept;

	std::pair<std::size_t, std::size_t> get_call_bet_for_player() const noexcept;
	std::pair<std::size_t, std::size_t> get_min_max_bet_raise_for_player() const noexcept;

	bool is_player_turn() noexcept;
	void set_player_turn(std::pair<Player_action, std::size_t> move) noexcept;

	void reset_for_new_game() noexcept;

	bool is_game_still_run() const noexcept;

	void call_next_move() noexcept;
};