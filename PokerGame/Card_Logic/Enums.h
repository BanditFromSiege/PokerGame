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

inline std::ostream& operator<<(std::ostream& out, Combination c) {
    switch (c) {
    case Combination::Royal_flush : out << "Flush-Royal";
        break;
    case Combination::Straight_flush : out << "Straight-Flush";
        break;
    case Combination::Care : out << "Care";
        break;
    case Combination::Full_house : out << "Full-House";
        break;
    case Combination::Flush : out << "Flush";
        break;
    case Combination::Straight : out << "Straight";
        break;
    case Combination::Set : out << "Set";
        break;
    case Combination::Two_pair : out << "Two pair";
        break;
    case Combination::Pair : out << "Pair";
        break;
    case Combination::High_card : out << "High cart";
        break;
    }

    return out;
}

inline std::ostream& operator<<(std::ostream& out, Card_suit s) {
    switch (s) {
    case Card_suit::Hearts : out << 'H';
        break;
    case Card_suit::Diamonds : out << 'D';
        break;
    case Card_suit::Clubs : out << 'C';
        break;
    case Card_suit::Spades : out << 'S';
        break;
    }

    return out;
}

inline std::ostream& operator<<(std::ostream& out, Card_value v) {
    switch (v) {
    case Card_value::Two : out << '2';
        break;
    case Card_value::Three : out << '3';
        break;
    case Card_value::Four : out << '4';
        break;
    case Card_value::Five : out << '5';
        break;
    case Card_value::Six : out << '6';
        break;
    case Card_value::Seven : out << '7';
        break;
    case Card_value::Eight : out << '8';
        break;
    case Card_value::Nine : out << '9';
        break;
    case Card_value::Ten : out << 'T';
        break;
    case Card_value::Jack : out << 'J';
        break;
    case Card_value::Queen : out << 'Q';
        break;
    case Card_value::King : out << 'K';
        break;
    case Card_value::Ace : out << 'A';
        break;
    }

    return out;
}