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

	static std::shared_ptr<tgui::Label> make_label(
		tgui::Gui& gui,
		std::pair<std::uint16_t, std::uint16_t> coords,
		std::uint8_t text_size,
		tgui::Color text_color,
		const std::string& text
	) noexcept;

	static std::shared_ptr<tgui::Button> make_button(
		tgui::Gui& gui,
		std::pair<std::uint16_t, std::uint16_t> coords,
		std::uint8_t text_size,
		const std::string& text,
		std::function<void()> func
	) noexcept;

	static std::shared_ptr<tgui::RadioButton> make_radio_button(
		std::pair<std::uint16_t, std::uint16_t> coords,
		std::uint8_t text_size,
		tgui::Color text_color,
		const std::string& text,
		std::function<void()> func
	) noexcept;

	const tgui::Texture& get_card_texture(std::uint8_t index) const noexcept;
	const tgui::Texture& get_back_card_texture() const noexcept;

	const tgui::Texture& get_dealer_button_texture() const noexcept;
	const tgui::Texture& get_current_player_arrow_texture() const noexcept;
};