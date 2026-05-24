#include "Game_stage.h"

void Game_stage::create_players() noexcept {
    current_game_is_running = true;
    paused = false;

    players.clear();

    for (std::uint8_t i = 0; i < selected_players; ++i) {
        if (i == 0 && current_game_mode == Game_mode::Player) {
            players.emplace_back(Player{
                rng, "Player", i, current_initial_money, current_diff
            });

            continue;
        }

        std::string name = "Bot_";
        name += std::to_string(i);

        players.emplace_back(Player{
            rng, std::move(name), i, current_initial_money, current_diff
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

void Game_stage::end_game() noexcept {
    for (auto& pr : players_render) {
        pr.set_visible(false);
    }

    ptr_table_render->set_visible(false);
    ptr_showdown_render->set_visible(false);

    if (current_game_mode == Game_mode::Player) {
        if (!players[0].is_in_game()) {
            win_label->setText("You lose\nRestart the game ?");
        }
        else {
            win_label->setText("You win\nRestart the game ?");
        }
    }
    else {
        auto opt_id = ptr_manager->get_winner_id();

        if (!opt_id) {
            win_label->setText("Winner: ERROR\nRestart the game ?");
        }
        else {
            win_label->setText("Winner: " + players[*opt_id].get_name() + "\nRestart the game ?");
        }
    }

    win_label->setVisible(true);

    win_button_yes->setVisible(true);
    win_button_no->setVisible(true);

    menu_button->setVisible(false);
    pause_button->setVisible(false);

    delay_label->setVisible(false);
    delay_slider->setVisible(false);

    fold_button->setVisible(false);
    call_or_check_button->setVisible(false);
    raise_button->setVisible(false);

    current_game_is_running = false;
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

    fold_button->setVisible(true);
    call_or_check_button->setVisible(true);
    raise_button->setVisible(true);

    for (auto& pr : players_render) {
        pr.set_visible(true);
    }

    console_logger->setText("");
    log_buffer.clear();

    paused = false;
    clock.restart();
}

void Game_stage::add_to_logs(std::string str) noexcept {
    if (!console_logger) {
        return;
    }

    constexpr std::size_t max_strings_in_console_logs = 400;

    log_buffer.push_back(std::move(str));

    if (log_buffer.size() > max_strings_in_console_logs) {
        log_buffer.pop_front();
    }

    std::string full_str;
    full_str.reserve(log_buffer.size() * 150);

    for (const auto& s : log_buffer) {
        full_str += s;
    }

    console_logger->setText(full_str);
}

void Game_stage::check_is_player_turn() noexcept {
    if (ptr_manager->is_player_turn()) {
        player_make_turn = true;

        fold_button->setEnabled(true);
        call_or_check_button->setEnabled(true);
        raise_button->setEnabled(true);

        new_bet_label->setVisible(true);
        bet_slider->setVisible(true);

        auto [min, max] = ptr_manager->get_min_max_bet_raise_for_player();

        player_bet = min;

        new_bet_label->setText(std::to_string(player_bet));

        bet_slider->setMinimum(min);
        bet_slider->setMaximum(max);
        bet_slider->setValue(player_bet);

        bet_slider->setEnabled(min < max);
    }
    else {
        player_make_turn = false;

        fold_button->setEnabled(false);
        call_or_check_button->setEnabled(false);
        raise_button->setEnabled(false);

        new_bet_label->setVisible(false);
        bet_slider->setVisible(false);
    }
}

Game_stage::Game_stage(
    tgui::Gui& gui,
    Render_stages& stage,
    Render_color& color,
    Player_difficulty& diff,
    Game_mode& game_mode,
    std::size_t& initial_money,
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
    , current_game_mode(game_mode)
    , current_initial_money(initial_money)
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

    paused_label = Texture_manager::make_label(gui, { center_x - 110, 5 }, 56, tgui::Color::Red, "Paused");
    paused_label->setVisible(false);

    round_label = Texture_manager::make_label(gui, { 5, 5 }, 56, tgui::Color::White, "Round: 1");
    round_label->setVisible(true);

    win_label = Texture_manager::make_label(gui, { center_x - 250, center_y - 150 }, 64, tgui::Color::White, "");
    win_label->setVisible(false);

    win_button_yes = Texture_manager::make_button(gui, { center_x - 200, center_y + 20 }, 64, "Yes",
        [this](){
            current_game_is_running = true;
            reset_game();
        }
    );
    win_button_yes->setVisible(false);

    win_button_no = Texture_manager::make_button(gui, { center_x, center_y + 20 }, 64, "No",
        [this]() {
            current_game_is_running = false;
            current_stage = Render_stages::Menu;
        }
    );
    win_button_no->setVisible(false);

    menu_button = Texture_manager::make_button(gui, { 20, 105 }, 48, "Menu",
        [this]() {
            paused = true;
            current_stage = Render_stages::Menu;
        }
    );

    pause_button = Texture_manager::make_button(gui, { 20, 205 }, 48, "Pause",
        [this]() {
            paused = !paused;
            paused_label->setVisible(paused);

            if (!paused) {
                clock.restart();

                fold_button->setEnabled(ptr_manager && ptr_manager->is_player_turn());
                call_or_check_button->setEnabled(ptr_manager && ptr_manager->is_player_turn());
                raise_button->setEnabled(ptr_manager && ptr_manager->is_player_turn());

                new_bet_label->setVisible(ptr_manager && ptr_manager->is_player_turn());
                bet_slider->setVisible(ptr_manager && ptr_manager->is_player_turn());
            }

            if (paused) {
                fold_button->setEnabled(false);
                call_or_check_button->setEnabled(false);
                raise_button->setEnabled(false);

                new_bet_label->setVisible(false);
                bet_slider->setVisible(false);
            }
        }
    );

    float maxWidth = std::max({
        menu_button->getSize().x,
        pause_button->getSize().x
    });

    menu_button->setWidth(maxWidth);
    pause_button->setWidth(maxWidth);

    delay_label
        = Texture_manager::make_label(gui, { center_x - 300, 975 }, 36, tgui::Color::White, std::format("{:.1f}", delay));

    delay_slider = Texture_manager::make_slider(gui, { center_x - 200, 980 }, { 400, 30 }, { 0.1f, 2.0f }, 0.1f, delay,
        [this](float value) {
            delay = value;
            delay_label->setText(std::format("{:.1f}", value));
        }
    );

    console_logger = tgui::TextArea::create();
    console_logger->setSize({ 610, 180 });
    console_logger->setPosition({ 20, 890 });
    console_logger->setReadOnly(true);
    console_logger->setTextSize(18);

    gui.add(console_logger);

    fold_button = Texture_manager::make_button(gui, { 1220, 960 }, 48, "Fold",
        [this]() {
            ptr_manager->set_player_turn({ Player_action::Fold, 0 });
            player_make_turn = false;

            clock.restart();
        }
    );

    call_or_check_button = Texture_manager::make_button(gui, { 1383, 960 }, 48, "Call/Check",
        [this]() {
            auto [bet_for_call, table_current_bet] = ptr_manager->get_call_bet_for_player();

            if (bet_for_call == 0) {
                ptr_manager->set_player_turn({ Player_action::Check, 0 });
            }
            else {
                ptr_manager->set_player_turn({ Player_action::Call, table_current_bet });
            }

            player_make_turn = false;

            clock.restart();
        }
    );

    raise_button = Texture_manager::make_button(gui, { 1700, 960 }, 48, "Raise",
        [this]() {
            ptr_manager->set_player_turn({ Player_action::Raise, player_bet });
            player_make_turn = false;

            clock.restart();
        }
    );

    fold_button->setVisible(current_game_mode == Game_mode::Player);
    call_or_check_button->setVisible(current_game_mode == Game_mode::Player);
    raise_button->setVisible(current_game_mode == Game_mode::Player);

    fold_button->setEnabled(false);
    call_or_check_button->setEnabled(false);
    raise_button->setEnabled(false);

    new_bet_label
        = Texture_manager::make_label(gui, { 1745, 905 }, 36, tgui::Color::White, std::to_string(player_bet));

    bet_slider = Texture_manager::make_slider(gui, { 1230, 910 }, { 450, 30 }, {20, 1000}, 1, player_bet,
        [this](std::size_t value) {
            player_bet = value;
            new_bet_label->setText(std::to_string(player_bet));
        }
    );

    new_bet_label->setVisible(false);
    bet_slider->setVisible(false);
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

        ptr_manager = std::make_unique<Poker_game_manager>(rng, players, eval, current_game_mode);
        ptr_table_render = std::make_unique<Table_render>(gui, *ptr_manager, std::pair{ center_x - 190, center_y - 100 });
        ptr_showdown_render = std::make_unique<Showdown_render>(gui, *ptr_manager, std::pair{ 1200, 20 });
        ptr_logger = std::make_unique<Logger>(*ptr_manager);

        console_logger->setText("");
        log_buffer.clear();

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

        check_is_player_turn();

        clock.restart();
    }

    if (!paused && ptr_manager && !player_make_turn && ptr_manager->is_game_still_run()) {
        if (clock.getElapsedTime().asSeconds() >= delay) {
            clock.restart();

            ptr_manager->call_next_move();
            check_is_player_turn();

            if (!player_make_turn) {
                auto str = ptr_logger->get_message();
                if (!str.empty()) {
                    file << str;
                    file.flush();
                    add_to_logs(std::move(str));
                }
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
                end_game();
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

    fold_button->setVisible(flag && current_game_mode == Game_mode::Player);
    call_or_check_button->setVisible(flag && current_game_mode == Game_mode::Player);
    raise_button->setVisible(flag && current_game_mode == Game_mode::Player);

    fold_button->setEnabled(ptr_manager && ptr_manager->is_player_turn() && !paused);
    call_or_check_button->setEnabled(ptr_manager && ptr_manager->is_player_turn() && !paused);
    raise_button->setEnabled(ptr_manager && ptr_manager->is_player_turn() && !paused);

    new_bet_label->setVisible(flag && ptr_manager && ptr_manager->is_player_turn() && !paused);
    bet_slider->setVisible(flag && ptr_manager && ptr_manager->is_player_turn() && !paused);

    clock.restart();
}