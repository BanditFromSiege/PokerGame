#pragma once
#include "Menu_stage.h"

class Settings_stage final {
private:
    tgui::Gui& gui;

    Render_stages& current_stage;
    Render_color& current_color;
    bool& current_execution_mode_sequenced;

    std::shared_ptr<tgui::Label> settings_label = nullptr;

    std::shared_ptr<tgui::Label> choice_execution_mode_label = nullptr;
    std::shared_ptr<tgui::RadioButtonGroup> execution_mode_group = nullptr;
    std::shared_ptr<tgui::RadioButton> sequenced_radio_button = nullptr;
    std::shared_ptr<tgui::RadioButton> parallel_radio_button = nullptr;

    std::shared_ptr<tgui::Label> choice_background_color_label = nullptr;
    std::shared_ptr<tgui::RadioButtonGroup> colors_group = nullptr;
    std::shared_ptr<tgui::RadioButton> red_radio_button = nullptr;
    std::shared_ptr<tgui::RadioButton> green_radio_button = nullptr;
    std::shared_ptr<tgui::RadioButton> blue_radio_button = nullptr;
    std::shared_ptr<tgui::RadioButton> purple_radio_button = nullptr;

    std::shared_ptr<tgui::Button> back_button = nullptr;

public:
    Settings_stage(
        tgui::Gui& gui,
        Render_stages& stage,
        Render_color& color,
        bool& execution_mode_sequenced
    ) noexcept;

    void set_visible(bool flag) noexcept;
};