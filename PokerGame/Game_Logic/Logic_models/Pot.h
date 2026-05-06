#pragma once
#include "Player.h"

class Pot final {
private:
	std::vector<std::uint8_t> players_id_in_pot = {};
	std::size_t bank = 0;

public:
	Pot() noexcept;
	Pot(std::vector<std::uint8_t> vec, std::size_t bank) noexcept;

	bool player_is_in_pot(const Player& p) const noexcept;

	const std::vector<std::uint8_t>& get_players_id() const noexcept;
	std::size_t get_bank() const noexcept;

	void set_players_id_in_pot(std::vector<std::uint8_t> vec) noexcept;
	void set_bank(std::size_t b) noexcept;
};