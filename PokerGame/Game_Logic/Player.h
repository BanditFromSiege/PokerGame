#pragma once
#include "Enums.h"
#include "../Card_Logic/Probability_evaluator.h"

class Player final {
private:
	std::string name;
	std::array<Card, Card::COUNT_OF_CARDS_IN_HAND> cards = {};

	std::size_t money = 0;
	std::size_t current_bet = 0;
	std::size_t sum_of_bets = 0;
	std::size_t current_big_blind = 0;

	std::uint8_t id = 0;

	Player_status status = Player_status::Active;

public:
	inline static std::size_t count_of_big_blinds = 50;

	Player() noexcept;
	Player(const char* name, std::uint8_t id, std::size_t money);

	std::string get_name() const noexcept;
	std::array<Card, Card::COUNT_OF_CARDS_IN_HAND> get_cards() const noexcept;
	std::pair<Card, Card> get_pair_of_cards() const noexcept;

	std::size_t get_money() const noexcept;
	std::size_t get_current_bet() const noexcept;

	std::size_t get_sum_of_bets() const noexcept;

	Player_status get_player_status() const noexcept;
	std::uint8_t get_id() const noexcept;
	std::size_t get_current_big_blind() const noexcept;

	bool is_active() const noexcept;
	bool is_in_game() const noexcept;
	bool is_all_in() const noexcept;

	void check_money_enough() noexcept;

	void set_current_bet(std::size_t bet) noexcept;
	void set_sum_of_bets(std::size_t bet) noexcept;
	void set_cards(Card c1, Card c2) noexcept;
	void set_id(std::uint8_t index) noexcept;

	void make_fold() noexcept;
	void get_win(std::size_t share) noexcept;
	void reset_for_new_hand() noexcept;

	std::size_t make_bet_or_check(std::size_t bet) noexcept;

	template <typename T = std::execution::sequenced_policy>
	requires (std::is_same_v<T, std::execution::sequenced_policy> || std::is_same_v<T, std::execution::parallel_policy>)
	std::pair<Player_action, std::size_t> make_decision(
		std::mt19937_64& rng,
		Probability_evaluator<T> evaluator,
		std::span<const Card> table_cards,
		std::uint8_t active_players,
		std::size_t bank_with_all_current_bets_on_street,
		std::size_t table_current_bet
	) noexcept {
		std::size_t bet_for_call = table_current_bet - current_bet;

		double win_prob = evaluator.get_relative_probability(
			this->get_cards(),
			table_cards,
			active_players).value_or(0.0);

		std::uniform_real_distribution<double> noise(-5.0, 5.0);

		win_prob += (noise(rng) / 100.0);

		win_prob = (win_prob < 0.0) ? 0.0 : win_prob;

		std::size_t player_money = money;

		std::array<std::size_t, 6> bets = {
			bet_for_call, 2 * table_current_bet, 3 * table_current_bet,
			4 * table_current_bet, 5 * table_current_bet, player_money / 2
		};

		if (table_current_bet == 0) {
			std::size_t small_blind = current_big_blind / 2;

			bets = {
				table_current_bet, 2 * small_blind, 3 * small_blind,
				4 * small_blind, 5 * small_blind, player_money / 2
			};

			if (bets[5] < bets[4]) {
				bets[5] = 6 * small_blind;
			}
		}

		if (bets[5] < bets[4]) {
			bets[5] = 6 * table_current_bet;
		}

		double pot_odds = 0.0;
		std::size_t new_bet = bet_for_call;

		for (std::uint8_t i = 0; i < bets.size(); ++i) {
			pot_odds
				= static_cast<double>(bets[i]) / static_cast<double>(bank_with_all_current_bets_on_street + bets[i]);

			double pot_odds_with_buffer = (i == 0) ? pot_odds : pot_odds + 0.1;

			if (win_prob < pot_odds_with_buffer) {
				if (i == 0) {
					return{ Player_action::Fold, 0 };
				}
				break;
			}

			new_bet = bets[i];
		}

		if (new_bet == bet_for_call) {
			if (current_bet == table_current_bet) {
				return { Player_action::Check, 0 };
			}

			return { Player_action::Call, table_current_bet };
		}

		return { Player_action::Raise, new_bet };
	}
};