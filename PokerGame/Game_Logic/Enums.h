#pragma once
#include <iostream>

enum class Game_mode : std::uint8_t {
    Spectator,
    Player
};

enum class Poker_stage : std::uint8_t {
    Preparation_preflop,
	Preflop,
    Preparation_flop,
    Flop,
    Preparation_turn,
    Turn,
    Preparation_river,
    River,
	Showdown,
    After_showdown,
};

enum class Player_type : std::uint8_t {
    TAG,
    TP,
    LAG,
    LP
};

enum class Player_action : std::uint8_t {
	Fold,
	Call,
	Check,
	Raise,
    None
};

enum class Player_status : std::uint8_t {
    Out_game,
	Folded,
	Active,
	All_in
};

enum class Player_difficulty : std::uint8_t {
    Hard,
    Medium,
    Easy
};

constexpr const char* game_mode_to_c_str(Game_mode m) noexcept {
    switch (m) {
        case Game_mode::Spectator : return "Spectator";
        case Game_mode::Player : return "Player";
        default : return "?";
    }
}

constexpr const char* poker_stage_to_c_str(Poker_stage v) noexcept {
    switch (v) {
        case Poker_stage::Preparation_preflop : return "Preparation_preflop";
        case Poker_stage::Preflop : return "Preflop";
        case Poker_stage::Preparation_flop : return "Preparation_flop";
        case Poker_stage::Flop : return "Flop";
        case Poker_stage::Preparation_turn : return "Preparation_turn";
        case Poker_stage::Turn : return "Turn";
        case Poker_stage::Preparation_river : return "Preparation_river";
        case Poker_stage::River : return "River";
        case Poker_stage::Showdown : return "Showdown";
        case Poker_stage::After_showdown : return "After_showdown";
        default : return "?";
    }
}

constexpr const char* player_type_to_c_str(Player_type pt) noexcept {
    switch (pt) {
        case Player_type::TAG : return "TAG";
        case Player_type::TP : return "TP";
        case Player_type::LAG : return "LAG";
        case Player_type::LP : return "LP";
        default: return "?";
    }
}

constexpr const char* player_action_to_c_str(Player_action v) noexcept {
    switch (v) {
        case Player_action::Fold : return "Fold";
        case Player_action::Call : return "Call";
        case Player_action::Check : return "Check";
        case Player_action::Raise : return "Raise";
        case Player_action::None : return "None";
        default : return "?";
    }
}

constexpr const char* player_status_to_c_str(Player_status v) noexcept {
    switch (v) {
        case Player_status::Out_game : return "Out_game";
        case Player_status::Folded : return "Folded";
        case Player_status::Active : return "Active";
        case Player_status::All_in : return "All_in";
        default : return "?";
    }
}

constexpr const char* player_difficulty_to_c_str(Player_difficulty d) noexcept {
    switch (d) {
        case Player_difficulty::Hard : return "Hard";
        case Player_difficulty::Medium : return "Medium";
        case Player_difficulty::Easy : return "Easy";
        default : return "?";
    }
}

inline std::ostream& operator<<(std::ostream& out, Game_mode m) {
    out << game_mode_to_c_str(m);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, Poker_stage v) {
    out << poker_stage_to_c_str(v);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, Player_type v) {
    out << player_type_to_c_str(v);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, Player_action v) {
    out << player_action_to_c_str(v);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, Player_status v) {
    out << player_status_to_c_str(v);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, Player_difficulty d) {
    out << player_difficulty_to_c_str(d);
    return out;
}