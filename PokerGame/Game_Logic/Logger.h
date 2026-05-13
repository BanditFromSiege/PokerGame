#pragma once
#include "Poker_game_manager.h"
#include <chrono>
#include <format>

class Logger final {
private:
	const Poker_game_manager& c_ref_manager;

	std::optional<Poker_stage> last_stage = std::nullopt;
	std::array<bool, Probability_evaluator::MAX_PLAYERS> outed_players = { false };

	std::string get_time() noexcept;
	std::string get_data_time() noexcept;

	std::string get_str_table() noexcept;
	std::string get_str_showdown() noexcept;

	std::string log_player_action(std::uint8_t i) noexcept;
	std::string log_stage(Poker_stage stage) noexcept;

public:
	Logger(const Poker_game_manager& manager) noexcept;

	void reset() noexcept;

	std::string get_message() noexcept;
};
