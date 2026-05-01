#include "Render_manager.h"

Render_manager::Render_manager(std::pair<std::uint16_t, std::uint16_t> coords) noexcept
    : window(sf::VideoMode({ coords.first, coords.second }), "Poker", sf::Style::None)
    , gui(window)
{
    window.setFramerateLimit(60);

    rng = std::make_unique<std::mt19937_64>(std::random_device{}());

    menu_stage_ptr = std::make_unique<Menu_stage>(gui, stage, game_is_running);
    menu_stage_ptr->set_visible(false);

    game_stage_ptr = std::make_unique<Game_stage>(
        gui,
        stage,
        current_color,
        current_diff,
        execution_mode_sequenced,
        create_new_game,
        game_is_running,
        selected_players,
        *rng,
        coords
    );
    game_stage_ptr->set_visible(false);

    settings_stage_ptr = std::make_unique<Settings_stage>(gui, stage, current_color, execution_mode_sequenced);
    settings_stage_ptr->set_visible(false);
    
    preparing_stage_ptr = std::make_unique<Preparing_stage>(
        gui,
        stage,
        current_diff,
        selected_players,
        create_new_game,
        game_is_running
    );

    preparing_stage_ptr->set_visible(false);
}

void Render_manager::input() noexcept {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        gui.handleEvent(*event);

        if (current_stage == Render_stages::Game) {
            game_stage_ptr->input(event);
        }
    }
}

void Render_manager::update() noexcept {
    if (current_stage != stage) {
        if (stage == Render_stages::Menu) {
            menu_stage_ptr->set_visible(true);

            game_stage_ptr->set_visible(false);
            settings_stage_ptr->set_visible(false);
            preparing_stage_ptr->set_visible(false);
        }
        else if (stage == Render_stages::Preparing) {
            preparing_stage_ptr->set_visible(true);

            menu_stage_ptr->set_visible(false);
            game_stage_ptr->set_visible(false);
            settings_stage_ptr->set_visible(false);
        }
        else if (stage == Render_stages::Settings) {
            settings_stage_ptr->set_visible(true);

            menu_stage_ptr->set_visible(false);
            game_stage_ptr->set_visible(false);
            preparing_stage_ptr->set_visible(false);
        }
        else if (stage == Render_stages::Game) {
            game_stage_ptr->set_visible(true);

            settings_stage_ptr->set_visible(false);
            menu_stage_ptr->set_visible(false);
            preparing_stage_ptr->set_visible(false);
        }
        else if (stage == Render_stages::Exit) {
            window.close();
        }

        current_stage = stage;
    }

    if (current_stage == Render_stages::Game) {
        game_stage_ptr->update();
    }
}

void Render_manager::show() noexcept {
    auto tuple = get_tuple_of_color(current_color);

    window.clear(sf::Color(
        std::get<0>(tuple),
        std::get<1>(tuple),
        std::get<2>(tuple))
    );
    
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