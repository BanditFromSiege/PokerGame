#include "Settings_stage.h"

std::shared_ptr<tgui::Label> Settings_stage::make_label(
	std::pair<std::uint16_t, std::uint16_t> coords,
	std::uint8_t text_size,
	tgui::Color text_color,
	const std::string& text
) noexcept
{
	std::shared_ptr<tgui::Label> label = tgui::Label::create();
	label->setPosition({ coords.first, coords.second });
	label->setTextSize(text_size);
	label->getRenderer()->setTextColor(text_color);
	label->setText(text);

	gui.add(label);

	return label;
}

std::shared_ptr<tgui::Button> Settings_stage::make_button(
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

std::shared_ptr<tgui::RadioButton> Settings_stage::make_radio_button(
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

Settings_stage::Settings_stage(
	tgui::Gui& gui,
	Render_stages& stage,
	Render_color& color,
	bool& execution_mode_sequenced
) noexcept
	: gui(gui)
	, current_stage(stage)
	, current_color(color)
	, current_execution_mode_sequenced(execution_mode_sequenced)
{
	settings_label
		= make_label({ 100, 100 }, 76, tgui::Color::White, "Settings");

	choice_execution_mode_label
		= make_label({ 100, 300 }, 48, tgui::Color::White, "Select execution mode (used only for AI calculations)");

	execution_mode_group = tgui::RadioButtonGroup::create();
	gui.add(execution_mode_group);

	sequenced_radio_button = make_radio_button({ 100, 400 }, 36, tgui::Color::White, "Sequenced mode",
		[this]() {
			current_execution_mode_sequenced = true;
		}
	);

	parallel_radio_button = make_radio_button({ 100, 450 }, 36, tgui::Color::White, "Parallel mode",
		[this]() {
			current_execution_mode_sequenced = false;
		}
	);

	execution_mode_group->add(sequenced_radio_button);
	execution_mode_group->add(parallel_radio_button);

	if (current_execution_mode_sequenced) {
		sequenced_radio_button->setChecked(true);
	}
	else {
		parallel_radio_button->setChecked(true);
	}

	choice_background_color_label
		= make_label({ 100, 550 }, 48, tgui::Color::White, "Select background color");

	colors_group = tgui::RadioButtonGroup::create();
	gui.add(colors_group);

	red_radio_button = make_radio_button({ 100, 650 }, 36, tgui::Color::White, "Red color",
		[this]() {
			current_color = Render_color::Red;
		}
	);

	green_radio_button = make_radio_button({ 100, 700 }, 36, tgui::Color::White, "Green color",
		[this]() {
			current_color = Render_color::Green;
		}
	);

	blue_radio_button = make_radio_button({ 100, 750 }, 36, tgui::Color::White, "Blue color",
		[this]() {
			current_color = Render_color::Blue;
		}
	);

	purple_radio_button = make_radio_button({ 100, 800 }, 36, tgui::Color::White, "Purple color",
		[this]() {
			current_color = Render_color::Purple;
		}
	);

	colors_group->add(red_radio_button);
	colors_group->add(green_radio_button);
	colors_group->add(blue_radio_button);
	colors_group->add(purple_radio_button);

	if (current_color == Render_color::Red) {
		red_radio_button->setChecked(true);
	}
	else if (current_color == Render_color::Green) {
		green_radio_button->setChecked(true);
	}
	else if (current_color == Render_color::Blue) {
		blue_radio_button->setChecked(true);
	}
	else if (current_color == Render_color::Purple) {
		purple_radio_button->setChecked(true);
	}

	back_button = make_button({ 100, 900 }, 48, "Return to menu",
		[this]() {
			current_stage = Render_stages::Menu;
		}
	);
}

void Settings_stage::set_visible(bool flag) noexcept {
	settings_label->setVisible(flag);

	choice_execution_mode_label->setVisible(flag);
	execution_mode_group->setVisible(flag);
	sequenced_radio_button->setVisible(flag);
	parallel_radio_button->setVisible(flag);

	choice_background_color_label->setVisible(flag);
	colors_group->setVisible(flag);
	red_radio_button->setVisible(flag);
	green_radio_button->setVisible(flag);
	blue_radio_button->setVisible(flag);
	purple_radio_button->setVisible(flag);

	back_button->setVisible(flag);
}