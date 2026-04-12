#include "Render_manager.h"

Render_manager::Render_manager(
    std::mt19937_64& rng,
    std::pair<std::uint16_t, std::uint16_t> coords,
    std::uint8_t number_of_players
) noexcept
    : window(sf::VideoMode({ coords.first, coords.second }), "Poker", sf::Style::None)
    , gui(window)
{
    window.setFramerateLimit(60);

    for (std::uint8_t i = 0; i < number_of_players; ++i) {
        std::string name = "Bot_";
        name += std::to_string(i);

        players.emplace_back(Player{
            std::move(name), i, 1000, Player_difficulty::Hard
        });
    }

    const float centerX = coords.first / 2.f;
    const float centerY = coords.second / 2.f;

    const float radiusX = 600.f;
    const float radiusY = 280.f;

    const float playerWidth = 150.f;
    const float playerHeight = 320.f;

    const std::size_t playerCount = players.size();

    float angleOffset = std::numbers::pi_v<float> / 2.f;

    if (playerCount % 2 != 0) {
        angleOffset += std::numbers::pi_v<float> / playerCount;
    }

    for (std::size_t i = 0; i < playerCount; ++i) {
        float angle = (2 * std::numbers::pi_v<float> * i) / playerCount + angleOffset;

        float x = centerX + std::cos(angle) * radiusX;
        float y = centerY + std::sin(angle) * radiusY;

        x -= playerWidth / 2.f;
        y -= playerHeight / 2.f;

        players_render.emplace_back(gui, players[i], std::pair{
            static_cast<std::uint16_t>(x),
            static_cast<std::uint16_t>(y)
        });
    }

    ptr_table_render = std::make_unique<Table_render>(gui, table, std::pair{ centerX - 200, centerY - 100 });
    ptr_showdown_render = std::make_unique<Showdown_render>(gui, players, std::pair{ centerX - 250, centerY + 100 });
    ptr_manager = std::make_unique<Poker_game_manager>(table, rng, players, eval_seq, eval_par);

    ptr_manager->set_evaluator_parallel_policy();

    paused_label = tgui::Label::create();
    paused_label->setPosition({ centerX - 100, centerY - 180 });
    paused_label->setTextSize(56);
    paused_label->getRenderer()->setTextColor(tgui::Color::Red);
    paused_label->setText("Paused");
    paused_label->setVisible(false);

    gui.add(paused_label);

    round_label = tgui::Label::create();
    round_label->setPosition({ 5, 5 });
    round_label->setTextSize(56);
    round_label->getRenderer()->setTextColor(tgui::Color::White);
    round_label->setText("");
    round_label->setVisible(true);

    gui.add(round_label);

    win_label = tgui::Label::create();
    win_label->setPosition({ centerX - 250, centerY - 150});
    win_label->setTextSize(64);
    win_label->getRenderer()->setTextColor(tgui::Color::White);
    win_label->setText("");
    win_label->setVisible(false);

    gui.add(win_label);

    win_button_yes = tgui::Button::create();
    win_button_yes->setPosition({ centerX - 200, centerY + 20});
    win_button_yes->setTextSize(64);
    win_button_yes->setText("Yes");
    win_button_yes->setVisible(false);

    win_button_yes->onPress([this]() {
        for (auto& p : players) {
            p.reset_for_new_game();
        }

        ptr_manager->reset_for_new_game();

        win_label->setVisible(false);
        win_button_yes->setVisible(false);
        win_button_no->setVisible(false);

        for (auto& pr : players_render) {
            pr.unhide_player();
        }

        paused = false;
        clock.restart();
    });

    gui.add(win_button_yes);

    win_button_no = tgui::Button::create();
    win_button_no->setPosition({ centerX, centerY + 20});
    win_button_no->setTextSize(64);
    win_button_no->setText("No");
    win_button_no->setVisible(false);

    win_button_no->onPress([this]() {
        window.close();
    });

    gui.add(win_button_no);
}

void Render_manager::input() noexcept {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (event->is<sf::Event::FocusLost>()) {
            paused = true;
            paused_label->setVisible(paused);
        }
        if (event->is<sf::Event::FocusGained>()) {
            clock.restart();
        }

        if (event->is<sf::Event::KeyPressed>()) {
            if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::E) {
                window.close();
            }

            if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape) {
                paused = !paused;
                paused_label->setVisible(paused);
            }

            if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::P) {
                ptr_manager->set_evaluator_parallel_policy();
            }

            if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::S) {
                ptr_manager->set_evaluator_sequenced_policy();
            }
        }

        gui.handleEvent(*event);
    }
}

void Render_manager::update() noexcept {
    if (!paused && ptr_manager->is_game_still_run()) {
        if (clock.getElapsedTime().asSeconds() >= delay) {
            clock.restart();
            ptr_manager->call_next_move();

            if (ptr_manager->get_current_stage() == Poker_stage::Preflop) {
                round_label->setText("Round: " + std::to_string(ptr_manager->get_number_of_rounds()));
                dealer_id = ptr_manager->get_dealer_player_id();
            }

            for (auto& pr : players_render) {
                pr.update_player(dealer_id, ptr_manager->get_current_player_id());
            }

            ptr_table_render->update_table();
            ptr_showdown_render->update_showdown(ptr_manager->get_winners_and_rewards());

            if (!ptr_manager->is_game_still_run()) {
                for (auto& pr : players_render) {
                    pr.hide_player();
                }

                ptr_table_render->hide_table();
                ptr_showdown_render->hide_showdown();

                auto it = std::find_if(players.begin(), players.end(),
                    [](const Player& p) {
                        return p.get_money() != 0;
                    }
                );

                win_label->setText("Winner: " + it->get_name() + "\nRestart the game ?");
                win_label->setVisible(true);

                win_button_yes->setVisible(true);
                win_button_no->setVisible(true);
            }
        }
    }
}

void Render_manager::show() noexcept {
    window.clear(sf::Color(90, 0, 0));
    gui.draw();
    window.display();
}

void Render_manager::run() noexcept {
    while (window.isOpen()) {
        input();
        update();
        show();
    }
}