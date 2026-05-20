#pragma once
#include "../Enums.h"
#include "../Render_models/Showdown_render.h"
#include <numbers>
#include <fstream>

class Game_stage final {
private:
	tgui::Gui& gui;

	std::ofstream file;

	Render_stages& current_stage;
	Render_color& current_color;
	Player_difficulty& current_diff;
	std::size_t& current_initial_money;

	bool& execution_mode_sequenced;
	bool& create_new_game;
	bool& current_game_is_running;

	std::uint8_t& selected_players;
	std::mt19937_64& rng;

	std::vector<Player> players{};
	std::vector<Player_render> players_render{};

	std::deque<std::string> log_buffer;

	std::shared_ptr<tgui::Label> paused_label = nullptr;
	std::shared_ptr<tgui::Label> round_label = nullptr;

	std::shared_ptr<tgui::Label> win_label = nullptr;
	std::shared_ptr<tgui::Button> win_button_yes = nullptr;
	std::shared_ptr<tgui::Button> win_button_no = nullptr;

	std::shared_ptr<tgui::Button> menu_button = nullptr;
	std::shared_ptr<tgui::Button> pause_button = nullptr;

	std::shared_ptr<tgui::Label> delay_label = nullptr;
	std::shared_ptr<tgui::Slider> delay_slider = nullptr;

	std::shared_ptr<tgui::TextArea> console_logger = nullptr;

	std::unique_ptr<Poker_game_manager> ptr_manager = nullptr;
	std::unique_ptr<Table_render> ptr_table_render = nullptr;
	std::unique_ptr<Showdown_render> ptr_showdown_render = nullptr;
	std::unique_ptr<Logger> ptr_logger = nullptr;

	sf::Clock clock;

	Probability_evaluator eval;

	const float default_delay = 1.0f;
	float delay = default_delay;

	std::pair<std::uint16_t, std::uint16_t> coords;

	std::uint16_t center_x;
	std::uint16_t center_y;

	bool current_execution_mode_sequenced = true;
	bool paused = false;

	std::uint8_t dealer_id = 0;

	void create_players() noexcept;
	void reset_game() noexcept;

	void add_to_logs(std::string str) noexcept;

public:
	Game_stage(
		tgui::Gui& gui,
		Render_stages& stage,
		Render_color& color,
		Player_difficulty& diff,
		std::size_t& initial_money,
		bool& execution_mode_sequenced,
		bool& new_game,
		bool& game_is_running,
		std::uint8_t& number_of_players,
		std::mt19937_64& rng,
		std::pair<std::uint16_t, std::uint16_t> coords
	) noexcept;

	void input(const std::optional<sf::Event> event) noexcept;
	void update() noexcept;

	void set_visible(bool flag) noexcept;
};