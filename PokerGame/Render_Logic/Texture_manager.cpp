#include "Texture_manager.h"

Texture_manager::Texture_manager() noexcept {
    card_textures_pool.resize(Card::COUNT_OF_ALL_CARDS + 3);

    for (std::uint8_t i = 0; i < Card::COUNT_OF_ALL_CARDS; ++i) {
        std::string path = "Images/Card_" + std::to_string(i) + ".png";
        card_textures_pool[i].load(path);
    }

    std::string path = "Images/Back.png";
    card_textures_pool[Card::COUNT_OF_ALL_CARDS].load(path);

    path = "Images/Dealer_button.png";
    card_textures_pool[Card::COUNT_OF_ALL_CARDS + 1].load(path);

    path = "Images/Current_player_arrow.png";
    card_textures_pool[Card::COUNT_OF_ALL_CARDS + 2].load(path);
}

const Texture_manager& Texture_manager::get_instance() noexcept {
	static Texture_manager manager;
	return manager;
}

const tgui::Texture& Texture_manager::get_card_texture(std::uint8_t index) const noexcept {
	return card_textures_pool[index];
}

const tgui::Texture& Texture_manager::get_back_card_texture() const noexcept {
    return card_textures_pool[Card::COUNT_OF_ALL_CARDS];
}

const tgui::Texture& Texture_manager::get_dealer_button_texture() const noexcept {
    return card_textures_pool[Card::COUNT_OF_ALL_CARDS + 1];
}

const tgui::Texture& Texture_manager::get_current_player_arrow_texture() const noexcept {
    return card_textures_pool[Card::COUNT_OF_ALL_CARDS + 2];
}