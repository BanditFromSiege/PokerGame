#pragma once
#include "Game_stage.h"

class Menu_stage final {
private:
    tgui::Gui& gui;

    Render_stages& current_stage;
    bool& current_game_is_running;

    std::shared_ptr<tgui::Label> game_name_label = nullptr;

    std::shared_ptr<tgui::Button> new_game_button = nullptr;
    std::shared_ptr<tgui::Button> resume_game_button = nullptr;
    std::shared_ptr<tgui::Button> settings_button = nullptr;
    std::shared_ptr<tgui::Button> exit_button = nullptr;

    std::shared_ptr<tgui::Label> make_label(
        std::pair<std::uint16_t, std::uint16_t> coords,
        std::uint8_t text_size,
        tgui::Color text_color,
        const std::string& text
    ) noexcept;

    std::shared_ptr<tgui::Button> make_button(
        std::pair<std::uint16_t, std::uint16_t> coords,
        std::uint8_t text_size,
        const std::string& text,
        std::function<void()> func
    ) noexcept;

public:
    Menu_stage(
        tgui::Gui& gui,
        Render_stages& stage,
        bool& game_is_running
    ) noexcept;

    void set_visible(bool flag) noexcept;
};