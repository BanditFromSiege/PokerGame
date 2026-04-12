#include "Showdown_render.h"

Showdown_render::Showdown_render(
	tgui::Gui& gui,
	const std::vector<Player>& players,
	std::pair<std::uint16_t, std::uint16_t> coords
) noexcept : players(players) {
	for (std::size_t i = 0; auto& label : winners_and_rewards_label) {
		label = tgui::Label::create();
		label->setTextSize(20);
		label->setPosition(coords.first, coords.second + i * 30);
		label->getRenderer()->setTextColor(tgui::Color::White);
		label->setVisible(false);

		gui.add(label);

		++i;
	}
}

void Showdown_render::update_showdown(
	const std::vector<std::pair<std::size_t, std::vector<std::uint8_t>>>& winners_and_rewards
) noexcept {
	std::size_t i = 0;

	for (; auto& [money, ids] : winners_and_rewards) {
		std::string text;
		text += "Pot " + std::to_string(i + 1) + ": ";
		text += std::to_string(money) + ", Winners: ";

		for (std::uint8_t id : ids) {
			text += players[id].get_name() + ' ';
		}

		if (auto opt_comb = players[ids.front()].get_combination(); opt_comb) {
			text += ": Combination: ";
			std::stringstream ss;
			ss << players[ids.front()].get_combination()->get_power();

			text += ss.str();

			winners_and_rewards_label[i]->setVisible(true);
			winners_and_rewards_label[i]->setText(std::move(text));
		}
		
		++i;
	}

	for (; i < winners_and_rewards_label.size(); ++i) {
		winners_and_rewards_label[i]->setText("");
		winners_and_rewards_label[i]->setVisible(false);
	}
}

void Showdown_render::hide_showdown() noexcept {
	for (auto& label : winners_and_rewards_label) {
		label->setText("");
		label->setVisible(false);
	}
}