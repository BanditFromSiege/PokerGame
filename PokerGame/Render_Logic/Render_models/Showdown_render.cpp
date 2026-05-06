#include "Showdown_render.h"

Showdown_render::Showdown_render(
	tgui::Gui& gui,
	const Poker_game_manager& manager,
	std::pair<std::uint16_t, std::uint16_t> coords
) noexcept
	: gui(gui)
	, c_ref_manager(manager)
{
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

void Showdown_render::update_showdown() noexcept {
	const auto& c_ref_winners_and_rewards = c_ref_manager.get_winners_and_rewards();
	const auto& c_ref_players = c_ref_manager.get_players();

	std::size_t i = 0;

	for (; const auto& [money, ids] : c_ref_winners_and_rewards) {
		std::string text;
		text += "Pot " + std::to_string(i + 1);
		text += " [" + std::to_string(money) + "] Winners: ";

		for (std::uint8_t id : ids) {
			text += c_ref_players[id].get_name() + ", ";
		}
		text.pop_back();
		text.pop_back();

		if (auto opt_comb = c_ref_players[ids.front()].get_combination(); opt_comb) {
			text += " (Combination - ";
			text += combination_to_c_str(opt_comb->get_power());
			text += ')';

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

void Showdown_render::set_visible(bool flag) noexcept {
	const auto& c_ref_winners_and_rewards = c_ref_manager.get_winners_and_rewards();

	if (!flag) {
		for (auto& label : winners_and_rewards_label) {
			label->setVisible(false);
		}
	}
	else {
		for (std::size_t i = 0; i < c_ref_winners_and_rewards.size(); ++i) {
			winners_and_rewards_label[i]->setVisible(true);
		}
	}
}

void Showdown_render::remove_from_gui() noexcept {
	for (auto& label : winners_and_rewards_label) {
		gui.remove(label);
	}
}