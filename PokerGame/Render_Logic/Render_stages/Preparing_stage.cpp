#include "Preparing_stage.h"

std::shared_ptr<tgui::Label> Preparing_stage::make_label(
	std::pair<std::uint16_t, std::uint16_t> coords,
	std::uint8_t text_size,
	tgui::Color text_color,
	const std::string& text
) noexcept
{
	std::shared_ptr<tgui::Label> label = tgui::Label::create();
	label->setPosition({ coords.first, coords.second });
	label->setTextSize(text_size);
	label->getRenderer()->setTextColor(text_color);
	label->setText(text);

	gui.add(label);

	return label;
}

std::shared_ptr<tgui::Button> Preparing_stage::make_button(
	std::pair<std::uint16_t, std::uint16_t> coords,
	std::uint8_t text_size,
	const std::string& text,
	std::function<void()> func
) noexcept
{
	std::shared_ptr<tgui::Button> button = tgui::Button::create();
	button->setPosition({ coords.first, coords.second });
	button->setTextSize(text_size);
	button->setText(text);
	button->onPress(func);

	gui.add(button);

	return button;
}

std::shared_ptr<tgui::RadioButton> Preparing_stage::make_radio_button(
	std::pair<std::uint16_t, std::uint16_t> coords,
	std::uint8_t text_size,
	tgui::Color text_color,
	const std::string& text,
	std::function<void()> func
) noexcept
{
	std::shared_ptr<tgui::RadioButton> radio_button = tgui::RadioButton::create();
	radio_button->setPosition({ coords.first, coords.second });
	radio_button->setTextSize(text_size);
	radio_button->getRenderer()->setTextColor(text_color);
	radio_button->setText(text);
	radio_button->onCheck(func);

	return radio_button;
}

Preparing_stage::Preparing_stage(
	tgui::Gui& gui,
	Render_stages& stage,
	Player_difficulty& diff,
	std::uint8_t& number_of_players,
	bool& new_game,
	bool& game_is_running
) noexcept
	: gui(gui)
	, current_stage(stage)
	, current_diff(diff)
	, selected_players(number_of_players)
	, create_new_game(new_game)
	, current_game_is_running(game_is_running)
{
	preparing_label = make_label({ 100, 100 }, 76, tgui::Color::White, "Preparing game");

	select_number_of_players_label = make_label({ 100, 300 }, 48, tgui::Color::White, "Select a number of players");

	players_group = tgui::RadioButtonGroup::create();
	gui.add(players_group);

	for (std::size_t i = 0; auto& radio_button : array_of_buttons_for_selected_player) {
		int number = i + Probability_evaluator::MIN_PLAYERS;

		radio_button = make_radio_button({ 100 + 65 * i, 400 }, 36, tgui::Color::White, std::to_string(number),
			[this, number]() {
				selected_players = number;
			}
		);

		players_group->add(radio_button);

		if (selected_players == number) {
			radio_button->setChecked(true);
		}

		++i;
	}

	select_diff_of_players_label = make_label({ 100, 450 }, 48, tgui::Color::White, "Select player difficulty");

	easy_diff = make_radio_button({ 100, 550 }, 36, tgui::Color::White, "Easy",
		[this]() {
			current_diff = Player_difficulty::Easy;
		}
	);

	medium_diff = make_radio_button({ 100, 600 }, 36, tgui::Color::White, "Medium",
		[this]() {
			current_diff = Player_difficulty::Medium;
		}
	);

	hard_diff = make_radio_button({ 100, 650 }, 36, tgui::Color::White, "Hard", 
		[this]() {
			current_diff = Player_difficulty::Hard;
		}
	);

	diff_group = tgui::RadioButtonGroup::create();
	gui.add(diff_group);
	diff_group->add(easy_diff);
	diff_group->add(medium_diff);
	diff_group->add(hard_diff);

	if (current_diff == Player_difficulty::Easy) {
		easy_diff->setChecked(true);
	}
	else if (current_diff == Player_difficulty::Medium) {
		medium_diff->setChecked(true);
	}
	else {
		hard_diff->setChecked(true);
	}

	choice_poker_mode_label = make_label({ 900, 300 }, 48, tgui::Color::White, "Select type of Poker");

	no_limit_texas_holdem_radio_button = make_radio_button({ 900, 400 }, 36, tgui::Color::White, "No-Limit Texas Hold'em",
		[](){}
	);

	no_limit_texas_holdem_radio_button->setChecked(true);
	gui.add(no_limit_texas_holdem_radio_button);

	choice_game_mode_label = make_label({ 900, 450 }, 48, tgui::Color::White, "Select game mode");

	spectator_mode_radio_button = make_radio_button({ 900, 550 }, 36, tgui::Color::White, "Spectator", [](){});

	spectator_mode_radio_button->setChecked(true);
	gui.add(spectator_mode_radio_button);

	create_game_button = make_button({ 100, 800 }, 48, "Create game",
		[this]() {
			create_new_game = true;
			current_game_is_running = true;

			current_stage = Render_stages::Game;
		}
	);

	back_button = make_button({ 100, 900 }, 48, "Back to menu",
		[this]() {
			current_stage = Render_stages::Menu;
		}
	);

	float maxWidth = std::max({
		create_game_button->getSize().x,
		back_button->getSize().x,
	});

	create_game_button->setWidth(maxWidth);
	back_button->setWidth(maxWidth);
}

void Preparing_stage::set_visible(bool flag) noexcept {
	preparing_label->setVisible(flag);

	select_number_of_players_label->setVisible(flag);

	players_group->setVisible(flag);

	for (auto& radio_button : array_of_buttons_for_selected_player) {
		radio_button->setVisible(flag);
	}

	select_diff_of_players_label->setVisible(flag);

	diff_group->setVisible(flag);
	easy_diff->setVisible(flag);
	medium_diff->setVisible(flag);
	hard_diff->setVisible(flag);

	choice_poker_mode_label->setVisible(flag);
	no_limit_texas_holdem_radio_button->setVisible(flag);

	choice_game_mode_label->setVisible(flag);
	spectator_mode_radio_button->setVisible(flag);

	back_button->setVisible(flag);
	create_game_button->setVisible(flag);
}