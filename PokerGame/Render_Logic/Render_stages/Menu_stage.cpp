#include "Menu_stage.h"

Menu_stage::Menu_stage(
	tgui::Gui& gui,
	Render_stages& stage,
	bool& game_is_running
) noexcept
	: gui(gui)
	, current_stage(stage)
	, current_game_is_running(game_is_running)
{
	game_name_label = Texture_manager::make_label(gui, { 100, 100 }, 76, tgui::Color::White, "Poker");

	new_game_button = Texture_manager::make_button(gui, { 100, 300 }, 48, "New game",
		[this]() {
			current_stage = Render_stages::Preparing;
		}
	);

	resume_game_button = Texture_manager::make_button(gui, { 100, 400 }, 48, "Resume game",
		[this]() {
			current_stage = Render_stages::Game;
		}
	);
	resume_game_button->setEnabled(false);

	settings_button = Texture_manager::make_button(gui, { 100, 500 }, 48, "Settings",
		[this]() {
			current_stage = Render_stages::Settings;
		}
	);

	exit_button = Texture_manager::make_button(gui, { 100, 600 }, 48, "Exit",
		[this]() {
			current_stage = Render_stages::Exit;
		}
	);

	float maxWidth = std::max({
		new_game_button->getSize().x,
		resume_game_button->getSize().x,
		settings_button->getSize().x,
		exit_button->getSize().x
	});

	new_game_button->setWidth(maxWidth);
	settings_button->setWidth(maxWidth);
	exit_button->setWidth(maxWidth);
}

void Menu_stage::set_visible(bool flag) noexcept {
	game_name_label->setVisible(flag);
	new_game_button->setVisible(flag);
	resume_game_button->setVisible(flag);
	settings_button->setVisible(flag);
	exit_button->setVisible(flag);

	resume_game_button->setEnabled(current_game_is_running);
}