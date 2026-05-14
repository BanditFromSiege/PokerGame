#pragma once
#include "Settings_stage.h"

class Preparing_stage final {
private:
    tgui::Gui& gui;

    Render_stages& current_stage;
    Player_difficulty& current_diff;
    std::uint8_t& selected_players;
    std::size_t& current_initial_money;
    bool& create_new_game;
    bool& current_game_is_running;

    std::shared_ptr<tgui::Label> preparing_label = nullptr;
 
    std::shared_ptr<tgui::Label> select_number_of_players_label = nullptr;
    std::shared_ptr<tgui::RadioButtonGroup> players_group = nullptr;
    std::array<std::shared_ptr<tgui::RadioButton>, Probability_evaluator::MAX_PLAYERS - Probability_evaluator::MIN_PLAYERS + 1>
        array_of_buttons_for_selected_player{ nullptr };

    std::shared_ptr<tgui::Label> select_diff_of_players_label = nullptr;
    std::shared_ptr<tgui::RadioButtonGroup> diff_group = nullptr;
    std::shared_ptr<tgui::RadioButton> easy_diff = nullptr;
    std::shared_ptr<tgui::RadioButton> medium_diff = nullptr;
    std::shared_ptr<tgui::RadioButton> hard_diff = nullptr;

    std::shared_ptr<tgui::Label> choice_poker_mode_label = nullptr;
    std::shared_ptr<tgui::RadioButton> no_limit_texas_holdem_radio_button = nullptr;

    std::shared_ptr<tgui::Label> choice_game_mode_label = nullptr;
    std::shared_ptr<tgui::RadioButton> spectator_mode_radio_button = nullptr;

    std::shared_ptr<tgui::Label> choice_player_stack_label = nullptr;
    std::shared_ptr<tgui::RadioButtonGroup> player_stack_group = nullptr;
    std::shared_ptr<tgui::RadioButton> short_stack_radio_button = nullptr;
    std::shared_ptr<tgui::RadioButton> normal_stack_radio_button = nullptr;
    std::shared_ptr<tgui::RadioButton> long_stack_radio_button = nullptr;

    std::shared_ptr<tgui::Button> back_button = nullptr;
    std::shared_ptr<tgui::Button> create_game_button = nullptr;

public:
    Preparing_stage(
        tgui::Gui& gui,
        Render_stages& stage,
        Player_difficulty& diff,
        std::uint8_t& number_of_players,
        std::size_t& initial_money,
        bool& new_game,
        bool& game_is_running
    ) noexcept;

    void set_visible(bool flag) noexcept;
};