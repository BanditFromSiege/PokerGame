#include "Preparing_stage.h"

Preparing_stage::Preparing_stage(
	tgui::Gui& gui,
	Render_stages& stage,
	Player_difficulty& diff,
	Game_mode& game_mode,
	std::uint8_t& number_of_players,
	std::size_t& initial_money,
	bool& new_game,
	bool& game_is_running
) noexcept
	: gui(gui)
	, current_stage(stage)
	, current_diff(diff)
	, current_game_mode(game_mode)
	, selected_players(number_of_players)
	, current_initial_money(initial_money)
	, create_new_game(new_game)
	, current_game_is_running(game_is_running)
{
	preparing_label = Texture_manager::make_label(gui, { 100, 100 }, 76, tgui::Color::White, "Preparing game");

	select_number_of_players_label
		= Texture_manager::make_label(gui, { 100, 300 }, 48, tgui::Color::White, "Select a number of players");

	players_group = tgui::RadioButtonGroup::create();
	gui.add(players_group);

	for (std::size_t i = 0; auto& radio_button : array_of_buttons_for_selected_player) {
		int number = i + Probability_evaluator::MIN_PLAYERS;

		radio_button = Texture_manager::make_radio_button({ 100 + 65 * i, 400 }, 36, tgui::Color::White, std::to_string(number),
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

	select_diff_of_players_label
		= Texture_manager::make_label(gui, { 100, 450 }, 48, tgui::Color::White, "Select player difficulty");

	easy_diff = Texture_manager::make_radio_button({ 100, 550 }, 36, tgui::Color::White, "Easy",
		[this]() {
			current_diff = Player_difficulty::Easy;
		}
	);

	medium_diff = Texture_manager::make_radio_button({ 100, 600 }, 36, tgui::Color::White, "Medium",
		[this]() {
			current_diff = Player_difficulty::Medium;
		}
	);

	hard_diff = Texture_manager::make_radio_button({ 100, 650 }, 36, tgui::Color::White, "Hard",
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

	choice_poker_mode_label
		= Texture_manager::make_label(gui, { 900, 300 }, 48, tgui::Color::White, "Select type of Poker");

	no_limit_texas_holdem_radio_button = Texture_manager::make_radio_button({ 900, 400 }, 36, tgui::Color::White, "No-Limit Texas Hold'em",
		[](){}
	);

	no_limit_texas_holdem_radio_button->setChecked(true);
	gui.add(no_limit_texas_holdem_radio_button);

	choice_game_mode_label
		= Texture_manager::make_label(gui, { 900, 450 }, 48, tgui::Color::White, "Select game mode");

	spectator_mode_radio_button = Texture_manager::make_radio_button({ 900, 550 }, 36, tgui::Color::White, "Spectator",
		[this]() {
			current_game_mode = Game_mode::Spectator;
		}
	);

	player_mode_radio_button = Texture_manager::make_radio_button({ 900, 600 }, 36, tgui::Color::White, "Player",
		[this]() {
			current_game_mode = Game_mode::Player;
		}
	);

	game_mode_group = tgui::RadioButtonGroup::create();
	gui.add(game_mode_group);
	game_mode_group->add(spectator_mode_radio_button);
	game_mode_group->add(player_mode_radio_button);

	if (current_game_mode == Game_mode::Spectator) {
		spectator_mode_radio_button->setChecked(true);
	}
	else if (current_game_mode == Game_mode::Player) {
		player_mode_radio_button->setChecked(true);
	}

	choice_player_stack_label
		= Texture_manager::make_label(gui, { 900, 650 }, 48, tgui::Color::White, "Select initial player stack");

	short_stack_radio_button = Texture_manager::make_radio_button({ 900, 750 }, 36, tgui::Color::White, "Short (500)",
		[this](){
			current_initial_money = 500;
		}
	);

	normal_stack_radio_button = Texture_manager::make_radio_button({ 900, 800 }, 36, tgui::Color::White, "Normal (1000)",
		[this]() {
			current_initial_money = 1000;
		}
	);

	long_stack_radio_button = Texture_manager::make_radio_button({ 900, 850 }, 36, tgui::Color::White, "Long (2000)",
		[this]() {
			current_initial_money = 2000;
		}
	);

	player_stack_group = tgui::RadioButtonGroup::create();
	gui.add(player_stack_group);
	player_stack_group->add(short_stack_radio_button);
	player_stack_group->add(normal_stack_radio_button);
	player_stack_group->add(long_stack_radio_button);

	if (current_initial_money == 500) {
		short_stack_radio_button->setChecked(true);
	}
	else if (current_initial_money == 1000) {
		normal_stack_radio_button->setChecked(true);
	}
	else if (current_initial_money == 2000) {
		long_stack_radio_button->setChecked(true);
	}

	create_game_button = Texture_manager::make_button(gui, { 100, 800 }, 48, "Create game",
		[this]() {
			create_new_game = true;
			current_game_is_running = true;
			current_stage = Render_stages::Game;
		}
	);

	back_button = Texture_manager::make_button(gui, { 100, 900 }, 48, "Back to menu",
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

	game_mode_group->setVisible(flag);
	spectator_mode_radio_button->setVisible(flag);
	player_mode_radio_button->setVisible(flag);

	choice_player_stack_label->setVisible(flag);
	player_stack_group->setVisible(flag);
	short_stack_radio_button->setVisible(flag);
	normal_stack_radio_button->setVisible(flag);
	long_stack_radio_button->setVisible(flag);

	back_button->setVisible(flag);
	create_game_button->setVisible(flag);
}