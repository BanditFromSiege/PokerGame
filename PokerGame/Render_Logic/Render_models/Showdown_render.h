#pragma once
#include "Table_render.h"

class Showdown_render final {
private:
	tgui::Gui& gui;

	const Poker_game_manager& c_ref_manager;
	std::array<std::shared_ptr<tgui::Label>, Probability_evaluator<>::MAX_PLAYERS - 1> winners_and_rewards_label
		= { nullptr };

public:
	Showdown_render(
		tgui::Gui& gui,
		const Poker_game_manager& manager,
		std::pair<std::uint16_t, std::uint16_t> coords
	) noexcept;

	void update_showdown() noexcept;

	void set_visible(bool flag) noexcept;

	void remove_from_gui() noexcept;
};