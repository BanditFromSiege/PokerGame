#include "Player_render.h"

Player_render::Player_render(
    tgui::Gui& gui,
    const Player& player,
    std::pair<std::uint16_t, std::uint16_t> coords
) noexcept
    : gui(gui)
    , player(player)
    , card1(gui)
    , card2(gui)
{
    current_player_arrow = tgui::Picture::create();
    current_player_arrow->getRenderer()->setTexture(Texture_manager::get_instance().get_current_player_arrow_texture());
    current_player_arrow->setPosition(coords.first + 58, coords.second);
    current_player_arrow->setVisible(false);

    card1.set_position({coords.first, coords.second + 50});
    card2.set_position({coords.first + 75, coords.second + 50});
    card1.set_visible(true);
    card2.set_visible(true);

    name_and_money_label = tgui::Label::create();
    name_and_money_label->setPosition({ coords.first + 25, coords.second + 160});

    name_and_money_label->setText(
        player.get_name() + " "
        + player_type_to_c_str(player.get_type())
        + " [" + std::to_string(player.get_money()) + "]"
    );

    name_and_money_label->setTextSize(20);
    name_and_money_label->getRenderer()->setTextColor(tgui::Color::White);
   
    player_action_and_bet_label = tgui::Label::create();
    player_action_and_bet_label->setPosition({ coords.first + 25, coords.second + 185 });
    player_action_and_bet_label->setTextSize(20);
    player_action_and_bet_label->getRenderer()->setTextColor(tgui::Color::White);

    probabilities_label = tgui::Label::create();
    probabilities_label->setPosition({ coords.first + 25, coords.second + 210 });
    probabilities_label->setTextSize(20);
    probabilities_label->getRenderer()->setTextColor(tgui::Color::Yellow);
    
    combination_label = tgui::Label::create();
    combination_label->setPosition({ coords.first + 25, coords.second + 235 });
    combination_label->setTextSize(20);
    combination_label->getRenderer()->setTextColor(tgui::Color::Red);

    dealer_button = tgui::Picture::create();
    dealer_button->getRenderer()->setTexture(Texture_manager::get_instance().get_dealer_button_texture());
    dealer_button->setPosition(coords.first, coords.second + 160);
    dealer_button->setVisible(false);
    
    gui.add(current_player_arrow);
    gui.add(name_and_money_label);
    gui.add(player_action_and_bet_label);
    gui.add(probabilities_label);
    gui.add(combination_label);
    gui.add(dealer_button);
}

void Player_render::update_player(std::uint8_t dealer_player_id, std::optional<std::uint8_t> current_player_id) noexcept {
    if (player.is_in_game()) {
        auto cards = player.get_cards();
        card1.set_new_card(cards[0]);
        card2.set_new_card(cards[1]);

        name_and_money_label->setText(
            player.get_name() + " "
            + player_type_to_c_str(player.get_type())
            + " [" + std::to_string(player.get_money()) + "]"
        );
    }
    else {
        card1.set_back_card();
        card2.set_back_card();
    }

    if (auto opt_move = player.get_last_move(); opt_move) {
        player_action_and_bet_label->setVisible(true);

        std::string str = player_action_to_c_str(*opt_move);
        auto bet = player.get_current_player_bet();

        if (bet != 0 && (*opt_move == Player_action::Call || *opt_move == Player_action::Raise)) {
            str += ' ' + std::to_string(bet);

            if (player.get_money() == 0) {
                str += " (ALL-IN)";
            }
        }

        player_action_and_bet_label->setText(std::move(str));
    }
    else if (player.get_current_player_bet() != 0) {
        player_action_and_bet_label->setVisible(true);

        auto bet = player.get_current_player_bet();

        std::string str;

        str += std::to_string(bet);

        if (player.get_money() == 0) {
            str += " (ALL-IN)";
        }

        player_action_and_bet_label->setText(std::move(str));
    }
    else if (player.get_money() == 0 && player.is_in_game()) {
        player_action_and_bet_label->setVisible(true);
        player_action_and_bet_label->setText("(ALL-IN)");
    }
    else {
        player_action_and_bet_label->setVisible(false);
    }

    if (player.get_id() == dealer_player_id) {
        dealer_button->setVisible(true);
        is_has_dealer_button = true;
    }
    else {
        dealer_button->setVisible(false);
        is_has_dealer_button = false;
    }

    if (current_player_id && player.get_id() == *current_player_id) {
        current_player_arrow->setVisible(true);
        is_has_current_player_arrow = true;
    }
    else {
        current_player_arrow->setVisible(false);
        is_has_current_player_arrow = false;
    }

    if (auto opt_absolute_probability = player.get_absolute_probability(),
        opt_relative_probability = player.get_relative_probability(); 
        opt_absolute_probability || opt_relative_probability)
    {
        std::string str;

        if (opt_absolute_probability) {
            str += std::format("AP ({:.2f}) ", *opt_absolute_probability) + ' ';
        }

        if (opt_relative_probability) {
            str += std::format("RP ({:.2f}) ", *opt_relative_probability);
        }

        probabilities_label->setText(std::move(str));
        probabilities_label->setVisible(true);
    }
    else {
        probabilities_label->setVisible(false);
    }

    if (auto opt_comb = player.get_combination(); opt_comb && (player.is_active() || player.is_all_in())) {
        combination_label->setVisible(true);
        combination_label->setText(combination_to_c_str(opt_comb->get_power()));
    }
    else {
        combination_label->setVisible(false);
    }

    if (!(player.is_active() || player.is_all_in())) {
        name_and_money_label->getRenderer()->setTextColor(tgui::Color::Black);
        player_action_and_bet_label->getRenderer()->setTextColor(tgui::Color::Black);
    }
    else {
        name_and_money_label->getRenderer()->setTextColor(tgui::Color::White);
        player_action_and_bet_label->getRenderer()->setTextColor(tgui::Color::White);
    }
}

void Player_render::set_visible(bool flag) noexcept {
    if (!flag) {
        card1.set_visible(false);
        card2.set_visible(false);

        name_and_money_label->setVisible(false);
        player_action_and_bet_label->setVisible(false);
        probabilities_label->setVisible(false);
        combination_label->setVisible(false);

        dealer_button->setVisible(false);
        current_player_arrow->setVisible(false);
    }
    else {
        if (!player.is_in_game()) {
            card1.set_back_card();
            card2.set_back_card();
        }
        else {
            player_action_and_bet_label->setVisible(
                player.get_last_move().has_value() || player.get_current_player_bet() > 0 || player.get_money() == 0
            );

            probabilities_label->setVisible(
                player.get_absolute_probability().has_value() || player.get_relative_probability().has_value()
            );

            combination_label->setVisible(
                player.get_combination().has_value() && (player.is_active() || player.is_all_in())
            );
        }

        card1.set_visible(true);
        card2.set_visible(true);

        name_and_money_label->setVisible(true);

        dealer_button->setVisible(is_has_dealer_button);
        current_player_arrow->setVisible(is_has_current_player_arrow);
    }
}

void Player_render::remove_from_gui() noexcept {
    card1.remove_from_gui();
    card2.remove_from_gui();

    gui.remove(name_and_money_label);
    gui.remove(player_action_and_bet_label);
    gui.remove(probabilities_label);
    gui.remove(combination_label);

    gui.remove(dealer_button);
    gui.remove(current_player_arrow);
}