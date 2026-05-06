#include "Poker_game_manager.h"

void Poker_game_manager::rotate_players(std::uint8_t new_index) noexcept {
	std::uint8_t current_index = players_index.front();

	while (current_index != new_index) {
		std::rotate(players_index.begin(), players_index.begin() + 1, players_index.end());
		current_index = players_index.front();
	}
}

std::uint8_t Poker_game_manager::get_next_id(std::uint8_t index) noexcept {
	std::uint8_t new_index = (index + 1) % players.size();

	while (!players_ids_in_game.contains(new_index)) {
		new_index = (new_index + 1) % players.size();
	}

	return new_index;
}

void Poker_game_manager::find_absolute_probabilities_for_players() noexcept {
	std::vector<std::uint8_t> active_players_ids;

	for (const auto& p : players) {
		if (p.is_active() || p.is_all_in()) {
			active_players_ids.push_back(p.get_id());
		}
	}

	auto cards_of_first_player = players[active_players_ids.front()].get_cards();

	std::vector<std::pair<Card, Card>> players_cards(active_players_ids.size() - 1);

	for (std::size_t i = 1; i < active_players_ids.size(); ++i) {
		players_cards[i - 1] = players[active_players_ids[i]].get_pair_of_cards();
	}

	std::vector<double> absolute_probabilities;

	if (use_evaluator_sequenced) {
		absolute_probabilities = *evaluator_sequenced.get_absolute_probability(
			cards_of_first_player,
			table.get_cards(),
			players_cards,
			active_players
		);
	}
	else {
		absolute_probabilities = *evaluator_parallel.get_absolute_probability(
			cards_of_first_player,
			table.get_cards(),
			players_cards,
			active_players
		);
	}

	for (std::size_t i = 0; i < active_players_ids.size(); ++i) {
		players[active_players_ids[i]].set_absolute_probability(absolute_probabilities[i]);
	}
}

void Poker_game_manager::make_initial_bets_for_2_players(std::uint8_t& current_player_index) noexcept {
	std::uint8_t small_blind_position = dealer_id;
	std::uint8_t big_blind_position = small_blind_id;

	std::size_t big_blind = players[big_blind_position].get_current_big_blind();
	std::size_t small_blind = big_blind / 2;

	std::size_t sb_bet
		= players[small_blind_position].make_bet_or_check(small_blind);

	std::size_t bb_bet
		= players[big_blind_position].make_bet_or_check(big_blind);

	table.set_current_bet(big_blind);

	table.add_to_sum_of_bets_on_current_stage(sb_bet);
	table.add_to_sum_of_bets_on_current_stage(bb_bet);

	current_player_index = dealer_id;
}

void Poker_game_manager::make_initial_bets_for_3_players(std::uint8_t& current_player_index) noexcept {
	std::uint8_t small_blind_position = small_blind_id;
	std::uint8_t big_blind_position = get_next_id(small_blind_position);

	std::size_t big_blind = players[big_blind_position].get_current_big_blind();
	std::size_t small_blind = big_blind / 2;

	std::size_t sb_bet
		= players[small_blind_position].make_bet_or_check(small_blind);

	std::size_t bb_bet
		= players[big_blind_position].make_bet_or_check(big_blind);

	table.set_current_bet(big_blind);

	table.add_to_sum_of_bets_on_current_stage(sb_bet);
	table.add_to_sum_of_bets_on_current_stage(bb_bet);

	current_player_index = dealer_id;
}

void Poker_game_manager::make_initial_bets_for_more_then_3_players(std::uint8_t& current_player_index) noexcept {
	std::uint8_t small_blind_position = small_blind_id;
	std::uint8_t big_blind_position = get_next_id(small_blind_position);
	std::uint8_t new_curret_player_id = get_next_id(big_blind_position);

	std::size_t big_blind = players[big_blind_position].get_current_big_blind();
	std::size_t small_blind = big_blind / 2;

	std::size_t sb_bet
		= players[small_blind_position].make_bet_or_check(small_blind);

	std::size_t bb_bet
		= players[big_blind_position].make_bet_or_check(big_blind);

	table.set_current_bet(big_blind);

	table.add_to_sum_of_bets_on_current_stage(sb_bet);
	table.add_to_sum_of_bets_on_current_stage(bb_bet);

	current_player_index = new_curret_player_id;
}

void Poker_game_manager::prepare_to_Preflop() noexcept {
	winners_and_rewards.clear();

	++number_of_rounds;

	active_players = players_ids_in_game.size();

	players_index = std::vector(players_ids_in_game.begin(), players_ids_in_game.end());
	std::sort(players_index.begin(), players_index.end());

	for (std::uint8_t id : players_index) {
		Player& player = players[id];

		Card c1 = *deck.get_card();
		Card c2 = *deck.get_card();
		player.set_cards(c1, c2);
	}

	if (active_players > 1) {
		find_absolute_probabilities_for_players();
	}
}

void Poker_game_manager::prepare_to_Flop() noexcept {
	Card c1 = *deck.get_card();
	Card c2 = *deck.get_card();
	Card c3 = *deck.get_card();

	table.add_card(c1);
	table.add_card(c2);
	table.add_card(c3);

	table.set_current_bet(0);
	table.reset_sum_of_bets_on_current_stage();

	for (auto& p : players) {
		p.set_current_bet(0);
		p.set_last_move(std::nullopt);
		p.set_relative_probability(std::nullopt);
		if (p.is_active() || p.is_all_in()) {
			p.set_combination(table.get_cards());
		}
	}

	if (active_players > 1) {
		find_absolute_probabilities_for_players();
	}
}

void Poker_game_manager::prepare_to_Turn() noexcept {
	Card c4 = *deck.get_card();

	table.add_card(c4);

	table.set_current_bet(0);
	table.reset_sum_of_bets_on_current_stage();

	for (auto& p : players) {
		p.set_current_bet(0);
		p.set_last_move(std::nullopt);
		p.set_relative_probability(std::nullopt);
		if (p.is_active() || p.is_all_in()) {
			p.set_combination(table.get_cards());
		}
	}

	if (active_players > 1) {
		find_absolute_probabilities_for_players();
	}
}

void Poker_game_manager::prepare_to_River() noexcept {
	Card c5 = *deck.get_card();

	table.add_card(c5);

	table.set_current_bet(0);
	table.reset_sum_of_bets_on_current_stage();

	for (auto& p : players) {
		p.set_current_bet(0);
		p.set_last_move(std::nullopt);
		p.set_relative_probability(std::nullopt);
		if (p.is_active() || p.is_all_in()) {
			p.set_combination(table.get_cards());
		}
	}

	if (active_players > 1) {
		find_absolute_probabilities_for_players();
	}
}

void Poker_game_manager::prepare_to_Showdown() noexcept {
	for (auto& p : players) {
		p.set_current_bet(0);
		p.set_last_move(std::nullopt);
		p.set_relative_probability(std::nullopt);
	}
}

void Poker_game_manager::add_bets_to_pots() noexcept {
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

	std::vector<std::pair<std::uint8_t, std::size_t>> merged{};
	merged.reserve(Probability_evaluator<>::MAX_PLAYERS);

	std::size_t prev = 0;

	for (std::size_t i = 0; i < contribs.size(); ++i) {
		std::size_t level = contribs[i].second;
		std::size_t diff = level - prev;

		if (diff == 0) {
			continue;
		}

		std::size_t depositors_players_count = 0;
		std::size_t present_players_mask = 0;

		for (std::size_t j = i; j < contribs.size(); ++j) {
			++depositors_players_count;

			auto id = contribs[j].first;

			if (players[id].is_active() || players[id].is_all_in()) {
				present_players_mask |= (1ull << id);
			}
		}

		if (present_players_mask != 0) {
			auto it = std::find_if(merged.begin(), merged.end(), [&present_players_mask](auto& p) {
				return p.first == present_players_mask;
			});

			if (it != merged.end()) {
				it->second += diff * depositors_players_count;
			}
			else {
				merged.emplace_back(present_players_mask, diff * depositors_players_count);
			}
		}

		prev = level;
	}

	auto& pots = table.get_pots();

	pots.clear();

	for (const auto& [mask, bank] : merged) {
		std::vector<std::uint8_t> ids;

		for (std::uint8_t id = 0; id < Probability_evaluator<>::MAX_PLAYERS; ++id) {
			if (mask & (1ull << id)) {
				ids.push_back(id);
			}
		}

		if (ids.size() == 1) {
			std::uint8_t winner_id = ids.front();
			std::size_t sum_of_bets = players[winner_id].get_sum_of_bets();

			players[winner_id].get_win(bank);

			if (bank < sum_of_bets) {
				players[winner_id].set_sum_of_bets(sum_of_bets - bank);
			}
			else {
				players[winner_id].set_sum_of_bets(0);
			}
		}
		else {
			Pot pot;
			pot.set_players_id_in_pot(std::move(ids));
			pot.set_bank(bank);
			pots.push_back(std::move(pot));
		}
	}
}

void Poker_game_manager::perform_stage() noexcept {
	if (stage == Poker_stage::Preparation_preflop) {
		prepare_to_Preflop();

		current_player_id = std::nullopt;

		std::uint8_t current_player_id = 0;

		if (players_ids_in_game.size() == 2) {
			make_initial_bets_for_2_players(current_player_id);
		}
		else if (players_ids_in_game.size() == 3) {
			make_initial_bets_for_3_players(current_player_id);
		}
		else {
			make_initial_bets_for_more_then_3_players(current_player_id);
		}

		rotate_players(current_player_id);

		stage = Poker_stage::Preflop;

		current_player_index_id = 0;
	}
	else if (stage == Poker_stage::Preparation_flop) {
		current_player_id = std::nullopt;
		
		prepare_to_Flop();

		rotate_players(small_blind_id);

		stage = Poker_stage::Flop;
		current_player_index_id = 0;
	}
	else if (stage == Poker_stage::Preparation_turn) {
		current_player_id = std::nullopt;
		
		prepare_to_Turn();

		rotate_players(small_blind_id);

		stage = Poker_stage::Turn;
		current_player_index_id = 0;
	}
	else if (stage == Poker_stage::Preparation_river) {
		current_player_id = std::nullopt;
		
		prepare_to_River();

		rotate_players(small_blind_id);

		stage = Poker_stage::River;
		current_player_index_id = 0;
	}
	else if (stage == Poker_stage::Showdown) {
		current_player_id = std::nullopt;

		prepare_to_Showdown();

		perform_showdown_step();
	}
	else if (stage == Poker_stage::After_showdown) {
		is_game_run = reset_for_next_round();

		if (is_game_run) {
			stage = Poker_stage::Preparation_preflop;
		}
	}
	else {
		perform_player_step();
	}
}

void Poker_game_manager::perform_player_step() noexcept {
	if (current_player_index_id < players_ids_in_game.size()) {
		while (!players[players_index[current_player_index_id]].is_active()) {
			++current_player_index_id;

			if (current_player_index_id >= players_ids_in_game.size()) {
				break;
			}
		}
	}

	if (current_player_index_id < players_ids_in_game.size()) {
		Player& p = players[players_index[current_player_index_id]];

		current_player_id = p.get_id();

		std::pair<Player_action, std::size_t> pair{};

		if (use_evaluator_sequenced) {
			pair = p.make_decision(
				rng,
				evaluator_sequenced,
				std::span(table.get_cards()),
				std::span(players),
				table.get_bank_for_player(p) + table.get_sum_of_bets_on_current_stage(),
				table.get_current_bet(),
				table.get_last_bet_diff()
			);
		}
		else {
			pair = p.make_decision(
				rng,
				evaluator_parallel,
				std::span(table.get_cards()),
				std::span(players),
				table.get_bank_for_player(p) + table.get_sum_of_bets_on_current_stage(),
				table.get_current_bet(),
				table.get_last_bet_diff()
			);
		}

		auto& move = pair.first;
		auto& new_bet = pair.second;

		p.set_last_move(move);

		if (move == Player_action::Call || move == Player_action::Raise) {
			auto diff = p.make_bet_or_check(new_bet);
			table.add_to_sum_of_bets_on_current_stage(diff);

			if (new_bet > table.get_current_bet()) {
				table.set_current_bet(new_bet);
			}

			if (move == Player_action::Raise) {
				rotate_players(players_index[current_player_index_id]);
				current_player_index_id = 0;
			}
		}
		else if (move == Player_action::Fold) {
			p.make_fold();
			--active_players;

			if (active_players > 1) {
				find_absolute_probabilities_for_players();
			}
		}

		if (active_players == 1) {
			auto p_it = std::find_if(players.begin(), players.end(), [](auto& p) {
				return p.is_active() || p.is_all_in();
			});

			auto& pots = table.get_pots();
			std::size_t total_bank = 0;

			for (const auto& pot : pots) {
				total_bank += pot.get_bank();
			}

			total_bank += table.get_sum_of_bets_on_current_stage();

			pots.clear();
			pots.emplace_back(std::vector{ p_it->get_id() }, total_bank);

			winners_and_rewards.push_back({ total_bank, std::vector{ p_it->get_id() }});

			p_it->set_absolute_probability(1.0);

			prepare_to_Showdown();

			stage = Poker_stage::After_showdown;

			return;
		}

		++current_player_index_id;
	}
	else {
		current_player_id = std::nullopt;

		if (table.get_sum_of_bets_on_current_stage() > 0) {
			add_bets_to_pots();
		}

		stage = static_cast<Poker_stage>(static_cast<std::uint8_t>(stage) + 1);
	}
}

void Poker_game_manager::perform_showdown_step() noexcept {
	const auto& banks = table.get_pots();

	if (current_bank_index_id < banks.size()) {
		const auto current_bank = banks[current_bank_index_id].get_bank();
		const auto& players_in_bank = banks[current_bank_index_id].get_players_id();

		auto best_id_comb = std::max_element(
			players_in_bank.begin(),
			players_in_bank.end(),
			[this](std::uint8_t id_1, std::uint8_t id_2) {
				return players[id_1].get_combination() < players[id_2].get_combination();
			}
		);

		auto opt_best_combination = players[*best_id_comb].get_combination();

		std::size_t count = std::count_if(
			players_in_bank.begin(),
			players_in_bank.end(),
			[this, &opt_best_combination](std::uint8_t id) {
				return players[id].get_combination() == opt_best_combination;
			}
		);

		if (count == 1) {
			winners_and_rewards.push_back({ current_bank, std::vector{ *best_id_comb } });
		}
		else {
			std::vector<std::uint8_t> winners_id(count);

			for (std::uint8_t i = 0; std::uint8_t id : players_in_bank) {
				if (players[id].get_combination() == opt_best_combination) {
					winners_id[i++] = id;
				}
			}

			std::size_t base_win = current_bank / count;
			std::size_t odd_chip = current_bank % count;

			if (odd_chip != 0) {
				rotate_players(dealer_id);

				for (std::uint8_t id : players_index) {
					auto it = std::find(winners_id.begin(), winners_id.end(), id);

					if (it != winners_id.end()) {
						winners_and_rewards.push_back({ base_win + odd_chip, std::vector{ id } });
						winners_id.erase(it);
						break;
					}
				}
			}

			winners_and_rewards.push_back({ base_win, std::move(winners_id) });
		}

		++current_bank_index_id;
	}
	else {
		stage = Poker_stage::After_showdown;

		current_bank_index_id = 0;
	}
}

bool Poker_game_manager::reset_for_next_round() noexcept {
	for (const auto& pair : winners_and_rewards) {
		for (std::uint8_t id : pair.second) {
			players[id].get_win(pair.first);
		}
	}

	deck.shuffle(rng);

	for (auto& p : players) {
		p.reset_for_new_hand();

		p.check_money_enough();

		if (!p.is_in_game()) {
			players_ids_in_game.erase(p.get_id());
		}
	}

	if (players_ids_in_game.size() == 1) {
		return false;
	}

	dealer_id = get_next_id(dealer_id);
	small_blind_id = get_next_id(dealer_id);

	table.clear();

	return true;
}

Poker_game_manager::Poker_game_manager(
	std::mt19937_64& rng,
	std::vector<Player>& players,
	Probability_evaluator<std::execution::sequenced_policy>& eval_seq,
	Probability_evaluator<std::execution::parallel_policy>& eval_par
) noexcept
	: rng(rng)
	, players(players)
	, evaluator_sequenced(eval_seq)
	, evaluator_parallel(eval_par)
{
	deck = *Poker_deck::create_poker_deck();
	reset_for_new_game();
}

const Table& Poker_game_manager::get_table() const noexcept {
	return table;
}

const std::vector<Player>& Poker_game_manager::get_players() const noexcept {
	return players;
}

Poker_stage Poker_game_manager::get_current_stage() const noexcept {
	return stage;
}

const std::optional<std::uint8_t> Poker_game_manager::get_current_player_id() const noexcept {
	return current_player_id;
}

const std::uint8_t Poker_game_manager::get_dealer_player_id() const noexcept {
	return dealer_id;
}

std::uint8_t Poker_game_manager::get_active_players() const noexcept {
	return active_players;
}

auto Poker_game_manager::get_winners_and_rewards() const noexcept
	-> const std::vector<std::pair<std::size_t, std::vector<std::uint8_t>>>&
{
	return winners_and_rewards;
}

std::size_t Poker_game_manager::get_number_of_rounds() const noexcept {
	return number_of_rounds;
}

std::optional<std::uint8_t> Poker_game_manager::get_winner_id() const noexcept {
	if (!is_game_still_run()) {
		for (std::uint8_t i = 0; i < players.size(); ++i) {
			if (players[i].get_status() != Player_status::Out_game) {
				return i;
			}
		}
	}

	return std::nullopt;
}

void Poker_game_manager::set_evaluator_sequenced_policy() noexcept {
	use_evaluator_sequenced = true;
}

void Poker_game_manager::set_evaluator_parallel_policy() noexcept {
	use_evaluator_sequenced = false;
}

void Poker_game_manager::reset_for_new_game() noexcept {
	deck.shuffle(rng);

	table.clear();

	for (std::uint8_t i = 0; i < players.size(); ++i) {
		players_ids_in_game.insert(i);
	}

	std::uniform_int_distribution<> random_dealer_place(0, players_ids_in_game.size() - 1);

	dealer_id = random_dealer_place(rng);
	small_blind_id = get_next_id(dealer_id);

	number_of_rounds = 0;

	is_game_run = true;
	stage = Poker_stage::Preparation_preflop;
}

bool Poker_game_manager::is_game_still_run() const noexcept {
	return is_game_run;
}

void Poker_game_manager::call_next_move() noexcept {
	if (is_game_run) {
		perform_stage();
	}
}