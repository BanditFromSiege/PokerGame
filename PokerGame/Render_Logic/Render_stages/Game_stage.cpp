#include "Game_stage.h"

void Game_stage::create_players() noexcept {
    current_game_is_running = true;
    paused = false;

    players.clear();

    for (std::uint8_t i = 0; i < selected_players; ++i) {
        std::string name = "Bot_";
        name += std::to_string(i);

        players.emplace_back(Player{
            std::move(name), i, 1000, current_diff
        });
    }

    for (auto& pr : players_render) {
        pr.remove_from_gui();
    }

    players_render.clear();

    const float radius_x = 600.f;
    const float radius_y = 280.f;

    const float player_width = 150.f;
    const float player_height = 320.f;

    const std::size_t player_count = players.size();

    float angle_offset = std::numbers::pi_v<float> / 2.f;

    if (player_count % 2 != 0) {
        angle_offset += std::numbers::pi_v<float> / player_count;
    }

    for (std::size_t i = 0; i < player_count; ++i) {
        float angle = (2 * std::numbers::pi_v<float> *i) / player_count + angle_offset;

        float x = center_x + std::cos(angle) * radius_x;
        float y = center_y + std::sin(angle) * radius_y;

        x -= player_width / 2.f;
        y -= player_height / 2.f;

        players_render.emplace_back(gui, players[i], std::pair{
            static_cast<std::uint16_t>(x),
            static_cast<std::uint16_t>(y)
        });
    }
}

void Game_stage::reset_game() noexcept {
    for (auto& p : players) {
        p.reset_for_new_game();
    }

    ptr_manager->reset_for_new_game();

    ptr_logger->reset();

    win_label->setVisible(false);
    win_button_yes->setVisible(false);
    win_button_no->setVisible(false);

    menu_button->setVisible(true);
    pause_button->setVisible(true);

    delay_label->setVisible(true);
    delay_slider->setVisible(true);

    delay = default_delay;
    delay_label->setText(std::format("{:.1f}", delay));
    delay_slider->setValue(delay);

    for (auto& pr : players_render) {
        pr.set_visible(true);
    }

    console_logger->setText("");

    paused = false;
    clock.restart();
}

std::shared_ptr<tgui::Label> Game_stage::make_label(
    std::pair<std::uint16_t, std::uint16_t> coords,
    std::uint8_t text_size,
    tgui::Color text_color,
    const std::string& text
) noexcept
{
    std::shared_ptr<tgui::Label> label = tgui::Label::create();
    label->setPosition({ coords.first, coords.second });
    label->setTextSize(text_size);
    label->getRenderer()->setTextColor(text_color);
    label->setText(text);

    gui.add(label);

    return label;
}

std::shared_ptr<tgui::Button> Game_stage::make_button(
    std::pair<std::uint16_t, std::uint16_t> coords,
    std::uint8_t text_size,
    const std::string& text,
    std::function<void()> func
) noexcept
{
    std::shared_ptr<tgui::Button> button = tgui::Button::create();
    button->setPosition({ coords.first, coords.second });
    button->setTextSize(text_size);
    button->setText(text);
    button->onPress(func);

    gui.add(button);

    return button;
}

Game_stage::Game_stage(
    tgui::Gui& gui,
    Render_stages& stage,
    Render_color& color,
    Player_difficulty& diff,
    bool& execution_mode_sequenced,
    bool& new_game,
    bool& game_is_running,
    std::uint8_t& number_of_players,
    std::mt19937_64& rng,
    std::pair<std::uint16_t, std::uint16_t> coords
) noexcept
	: gui(gui)
    , current_stage(stage)
    , current_color(color)
    , current_diff(diff)
    , execution_mode_sequenced(execution_mode_sequenced)
    , create_new_game(new_game)
    , current_game_is_running(game_is_running)
	, selected_players(number_of_players)
    , rng(rng)
    , coords(coords)
{
    file.open("poker_game_log.txt", std::ios::out | std::ios::app);
    
    center_x = coords.first / 2.f;
    center_y = coords.second / 2.f;

    paused_label = make_label({ center_x - 100, center_y - 180 }, 56, tgui::Color::Red, "Paused");
    paused_label->setVisible(false);

    round_label = make_label({ 5, 5 }, 56, tgui::Color::White, "Round: 1");
    round_label->setVisible(true);

    win_label = make_label({ center_x - 250, center_y - 150 }, 64, tgui::Color::White, "");
    win_label->setVisible(false);

    win_button_yes = make_button({ center_x - 200, center_y + 20 }, 64, "Yes",
        [this](){
            current_game_is_running = true;
            reset_game();
        }
    );
    win_button_yes->setVisible(false);

    win_button_no = make_button({ center_x, center_y + 20 }, 64, "No",
        [this]() {
            current_game_is_running = false;
            current_stage = Render_stages::Menu;
        }
    );
    win_button_no->setVisible(false);

    menu_button = make_button({ 20, 105 }, 48, "Menu",
        [this]() {
            paused = true;
            current_stage = Render_stages::Menu;
        }
    );

    pause_button = make_button({ 20, 205 }, 48, "Pause",
        [this]() {
            paused = !paused;
            paused_label->setVisible(paused);

            if (!paused) {
                clock.restart();
            }
        }
    );

    float maxWidth = std::max({
        menu_button->getSize().x,
        pause_button->getSize().x
    });

    menu_button->setWidth(maxWidth);
    pause_button->setWidth(maxWidth);

    delay_label = make_label({ center_x - 300, 975 }, 36, tgui::Color::White, std::format("{:.1f}", delay));

    delay_slider = tgui::Slider::create();
    delay_slider->setPosition({ center_x - 200, 980 });
    delay_slider->setSize({ 400, 30 });

    delay_slider->setMinimum(0.1f);
    delay_slider->setMaximum(2.0f);
    delay_slider->setStep(0.1f);
    delay_slider->setValue(delay);

    delay_slider->onValueChange([this](float value) {
        delay = value;
        delay_label->setText(std::format("{:.1f}", value));
    });

    gui.add(delay_slider);

    console_logger = tgui::TextArea::create();
    console_logger->setSize({ 610, 180 });
    console_logger->setPosition({ 20, 890 });
    console_logger->setReadOnly(true);
    console_logger->setTextSize(18);

    gui.add(console_logger);
}

void Game_stage::input(const std::optional<sf::Event> event) noexcept {
    if (current_game_is_running) {
        if (event->is<sf::Event::FocusLost>()) {
            paused = true;
            paused_label->setVisible(paused);
        }
        if (event->is<sf::Event::FocusGained>()) {
            clock.restart();
        }
    }
}

void Game_stage::update() noexcept {
    if (current_execution_mode_sequenced != execution_mode_sequenced) {
        if (ptr_manager) {
            if (execution_mode_sequenced) {
                ptr_manager->set_evaluator_sequenced_policy();
            }
            else {
                ptr_manager->set_evaluator_parallel_policy();
            }
        }

        current_execution_mode_sequenced = execution_mode_sequenced;
    } 

    if (create_new_game) {
        if (ptr_table_render) {
            ptr_table_render->remove_from_gui();
        }

        if (ptr_showdown_render) {
            ptr_showdown_render->remove_from_gui();
        }

        create_players();

        ptr_manager = std::make_unique<Poker_game_manager>(rng, players, eval_seq, eval_par);
        ptr_table_render = std::make_unique<Table_render>(gui, *ptr_manager, std::pair{ center_x - 190, center_y - 100 });
        ptr_showdown_render = std::make_unique<Showdown_render>(gui, *ptr_manager, std::pair{ 1300, 20 });
        ptr_logger = std::make_unique<Logger>(*ptr_manager);

        console_logger->setText("");

        if (execution_mode_sequenced) {
            ptr_manager->set_evaluator_sequenced_policy();
        }
        else {
            ptr_manager->set_evaluator_parallel_policy();
        }

        ptr_table_render->set_visible(false);
        ptr_showdown_render->set_visible(false);

        create_new_game = false;
        paused = false;
        paused_label->setVisible(paused);

        delay = default_delay;
        delay_label->setText(std::format("{:.1f}", delay));
        delay_slider->setValue(delay);

        clock.restart();
    }

    if (!paused && ptr_manager && ptr_manager->is_game_still_run()) {
        if (clock.getElapsedTime().asSeconds() >= delay) {
            clock.restart();
            ptr_manager->call_next_move();

            auto str = ptr_logger->get_message();
            if (!str.empty()) {
                file << str;
                file.flush();
                console_logger->addText(std::move(str));
            }

            if (ptr_manager->get_current_stage() == Poker_stage::Preflop) {
                round_label->setText("Round: " + std::to_string(ptr_manager->get_number_of_rounds()));
                dealer_id = ptr_manager->get_dealer_player_id();
            }

            for (auto& pr : players_render) {
                pr.update_player(dealer_id, ptr_manager->get_current_player_id());
            }

            ptr_table_render->update_table();
            ptr_showdown_render->update_showdown();

            if (!ptr_manager->is_game_still_run()) {
                for (auto& pr : players_render) {
                    pr.set_visible(false);
                }

                ptr_table_render->set_visible(false);
                ptr_showdown_render->set_visible(false);

                auto opt_id = ptr_manager->get_winner_id();

                if (!opt_id) {
                    win_label->setText("Winner: ERROR\nRestart the game ?");
                }
                else {
                    win_label->setText("Winner: " + players[*opt_id].get_name() + "\nRestart the game ?");
                }

                win_label->setVisible(true);

                win_button_yes->setVisible(true);
                win_button_no->setVisible(true);

                menu_button->setVisible(false);
                pause_button->setVisible(false);

                delay_label->setVisible(false);
                delay_slider->setVisible(false);

                current_game_is_running = false;
            }
        }
    }
}

void Game_stage::set_visible(bool flag) noexcept {
    if (ptr_showdown_render) {
        ptr_showdown_render->set_visible(flag);
    }

    if (ptr_table_render) {
        ptr_table_render->set_visible(flag);
    }

    for (auto& pr : players_render) {
        pr.set_visible(flag);
    }

    round_label->setVisible(flag);
    paused_label->setVisible(flag && paused);
    
    win_label->setVisible(flag && !current_game_is_running);
    win_button_yes->setVisible(flag && !current_game_is_running);
    win_button_no->setVisible(flag && !current_game_is_running);

    menu_button->setVisible(flag && current_game_is_running);
    pause_button->setVisible(flag && current_game_is_running);

    delay_label->setVisible(flag && current_game_is_running);
    delay_slider->setVisible(flag && current_game_is_running);

    console_logger->setVisible(flag);

    clock.restart();
}