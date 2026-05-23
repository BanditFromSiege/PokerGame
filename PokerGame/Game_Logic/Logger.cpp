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
        result += c.get_str();
        result += ' ';
    }

    if (result.size() >= 1) {
        result.pop_back();
    }

    result += ";\n";

    for (std::size_t i = 0; const Pot& pot : c_ref_table.get_const_pots()) {
        result += "Pot_";
        result += std::to_string(++i);
        result += " [";
        result += std::to_string(pot.get_bank());
        result += "] (";

        for (std::uint8_t id : pot.get_players_id()) {
            result += c_ref_players[id].get_name();
            result += ", ";
        }

        if (result.size() >= 2) {
            result.pop_back();
            result.pop_back();
        }

        result += ");\n";
    }

    result += '\n';

    return result;
}

std::string Logger::get_str_showdown() noexcept {
    const auto& c_ref_winners_and_rewards = c_ref_manager.get_winners_and_rewards();
    const auto& c_ref_players = c_ref_manager.get_players();

    std::string result = "\nActive players:\n";
    result.reserve(256);

    for (const auto& p : c_ref_players) {
        if (p.is_active() || p.is_all_in()) {
            result += p.get_name();
            result += " {(";
            result += p.get_cards()[0].get_str();
            result += ", ";
            result += p.get_cards()[1].get_str();
            result += ")";

            if (auto opt_comb = p.get_combination(); opt_comb) {
                result += ' ';
                result += combination_to_c_str(opt_comb->get_power());
            }

            result += "};\n";
        }
    }

    result += '\n';

    for (std::size_t i = 0; const auto& [money, ids] : c_ref_winners_and_rewards) {
        result += "Pot ";
        result += std::to_string(++i);
        result += " [";
        result += std::to_string(money);
        result += "] Winners: ";

        for (std::uint8_t id : ids) {
            result += c_ref_players[id].get_name();
            result += ", ";
        }

        if (result.size() >= 2) {
            result.pop_back();
            result.pop_back();
        }

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
    result += p.get_name();

    if (p.get_is_can_show_type()) {
        result += ' ';
        result += player_type_to_c_str(p.get_type());
    }

    bool f = false;

    if (p.get_is_can_show_cards()) {
        result += " {(";
        result += p.get_cards()[0].get_str();
        result += ", ";
        result += p.get_cards()[1].get_str();
        result += ')';

        f = true;
    }

    if (p.get_is_can_show_absolute_probability()) {
        if (auto opt_absolute_probability = p.get_absolute_probability(); opt_absolute_probability) {
            result += std::format(", AP({:.2f})", *opt_absolute_probability);
        }

        f = true;
    }

    if (p.get_is_can_show_relative_probability()) {
        if (auto opt_relative_probability = p.get_relative_probability(); opt_relative_probability) {
            result += std::format(", RP({:.2f})", *opt_relative_probability);
        }

        f = true;
    }

    if (p.get_is_can_show_combination()) {
        if (auto opt_combination = p.get_combination(); opt_combination) {
            result += ", ";
            result += combination_to_c_str(opt_combination->get_power());
        }

        f = true;
    }

    if (f) {
        result += '}';
    }

    result += " [";
    result += std::to_string(p.get_money());
    result += "]";

    if (opt_move == Player_action::Fold) {
        result += ": Fold;";
    }
    else if (opt_move == Player_action::Call) {
        result += ": Call ";
        result += std::to_string(p.get_current_player_bet());
        
        if (p.get_status() == Player_status::All_in) {
            result += " (ALL-IN);";
        }
        else {
            result += ';';
        }
    }
    else if (opt_move == Player_action::Check) {
        auto bet = p.get_current_player_bet();

        result += ": Check";

        if (bet > 0) {
            result += ' ';
            result += std::to_string(bet);
        }

        result += ';';
    }
    else if (opt_move == Player_action::Raise) {
        result += ": Raise ";
        result += std::to_string(p.get_current_player_bet());

        if (p.get_status() == Player_status::All_in) {
            result += " (ALL-IN);";
        }
        else {
            result += ';';
        }
    }
    else if (opt_move == Player_action::None) {
        auto bet = p.get_current_player_bet();
        result += ": None";

        if (bet > 0) {
            result += ' ';
            result += std::to_string(bet);
        }

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
            result += "\nNEW GAME [";
            result += get_data_time();
            result += "];\n";
        }
        else {
            const auto& c_ref_players = c_ref_manager.get_players();

            for (std::size_t i = 0; i < c_ref_players.size(); ++i) {
                if (c_ref_players[i].get_status() == Player_status::Out_game && !outed_players[i]) {
                    result += "\n[";
                    result += get_time();
                    result += "] ";

                    result += c_ref_players[i].get_name();
                    result += " out from game;\n";

                    outed_players[i] = true;
                }
            }
        }

        const auto& c_ref_players = c_ref_manager.get_players();
        const auto& c_ref_table = c_ref_manager.get_table();
        auto diff = c_ref_players.front().get_difficulty();
        auto initial_money = c_ref_players.front().get_initial_money();
        auto game_mode = c_ref_manager.get_current_game_mode();

        result += "\nRound #";
        result += std::to_string(rounds);

        result += " (";
        result += std::to_string(c_ref_manager.get_count_active_players());
        result += '/';
        result += std::to_string(c_ref_players.size());
        result += " players:";

        for (const auto& p : c_ref_players) {
            if (p.is_in_game()) {
                result += ' ';
                result += p.get_name();
                result += ",";
            }
        }

        if (result.size() >= 1) {
            result.pop_back();
        }

        result += ");\n";

        result += "Difficulty: ";
        result += player_difficulty_to_c_str(diff);
        result += ";\n";

        result += "Initial stack: ";
        result += std::to_string(initial_money);
        result += ";\n";

        result += "Game mode: ";
        result += game_mode_to_c_str(game_mode);
        result += ";\n";

        result += "Blinds: ";
        result += std::to_string(c_ref_table.get_current_small_blind());
        result += '/';
        result += std::to_string(c_ref_table.get_current_big_blind());
        result += ";\n";

        result += "\nPREFLOP";
        result += " Dealer - ";
        result += c_ref_manager.get_players()[c_ref_manager.get_dealer_player_id()].get_name();
        result += ";\n\n";
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

        if (c_ref_manager.get_count_active_players() > 1) {
            result += get_str_table();

            if (result.size() >= 1) {
                result.pop_back();
            }
        }

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
        auto game_mode = c_ref_manager.get_current_game_mode();
        auto opt_id = c_ref_manager.get_winner_id();

        if (!opt_id && game_mode == Game_mode::Spectator) {
            return "";
        }

        const auto& c_ref_players = c_ref_manager.get_players();

        std::string result;
        result.reserve(256);

        for (std::size_t i = 0; i < c_ref_players.size(); ++i) {
            if (c_ref_players[i].get_status() == Player_status::Out_game && !outed_players[i]) {
                result += "\n[";
                result += get_time();
                result += "] ";

                result += c_ref_players[i].get_name();
                result += " out from game;\n";

                outed_players[i] = true;
            }
        }

        if (opt_id) {
            result += "\nWinner in game: ";
            result += c_ref_players[*opt_id].get_name();
            result += ";\n";
        }
        else {
            result += '\n';
            result += c_ref_players[0].get_name();
            result += " lose in game;\n";
        }

        return result;
    }

    return "";
}