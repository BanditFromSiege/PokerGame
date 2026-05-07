#pragma once
#include "../Enums.h"
#include "../../Card_Logic/Probability_evaluator.h"

class Player final {
private:
	std::string name;

	std::optional<Poker_combination> combination = std::nullopt;

	std::optional<double> relative_probability = std::nullopt;
	std::optional<double> absolute_probability = std::nullopt;

	const std::size_t initial_money = 0;

	std::size_t money = 0;
	std::size_t current_bet = 0;
	std::size_t sum_of_bets = 0;
	std::size_t current_big_blind = 0;

	std::array<Card, Card::COUNT_OF_CARDS_IN_HAND> cards = {};

	std::optional<Player_action> last_move = std::nullopt;
	std::uint8_t id = 0;

	Player_difficulty difficulty = Player_difficulty::Easy;
	Player_status status = Player_status::Active;

public:
	inline static std::size_t count_of_big_blinds = 50;

	Player() noexcept;
	Player(std::string name, std::uint8_t id, std::size_t money, Player_difficulty d) noexcept;

	std::string get_name() const noexcept;
	std::optional<Poker_combination> get_combination() const noexcept;

	std::optional<double> get_relative_probability() const noexcept;
	std::optional<double> get_absolute_probability() const noexcept;

	std::size_t get_money() const noexcept;
	std::size_t get_current_bet() const noexcept;
	std::size_t get_sum_of_bets() const noexcept;
	std::size_t get_current_big_blind() const noexcept;
	std::array<Card, Card::COUNT_OF_CARDS_IN_HAND> get_cards() const noexcept;
	std::pair<Card, Card> get_pair_of_cards() const noexcept;
	std::optional<Player_action> get_last_move() const noexcept;
	std::uint8_t get_id() const noexcept;
	Player_difficulty get_difficulty() const noexcept;
	Player_status get_status() const noexcept;

	bool is_active() const noexcept;
	bool is_in_game() const noexcept;
	bool is_all_in() const noexcept;

	void check_money_enough() noexcept;

	void set_combination(const std::vector<Card>& table_cards) noexcept;

	void set_relative_probability(std::optional<double> probability) noexcept;
	void set_absolute_probability(std::optional<double> probability) noexcept;

	void set_current_bet(std::size_t bet) noexcept;
	void set_sum_of_bets(std::size_t bet) noexcept;
	void set_cards(Card c1, Card c2) noexcept;
	void set_last_move(std::optional<Player_action> new_move) noexcept;
	void set_id(std::uint8_t index) noexcept;
	void set_status(Player_status new_status) noexcept;

	void make_fold() noexcept;
	void get_win(std::size_t share) noexcept;

	void reset_for_new_hand() noexcept;
	void reset_for_new_game() noexcept;

	std::size_t make_bet_or_check(std::size_t bet) noexcept;

	template <typename T = std::execution::sequenced_policy>
	requires (std::is_same_v<T, std::execution::sequenced_policy> || std::is_same_v<T, std::execution::parallel_policy>)
	std::pair<Player_action, std::size_t> make_decision(
		std::mt19937_64& rng,
		Probability_evaluator<T>& evaluator,
		std::span<const Card> table_cards,
		std::span<const Player> players,
		std::size_t bank_with_all_current_bets_on_street,
		std::size_t table_current_bet,
		std::size_t table_last_bet_diff
	) noexcept {
		const std::size_t bet_for_call = table_current_bet - current_bet;

		const std::uint8_t active_players = std::count_if(players.begin(), players.end(),
			[](const Player& p) {
				return p.is_active() || p.is_all_in();
			}
		);

		std::size_t max_bet = 0;

		for (const Player& p : players) {
			if (p.get_id() != id && p.get_money() != 0 && (p.is_active() || p.is_all_in())) {
				max_bet = std::max(max_bet, p.get_money());
			}
		}

		max_bet = std::min(max_bet, money);

		double win_prob = *evaluator.get_relative_probability(
			this->get_cards(),
			table_cards,
			active_players);

		std::uniform_real_distribution<double> noise(
			-10.0 * static_cast<double>(difficulty), 10.0 * static_cast<double>(difficulty)
		);

		win_prob += (noise(rng) / 100.0);

		win_prob = std::clamp(win_prob, 0.0, 1.0);

		relative_probability = win_prob;

		std::vector<std::size_t> bets;

		double max_ev = 0.0;

		std::size_t potential_bet = bet_for_call;

		bool begin_raise_bet = true;

		if (table_last_bet_diff == 0) {
			table_last_bet_diff = current_big_blind;
		}

		do {
			const double ev =
				(win_prob * (bank_with_all_current_bets_on_street + potential_bet)) - (potential_bet * (1 - win_prob));

			if (ev > max_ev) {
				max_ev = ev;
				bets.push_back(potential_bet);
			}

			if (begin_raise_bet) {
				potential_bet += table_last_bet_diff;
				begin_raise_bet = false;
			}
			else {
				potential_bet += current_big_blind;
			}
			
		} while (potential_bet <= max_bet);

		std::size_t new_bet = 0;

		if (!bets.empty()) {
			std::uniform_int_distribution<> random_bet_index(0, bets.size() - 1);
			new_bet = bets[random_bet_index(rng)];
		}

		if (bet_for_call == 0) {
			if (new_bet > 0) {
				return { Player_action::Raise, new_bet + table_current_bet };
			}
			else {
				return { Player_action::Check, 0 };
			}
		}
		else {
			if (new_bet > bet_for_call) {
				return { Player_action::Raise, table_current_bet + (new_bet - bet_for_call) };
			}
			else if (new_bet == bet_for_call) {
				return { Player_action::Call, table_current_bet };
			}
			else {
				return { Player_action::Fold, 0 };
			}
		}
	}
};