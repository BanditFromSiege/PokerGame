#include "Player_render.h"

Player_render::Player_render(
    tgui::Gui& gui,
    const Player& player,
    std::pair<std::uint16_t, std::uint16_t> coords
) noexcept : player(player), card1(gui), card2(gui)
{
    current_player_arrow = tgui::Picture::create();
    current_player_arrow->getRenderer()->setTexture(Texture_manager::get_instance().get_current_player_arrow_texture());
    current_player_arrow->setPosition(coords.first + 58, coords.second);
    current_player_arrow->setVisible(false);

    card1.set_position({coords.first, coords.second + 50});
    card2.set_position({coords.first + 75, coords.second + 50});
    card1.set_visible(true);
    card2.set_visible(true);

    name_label = tgui::Label::create();
    name_label->setPosition({ coords.first + 40, coords.second + 160});
    name_label->setText(player.get_name());
    name_label->setTextSize(20);
    name_label->getRenderer()->setTextColor(tgui::Color::White);
   
    money_label = tgui::Label::create();
    money_label->setPosition({ coords.first + 40, coords.second + 185 });
    money_label->setText(std::to_string(player.get_money()));
    money_label->setTextSize(20);
    money_label->getRenderer()->setTextColor(tgui::Color::White);
   
    player_action_label = tgui::Label::create();
    player_action_label->setPosition({ coords.first + 40, coords.second + 210 });
    player_action_label->setTextSize(20);
    player_action_label->getRenderer()->setTextColor(tgui::Color::White);
    
    bet_label = tgui::Label::create();
    bet_label->setPosition({ coords.first + 40, coords.second + 235 });
    bet_label->setTextSize(20);
    bet_label->getRenderer()->setTextColor(tgui::Color::White);

    combination_label = tgui::Label::create();
    combination_label->setPosition({ coords.first + 40, coords.second + 260 });
    combination_label->setTextSize(20);
    combination_label->getRenderer()->setTextColor(tgui::Color::Red);

    dealer_button = tgui::Picture::create();
    dealer_button->getRenderer()->setTexture(Texture_manager::get_instance().get_dealer_button_texture());
    dealer_button->setPosition(coords.first, coords.second + 20);
    dealer_button->setVisible(false);
    
    gui.add(current_player_arrow);
    gui.add(name_label);
    gui.add(money_label);
    gui.add(player_action_label);
    gui.add(bet_label);
    gui.add(combination_label);
    gui.add(dealer_button);
}

void Player_render::update_player(std::uint8_t dealer_player_id, std::optional<std::uint8_t> current_player_id) noexcept {
    if (player.is_in_game()) {
        auto cards = player.get_cards();
        card1.set_new_card(cards[0]);
        card2.set_new_card(cards[1]);

        money_label->setText(std::to_string(player.get_money()));
    } else {
        card1.set_back_card();
        card2.set_back_card();
    }

    if (auto opt_move = player.get_last_move(); opt_move) {
        std::stringstream ss;
        ss << *opt_move;

        player_action_label->setVisible(true);

        player_action_label->setText(ss.str());
    } else {
        player_action_label->setVisible(false);
    }
    
    if (player.get_current_bet() != 0) {
        bet_label->setVisible(true);
        bet_label->setText(std::to_string(player.get_current_bet()));
    } else {
        bet_label->setVisible(false);
    }

    if (player.get_id() == dealer_player_id) {
        dealer_button->setVisible(true);
    } else {
        dealer_button->setVisible(false);
    }

    if (current_player_id && player.get_id() == *current_player_id) {
        current_player_arrow->setVisible(true);
    } else {
        current_player_arrow->setVisible(false);
    }

    if (auto opt_comb = player.get_combination(); opt_comb && (player.is_active() || player.is_all_in())) {
        std::stringstream ss;
        ss << opt_comb->get_power();

        combination_label->setVisible(true);
        combination_label->setText(ss.str());
    } else {
        combination_label->setVisible(false);
    }

    if (!(player.is_active() || player.is_all_in())) {
        name_label->getRenderer()->setTextColor(tgui::Color::Black);
        money_label->getRenderer()->setTextColor(tgui::Color::Black);
        bet_label->getRenderer()->setTextColor(tgui::Color::Black);
        player_action_label->getRenderer()->setTextColor(tgui::Color::Black);
    } else {
        name_label->getRenderer()->setTextColor(tgui::Color::White);
        money_label->getRenderer()->setTextColor(tgui::Color::White);
        bet_label->getRenderer()->setTextColor(tgui::Color::White);
        player_action_label->getRenderer()->setTextColor(tgui::Color::White);
    }
}

void Player_render::hide_player() noexcept {
    card1.set_back_card();
    card1.set_visible(false);

    card2.set_back_card();
    card2.set_visible(false);

    name_label->setVisible(false);
    money_label->setVisible(false);

    bet_label->setVisible(false);
    player_action_label->setVisible(false);
    combination_label->setVisible(false);

    dealer_button->setVisible(false);
    current_player_arrow->setVisible(false);
}

void Player_render::unhide_player() noexcept {
    card1.set_visible(true);
    card2.set_visible(true);
    name_label->setVisible(true);
    money_label->setVisible(true);
}