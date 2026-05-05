#pragma once
#include "Card_render.h"

class Player_render final {
private:
    tgui::Gui& gui;

    const Player& player;
    Card_render card1;
    Card_render card2;

    bool is_has_dealer_button = false;
    bool is_has_current_player_arrow = false;

    std::shared_ptr<tgui::Label> name_and_money_label = nullptr;
    std::shared_ptr<tgui::Label> player_action_and_bet_label = nullptr;
    std::shared_ptr<tgui::Label> combination_label = nullptr;

    std::shared_ptr<tgui::Picture> dealer_button = nullptr;
    std::shared_ptr<tgui::Picture> current_player_arrow = nullptr;

public:
    Player_render(
        tgui::Gui& gui,
        const Player& player,
        std::pair<std::uint16_t, std::uint16_t> coords
    ) noexcept;

    void update_player(std::uint8_t dealer_player_id, std::optional<std::uint8_t> current_player_id) noexcept;

    void set_visible(bool flag) noexcept;

    void remove_from_gui() noexcept;
};