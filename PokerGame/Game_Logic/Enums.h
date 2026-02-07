#pragma once
#include <iostream>

enum class Poker_stage : std::uint8_t {
	Preflop,
	Flop,
	Turn,
	River,
	Showdown
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

inline std::ostream& operator<<(std::ostream& out, Poker_stage v) {
    switch (v) {
    case Poker_stage::Preflop: out << "Preflop";
        break;
    case Poker_stage::Flop: out << "Flop";
        break;
    case Poker_stage::Turn: out << "Turn";
        break;
    case Poker_stage::River: out << "River";
        break;
    case Poker_stage::Showdown: out << "Showdown";
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