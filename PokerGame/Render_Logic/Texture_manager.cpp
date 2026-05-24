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

    path = "Images/Poker_icon.png";
    poker_icon.loadFromFile(path);
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

std::shared_ptr<tgui::Slider> Texture_manager::make_slider(
    tgui::Gui& gui,
    std::pair<std::uint16_t, std::uint16_t> coords,
    std::pair<float, float> size,
    std::pair<float, float> min_max,
    float step,
    float value,
    std::function<void(float)> func
) noexcept
{
    std::shared_ptr<tgui::Slider> slider = tgui::Slider::create();
    slider->setPosition({ coords.first, coords.second });
    slider->setSize({ size.first, size.second });
    slider->setMinimum(min_max.first);
    slider->setMaximum(min_max.second);
    slider->setStep(step);
    slider->setValue(value);
    slider->onValueChange(func);

    gui.add(slider);

    return slider;
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

const sf::Image& Texture_manager::get_poker_icon() const noexcept {
    return poker_icon;
}