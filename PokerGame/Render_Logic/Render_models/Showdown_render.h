#pragma once
#include "Table_render.h"

class Showdown_render final {
private:
	const std::vector<Player>& players;
	std::array<std::shared_ptr<tgui::Label>, Probability_evaluator<>::MAX_PLAYERS - 1> winners_and_rewards_label;

public:
	Showdown_render(
		tgui::Gui& gui,
		const std::vector<Player>& players,
		std::pair<std::uint16_t, std::uint16_t> coords
	) noexcept;

	void update_showdown(const std::vector<std::pair<std::size_t, std::vector<std::uint8_t>>>& winners_and_rewards) noexcept;
	void hide_showdown() noexcept;
};