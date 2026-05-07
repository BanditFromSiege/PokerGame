#include "Table_render.h"

Table_render::Table_render(
	tgui::Gui& gui,
	const Poker_game_manager& manager,
	std::pair<std::uint16_t, std::uint16_t> coords
) noexcept
	: c_ref_manager(manager)
	, gui(gui)
	, table_cards{ (gui), (gui), (gui), (gui), (gui) }
{
	blinds_label = tgui::Label::create();
	blinds_label->setTextSize(20);
	blinds_label->setPosition(coords.first + 125, coords.second - 25);
	blinds_label->getRenderer()->setTextColor(tgui::Color::White);
	blinds_label->setVisible(false);

	const auto& c_ref_players = c_ref_manager.get_players();

	std::string str = "Blinds: ";
	str += std::to_string(c_ref_players.front().get_current_big_blind() / 2);
	str += "/" + std::to_string(c_ref_players.front().get_current_big_blind());

	blinds_label->setText(std::move(str));

	gui.add(blinds_label);

	for (std::size_t i = 0; auto& card : table_cards) {
		card.set_position({coords.first + i * 75, coords.second});
		++i;
	}

	for (std::size_t i = 0; auto& label : pots_label) {
		label = tgui::Label::create();
		label->setTextSize(20);
		label->setPosition(coords.first, coords.second + 110 + i * 25);
		label->getRenderer()->setTextColor(tgui::Color::White);
		label->setVisible(false);

		gui.add(label);

		++i;
	}
}

void Table_render::update_table() noexcept {
	const auto& cards = c_ref_manager.get_table().get_cards();
	const auto& players = c_ref_manager.get_players();

	std::size_t i = 0;

	blinds_label->setVisible(true);

	for (; i < cards.size(); ++i) {
		table_cards[i].set_new_card(cards[i]);
		table_cards[i].set_visible(true);
	}

	for (; i < Card::COUNT_OF_TABLE_CARDS_ON_RIVER; ++i) {
		table_cards[i].set_visible(false);
	}

	const auto& pots = c_ref_manager.get_table().get_const_pots();

	i = 0;

	for (; i < pots.size(); ++i) {
		std::string text;
		text += "Pot_" + std::to_string(i + 1) + ' ';
		text += "[" + std::to_string(pots[i].get_bank()) + "] (";
		
		for (std::uint8_t id : pots[i].get_players_id()) {
			text += players[id].get_name() + ", ";
		}
		text.pop_back();
		text.pop_back();

		text += ")";

		pots_label[i]->setVisible(true);
		pots_label[i]->setText(std::move(text));
	}

	for (; i < pots_label.size(); ++i) {
		pots_label[i]->setText("");
		pots_label[i]->setVisible(false);
	}
}

void Table_render::set_visible(bool flag) noexcept {
	if (!flag) {
		blinds_label->setVisible(false);

		for (auto& label : pots_label) {
			label->setVisible(false);
		}

		for (auto& card : table_cards) {
			card.set_visible(false);
		}
	}
	else {
		const auto& pots = c_ref_manager.get_table().get_const_pots();

		blinds_label->setVisible(true);

		for (std::size_t i = 0; i < pots.size(); ++i) {
			pots_label[i]->setVisible(true);
		}

		const auto& cards = c_ref_manager.get_table().get_cards();

		for (std::size_t i = 0; i < cards.size(); ++i) {
			table_cards[i].set_visible(true);
		}
	}
}

void Table_render::remove_from_gui() noexcept {
	gui.remove(blinds_label);

	for (auto& label : pots_label) {
		gui.remove(label);
	}

	for (auto& card : table_cards) {
		card.remove_from_gui();
	}
}