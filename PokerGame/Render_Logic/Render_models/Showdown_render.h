#pragma once
#include "Table_render.h"

class Showdown_render final {
private:
	tgui::Gui& gui;

	const std::vector<Player>& players;
	const std::vector<std::pair<std::size_t, std::vector<std::uint8_t>>>& winners_and_rewards;

	std::array<std::shared_ptr<tgui::Label>, Probability_evaluator<>::MAX_PLAYERS - 1> winners_and_rewards_label;

public:
	Showdown_render(
		tgui::Gui& gui,
		const std::vector<Player>& players,
		const std::vector<std::pair<std::size_t, std::vector<std::uint8_t>>>& winners_and_rewards,
		std::pair<std::uint16_t, std::uint16_t> coords
	) noexcept;

	void update_showdown() noexcept;

	void set_visible(bool flag) noexcept;

	void remove_from_gui() noexcept;
};