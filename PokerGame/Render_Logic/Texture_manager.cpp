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

std::shared_ptr<tgui::Label> Texture_manager::make_label(
    tgui::Gui& gui,
    std::pair<std::uint16_t,
    std::uint16_t> coords,
    std::uint8_t text_size,
    tgui::Color text_color,
    const std::string& text
) noexcept {
    std::shared_ptr<tgui::Label> label = tgui::Label::create();
    label->setPosition({ coords.first, coords.second });
    label->setTextSize(text_size);
    label->getRenderer()->setTextColor(text_color);
    label->setText(text);

    gui.add(label);

    return label;
}

std::shared_ptr<tgui::Button> Texture_manager::make_button(
    tgui::Gui& gui,
    std::pair<std::uint16_t, std::uint16_t> coords,
    std::uint8_t text_size,
    const std::string& text,
    std::function<void()> func
) noexcept
{
    std::shared_ptr<tgui::Button> button = tgui::Button::create();
    button->setPosition({ coords.first, coords.second });
    button->setTextSize(text_size);
    button->setText(text);
    button->onPress(func);

    gui.add(button);

    return button;
}

std::shared_ptr<tgui::RadioButton> Texture_manager::make_radio_button(
    std::pair<std::uint16_t, std::uint16_t> coords,
    std::uint8_t text_size,
    tgui::Color text_color,
    const std::string& text,
    std::function<void()> func
) noexcept
{
    std::shared_ptr<tgui::RadioButton> radio_button = tgui::RadioButton::create();
    radio_button->setPosition({ coords.first, coords.second });
    radio_button->setTextSize(text_size);
    radio_button->getRenderer()->setTextColor(text_color);
    radio_button->setText(text);
    radio_button->onCheck(func);

    return radio_button;
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