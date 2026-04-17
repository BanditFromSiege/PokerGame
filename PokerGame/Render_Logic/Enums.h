#pragma once
#include <iostream>

enum class Render_stages : std::uint8_t {
    Menu,
    Preparing,
    Settings,
    Game,
    Menugame,
    Exit
};

enum class Render_color : std::uint8_t {
    Red,
    Green,
    Blue,
    Purple
};

inline std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> get_tuple_of_color(Render_color color) noexcept {
    switch (color) {
        case Render_color::Red : return { 90, 0, 0 };
        case Render_color::Green : return { 20, 70, 40 };
        case Render_color::Blue : return { 40, 80, 160 };
        case Render_color::Purple : return { 90, 40, 140 };
        default : return { 20, 20, 20 };
    }
}