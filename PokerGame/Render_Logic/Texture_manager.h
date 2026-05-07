#pragma once
#include "../Game_Logic/Logger.h"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/AllWidgets.hpp>

class Texture_manager final {
private:
	std::vector<tgui::Texture> card_textures_pool;

	Texture_manager() noexcept;

public:
	static const Texture_manager& get_instance() noexcept;

	const tgui::Texture& get_card_texture(std::uint8_t index) const noexcept;
	const tgui::Texture& get_back_card_texture() const noexcept;

	const tgui::Texture& get_dealer_button_texture() const noexcept;
	const tgui::Texture& get_current_player_arrow_texture() const noexcept;
};