#pragma once
#include <iostream>

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

enum class Player_action : std::uint8_t {
	Fold,
	Call,
	Check,
	Raise
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

inline std::ostream& operator<<(std::ostream& out, Poker_stage v) {
    switch (v) {
    case Poker_stage::Preparation_preflop: out << "Preparation_preflop";
        break;
    case Poker_stage::Preflop: out << "Preflop";
        break;
    case Poker_stage::Preparation_flop: out << "Preparation_flop";
        break;
    case Poker_stage::Flop: out << "Flop";
        break;
    case Poker_stage::Preparation_turn: out << "Preparation_turn";
        break;
    case Poker_stage::Turn: out << "Turn";
        break;
    case Poker_stage::Preparation_river: out << "Preparation_river";
        break;
    case Poker_stage::River: out << "River";
        break;
    case Poker_stage::Showdown: out << "Showdown";
        break;
    case Poker_stage::After_showdown: out << "After_showdown";
        break;
    }

    return out;
}

inline std::ostream& operator<<(std::ostream& out, Player_action v) {
    switch (v) {
    case Player_action::Fold: out << "Fold";
        break;
    case Player_action::Call: out << "Call";
        break;
    case Player_action::Check: out << "Check";
        break;
    case Player_action::Raise: out << "Raise";
        break;
    }

    return out;
}

inline std::ostream& operator<<(std::ostream& out, Player_status v) {
    switch (v) {
    case Player_status::Out_game: out << "Out_game";
        break;
    case Player_status::Folded: out << "Folded";
        break;
    case Player_status::Active: out << "Active";
        break;
    case Player_status::All_in: out << "All_in";
        break;
    }

    return out;
}

inline std::ostream& operator<<(std::ostream& out, Player_difficulty d) {
    switch (d) {
    case Player_difficulty::Hard: out << "Hard";
        break;
    case Player_difficulty::Medium: out << "Medium";
        break;
    case Player_difficulty::Easy: out << "Easy";
        break;
    }

    return out;
}