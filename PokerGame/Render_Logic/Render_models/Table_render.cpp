#include "Table_render.h"

Table_render::Table_render(
	tgui::Gui& gui,
	const Table& table,
	std::pair<std::uint16_t, std::uint16_t> coords
) noexcept : table(table), table_cards{ (gui), (gui), (gui), (gui), (gui) } {
	for (std::size_t i = 0; auto& card : table_cards) {
		card.set_position({coords.first + i * 75, coords.second});
		++i;
	}

	for (std::size_t i = 0; auto& label : pots_label) {
		label = tgui::Label::create();
		label->setTextSize(20);
		label->setPosition(coords.first + 150, coords.second + 110 + i * 25);
		label->getRenderer()->setTextColor(tgui::Color::White);
		label->setVisible(false);

		gui.add(label);

		++i;
	}
}

void Table_render::update_table() noexcept {
	const auto& cards = table.get_cards();

	std::size_t i = 0;

	for (; i < cards.size(); ++i) {
		table_cards[i].set_new_card(cards[i]);
		table_cards[i].set_visible(true);
	}

	for (; i < Card::COUNT_OF_TABLE_CARDS_ON_RIVER; ++i) {
		table_cards[i].set_visible(false);
	}

	const auto& pots = table.get_const_pots();
	i = 0;

	for (; i < pots.size(); ++i) {
		std::string text;
		text += "Pot_" + std::to_string(i + 1) + ' ';
		text += std::to_string(pots[i].get_bank()) + ": ";
		
		for (std::uint8_t id : pots[i].get_players_id()) {
			text += std::to_string(id) + ' ';
		}

		pots_label[i]->setVisible(true);
		pots_label[i]->setText(std::move(text));
	}

	for (; i < pots_label.size(); ++i) {
		pots_label[i]->setText("");
		pots_label[i]->setVisible(false);
	}
}

void Table_render::hide_table() noexcept {
	for (auto& card : table_cards) {
		card.set_visible(false);
	}

	for (auto& pot : pots_label) {
		pot->setText("");
		pot->setVisible(false);
	}
}