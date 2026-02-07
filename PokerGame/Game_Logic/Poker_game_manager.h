#pragma once
#include "Table.h"
#include <string>
#include <unordered_map>

template <typename T = std::execution::sequenced_policy>
requires (std::is_same_v<T, std::execution::sequenced_policy> || std::is_same_v<T, std::execution::parallel_policy>)
class Poker_game_manager final {
private:
	Poker_deck deck;
	std::vector<Player> players{};
	std::vector<std::uint8_t> players_index{};
	Table table;

	std::mt19937_64 rng{ std::random_device{}() };

	Poker_stage stage = Poker_stage::Preflop;

	Probability_evaluator<T> ev;

	std::uint8_t dealer_position_index = 0;
	std::uint8_t small_blind_position_index = 0;

	std::uint8_t active_players = 0;

	void rotate_players(std::uint8_t new_index) noexcept {
		std::uint8_t current_index = players_index.front();

		while (current_index != new_index) {
			std::rotate(players_index.begin(), players_index.begin() + 1, players_index.end());
			current_index = players_index.front();
		}
	}

	void make_initial_bets_for_2_players(std::uint8_t& current_player_index) noexcept {
		std::uint8_t small_blind_position = players_index[small_blind_position_index];

		std::size_t sb_bet
			= players[small_blind_position].make_bet_or_check(players[small_blind_position].get_current_big_blind() / 2);

		table.set_current_bet(players[small_blind_position].get_current_big_blind());

		table.add_to_sum_of_bets(sb_bet);

		current_player_index = dealer_position_index;
	}

	void make_initial_bets_for_3_players(std::uint8_t& current_player_index) noexcept {
		std::uint8_t big_blind_position_index = (small_blind_position_index + 1) % players_index.size();

		std::uint8_t small_blind_position = players_index[small_blind_position_index];
		std::uint8_t big_blind_position = players_index[big_blind_position_index];

		std::size_t sb_bet
			= players[small_blind_position].make_bet_or_check(players[small_blind_position].get_current_big_blind() / 2);

		std::size_t bb_bet
			= players[big_blind_position].make_bet_or_check(players[big_blind_position].get_current_big_blind());

		table.set_current_bet(bb_bet);

		table.add_to_sum_of_bets(sb_bet);
		table.add_to_sum_of_bets(bb_bet);

		current_player_index = dealer_position_index;
	}

	void make_initial_bets_for_more_then_3_players(std::uint8_t& current_player_index) noexcept {
		std::uint8_t big_blind_position_index = (small_blind_position_index + 1) % players_index.size();

		std::uint8_t new_curret_player_index = (big_blind_position_index + 1) % players_index.size();

		std::uint8_t small_blind_position = players_index[small_blind_position_index];
		std::uint8_t big_blind_position = players_index[big_blind_position_index];

		std::size_t sb_bet
			= players[small_blind_position].make_bet_or_check(players[small_blind_position].get_current_big_blind() / 2);

		std::size_t bb_bet
			= players[big_blind_position].make_bet_or_check(players[big_blind_position].get_current_big_blind());

		table.set_current_bet(bb_bet);

		table.add_to_sum_of_bets(sb_bet);
		table.add_to_sum_of_bets(bb_bet);

		current_player_index = new_curret_player_index;
	}

	void prepare_to_Preflop() noexcept {
		dealer_position_index = (dealer_position_index + 1) % players_index.size();

		small_blind_position_index = (dealer_position_index + 1) % players_index.size();

		stage = Poker_stage::Preflop;
		active_players = 0;

		for (auto& player : players) {
			if (player.is_in_game()) {
				Card c1 = *deck.get_card();
				Card c2 = *deck.get_card();
				player.set_cards(c1, c2);

				++active_players;
			}
		}

		std::cout << "ACTIVE PLAYERS: " << (int)active_players << '\n';
		std::cout << "Dealer: " << players[players_index[dealer_position_index]].get_name() << '\n';
	}

	void prepare_to_Flop() noexcept {
		stage = Poker_stage::Flop;

		Card c1 = *deck.get_card();
		Card c2 = *deck.get_card();
		Card c3 = *deck.get_card();

		table.add_card(c1);
		table.add_card(c2);
		table.add_card(c3);

		table.set_current_bet(0);
		table.reset_sum_of_bets();

		for (auto& p : players) {
			if (p.is_active()) {
				p.set_current_bet(0);
			}
		}
	}

	void prepare_to_Turn() noexcept {
		stage = Poker_stage::Turn;

		Card c4 = *deck.get_card();

		table.add_card(c4);

		table.set_current_bet(0);
		table.reset_sum_of_bets();

		for (auto& p : players) {
			if (p.is_active()) {
				p.set_current_bet(0);
			}
		}
	}

	void prepare_to_River() noexcept {
		stage = Poker_stage::River;

		Card c5 = *deck.get_card();

		table.add_card(c5);

		table.set_current_bet(0);
		table.reset_sum_of_bets();

		for (auto& p : players) {
			if (p.is_active()) {
				p.set_current_bet(0);
			}
		}
	}

	void add_bets_to_pots() {
		std::vector<std::pair<std::uint8_t, std::size_t>> contribs;

		for (const Player& p : players) {
			if (p.get_sum_of_bets() > 0) {
				contribs.push_back({ p.get_id(), p.get_sum_of_bets() });
			}
		}

		std::sort(contribs.begin(), contribs.end(),
			[](auto& a, auto& b) {
				return a.second < b.second;
			}
		);

		auto& pots = table.get_pots();

		pots.clear();

		std::size_t prev = 0;

		for (std::size_t i = 0; i < contribs.size(); ++i) {
			std::size_t level = contribs[i].second;
			std::size_t diff = level - prev;

			if (diff == 0) {
				continue;
			}

			std::vector<std::uint8_t> vec_players_id;

			for (std::size_t j = i; j < contribs.size(); ++j) {
				if (players[contribs[j].first].is_active() || players[contribs[j].first].is_all_in()) {
					vec_players_id.push_back(contribs[j].first);
				}
			}

			std::size_t participants = vec_players_id.size();

			if (participants > 0) {
				Pot pot;
				pot.set_bank(diff * participants);
				pot.set_players_id_in_pot(std::move(vec_players_id));
				pots.push_back(std::move(pot));
			}

			prev = level;
		}

		static std::unordered_map<
			std::vector<uint8_t>,
			std::size_t,
			decltype([](const std::vector<uint8_t>& v) {
				std::size_t h = 0;
				for (auto x : v) {
					h = h * 31 + x;
				}
				return h;
			})
		> merged;

		merged.clear();

		for (auto& pot : pots) {
			auto ids = pot.get_players_id();
			std::sort(ids.begin(), ids.end());
			merged[ids] += pot.get_bank();
		}

		pots.clear();

		for (auto& [ids, bank] : merged) {
			Pot pot;
			pot.set_players_id_in_pot(ids);
			pot.set_bank(bank);
			pots.push_back(std::move(pot));
		}
	}

public:
	Poker_game_manager(std::uint8_t number_players, std::size_t money) noexcept {
		deck = *Poker_deck::create_poker_deck();
		deck.shuffle(rng);

		players.reserve(number_players);

		for (std::uint8_t i = 0; i < number_players; ++i) {
			std::string name = "Bot_";
			name += std::to_string(i + 1);
			players.emplace_back(Player{ name.c_str(), i, 1000 });
			players_index.emplace_back(i);
		}

		std::uniform_int_distribution<> random_dealer_place(0, players_index.size() - 1);

		dealer_position_index = random_dealer_place(rng);
	}

	bool perform_stage() noexcept {
		std::uint8_t current_player_index = 0;

		if (stage == Poker_stage::Preflop) {
			if (players_index.size() == 2) {
				make_initial_bets_for_2_players(current_player_index);
			}
			else if (players_index.size() == 3) {
				make_initial_bets_for_3_players(current_player_index);
			}
			else {
				make_initial_bets_for_more_then_3_players(current_player_index);
			}
		}
		else {
			std::cout << '\n';

			std::cout << "Table cards: ";

			for (Card c : table.get_cards()) {
				std::cout << c << ' ';
			}
			std::cout << '\n';

			std::cout << "Table banks:\n";
			for (const Pot& pot : table.get_pots()) {
				std::cout << pot.get_bank() << " Players_id: ";
				for (auto id : pot.get_players_id()) {
					std::cout << (int)id << ' ';
				}
				std::cout << '\n';
			}
			std::cout << '\n';

			current_player_index = small_blind_position_index;
		}

		rotate_players(players_index[current_player_index]);

		for (std::size_t i = 0; i < players_index.size(); ++i) {
			Player& p = players[players_index[i]];

			if (!p.is_active()) {
				if (p.is_all_in()) {
					std::cout << "Player: " << p.get_name() << " | Money: " << p.get_money()
						<< " | Cards: " << p.get_cards()[0] << ' ' << p.get_cards()[1]
						<< " | Status: " << Player_status::All_in << '\n';
				}

				continue;
			}

			std::cout << "Player: " << p.get_name() << " | Money: " << p.get_money()
				<< " | Cards: " << p.get_cards()[0] << ' ' << p.get_cards()[1];

			auto [move, new_bet] = p.make_decision(
				rng,
				ev,
				table.get_cards(),
				active_players,
				table.get_bank_for_player(p) + table.get_sum_of_bets(),
				table.get_current_bet());

			std::cout << " | Status: " << move;

			if (move == Player_action::Call || move == Player_action::Raise) {
				auto diff = p.make_bet_or_check(new_bet);
				table.add_to_sum_of_bets(diff);

				std::cout << " | Bet: " << diff << '\n';

				if (new_bet > table.get_current_bet()) {
					table.set_current_bet(new_bet);
				}

				if (move == Player_action::Raise) {
					rotate_players(players_index[i]);
					i = 0;
				}
			}
			else if (move == Player_action::Fold) {
				std::cout << " | Bet: " << new_bet << '\n';
				p.make_fold();
				--active_players;
			}
			else if (move == Player_action::Check) {
				std::cout << " | Bet: " << new_bet << '\n';
			}

			if (active_players == 1) {
				auto p_it = std::find_if(players.begin(), players.end(), [](auto& p) {
					return p.is_active() || p.is_all_in();
				});

				std::cout << "WIN: " << p_it->get_name() << '\n';
				p_it->get_win(table.get_sum_of_bets() + table.get_bank_for_player(*p_it));

				return false;
			}
		}

		add_bets_to_pots();

		return true;
	}

	void Showdown() {
		std::cout << "\nShowdown: " << '\n';

		const auto& banks = table.get_pots();

		std::cout << "Number of banks: " << banks.size() << '\n';

		for (std::uint8_t i = 0; i < banks.size(); ++i) {
			std::cout << i + 1 << ": " << banks[i].get_bank() << " pot, Players_id: ";
			for (int x : banks[i].get_players_id()) {
				std::cout << x << ' ';
			}
			std::cout << '\n';
		}
		std::cout << '\n';

		for (std::uint8_t i = 0; i < banks.size(); ++i) {
			auto current_bank = banks[i].get_bank();
			const auto& players_in_bank = banks[i].get_players_id();

			if (players_in_bank.size() == 1) {
				std::cout << "WIN this bank: " << players[players_in_bank.front()].get_name() << '\n';

				players[players_in_bank.front()].get_win(current_bank);

				continue;
			}

			std::vector<std::pair<Player*, Poker_combination>> active_p(players_in_bank.size());

			for (std::uint8_t i = 0; i < active_p.size(); ++i) {
				auto& p = players[players_in_bank[i]];

				auto arr_player_cards = p.get_cards();
				const auto& arr_table_cards = table.get_cards();

				std::array<Card, Card::COUNT_OF_CARDS_ON_RIVER> all_cards;

				all_cards[0] = arr_player_cards[0];
				all_cards[1] = arr_player_cards[1];

				for (std::uint8_t j = 0; j < arr_table_cards.size(); ++j) {
					all_cards[j + 2] = arr_table_cards[j];
				}

				active_p[i] = { &p, *Poker_combination::create_combination_by_cards(all_cards) };
			}

			auto best_comb = std::max_element(active_p.begin(), active_p.end(), [](auto& p1, auto& p2) {
				return p1.second < p2.second;
			});

			std::size_t count = std::count_if(active_p.begin(), active_p.end(), [&best_comb](auto& p) {
				return p.second == best_comb->second;
			});

			if (count == 1) {
				std::cout << "WIN this bank: " << best_comb->first->get_name() << '\n';

				best_comb->second.show_combination();

				best_comb->first->get_win(current_bank);
			}
			else {
				std::cout << "WIN this bank: ";

				current_bank /= count;

				for (auto& p : active_p) {
					if (p.second == best_comb->second) {
						std::cout << p.first->get_name() << ' ';
						p.first->get_win(current_bank);
					}
				}

				best_comb->second.show_combination();
			}
		}
	}

	bool reset() noexcept {
		deck.shuffle(rng);

		for (auto& p : players) {
			p.set_sum_of_bets(0);
			p.set_current_bet(0);

			p.check_money_enough();

			if (!p.is_in_game()) {
				auto it = std::find(players_index.begin(), players_index.end(), p.get_id());
				if (it != players_index.end()) {
					std::cout << players[*it].get_name() << " - Out from game\n";

					players_index.erase(it);
				}
			}
		}

		if (players_index.size() == 1) {
			std::cout << "WIN in game: " << players[players_index.front()].get_name() << '\n';
			return false;
		}

		std::sort(players_index.begin(), players_index.end());

		table.clear();

		std::cout << '\n';
		return true;
	}

	void Run() noexcept {
		bool continue_game = true;

		do {
			bool c = true;
			if (c) {
				prepare_to_Preflop();
				c = perform_stage();
			}

			if (c) {
				prepare_to_Flop();
				c = perform_stage();
			}

			if (c) {
				prepare_to_Turn();
				c = perform_stage();
			}

			if (c) {
				prepare_to_River();
				c = perform_stage();
			}

			if (c) {
				Showdown();
			}

			continue_game = reset();
		} while (continue_game);
	}
};