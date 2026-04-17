#pragma once
#include "Render_stages/Preparing_stage.h"
#include <numbers>

class Render_manager final {
private:
	sf::RenderWindow window;
	tgui::Gui gui;
	
	Render_stages stage = Render_stages::Menu;
	Render_stages current_stage = Render_stages::Exit;
	Render_color current_color = Render_color::Red;
	Player_difficulty current_diff = Player_difficulty::Easy;

	std::uint8_t selected_players = 4;

	bool execution_mode_sequenced = true;
	bool create_new_game = false;
	bool game_is_running = false;

	std::unique_ptr<std::mt19937_64> rng = nullptr;

	std::unique_ptr<Menu_stage> menu_stage_ptr = nullptr;
	std::unique_ptr<Game_stage> game_stage_ptr = nullptr;
	std::unique_ptr<Settings_stage> settings_stage_ptr = nullptr;
	std::unique_ptr<Preparing_stage> preparing_stage_ptr = nullptr;

public:
	Render_manager(std::pair<std::uint16_t, std::uint16_t> coords) noexcept;

	void input() noexcept;
	void update() noexcept;
	void show() noexcept;

	void run() noexcept;
};