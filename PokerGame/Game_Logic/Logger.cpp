#include "Logger.h"

std::string Logger::get_time() noexcept {
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);

    std::tm tm{};
    localtime_s(&tm, &t);

    return std::format("{:02}:{:02}:{:02}", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

std::string Logger::get_data_time() noexcept {
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);

    std::tm tm{};
    localtime_s(&tm, &t);

    return std::format("{:04}-{:02}-{:02}", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

std::string Logger::get_str_table() noexcept {
    const auto& c_ref_table = c_ref_manager.get_table();
    const auto& c_ref_players = c_ref_manager.get_players();

    std::string result = "\nTable cards: ";
    result.reserve(256);

    for (Card c : c_ref_table.get_cards()) {
        result += c.get_str() + ' ';
    }
    result.pop_back();
    result += ";\n";

    for (std::size_t i = 0; const Pot& pot : c_ref_table.get_const_pots()) {
        result += "Pot_" + std::to_string(++i);
        result += " [" + std::to_string(pot.get_bank()) + "] (";

        for (std::uint8_t id : pot.get_players_id()) {
            result += c_ref_players[id].get_name() + ", ";
        }
        result.pop_back();
        result.pop_back();

        result += ");\n";
    }

    result += '\n';

    return result;
}

std::string Logger::get_str_showdown() noexcept {
    const auto& c_ref_winners_and_rewards = c_ref_manager.get_winners_and_rewards();
    const auto& c_ref_players = c_ref_manager.get_players();

    std::string result = "\n";
    result.reserve(256);

    for (std::size_t i = 0; const auto& [money, ids] : c_ref_winners_and_rewards) {
        result += "Pot " + std::to_string(++i);
        result += " [" + std::to_string(money) + "] Winners: ";

        for (std::uint8_t id : ids) {
            result += c_ref_players[id].get_name() + ", ";
        }
        result.pop_back();
        result.pop_back();

        if (auto opt_comb = c_ref_players[ids.front()].get_combination(); opt_comb) {
            result += " (Combination - ";
            result += combination_to_c_str(opt_comb->get_power());
            result += ')';
        }
        result += ";\n";
    }

    return result;
}

std::string Logger::log_player_action(std::uint8_t id) noexcept {
    const Player& p = c_ref_manager.get_players()[id];
    const auto opt_move = p.get_last_move();

    if (!opt_move) {
        return "";
    }

    std::string result = "[";
    result.reserve(256);

    result += get_time();
    result += "] ";
    result += p.get_name() + ' ';
    result += player_type_to_c_str(p.get_type());

    result += " {(" + p.get_cards()[0].get_str() + ", " + p.get_cards()[1].get_str() + ')';

    if (auto opt_absolute_probability = p.get_absolute_probability(); opt_absolute_probability) {
        result += std::format(", AP({:.2f})", *opt_absolute_probability);
    }

    if (auto opt_relative_probability = p.get_relative_probability(); opt_relative_probability) {
        result += std::format(", RP({:.2f})", *opt_relative_probability);
    }
    
    if (auto opt_combination = p.get_combination(); opt_combination) {
        result += ", " + std::string(combination_to_c_str(opt_combination->get_power()));
    }

    result += "} [" + std::to_string(p.get_money()) + "]";

    if (opt_move == Player_action::Fold) {
        result += ": Fold;";
    }
    else if (opt_move == Player_action::Call) {
        result += ": Call " + std::to_string(p.get_current_player_bet());
        
        if (p.get_status() == Player_status::All_in) {
            result += " (ALL-IN);";
        }
        else {
            result += ';';
        }
    }
    else if (opt_move == Player_action::Check) {
        result += ": Check;";
    }
    else if (opt_move == Player_action::Raise) {
        result += ": Raise " + std::to_string(p.get_current_player_bet());

        if (p.get_status() == Player_status::All_in) {
            result += " (ALL-IN);";
        }
        else {
            result += ';';
        }
    }

    result += '\n';

    return result;
}

std::string Logger::log_stage(Poker_stage stage) noexcept {
    auto opt_id = c_ref_manager.get_current_player_id();

    std::string result;
    result.reserve(256);

    if (stage == Poker_stage::Preflop && !opt_id) {
        auto rounds = c_ref_manager.get_number_of_rounds();

        if (rounds == 1) {
            result += "------------------------------------------------------------------------------------";
            result += "\nNEW GAME [" + get_data_time() + "];\n";
        }
        else {
            const auto& c_ref_players = c_ref_manager.get_players();

            for (std::size_t i = 0; i < c_ref_players.size(); ++i) {
                if (c_ref_players[i].get_status() == Player_status::Out_game && !outed_players[i]) {
                    result += "\n[" + get_time() + "] ";
                    result += c_ref_players[i].get_name() + " out from game;\n";
                    outed_players[i] = true;
                }
            }
        }

        result += "\nRound #" + std::to_string(rounds);
        result += " (" + std::to_string(c_ref_manager.get_active_players())
            + "/" + std::to_string(c_ref_manager.get_players().size()) + " players);\n";

        const auto& c_ref_players = c_ref_manager.get_players();
        const auto& c_ref_table = c_ref_manager.get_table();
        auto diff = c_ref_players.front().get_difficulty();

        result += "Difficulty: " + std::string(player_difficulty_to_c_str(diff)) + ";\n";

        result += "Blinds: " + std::to_string(c_ref_table.get_current_small_blind());
        result += "/" + std::to_string(c_ref_table.get_current_big_blind()) + ";\n";

        result += "PREFLOP";
        result += " Dealer - " + c_ref_manager.get_players()[c_ref_manager.get_dealer_player_id()].get_name() + ";\n\n";
    }
    else if (stage == Poker_stage::Flop && !opt_id) {
        result += "\nFLOP;";
        result += get_str_table();
    }
    else if (stage == Poker_stage::Turn && !opt_id) {
        result += "\nTURN;";
        result += get_str_table();
    }
    else if (stage == Poker_stage::River && !opt_id) {
        result += "\nRIVER;";
        result += get_str_table();
    }
    else if (stage == Poker_stage::After_showdown && !opt_id) {
        result += "\nSHOWDOWN;";
        result += get_str_showdown();
    }

    return result;
}

Logger::Logger(const Poker_game_manager& manager) noexcept
    : c_ref_manager(manager) {}

void Logger::reset() noexcept {
    last_stage = std::nullopt;

    for (bool& el : outed_players) {
        el = false;
    }
}

std::string Logger::get_message() noexcept {
    auto stage = c_ref_manager.get_current_stage();

    if (c_ref_manager.is_game_still_run()) {
        if (last_stage != stage) {
            last_stage = stage;
            return log_stage(stage);
        }
        else {
            auto opt_id = c_ref_manager.get_current_player_id();

            if (!opt_id) {
                return "";
            }

            return log_player_action(*opt_id);
        }
    }
    else {
        auto opt_id = c_ref_manager.get_winner_id();

        if (!opt_id) {
            return "";
        }

        const auto& c_ref_players = c_ref_manager.get_players();

        std::string result;
        result.reserve(256);

        for (std::size_t i = 0; i < c_ref_players.size(); ++i) {
            if (c_ref_players[i].get_status() == Player_status::Out_game && !outed_players[i]) {
                result += "\n[" + get_time() + "] ";
                result += c_ref_players[i].get_name() + " out from game;\n";
                outed_players[i] = true;
            }
        }

        result += "\nWinner in game: " + c_ref_players[*opt_id].get_name() + ";\n";

        return result;
    }

    return "";
}