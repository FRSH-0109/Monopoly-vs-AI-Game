/**
 * @file Withdraw.cc
 *
 * @brief Source file for trade/withdraw mehanism
 * in monopoly game between players
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "Withdraw.h"

// =============================================================================
// Withdraw Class Implementation
// =============================================================================

Withdraw::Withdraw() {
	player_1_to_with_draw_ = nullptr;
	player_2_to_with_draw_ = nullptr;
}

void Withdraw::setChooseScreenVisible(bool is_visible) {
	if (is_visible) {
		choose_player_text_->setFillColor(sf::Color::Black);
	} else {
		choose_player_text_->setFillColor(sf::Color::Transparent);
	}
	choose_player_1_button_->setIsVisible(is_visible);
	choose_player_2_button_->setIsVisible(is_visible);
	choose_player_3_button_->setIsVisible(is_visible);
	choose_player_4_button_->setIsVisible(is_visible);
	resign_button_->setIsVisible(is_visible);
}

std::vector<unsigned int> Withdraw::getPlayer1IndexProperties() {
	return player_1_index_properties_;
}

std::vector<unsigned int> Withdraw::getPlayer2IndexProperties() {
	return player_2_index_properties_;
}

void Withdraw::setPlayer1IndexProperties(std::vector<unsigned int> new_index_properties) {
	player_1_index_properties_ = new_index_properties;
}

void Withdraw::setPlayer2IndexProperties(std::vector<unsigned int> new_index_properties) {
	player_2_index_properties_ = new_index_properties;
}

void Withdraw::setValueScreenVisible(bool is_visible) {
	sf::Color color = sf::Color::Black;
	if (!is_visible) {
		color = sf::Color::Transparent;
	}

	player_1_text_->setFillColor(color);
	player_1_money_->setFillColor(color);
	player_2_text_->setFillColor(color);
	player_2_money_->setFillColor(color);
	player_1_index_text_->setFillColor(color);
	player_1_index_money_->setFillColor(color);
	player_2_index_text_->setFillColor(color);
	player_2_index_money_->setFillColor(color);

	resign_value_button_->setIsVisible(is_visible);
	submit_value_button_->setIsVisible(is_visible);
	minus_1_player_1_button_->setIsVisible(is_visible);
	minus_10_player_1_button_->setIsVisible(is_visible);
	minus_100_player_1_button_->setIsVisible(is_visible);
	plus_1_player_1_button_->setIsVisible(is_visible);
	plus_10_player_1_button_->setIsVisible(is_visible);
	plus_100_player_1_button_->setIsVisible(is_visible);
	minus_1_player_2_button_->setIsVisible(is_visible);
	minus_10_player_2_button_->setIsVisible(is_visible);
	minus_100_player_2_button_->setIsVisible(is_visible);
	plus_1_player_2_button_->setIsVisible(is_visible);
	plus_10_player_2_button_->setIsVisible(is_visible);
	plus_100_player_2_button_->setIsVisible(is_visible);
	next_property_player_1_button_->setIsVisible(is_visible);
	previous_propertyplayer_1_button_->setIsVisible(is_visible);
	next_property_player_2_button_->setIsVisible(is_visible);
	previous_propertyplayer_2_button_->setIsVisible(is_visible);
	next_property_player_1_index_button_->setIsVisible(is_visible);
	previous_property_player_1_index_button_->setIsVisible(is_visible);
	next_property_player_2_index_button_->setIsVisible(is_visible);
	previousPropertyPlayer2IndexButton_->setIsVisible(is_visible);
	add_property_player_1_button_->setIsVisible(is_visible);
	remove_property_player_1_button_->setIsVisible(is_visible);
	add_property_player_2_button_->setIsVisible(is_visible);
	remove_property_player_2_button_->setIsVisible(is_visible);

	if (is_visible) {
		player_1_money_buffer_ = player_1_to_with_draw_->getMoney();
		player_2_money_buffer_ = player_2_to_with_draw_->getMoney();
		player_1_money_index_buffer_ = 0;
		player_2_money_index_buffer_ = 0;
		player1Properties_.clear();
		player_2_properties_.clear();
		player_1_index_properties_.clear();
		player_2_index_properties_.clear();
	}

	if (is_visible) {
		player_1_text_->setString("Gracz " + std::to_string(player_1_to_with_draw_->getId() + 1));
		player_1_money_->setString("Kasa: " + std::to_string(player_1_money_buffer_));

		for (auto prop : player_1_to_with_draw_->getFieldOwnedId()) {
			player1Properties_.push_back(prop);
		}

		if (player1Properties_.size() != 0) {
			current_property_player_1_showed_ = player1Properties_[0];
		}

		player_2_text_->setString("Gracz " + std::to_string(player_2_to_with_draw_->getId() + 1));
		player_2_money_->setString("Kasa: " + std::to_string(player_2_money_buffer_));

		for (auto prop : player_2_to_with_draw_->getFieldOwnedId()) {
			player_2_properties_.push_back(prop);
		}

		if (player_2_properties_.size() != 0) {
			current_property_player_2_showed_ = player_2_properties_[0];
		}

		player_1_index_text_->setString(player_1_text_->getString() + ": Oferta");
		player_1_index_money_->setString("Kasa: " + std::to_string(player_1_money_index_buffer_));
		current_property_player_1_index_showed_ = 0;

		player_2_index_text_->setString(player_2_text_->getString() + ": Oferta");
		player_2_index_money_->setString("Kasa: " + std::to_string(player_2_money_index_buffer_));
		current_property_player_2_index_showed_ = 0;

		showProperty(1);
		showProperty(2);
		showProperty(3);
		showProperty(4);
	}
}

void Withdraw::setDecisionScreenVisible(bool is_visible) {
	resign_decision_button_->setIsVisible(is_visible);
	accpet_decision_button_->setIsVisible(is_visible);

	sf::Color color = sf::Color::Black;
	if (!is_visible) {
		color = sf::Color::Transparent;
	}

	player_info_text_->setFillColor(color);
	player_info_money_->setFillColor(color);
	player_info_get_properties_->setFillColor(color);
	player_info_lose_properties_->setFillColor(color);

	if (is_visible) {
		player_info_text_->setString("Decyzja Gracza " + std::to_string(player_2_to_with_draw_->getId() + 1) + ": ");
		player_info_money_->setString("Transakcja kasy: " + std::to_string((int)player_1_money_index_buffer_ -
																		   (int)player_2_money_index_buffer_));
	}
	player_info_get_properties_->setString(L"Zdobyte nieruchomości: ");
	bool addComa = false;
	if (player_1_index_properties_.size() != 0) {
		for (auto property : player_1_index_properties_) {
			std::string name =
				std::visit([](Field& field) { return field.getName(); }, gameboard_->getFieldById(property));
			if (addComa) {
				player_info_get_properties_->setString(player_info_get_properties_->getString() + ", ");
			}
			player_info_get_properties_->setString(player_info_get_properties_->getString() + name);
			addComa = true;
		}
	} else {
		player_info_get_properties_->setString(player_info_get_properties_->getString() + "Brak");
	}

	player_info_lose_properties_->setString(L"Stracone nieruchomości: ");
	addComa = false;
	if (player_2_index_properties_.size() != 0) {
		for (auto property : player_2_index_properties_) {
			std::string name =
				std::visit([](Field& field) { return field.getName(); }, gameboard_->getFieldById(property));
			if (addComa) {
				player_info_lose_properties_->setString(player_info_lose_properties_->getString() + ", ");
			}
			player_info_lose_properties_->setString(player_info_lose_properties_->getString() + name);
			addComa = true;
		}
	} else {
		player_info_lose_properties_->setString(player_info_lose_properties_->getString() + "Brak");
	}

	player_info_text_->setOrigin(
		player_info_text_->getGlobalBounds().getSize() / 2.f + player_info_text_->getLocalBounds().getPosition());

	player_info_money_->setOrigin(
		player_info_money_->getGlobalBounds().getSize() / 2.f + player_info_money_->getLocalBounds().getPosition());

	player_info_get_properties_->setOrigin(player_info_get_properties_->getGlobalBounds().getSize() / 2.f +
										   player_info_get_properties_->getLocalBounds().getPosition());

	player_info_lose_properties_->setOrigin(player_info_lose_properties_->getGlobalBounds().getSize() / 2.f +
											player_info_lose_properties_->getLocalBounds().getPosition());
}

void Withdraw::createDecisionPlayerScreen() {
	std::shared_ptr<Button> buttonResign = createDefaultButton("Rezygnuj", 120, 50);
	buttonResign->setPosition(RESIGN_DECISION_BUTTON_POSITION);
	resign_decision_button_ = buttonResign;
	addButton(buttonResign);

	std::shared_ptr<Button> buttonAccpet = createDefaultButton("Akceptuj", 120, 50);
	buttonAccpet->setPosition(ACCEPT_DECISION_BUTTON_POSITION);
	accpet_decision_button_ = buttonAccpet;
	addButton(buttonAccpet);

	int ystep = 50;

	std::shared_ptr<sf::Text> playerInfoText(new sf::Text("", getFont(), getFontSize() - 2));
	playerInfoText->setPosition(sf::Vector2f(DECISION_TEXT_POSITION.x, DECISION_TEXT_POSITION.y + ystep * 0));
	playerInfoText->setFillColor(sf::Color::Transparent);
	player_info_text_ = playerInfoText;
	addText(playerInfoText);

	std::shared_ptr<sf::Text> playerInfoMoney(new sf::Text("", getFont(), getFontSize() - 2));
	playerInfoMoney->setPosition(sf::Vector2f(DECISION_TEXT_POSITION.x, DECISION_TEXT_POSITION.y + ystep * 1));
	playerInfoMoney->setFillColor(sf::Color::Transparent);
	playerInfoMoney->setOrigin(
		playerInfoMoney->getGlobalBounds().getSize() / 2.f + playerInfoMoney->getLocalBounds().getPosition());
	player_info_money_ = playerInfoMoney;
	addText(playerInfoMoney);

	std::shared_ptr<sf::Text> playerInfoGetProperties(new sf::Text("", getFont(), getFontSize() - 2));
	playerInfoGetProperties->setPosition(sf::Vector2f(DECISION_TEXT_POSITION.x, DECISION_TEXT_POSITION.y + ystep * 2));
	playerInfoGetProperties->setFillColor(sf::Color::Transparent);
	player_info_get_properties_ = playerInfoGetProperties;
	addText(playerInfoGetProperties);

	std::shared_ptr<sf::Text> playerInfoLoseProperties(new sf::Text("", getFont(), getFontSize() - 2));
	playerInfoLoseProperties->setPosition(sf::Vector2f(DECISION_TEXT_POSITION.x, DECISION_TEXT_POSITION.y + ystep * 3));
	playerInfoLoseProperties->setFillColor(sf::Color::Transparent);
	player_info_lose_properties_ = playerInfoLoseProperties;
	addText(playerInfoLoseProperties);
}

void Withdraw::createValuePlayerScreen() {
	std::shared_ptr<Button> buttonResign = createDefaultButton("Rezygnuj", 120, 50);
	buttonResign->setPosition(RESIGN_VALUE_BUTTON_POSITION);
	resign_value_button_ = buttonResign;
	addButton(buttonResign);

	std::shared_ptr<Button> buttonSubmit = createDefaultButton(L"Zakończ", 120, 50);
	buttonSubmit->setPosition(sf::Vector2f(RESIGN_VALUE_BUTTON_POSITION.x + 200, RESIGN_VALUE_BUTTON_POSITION.y));
	submit_value_button_ = buttonSubmit;
	addButton(buttonSubmit);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	std::shared_ptr<sf::Text> choosePlayer1NameText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlayer1NameText->setPosition(COLUMN_1_POSITION);
	choosePlayer1NameText->setFillColor(sf::Color::Transparent);
	choosePlayer1NameText->setOrigin(choosePlayer1NameText->getGlobalBounds().getSize() / 2.f +
									 choosePlayer1NameText->getLocalBounds().getPosition());
	player_1_text_ = choosePlayer1NameText;
	addText(choosePlayer1NameText);

	std::shared_ptr<sf::Text> choosePlaye1MoneyText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlaye1MoneyText->setPosition(sf::Vector2f(COLUMN_1_POSITION.x, COLUMN_1_POSITION.y + 30));
	choosePlaye1MoneyText->setFillColor(sf::Color::Transparent);
	choosePlaye1MoneyText->setOrigin(choosePlaye1MoneyText->getGlobalBounds().getSize() / 2.f +
									 choosePlaye1MoneyText->getLocalBounds().getPosition());
	player_1_money_ = choosePlaye1MoneyText;
	addText(choosePlaye1MoneyText);

	player1Properties_.clear();

	///////////////////////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<sf::Text> choosePlayer2NameText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlayer2NameText->setPosition(COLUMN_4_POSITION);
	choosePlayer2NameText->setFillColor(sf::Color::Transparent);
	choosePlayer2NameText->setOrigin(choosePlayer2NameText->getGlobalBounds().getSize() / 2.f +
									 choosePlayer2NameText->getLocalBounds().getPosition());
	player_2_text_ = choosePlayer2NameText;
	addText(choosePlayer2NameText);

	std::shared_ptr<sf::Text> choosePlaye2MoneyText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlaye2MoneyText->setPosition(sf::Vector2f(COLUMN_4_POSITION.x, COLUMN_4_POSITION.y + 30));
	choosePlaye2MoneyText->setFillColor(sf::Color::Transparent);
	choosePlaye2MoneyText->setOrigin(choosePlaye2MoneyText->getGlobalBounds().getSize() / 2.f +
									 choosePlaye2MoneyText->getLocalBounds().getPosition());
	player_2_money_ = choosePlaye2MoneyText;
	addText(choosePlaye2MoneyText);

	player_2_properties_.clear();

	///////////////////////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<sf::Text> choosePlayer1NameIndexText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlayer1NameIndexText->setPosition(COLUMN_2_POSITION);
	choosePlayer1NameIndexText->setFillColor(sf::Color::Transparent);
	choosePlayer1NameIndexText->setOrigin(choosePlayer1NameIndexText->getGlobalBounds().getSize() / 2.f +
										  choosePlayer1NameIndexText->getLocalBounds().getPosition());
	player_1_index_text_ = choosePlayer1NameIndexText;
	addText(choosePlayer1NameIndexText);

	std::shared_ptr<sf::Text> choosePlaye1MoneyIndexText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlaye1MoneyIndexText->setPosition(sf::Vector2f(COLUMN_2_POSITION.x, COLUMN_2_POSITION.y + 30));
	choosePlaye1MoneyIndexText->setFillColor(sf::Color::Transparent);
	choosePlaye1MoneyIndexText->setOrigin(choosePlaye1MoneyIndexText->getGlobalBounds().getSize() / 2.f +
										  choosePlaye1MoneyIndexText->getLocalBounds().getPosition());
	player_1_index_money_ = choosePlaye1MoneyIndexText;
	addText(choosePlaye1MoneyIndexText);

	player_1_index_properties_.clear();

	std::shared_ptr<sf::Text> choosePlayer2NameIndexText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlayer2NameIndexText->setPosition(COLUMN_3_POSITION);
	choosePlayer2NameIndexText->setFillColor(sf::Color::Transparent);
	choosePlayer2NameIndexText->setOrigin(choosePlayer2NameIndexText->getGlobalBounds().getSize() / 2.f +
										  choosePlayer2NameIndexText->getLocalBounds().getPosition());
	player_2_index_text_ = choosePlayer2NameIndexText;
	addText(choosePlayer2NameIndexText);

	std::shared_ptr<sf::Text> choosePlaye2MoneyIndexText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlaye2MoneyIndexText->setPosition(sf::Vector2f(COLUMN_3_POSITION.x, COLUMN_3_POSITION.y + 30));
	choosePlaye2MoneyIndexText->setFillColor(sf::Color::Transparent);
	choosePlaye2MoneyIndexText->setOrigin(choosePlaye2MoneyIndexText->getGlobalBounds().getSize() / 2.f +
										  choosePlaye2MoneyIndexText->getLocalBounds().getPosition());
	player_2_index_money_ = choosePlaye2MoneyIndexText;
	addText(choosePlaye2MoneyIndexText);

	player_2_index_properties_.clear();

	///////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<Button> buttonPlayer1minus1 = createDefaultButton("-1", 60, 50);
	buttonPlayer1minus1->setPosition(sf::Vector2f(COLUMN_1_POSITION.x, COLUMN_1_POSITION.y + 100));
	minus_1_player_1_button_ = buttonPlayer1minus1;
	addButton(buttonPlayer1minus1);

	std::shared_ptr<Button> buttonPlayer1minus10 = createDefaultButton("-10", 60, 50);
	buttonPlayer1minus10->setPosition(sf::Vector2f(COLUMN_1_POSITION.x + 70, COLUMN_1_POSITION.y + 100));
	minus_10_player_1_button_ = buttonPlayer1minus10;
	addButton(buttonPlayer1minus10);

	std::shared_ptr<Button> buttonPlayer1minus100 = createDefaultButton("-100", 60, 50);
	buttonPlayer1minus100->setPosition(sf::Vector2f(COLUMN_1_POSITION.x + 140, COLUMN_1_POSITION.y + 100));
	minus_100_player_1_button_ = buttonPlayer1minus100;
	addButton(buttonPlayer1minus100);

	std::shared_ptr<Button> buttonPlayer1plus1 = createDefaultButton("+1", 60, 50);
	buttonPlayer1plus1->setPosition(sf::Vector2f(COLUMN_1_POSITION.x, COLUMN_1_POSITION.y + 160));
	plus_1_player_1_button_ = buttonPlayer1plus1;
	addButton(buttonPlayer1plus1);

	std::shared_ptr<Button> buttonPlayer1plus10 = createDefaultButton("+10", 60, 50);
	buttonPlayer1plus10->setPosition(sf::Vector2f(COLUMN_1_POSITION.x + 70, COLUMN_1_POSITION.y + 160));
	plus_10_player_1_button_ = buttonPlayer1plus10;
	addButton(buttonPlayer1plus10);

	std::shared_ptr<Button> buttonPlayer1plus100 = createDefaultButton("+100", 60, 50);
	buttonPlayer1plus100->setPosition(sf::Vector2f(COLUMN_1_POSITION.x + 140, COLUMN_1_POSITION.y + 160));
	plus_100_player_1_button_ = buttonPlayer1plus100;
	addButton(buttonPlayer1plus100);

	std::shared_ptr<Button> buttonPlayer2minus1 = createDefaultButton("-1", 60, 50);
	buttonPlayer2minus1->setPosition(sf::Vector2f(COLUMN_4_POSITION.x, COLUMN_4_POSITION.y + 100));
	minus_1_player_2_button_ = buttonPlayer2minus1;
	addButton(buttonPlayer2minus1);

	std::shared_ptr<Button> buttonPlayer2minus10 = createDefaultButton("-10", 60, 50);
	buttonPlayer2minus10->setPosition(sf::Vector2f(COLUMN_4_POSITION.x + 70, COLUMN_4_POSITION.y + 100));
	minus_10_player_2_button_ = buttonPlayer2minus10;
	addButton(buttonPlayer2minus10);

	std::shared_ptr<Button> buttonPlayer2minus100 = createDefaultButton("-100", 60, 50);
	buttonPlayer2minus100->setPosition(sf::Vector2f(COLUMN_4_POSITION.x + 140, COLUMN_4_POSITION.y + 100));
	minus_100_player_2_button_ = buttonPlayer2minus100;
	addButton(buttonPlayer2minus100);

	std::shared_ptr<Button> buttonPlayer2plus1 = createDefaultButton("+1", 60, 50);
	buttonPlayer2plus1->setPosition(sf::Vector2f(COLUMN_4_POSITION.x, COLUMN_4_POSITION.y + 160));
	plus_1_player_2_button_ = buttonPlayer2plus1;
	addButton(buttonPlayer2plus1);

	std::shared_ptr<Button> buttonPlayer2plus10 = createDefaultButton("+10", 60, 50);
	buttonPlayer2plus10->setPosition(sf::Vector2f(COLUMN_4_POSITION.x + 70, COLUMN_4_POSITION.y + 160));
	plus_10_player_2_button_ = buttonPlayer2plus10;
	addButton(buttonPlayer2plus10);

	std::shared_ptr<Button> buttonPlayer2plus100 = createDefaultButton("+100", 60, 50);
	buttonPlayer2plus100->setPosition(sf::Vector2f(COLUMN_4_POSITION.x + 140, COLUMN_4_POSITION.y + 160));
	plus_100_player_2_button_ = buttonPlayer2plus100;
	addButton(buttonPlayer2plus100);

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<Button> buttonNext = createDefaultButton(L"Następny", 120, 50);
	buttonNext->setPosition(NEXT_PROPERTY_PLAYER1_BUTTON_POSITION);
	next_property_player_1_button_ = buttonNext;
	addButton(buttonNext);

	std::shared_ptr<Button> buttonPrev = createDefaultButton("Poprzedni", 120, 50);
	buttonPrev->setPosition(PREVIOUS_PROPERTY_PLAYER1_BUTTON_POSITION);
	previous_propertyplayer_1_button_ = buttonPrev;
	addButton(buttonPrev);

	std::shared_ptr<Button> buttonNext2 = createDefaultButton(L"Następny", 120, 50);
	buttonNext2->setPosition(NEXT_PROPERTY_PLAYER2_BUTTON_POSITION);
	next_property_player_2_button_ = buttonNext2;
	addButton(buttonNext2);

	std::shared_ptr<Button> buttonPrev2 = createDefaultButton("Poprzedni", 120, 50);
	buttonPrev2->setPosition(PREVIOUS_PROPERTY_PLAYER2_BUTTON_POSITION);
	previous_propertyplayer_2_button_ = buttonPrev2;
	addButton(buttonPrev2);

	std::shared_ptr<Button> buttonNextIndex = createDefaultButton(L"Następny", 120, 50);
	buttonNextIndex->setPosition(NEXT_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION);
	next_property_player_1_index_button_ = buttonNextIndex;
	addButton(buttonNextIndex);

	std::shared_ptr<Button> buttonPrevIndex = createDefaultButton("Poprzedni", 120, 50);
	buttonPrevIndex->setPosition(PREVIOUS_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION);
	previous_property_player_1_index_button_ = buttonPrevIndex;
	addButton(buttonPrevIndex);

	std::shared_ptr<Button> buttonNextIndex2 = createDefaultButton(L"Następny", 120, 50);
	buttonNextIndex2->setPosition(NEXT_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION);
	next_property_player_2_index_button_ = buttonNextIndex2;
	addButton(buttonNextIndex2);

	std::shared_ptr<Button> buttonPrevIndex2 = createDefaultButton("Poprzedni", 120, 50);
	buttonPrevIndex2->setPosition(PREVIOUS_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION);
	previousPropertyPlayer2IndexButton_ = buttonPrevIndex2;
	addButton(buttonPrevIndex2);

	std::shared_ptr<Button> buttoAdd1 = createDefaultButton(">>>", 120, 50);
	buttoAdd1->setPosition(ADD_PROPERTY_PLAYER1_BUTTON_POSITION);
	add_property_player_1_button_ = buttoAdd1;
	addButton(buttoAdd1);

	std::shared_ptr<Button> buttonRemove1 = createDefaultButton("<<<", 120, 50);
	buttonRemove1->setPosition(REMOVE_PROPERTY_PLAYER1_BUTTON_POSITION);
	remove_property_player_1_button_ = buttonRemove1;
	addButton(buttonRemove1);

	std::shared_ptr<Button> buttoAdd2 = createDefaultButton("<<<", 120, 50);
	buttoAdd2->setPosition(ADD_PROPERTY_PLAYER2_BUTTON_POSITION);
	add_property_player_2_button_ = buttoAdd2;
	addButton(buttoAdd2);

	std::shared_ptr<Button> buttonRemove2 = createDefaultButton(">>>", 120, 50);
	buttonRemove2->setPosition(REMOVE_PROPERTY_PLAYER2_BUTTON_POSITION);
	remove_property_player_2_button_ = buttonRemove2;
	addButton(buttonRemove2);
}

std::shared_ptr<Button> Withdraw::createDefaultButton(sf::String text, unsigned int width, unsigned int height) {
	sf::Vector2f buttonSize = sf::Vector2f(width, height);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonDefault = (std::make_shared<Button>(IDLE, text, buttonSize, getFontSize()));
	buttonDefault->setFont(getFont());
	buttonDefault->setActiveBackColor(activeButtonBackColor);
	buttonDefault->setActiveTextColor(activeButtonTextColor);
	buttonDefault->setInactiveBackColor(inActiveButtonBackColor);
	buttonDefault->setInactiveTextColor(inActiveButtonTextColor);
	buttonDefault->setFocusBackColor(FocusButtonBackColor);
	buttonDefault->setFocusTextColor(FocusButtonTextColor);
	buttonDefault->setIsClicked(false);
	buttonDefault->setIsVisible(false);
	buttonDefault->setIsActive(false);
	buttonDefault->setIsFocus(false);

	return buttonDefault;
}

void Withdraw::createChoosePlayerScreen() {
	std::shared_ptr<sf::Text> choosePlayerText(new sf::Text("Wybierz gracza do wymiany", getFont(), getFontSize() - 2));
	choosePlayerText->setPosition(CHOOSE_PLAYER_TEXT_POSITION);
	choosePlayerText->setFillColor(sf::Color::Transparent);
	choosePlayerText->setOrigin(
		choosePlayerText->getGlobalBounds().getSize() / 2.f + choosePlayerText->getLocalBounds().getPosition());
	choose_player_text_ = choosePlayerText;
	addText(choosePlayerText);

	std::shared_ptr<Button> buttonChoosePlayer1 = createDefaultButton("Gracz 1", 120, 50);
	buttonChoosePlayer1->setPosition(sf::Vector2f(CHOOSE_PLAYER_TEXT_POSITION.x, CHOOSE_PLAYER_TEXT_POSITION.y + 100));
	choose_player_1_button_ = buttonChoosePlayer1;
	addButton(buttonChoosePlayer1);

	std::shared_ptr<Button> buttonChoosePlayer2 = createDefaultButton("Gracz 2", 120, 50);
	buttonChoosePlayer2->setPosition(sf::Vector2f(CHOOSE_PLAYER_TEXT_POSITION.x, CHOOSE_PLAYER_TEXT_POSITION.y + 200));
	choose_player_2_button_ = buttonChoosePlayer2;
	addButton(buttonChoosePlayer2);

	std::shared_ptr<Button> buttonChoosePlayer3 = createDefaultButton("Gracz 3", 120, 50);
	buttonChoosePlayer3->setPosition(sf::Vector2f(CHOOSE_PLAYER_TEXT_POSITION.x, CHOOSE_PLAYER_TEXT_POSITION.y + 300));
	choose_player_3_button_ = buttonChoosePlayer3;
	addButton(buttonChoosePlayer3);

	std::shared_ptr<Button> buttonChoosePlayer4 = createDefaultButton("Gracz 4", 120, 50);
	buttonChoosePlayer4->setPosition(sf::Vector2f(CHOOSE_PLAYER_TEXT_POSITION.x, CHOOSE_PLAYER_TEXT_POSITION.y + 400));
	choose_player_4_button_ = buttonChoosePlayer4;
	addButton(buttonChoosePlayer4);

	std::shared_ptr<Button> buttonResign = createDefaultButton("Rezygnuj", 120, 50);
	buttonResign->setPosition(RESIGN_BUTTON_POSITION);
	resign_button_ = buttonResign;
	addButton(buttonResign);
}

void Withdraw::setBoard(std::shared_ptr<Board> board_ptr) {
	gameboard_ = board_ptr;
}

void Withdraw::setTurnState(TurnState state) {
	turn_state_prev_ = state;
}

TurnState Withdraw::getTurnState() {
	return turn_state_prev_;
}

void Withdraw::setFont(sf::Font font) {
	font_ = font;
}

sf::Font& Withdraw::getFont() {
	return font_;
}

unsigned int Withdraw::getFontSize() const {
	return FONT_SIZE;
}

void Withdraw::addButton(std::shared_ptr<Button> button_tmp) {
	std::shared_ptr<Button> button = button_tmp;
	buttons_.push_back(button);
}

void Withdraw::addText(std::shared_ptr<sf::Text> text_tmp) {
	std::shared_ptr<sf::Text> text = text_tmp;
	texts_.push_back(text);
}

std::vector<std::shared_ptr<Button>>& Withdraw::getButtons() {
	return buttons_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTexts() {
	return texts_;
}

std::shared_ptr<Button> Withdraw::getPlayer1Button() {
	return choose_player_1_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2Button() {
	return choose_player_2_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer3Button() {
	return choose_player_3_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer4Button() {
	return choose_player_4_button_;
}

std::shared_ptr<Button> Withdraw::getResignButton() {
	return resign_button_;
}

void Withdraw::setPlayer1ToWithdraw(std::shared_ptr<Player> player_ptr) {
	player_1_to_with_draw_ = player_ptr;
}

void Withdraw::setPlayer2ToWithdraw(std::shared_ptr<Player> player_ptr) {
	player_2_to_with_draw_ = player_ptr;
}

std::shared_ptr<Button> Withdraw::getResignValueButton() {
	return resign_value_button_;
}

std::shared_ptr<Button> Withdraw::getSubmitValueButton() {
	return submit_value_button_;
}

std::shared_ptr<Player> Withdraw::getPlayer1ToWithdraw() {
	return player_1_to_with_draw_;
}

std::shared_ptr<Player> Withdraw::getPlayer2ToWithdraw() {
	return player_2_to_with_draw_;
}

std::shared_ptr<Button> Withdraw::getPlayer1minus1() {
	return minus_1_player_1_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer1minus10() {
	return minus_10_player_1_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer1minus100() {
	return minus_100_player_1_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer1plus1() {
	return plus_1_player_1_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1plus10() {
	return plus_10_player_1_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1plus100() {
	return plus_100_player_1_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2minus1() {
	return minus_1_player_2_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2minus10() {
	return minus_10_player_2_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2minus100() {
	return minus_100_player_2_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2plus1() {
	return plus_1_player_2_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2plus10() {
	return plus_10_player_2_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2plus100() {
	return plus_100_player_2_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1NextButton() {
	return next_property_player_1_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1PreviousButton() {
	return previous_propertyplayer_1_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2NextButton() {
	return next_property_player_2_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2PreviousButton() {
	return previous_propertyplayer_2_button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1IndexNextButton() {
	return next_property_player_1_index_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer1IndexPreviousButton() {
	return previous_property_player_1_index_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2IndexNextButton() {
	return next_property_player_2_index_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2IndexPreviousButton() {
	return previousPropertyPlayer2IndexButton_;
}

std::shared_ptr<Button> Withdraw::getPlayer1AddButton() {
	return add_property_player_1_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer1RemoveButton() {
	return remove_property_player_1_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2AddButton() {
	return add_property_player_2_button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2RemoveButton() {
	return remove_property_player_2_button_;
}

void Withdraw::moneyTransferIndex(unsigned int player_num, int money) {
	if (player_num == 1) {
		if (money > 0 && money > (int)player_1_money_buffer_) {
			return;
		} else if (money < 0 && (money * -1) > (int)player_1_money_index_buffer_) {
			return;
		}
		player_1_money_buffer_ -= money;
		player_1_money_index_buffer_ += money;
	} else if (player_num == 2) {
		if (money > 0 && money > (int)player_2_money_buffer_) {
			return;
		} else if (money < 0 && (money * -1) > (int)player_2_money_index_buffer_) {
			return;
		}
		player_2_money_buffer_ -= money;
		player_2_money_index_buffer_ += money;
	}
}

void Withdraw::moneyTextUpdate() {
	player_1_money_->setString("Kasa: " + std::to_string(player_1_money_buffer_));
	player_2_money_->setString("Kasa: " + std::to_string(player_2_money_buffer_));
	player_1_index_money_->setString("Kasa: " + std::to_string(player_1_money_index_buffer_));
	player_2_index_money_->setString("Kasa: " + std::to_string(player_2_money_index_buffer_));
}

void Withdraw::showProperty(int column) {
	unsigned int pos = 0;
	std::vector<std::shared_ptr<sf::Text>> propertyPlayerTexts;
	sf::Vector2f PROPERTY_POSITION;
	sf::Sprite propertySprite;
	std::shared_ptr<sf::Texture> propertyTexture = std::make_shared<sf::Texture>();
	bool isEmpty = true;

	auto it = find(player1Properties_.begin(), player1Properties_.end(), current_property_player_1_showed_);
	int index;
	switch (column) {
		case 1:
			it = find(player1Properties_.begin(), player1Properties_.end(), current_property_player_1_showed_);
			index = it - player1Properties_.begin();
			isEmpty = player1Properties_.size() == 0;
			pos = (isEmpty) ? 0 : player1Properties_[index];
			current_property_player_1_showed_ = pos;
			PROPERTY_POSITION = PROPERTY_PLAYER1_POSITION;
			break;

		case 2:
			it = find(player_1_index_properties_.begin(), player_1_index_properties_.end(),
				current_property_player_1_index_showed_);
			index = it - player_1_index_properties_.begin();
			isEmpty = player_1_index_properties_.size() == 0;
			pos = (isEmpty) ? 0 : player_1_index_properties_[index];
			current_property_player_1_index_showed_ = pos;
			PROPERTY_POSITION = PROPERTY_PLAYER1_INDEX_POSITION;
			break;

		case 3:
			it = find(player_2_index_properties_.begin(), player_2_index_properties_.end(),
				current_property_player_2_index_showed_);
			index = it - player_2_index_properties_.begin();
			isEmpty = player_2_index_properties_.size() == 0;
			pos = (isEmpty) ? 0 : player_2_index_properties_[index];
			current_property_player_2_index_showed_ = pos;
			PROPERTY_POSITION = PROPERTY_PLAYER2_INDEX_POSITION;
			break;

		case 4:
			it = find(player_2_properties_.begin(), player_2_properties_.end(), current_property_player_2_showed_);
			index = it - player_2_properties_.begin();
			isEmpty = player_2_properties_.size() == 0;
			pos = (isEmpty) ? 0 : player_2_properties_[index];
			current_property_player_2_showed_ = pos;
			PROPERTY_POSITION = PROPERTY_PLAYER2_POSITION;
			break;

		default:
			return;
	}

	if (!isEmpty) {
		FieldType fieldType = std::visit([](Field& field) { return field.getType(); }, gameboard_->getFieldById(pos));
		unsigned int price = 0;
		unsigned int Mortgage = 0;
		unsigned int housePrice = 0;
		unsigned int hotelPrice = 0;
		unsigned int rents[7] = {0};
		if (fieldType == STREET) {
			StreetField field = std::get<StreetField>(gameboard_->getFieldById(pos));
			price = field.getPrice();
			std::map<StreetTiers, unsigned int> rentsMap = field.getRentValues();
			rents[0] = rentsMap[NO_HOUSES];
			rents[1] = rentsMap[NO_HOUSES] * 2;
			rents[2] = rentsMap[ONE_HOUSE];
			rents[3] = rentsMap[TWO_HOUESES];
			rents[4] = rentsMap[THREE_HOUSES];
			rents[5] = rentsMap[FOUR_HOUSES];
			rents[6] = rentsMap[HOTEL];
			Mortgage = field.getMortgage();
			housePrice = field.getHousePrice();
			hotelPrice = field.getHotelPrice();
		} else if (fieldType == STATION) {
			StationField field = std::get<StationField>(gameboard_->getFieldById(pos));
			price = field.getPrice();
			std::map<StationTiers, unsigned int> rentsMap = field.getRentValues();
			rents[0] = rentsMap[ONE_STATION];
			rents[1] = rentsMap[TWO_STATIONS];
			rents[2] = rentsMap[THREE_STATIONS];
			rents[3] = rentsMap[FOUR_STATIONS];
			Mortgage = field.getMortgage();
		} else	// fieldType == UTILITY
		{
			UtilityField field = std::get<UtilityField>(gameboard_->getFieldById(pos));
			price = field.getPrice();
			std::map<UtilityTiers, unsigned int> rentsMap = field.getRentMultipliers();
			rents[0] = rentsMap[ONE_UTILITY];
			rents[1] = rentsMap[TWO_UTILITIES];
			Mortgage = field.getMortgage();
		}

		unsigned int width = std::visit([](Field& field) { return field.getWidth(); }, gameboard_->getFieldById(pos));
		unsigned int height = std::visit([](Field& field) { return field.getHeight(); }, gameboard_->getFieldById(pos));
		std::string graphic_path =
			std::visit([](Field& field) { return field.getGraphicPath(); }, gameboard_->getFieldById(pos));

		sf::Vector2f dataPos;

		dataPos = PROPERTY_POSITION;						 ////////////////////////////
		if (!propertyTexture->loadFromFile(graphic_path)) {	 ////////////////////////
			propertySprite.setColor(sf::Color::Green);
		}
		propertySprite.setTexture(*propertyTexture, true);
		sf::Vector2u texture_dim = propertyTexture->getSize();
		float scale_x = (float)width / (float)texture_dim.x;
		float scale_y = (float)height / (float)texture_dim.y;
		const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
		propertySprite.setScale(SCALE_VECT * PROPERTY_SHOW_SCALE);
		propertySprite.setPosition(dataPos.x, dataPos.y);
		propertySprite.setRotation(0);

		const std::string streetName =
			std::visit([](Field& field) { return field.getName(); }, gameboard_->getFieldById(pos));
		std::shared_ptr<sf::Text> propertyName(new sf::Text(streetName, getFont(), getFontSize() - 2));
		propertyName->setOrigin(
			propertyName->getGlobalBounds().getSize() / 2.f + propertyName->getLocalBounds().getPosition());
		propertyName->setPosition(
			sf::Vector2f(dataPos.x + (propertySprite.getGlobalBounds().getSize().x / 2.f), dataPos.y + 80));
		propertyName->setFillColor(sf::Color::Black);

		const float yOffset = 90;
		const float yOffset_step = 25;
		const float rentPricesOffsetX = 180;

		std::shared_ptr<sf::Text> propertyPrice(
			new sf::Text("Cena: " + std::to_string(price), getFont(), getFontSize() - 2));
		propertyPrice->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset));
		propertyPrice->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyMortgage(new sf::Text("Zastaw:", getFont(), getFontSize() - 2));
		propertyMortgage->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 8));
		propertyMortgage->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyMortgagePrice(
			new sf::Text(std::to_string(Mortgage), getFont(), getFontSize() - 2));
		propertyMortgagePrice->setPosition(
			sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 8));
		propertyMortgagePrice->setFillColor(sf::Color::Black);

		propertyPlayerTexts.push_back(propertyName);
		propertyPlayerTexts.push_back(propertyMortgagePrice);
		propertyPlayerTexts.push_back(propertyPrice);
		propertyPlayerTexts.push_back(propertyMortgage);

		if (fieldType == STREET) {
			std::shared_ptr<sf::Text> propertyRent1(new sf::Text("Czynsz: ", getFont(), getFontSize() - 2));
			propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
			propertyRent1->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent2(new sf::Text("  kompletny kolor:", getFont(), getFontSize() - 2));
			propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 2));
			propertyRent2->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent3(new sf::Text("  z 1 domem:", getFont(), getFontSize() - 2));
			propertyRent3->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
			propertyRent3->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent4(new sf::Text("  z 2 domami:", getFont(), getFontSize() - 2));
			propertyRent4->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 4));
			propertyRent4->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent5(new sf::Text("  z 3 domami:", getFont(), getFontSize() - 2));
			propertyRent5->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 5));
			propertyRent5->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent6(new sf::Text("  z 4 domami:", getFont(), getFontSize() - 2));
			propertyRent6->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 6));
			propertyRent6->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent7(new sf::Text("  z hotelem:", getFont(), getFontSize() - 2));
			propertyRent7->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 7));
			propertyRent7->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyHouseCost(new sf::Text("Cena domu:", getFont(), getFontSize() - 2));
			propertyHouseCost->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 9));
			propertyHouseCost->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyHotelCost(new sf::Text("Cena hotelu:", getFont(), getFontSize() - 2));
			propertyHotelCost->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 10));
			propertyHotelCost->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost1(
				new sf::Text(std::to_string(rents[0]), getFont(), getFontSize() - 2));
			propertyRentCost1->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 1));
			propertyRentCost1->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost2(
				new sf::Text(std::to_string(rents[1]), getFont(), getFontSize() - 2));
			propertyRentCost2->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 2));
			propertyRentCost2->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost3(
				new sf::Text(std::to_string(rents[2]), getFont(), getFontSize() - 2));
			propertyRentCost3->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 3));
			propertyRentCost3->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost4(
				new sf::Text(std::to_string(rents[3]), getFont(), getFontSize() - 2));
			propertyRentCost4->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 4));
			propertyRentCost4->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost5(
				new sf::Text(std::to_string(rents[4]), getFont(), getFontSize() - 2));
			propertyRentCost5->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 5));
			propertyRentCost5->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost6(
				new sf::Text(std::to_string(rents[5]), getFont(), getFontSize() - 2));
			propertyRentCost6->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 6));
			propertyRentCost6->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost7(
				new sf::Text(std::to_string(rents[6]), getFont(), getFontSize() - 2));
			propertyRentCost7->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 7));
			propertyRentCost7->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyHousePrice(
				new sf::Text(std::to_string(housePrice), getFont(), getFontSize() - 2));
			propertyHousePrice->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 9));
			propertyHousePrice->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyHotelPrice(
				new sf::Text(std::to_string(hotelPrice), getFont(), getFontSize() - 2));
			propertyHotelPrice->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 10));
			propertyHotelPrice->setFillColor(sf::Color::Black);

			///////////////////////////////
			propertyPlayerTexts.push_back(propertyRent1);
			propertyPlayerTexts.push_back(propertyRent2);
			propertyPlayerTexts.push_back(propertyRent3);
			propertyPlayerTexts.push_back(propertyRent4);
			propertyPlayerTexts.push_back(propertyRent5);
			propertyPlayerTexts.push_back(propertyRent6);
			propertyPlayerTexts.push_back(propertyRent7);
			propertyPlayerTexts.push_back(propertyHouseCost);
			propertyPlayerTexts.push_back(propertyRentCost1);
			propertyPlayerTexts.push_back(propertyRentCost2);
			propertyPlayerTexts.push_back(propertyRentCost3);
			propertyPlayerTexts.push_back(propertyRentCost4);
			propertyPlayerTexts.push_back(propertyRentCost5);
			propertyPlayerTexts.push_back(propertyRentCost6);
			propertyPlayerTexts.push_back(propertyRentCost7);
			propertyPlayerTexts.push_back(propertyHousePrice);
			propertyPlayerTexts.push_back(propertyHotelPrice);

		} else if (fieldType == STATION) {
			std::shared_ptr<sf::Text> propertyRent1(new sf::Text("Czynsz: ", getFont(), getFontSize() - 2));
			propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
			propertyRent1->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent2(new sf::Text(" if 2 are owned:", getFont(), getFontSize() - 2));
			propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 2));
			propertyRent2->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent3(new sf::Text(" if 3 are owned:", getFont(), getFontSize() - 2));
			propertyRent3->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
			propertyRent3->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent4(new sf::Text(" if 4 are owned:", getFont(), getFontSize() - 2));
			propertyRent4->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 4));
			propertyRent4->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost1(
				new sf::Text(std::to_string(rents[0]), getFont(), getFontSize() - 2));
			propertyRentCost1->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 1));
			propertyRentCost1->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost2(
				new sf::Text(std::to_string(rents[1]), getFont(), getFontSize() - 2));
			propertyRentCost2->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 2));
			propertyRentCost2->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost3(
				new sf::Text(std::to_string(rents[2]), getFont(), getFontSize() - 2));
			propertyRentCost3->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 3));
			propertyRentCost3->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost4(
				new sf::Text(std::to_string(rents[3]), getFont(), getFontSize() - 2));
			propertyRentCost4->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 4));
			propertyRentCost4->setFillColor(sf::Color::Black);

			//////////////////////
			propertyPlayerTexts.push_back(propertyRent1);
			propertyPlayerTexts.push_back(propertyRent2);
			propertyPlayerTexts.push_back(propertyRent3);
			propertyPlayerTexts.push_back(propertyRent4);
			propertyPlayerTexts.push_back(propertyRentCost1);
			propertyPlayerTexts.push_back(propertyRentCost2);
			propertyPlayerTexts.push_back(propertyRentCost3);
			propertyPlayerTexts.push_back(propertyRentCost4);
		} else {
			std::shared_ptr<sf::Text> propertyRent(new sf::Text("Czynsz:", getFont(), getFontSize() - 2));
			propertyRent->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
			propertyRent->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent1(new sf::Text("1 utility is owned: ", getFont(), getFontSize() - 2));
			propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
			propertyRent1->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost1(
				new sf::Text(std::to_string(rents[0]), getFont(), getFontSize() - 2));
			propertyRentCost1->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 4));
			propertyRentCost1->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent2(
				new sf::Text("2 utilities are owned: ", getFont(), getFontSize() - 2));
			propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 6));
			propertyRent2->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRentCost2(
				new sf::Text(std::to_string(rents[1]), getFont(), getFontSize() - 2));
			propertyRentCost2->setPosition(
				sf::Vector2f(dataPos.x + rentPricesOffsetX, dataPos.y + yOffset + yOffset_step * 7));
			propertyRentCost2->setFillColor(sf::Color::Black);

			//////////////////////////////////
			propertyPlayerTexts.push_back(propertyRent);
			propertyPlayerTexts.push_back(propertyRent1);
			propertyPlayerTexts.push_back(propertyRent2);
			propertyPlayerTexts.push_back(propertyRentCost1);
			propertyPlayerTexts.push_back(propertyRentCost2);
		}
	} else {
		unsigned int width = std::visit([](Field& field) { return field.getWidth(); }, gameboard_->getFieldById(1));
		unsigned int height = std::visit([](Field& field) { return field.getHeight(); }, gameboard_->getFieldById(1));
		if (!propertyTexture->loadFromFile("textures_and_fonts/textures/monopoly_single_square_empty.png")) {
			propertySprite.setColor(sf::Color::Black);
		}
		propertySprite.setTexture(*propertyTexture, true);
		sf::Vector2u texture_dim = propertyTexture->getSize();
		float scale_x = (float)width / (float)texture_dim.x;
		float scale_y = (float)height / (float)texture_dim.y;
		const sf::Vector2f SCALE_VECT = sf::Vector2f(scale_x, scale_y);
		propertySprite.setScale(SCALE_VECT * PROPERTY_SHOW_SCALE);
		propertySprite.setPosition(PROPERTY_POSITION.x, PROPERTY_POSITION.y);
		propertySprite.setRotation(0);

		const std::string streetName = "Brak";
		std::shared_ptr<sf::Text> propertyName(new sf::Text(streetName, getFont(), getFontSize() - 2));
		propertyName->setOrigin(
			propertyName->getGlobalBounds().getSize() / 2.f + propertyName->getLocalBounds().getPosition());
		propertyName->setPosition(sf::Vector2f(
			PROPERTY_POSITION.x + (propertySprite.getGlobalBounds().getSize().x / 2.f), PROPERTY_POSITION.y + 80));
		propertyName->setFillColor(sf::Color::Black);
		propertyPlayerTexts.push_back(propertyName);
	}

	switch (column) {
		case 1:
			property_player_1_texts_.clear();
			property_player_1_texts_ = propertyPlayerTexts;
			property_player_1_sprite_ = propertySprite;
			property_player_1_texture_ = propertyTexture;
			break;

		case 2:
			property_player_1_index_texts_.clear();
			property_player_1_index_texts_ = propertyPlayerTexts;
			property_player_1_index_sprite_ = propertySprite;
			property_player_1_index_texture_ = propertyTexture;
			break;

		case 3:
			property_player_2_index_texts_.clear();
			property_player_2_index_texts_ = propertyPlayerTexts;
			property_player_2_index_sprite_ = propertySprite;
			property_player_2_index_texture_ = propertyTexture;
			break;

		case 4:
			propertyPlayer2Texts_.clear();
			propertyPlayer2Texts_ = propertyPlayerTexts;
			property_player_2_sprite_ = propertySprite;
			property_player_2_texture_ = propertyTexture;
			break;
	}
}

sf::Sprite& Withdraw::getSpritePropertyPlayer1() {
	return property_player_1_sprite_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTextsPropertyPlayer1() {
	return property_player_1_texts_;
}

sf::Sprite& Withdraw::getSpritePropertyPlayer1Index() {
	return property_player_1_index_sprite_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTextsPropertyPlayer1Index() {
	return property_player_1_index_texts_;
}

sf::Sprite& Withdraw::getSpritePropertyPlayer2() {
	return property_player_2_sprite_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTextsPropertyPlayer2() {
	return propertyPlayer2Texts_;
}

sf::Sprite& Withdraw::getSpritePropertyPlayer2Index() {
	return property_player_2_index_sprite_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTextsPropertyPlayer2Index() {
	return property_player_2_index_texts_;
}

std::shared_ptr<sf::Texture> Withdraw::getTexturePropertyPlayer1() {
	return property_player_1_texture_;
}

std::shared_ptr<sf::Texture> Withdraw::getTexturePropertyPlayer1Index() {
	return property_player_1_index_texture_;
}

std::shared_ptr<sf::Texture> Withdraw::getTexturePropertyPlayer2() {
	return property_player_2_texture_;
}

std::shared_ptr<sf::Texture> Withdraw::getTexturePropertyPlayer2Index() {
	return property_player_2_index_texture_;
}

void Withdraw::addPropertyPlayerShowed(int i, unsigned int col) {
	std::vector<unsigned int> properties;
	unsigned int currentProperty;

	switch (col) {
		case 1:
			properties = player1Properties_;
			currentProperty = current_property_player_1_showed_;
			break;
		case 2:
			properties = player_1_index_properties_;
			currentProperty = current_property_player_1_index_showed_;
			break;
		case 3:
			properties = player_2_index_properties_;
			currentProperty = current_property_player_2_index_showed_;
			break;
		case 4:
			properties = player_2_properties_;
			currentProperty = current_property_player_2_showed_;
			break;
		default:
			return;
	}

	auto it = find(properties.begin(), properties.end(), currentProperty);
	unsigned int index = it - properties.begin();

	if (properties.size() != 0) {
		if (i > 0) {
			if (properties.size() - 1 == index) {
				index = 0;
			} else {
				index += 1;
			}
		} else if (i < 0) {
			if (0 == index) {
				index = properties.size() - 1;
			} else {
				index -= 1;
			}
		} else {
			return;
		}
	} else {
		return;
	}

	switch (col) {
		case 1:
			current_property_player_1_showed_ = properties[index];
			break;
		case 2:
			current_property_player_1_index_showed_ = properties[index];
			break;
		case 3:
			current_property_player_2_index_showed_ = properties[index];
			break;
		case 4:
			current_property_player_2_showed_ = properties[index];
			break;
	}
}

void Withdraw::propertyPlayerMoveIndex(int dir, unsigned int plr_num) {
	std::vector<unsigned int> properties;
	unsigned int currentProperty;

	std::vector<unsigned int> propertiesIndex;
	unsigned int currentPropertyIndex;

	switch (plr_num) {
		case 1:
			properties = player1Properties_;
			currentProperty = current_property_player_1_showed_;
			propertiesIndex = player_1_index_properties_;
			currentPropertyIndex = current_property_player_1_index_showed_;
			break;

		case 2:
			properties = player_2_properties_;
			currentProperty = current_property_player_2_showed_;
			propertiesIndex = player_2_index_properties_;
			currentPropertyIndex = current_property_player_2_index_showed_;
			break;
		default:
			return;
	}

	auto it = find(properties.begin(), properties.end(), currentProperty);
	int index;

	switch (dir) {
		case -1:
			if (propertiesIndex.size() > 0) {
				it = find(propertiesIndex.begin(), propertiesIndex.end(), currentPropertyIndex);
				index = it - propertiesIndex.begin();
				properties.push_back(propertiesIndex[index]);
				propertiesIndex.erase(propertiesIndex.begin() + index);
			}
			break;
		case 1:
			if (properties.size() > 0) {
				it = find(properties.begin(), properties.end(), currentProperty);
				index = it - properties.begin();
				propertiesIndex.push_back(properties[index]);
				properties.erase(properties.begin() + index);
			}
			break;
		default:
			return;
			break;
	}

	switch (plr_num) {
		case 1:
			player1Properties_ = properties;
			player_1_index_properties_ = propertiesIndex;
			if (std::find(properties.begin(), properties.end(), currentProperty) == properties.end()) {
				if (properties.size() != 0) {
					current_property_player_1_showed_ = properties[0];
				} else {
					current_property_player_1_showed_ = 0;
				}
			} else {
				current_property_player_1_showed_ = currentProperty;
			}
			if (std::find(propertiesIndex.begin(), propertiesIndex.end(), currentPropertyIndex) ==
				propertiesIndex.end()) {
				if (propertiesIndex.size() != 0) {
					current_property_player_1_index_showed_ = propertiesIndex[0];
				} else {
					current_property_player_1_index_showed_ = 0;
				}
			} else {
				current_property_player_1_index_showed_ = currentPropertyIndex;
			}
			break;

		case 2:
			player_2_properties_ = properties;
			player_2_index_properties_ = propertiesIndex;
			if (std::find(properties.begin(), properties.end(), currentProperty) == properties.end()) {
				if (properties.size() != 0) {
					current_property_player_2_showed_ = properties[0];
				} else {
					current_property_player_2_showed_ = 0;
				}
			} else {
				current_property_player_2_showed_ = currentProperty;
			}
			if (std::find(propertiesIndex.begin(), propertiesIndex.end(), currentPropertyIndex) ==
				propertiesIndex.end()) {
				if (propertiesIndex.size() != 0) {
					current_property_player_2_index_showed_ = propertiesIndex[0];
				} else {
					current_property_player_2_index_showed_ = 0;
				}
			} else {
				current_property_player_2_index_showed_ = currentPropertyIndex;
			}
			break;
		default:
			return;
	}
}

std::shared_ptr<Button> Withdraw::getResignDecisionButton() {
	return resign_decision_button_;
}

std::shared_ptr<Button> Withdraw::getAcceptDecisionButton() {
	return accpet_decision_button_;
}

void Withdraw::makeWithdraw() {
	// money flow
	player_1_to_with_draw_->addMoney(player_2_money_index_buffer_);
	player_2_to_with_draw_->addMoney(player_1_money_index_buffer_);
	player_1_to_with_draw_->substractMoney(player_1_money_index_buffer_);
	player_2_to_with_draw_->substractMoney(player_2_money_index_buffer_);

	// properties flow
	auto it = find(player_1_index_properties_.begin(), player_1_index_properties_.end(), 0);

	// 1 -> 2
	for (auto property : player_1_index_properties_) {
		it = find(player_1_index_properties_.begin(), player_1_index_properties_.end(), property);
		player_2_to_with_draw_->addFieldOwnedId(property);
		player_1_to_with_draw_->removeFieldOwnedId(property);
		FieldType type = std::visit(
			[](Field& visited_field) { return visited_field.getType(); }, gameboard_->getFieldById(property));
		switch (type) {
			case STREET: {
				StreetField& visited_field = std::get<StreetField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player_2_to_with_draw_);
			} break;

			case UTILITY: {
				UtilityField& visited_field = std::get<UtilityField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player_2_to_with_draw_);
			} break;

			case STATION: {
				StationField& visited_field = std::get<StationField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player_2_to_with_draw_);
			} break;

			default:
				break;
		}
	}
	// 2 -> 1
	for (auto property : player_2_index_properties_) {
		it = find(player_2_index_properties_.begin(), player_2_index_properties_.end(), property);
		player_1_to_with_draw_->addFieldOwnedId(property);
		player_2_to_with_draw_->removeFieldOwnedId(property);
		FieldType type = std::visit(
			[](Field& visited_field) { return visited_field.getType(); }, gameboard_->getFieldById(property));
		switch (type) {
			case STREET: {
				StreetField& visited_field = std::get<StreetField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player_1_to_with_draw_);
			} break;

			case UTILITY: {
				UtilityField& visited_field = std::get<UtilityField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player_1_to_with_draw_);
			} break;

			case STATION: {
				StationField& visited_field = std::get<StationField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player_1_to_with_draw_);
			} break;
			default:
				break;
		}
	}
}

bool Withdraw::isNonZeroValue() {
	if (ALLOW_ONE_SIDE_BENEFIT_WITHDRAW) {
		if (player_1_money_index_buffer_ != 0) {
			return true;
		} else if (player_2_money_index_buffer_ != 0) {
			return true;
		} else if (player_1_index_properties_.size() != 0) {
			return true;
		} else if (player_2_index_properties_.size() != 0) {
			return true;
		}
	} else {
		if ((player_1_money_index_buffer_ != 0 || player_1_index_properties_.size() != 0) &&
			(player_2_money_index_buffer_ != 0 || player_2_index_properties_.size() != 0)) {
			return true;
		}
	}

	return false;
}