#include "Card_render.h"

Card_render::Card_render(tgui::Gui& gui) noexcept : gui(gui) {
	picture = tgui::Picture::create();
	picture->setScale(0.5);

	set_back_card();
	set_visible(false);

	gui.add(picture);
}

void Card_render::set_position(std::pair<std::uint16_t, std::uint16_t> coords) noexcept {
	picture->setPosition({coords.first, coords.second});
}

void Card_render::set_visible(bool flag) noexcept {
	picture->setVisible(flag);
}

void Card_render::set_new_card(Card c) noexcept {
	picture->getRenderer()->setTexture(Texture_manager::get_instance().get_card_texture(c.get_card_index()));
}

void Card_render::set_back_card() noexcept {
	picture->getRenderer()->setTexture(Texture_manager::get_instance().get_back_card_texture());
}

void Card_render::remove_from_gui() noexcept {
	gui.remove(picture);
}