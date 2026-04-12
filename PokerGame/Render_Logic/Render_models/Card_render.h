#pragma once
#include "../Texture_manager.h"

class Card_render final {
private:
	std::shared_ptr<tgui::Picture> picture = nullptr;

public:
	Card_render(tgui::Gui& gui) noexcept;

	void set_position(std::pair<std::uint16_t, std::uint16_t> coords) noexcept;
	void set_visible(bool flag) noexcept;
	void set_new_card(Card c) noexcept;
	void set_back_card() noexcept;
};