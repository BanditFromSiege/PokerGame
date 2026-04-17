#pragma once
#include "Table.h"
#include <unordered_set>

class Poker_game_manager final {
private:
	Poker_deck deck;

	std::unordered_set<std::uint8_t> players_ids_in_game{};

	std::vector<std::uint8_t> players_index{};
	std::vector<std::pair<std::size_t, std::vector<std::uint8_t>>> winners_and_rewards{};

	Table& table;
	std::vector<Player>& players;
	std::mt19937_64& rng;
	Probability_evaluator<std::execution::sequenced_policy>& evaluator_sequenced;
	Probability_evaluator<std::execution::parallel_policy>& evaluator_parallel;

	std::size_t number_of_rounds = 0;

	std::optional<std::uint8_t> current_player_id = std::nullopt;

	Poker_stage stage = Poker_stage::Preparation_preflop;

	std::uint8_t dealer_id = 0;
	std::uint8_t small_blind_id = 0;

	std::uint8_t active_players = 0;

	std::uint8_t current_player_index_id = 0;
	std::uint8_t current_bank_index_id = 0;

	bool use_evaluator_sequenced = true;
	bool is_game_run = true;

	void rotate_players(std::uint8_t new_index) noexcept;
	std::uint8_t get_next_id(std::uint8_t index) noexcept;

	void make_initial_bets_for_2_players(std::uint8_t& current_player_index) noexcept;
	void make_initial_bets_for_3_players(std::uint8_t& current_player_index) noexcept;
	void make_initial_bets_for_more_then_3_players(std::uint8_t& current_player_index) noexcept;

	void prepare_to_Preflop() noexcept;
	void prepare_to_Flop() noexcept;
	void prepare_to_Turn() noexcept;
	void prepare_to_River() noexcept;
	void prepare_to_Showdown() noexcept;

	void add_bets_to_pots() noexcept;

	void perform_stage() noexcept;
	void perform_player_step() noexcept;
	void perform_showdown_step() noexcept;

	bool reset_for_next_round() noexcept;

public:
	Poker_game_manager(
		Table& table,
		std::mt19937_64& rng,
		std::vector<Player>& players,
		Probability_evaluator<std::execution::sequenced_policy>& eval_seq,
		Probability_evaluator<std::execution::parallel_policy>& eval_par
	) noexcept;

	const Table& get_table() noexcept;
	Poker_stage get_current_stage() noexcept;

	const std::optional<std::uint8_t> get_current_player_id() noexcept;
	const std::uint8_t get_dealer_player_id() noexcept;

	std::uint8_t get_active_players() noexcept;
	auto get_winners_and_rewards() noexcept
		-> const std::vector<std::pair<std::size_t, std::vector<std::uint8_t>>>&;

	std::size_t get_number_of_rounds() noexcept;

	void set_evaluator_sequenced_policy() noexcept;
	void set_evaluator_parallel_policy() noexcept;

	void reset_for_new_game() noexcept;
	bool is_game_still_run() noexcept;

	void call_next_move() noexcept;
};