#pragma once
#include "Player_render.h"

class Table_render final {
private:
	tgui::Gui& gui;

	const Poker_game_manager& c_ref_manager;

	std::shared_ptr<tgui::Label> blinds_label = nullptr;
	std::shared_ptr<tgui::Label> current_bet_label = nullptr;
	std::array<std::shared_ptr<tgui::Label>, Probability_evaluator::MAX_PLAYERS - 1> pots_label
		= { nullptr };

	std::array<Card_render, Card::COUNT_OF_TABLE_CARDS_ON_RIVER> table_cards;

public:
	Table_render(
		tgui::Gui& gui,
		const Poker_game_manager& manager,
		std::pair<std::uint16_t, std::uint16_t> coords
	) noexcept;

	void update_table() noexcept;
	
	void set_visible(bool flag) noexcept;

	void remove_from_gui() noexcept;
};