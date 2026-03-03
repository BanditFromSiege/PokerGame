#include "Pot.h"

Pot::Pot() noexcept = default;

Pot::Pot(std::vector<std::uint8_t> vec, std::size_t bank) noexcept
    : players_id_in_pot(std::move(vec)), bank(bank) {}

bool Pot::player_is_in_pot(const Player& p) const noexcept {
    auto it = std::find_if(
        players_id_in_pot.begin(),
        players_id_in_pot.end(),
        [&p](auto id) {
            return id == p.get_id();
        }
    );

    return it != players_id_in_pot.end();
}

const std::vector<std::uint8_t>& Pot::get_players_id() const noexcept {
    return players_id_in_pot;
}

std::size_t Pot::get_bank() const noexcept {
    return bank;
}

void Pot::set_players_id_in_pot(std::vector<std::uint8_t> vec) noexcept {
    players_id_in_pot = std::move(vec);
}

void Pot::set_bank(std::size_t b) noexcept {
    bank = b;
}