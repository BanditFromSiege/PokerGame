#pragma once
#include "Render_models/Showdown_render.h"
#include <numbers>

class Render_manager final {
private:
	sf::RenderWindow window;
	tgui::Gui gui;

	Table table;

	std::vector<Player> players;
	std::vector<Player_render> players_render;

	std::unique_ptr<Poker_game_manager> ptr_manager = nullptr;
	std::unique_ptr<Table_render> ptr_table_render = nullptr;

	std::unique_ptr<Showdown_render> ptr_showdown_render = nullptr;
	
	std::shared_ptr<tgui::Label> paused_label = nullptr;
	std::shared_ptr<tgui::Label> round_label = nullptr;
	std::shared_ptr<tgui::Label> win_label = nullptr;

	std::shared_ptr<tgui::Button> win_button_yes = nullptr;
	std::shared_ptr<tgui::Button> win_button_no = nullptr;

	sf::Clock clock;

	Probability_evaluator<std::execution::sequenced_policy> eval_seq;
	Probability_evaluator<std::execution::parallel_policy> eval_par;

	float delay = 1.f;
	bool paused = false;
	std::uint8_t dealer_id = 0;

public:
	Render_manager(
		std::mt19937_64& rng,
		std::pair<std::uint16_t, std::uint16_t> coords,
		std::uint8_t number_of_players
	) noexcept;

	void input() noexcept;
	void update() noexcept;
	void show() noexcept;

	void run() noexcept;
};