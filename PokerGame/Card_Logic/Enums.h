#pragma once
#include <iostream>

enum class Card_suit : std::uint8_t {
    Hearts,
    Diamonds,
    Clubs,
    Spades
};

enum class Card_value : std::uint8_t {
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

enum class Combination : std::uint8_t {
    High_card,
    Pair,
    Two_pair,
    Set,
    Straight,
    Flush,
    Full_house,
    Care,
    Straight_flush,
    Royal_flush
};

enum class Result : std::uint8_t {
    Draw,
    Win,
    Loss
};

constexpr char card_suit_to_char(Card_suit s) noexcept {
    switch (s) {
        case Card_suit::Hearts : return 'H';
        case Card_suit::Diamonds : return 'D';
        case Card_suit::Clubs : return 'C';
        case Card_suit::Spades : return 'S';
        default : return '?';
    }
}

constexpr char card_value_to_char(Card_value v) noexcept {
    switch (v) {
        case Card_value::Two : return '2';
        case Card_value::Three : return '3';
        case Card_value::Four : return '4';
        case Card_value::Five : return '5';
        case Card_value::Six : return '6';
        case Card_value::Seven : return '7';
        case Card_value::Eight : return '8';
        case Card_value::Nine : return '9';
        case Card_value::Ten : return 'T';
        case Card_value::Jack : return 'J';
        case Card_value::Queen : return 'Q';
        case Card_value::King : return 'K';
        case Card_value::Ace : return 'A';
        default : return '?';
    }
}

constexpr const char* combination_to_c_str(Combination c) noexcept {
    switch (c) {
        case Combination::Royal_flush : return "Flush-Royal";
        case Combination::Straight_flush : return "Straight-Flush";
        case Combination::Care : return "Care";
        case Combination::Full_house : return "Full-House";
        case Combination::Flush : return "Flush";
        case Combination::Straight : return "Straight";
        case Combination::Set : return "Set";
        case Combination::Two_pair : return "Two pair";
        case Combination::Pair : return "Pair";
        case Combination::High_card : return "High card";
        default : return "?";
    }
}

inline std::ostream& operator<<(std::ostream& out, Combination c) {
    out << combination_to_c_str(c);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, Card_suit s) {
    out << card_suit_to_char(s);
    return out;
}

inline std::ostream& operator<<(std::ostream& out, Card_value v) {
    out << card_value_to_char(v);
    return out;
}