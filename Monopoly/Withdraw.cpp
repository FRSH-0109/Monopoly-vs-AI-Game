#include "Withdraw.h"

Withdraw::Withdraw() {
	player1ToWithDraw_ = nullptr;
	player2ToWithDraw_ = nullptr;
}

void Withdraw::setChooseScreenVisible(bool isVisible) {
	if (isVisible) {
		choosePlayerText_->setFillColor(sf::Color::Black);
	} else {
		choosePlayerText_->setFillColor(sf::Color::Transparent);
	}
	choosePlayer1Button_->setIsVisible(isVisible);
	choosePlayer2Button_->setIsVisible(isVisible);
	choosePlayer3Button_->setIsVisible(isVisible);
	choosePlayer4Button_->setIsVisible(isVisible);
	resignButton_->setIsVisible(isVisible);
}

void Withdraw::setValueScreenVisible(bool isVisible) {
	sf::Color color = sf::Color::Black;
	if (!isVisible) {
		color = sf::Color::Transparent;
	}

	player1Text_->setFillColor(color);
	player1Money_->setFillColor(color);
	player2Text_->setFillColor(color);
	player2Money_->setFillColor(color);
	player1IndexText_->setFillColor(color);
	player1IndexMoney_->setFillColor(color);
	player2IndexText_->setFillColor(color);
	player2IndexMoney_->setFillColor(color);

	resignValueButton_->setIsVisible(isVisible);
	submitValueButton_->setIsVisible(isVisible);
	minus1Player1Button_->setIsVisible(isVisible);
	minus10Player1Button_->setIsVisible(isVisible);
	minus100Player1Button_->setIsVisible(isVisible);
	plus1Player1Button_->setIsVisible(isVisible);
	plus10Player1Button_->setIsVisible(isVisible);
	plus100Player1Button_->setIsVisible(isVisible);
	minus1Player2Button_->setIsVisible(isVisible);
	minus10Player2Button_->setIsVisible(isVisible);
	minus100Player2Button_->setIsVisible(isVisible);
	plus1Player2Button_->setIsVisible(isVisible);
	plus10Player2Button_->setIsVisible(isVisible);
	plus100Player2Button_->setIsVisible(isVisible);
	nextPropertyPlayer1Button_->setIsVisible(isVisible);
	previousPropertyPlayer1Button_->setIsVisible(isVisible);
	nextPropertyPlayer2Button_->setIsVisible(isVisible);
	previousPropertyPlayer2Button_->setIsVisible(isVisible);
	nextPropertyPlayer1IndexButton_->setIsVisible(isVisible);
	previousPropertyPlayer1IndexButton_->setIsVisible(isVisible);
	nextPropertyPlayer2IndexButton_->setIsVisible(isVisible);
	previousPropertyPlayer2IndexButton_->setIsVisible(isVisible);
	addPropertyPlayer1Button_->setIsVisible(isVisible);
	removePropertyPlayer1Button_->setIsVisible(isVisible);
	addPropertyPlayer2Button_->setIsVisible(isVisible);
	removePropertyPlayer2Button_->setIsVisible(isVisible);

	if (isVisible) {
		player1MoneyBuffer_ = player1ToWithDraw_->getMoney();
		player2MoneyBuffer_ = player2ToWithDraw_->getMoney();
		player1MoneyIndexBuffer_ = 0;
		player2MoneyIndexBuffer_ = 0;
		player1Properties_.clear();
		player2Properties_.clear();
		player1IndexProperties_.clear();
		player2IndexProperties_.clear();
	}

	if (isVisible) {
		player1Text_->setString("Player " + std::to_string(player1ToWithDraw_->getId() + 1));
		player1Money_->setString("Money: " + std::to_string(player1MoneyBuffer_));

		for (auto prop : player1ToWithDraw_->getFiledOwnedId()) {
			player1Properties_.push_back(prop);
		}

		if (player1Properties_.size() != 0) {
			currentPropertyPlayer1Showed_ = player1Properties_[0];
		}

		player2Text_->setString("Player " + std::to_string(player2ToWithDraw_->getId() + 1));
		player2Money_->setString("Money: " + std::to_string(player2MoneyBuffer_));

		for (auto prop : player2ToWithDraw_->getFiledOwnedId()) {
			player2Properties_.push_back(prop);
		}

		if (player2Properties_.size() != 0) {
			currentPropertyPlayer2Showed_ = player2Properties_[0];
		}

		player1IndexText_->setString(player1Text_->getString() + " Index");
		player1IndexMoney_->setString("Money: " + std::to_string(player1MoneyIndexBuffer_));
		currentPropertyPlayer1IndexShowed_ = 0;

		player2IndexText_->setString(player2Text_->getString() + " Index");
		player2IndexMoney_->setString("Money: " + std::to_string(player2MoneyIndexBuffer_));
		currentPropertyPlayer2IndexShowed_ = 0;

		showProperty(1);
		showProperty(2);
		showProperty(3);
		showProperty(4);
	}
}

void Withdraw::setDecisionScreenVisible(bool isVisible) {
	resignDecisionButton_->setIsVisible(isVisible);
	acceptDecisionButton_->setIsVisible(isVisible);

	sf::Color color = sf::Color::Black;
	if (!isVisible) {
		color = sf::Color::Transparent;
	}

	playerInfoText_->setFillColor(color);
	playerInfoMoney_->setFillColor(color);
	playerInfoGetProperties_->setFillColor(color);
	playerInfoLoseProperties_->setFillColor(color);

	if (isVisible) {
		playerInfoText_->setString("Player " + std::to_string(player2ToWithDraw_->getId() + 1) + " decision:");
		playerInfoMoney_->setString(
			"Money transaction: " + std::to_string((int)player1MoneyIndexBuffer_ - (int)player2MoneyIndexBuffer_));
	}
	playerInfoGetProperties_->setString("Get properties: ");
	bool addComa = false;
	if (player1IndexProperties_.size() != 0) {
		for (auto property : player1IndexProperties_) {
			std::string name =
				std::visit([](Field& field) { return field.getName(); }, gameboard_->getFieldById(property));
			if (addComa) {
				playerInfoGetProperties_->setString(playerInfoGetProperties_->getString() + ", ");
			}
			playerInfoGetProperties_->setString(playerInfoGetProperties_->getString() + name);
			addComa = true;
		}
	} else {
		playerInfoGetProperties_->setString(playerInfoGetProperties_->getString() + "None");
	}

	playerInfoLoseProperties_->setString("Lose properties: ");
	addComa = false;
	if (player2IndexProperties_.size() != 0) {
		for (auto property : player2IndexProperties_) {
			std::string name =
				std::visit([](Field& field) { return field.getName(); }, gameboard_->getFieldById(property));
			if (addComa) {
				playerInfoLoseProperties_->setString(playerInfoLoseProperties_->getString() + ", ");
			}
			playerInfoLoseProperties_->setString(playerInfoLoseProperties_->getString() + name);
			addComa = true;
		}
	} else {
		playerInfoLoseProperties_->setString(playerInfoLoseProperties_->getString() + "None");
	}

	playerInfoText_->setOrigin(
		playerInfoText_->getGlobalBounds().getSize() / 2.f + playerInfoText_->getLocalBounds().getPosition());

	playerInfoMoney_->setOrigin(
		playerInfoMoney_->getGlobalBounds().getSize() / 2.f + playerInfoMoney_->getLocalBounds().getPosition());

	playerInfoGetProperties_->setOrigin(playerInfoGetProperties_->getGlobalBounds().getSize() / 2.f +
										playerInfoGetProperties_->getLocalBounds().getPosition());

	playerInfoLoseProperties_->setOrigin(playerInfoLoseProperties_->getGlobalBounds().getSize() / 2.f +
										 playerInfoLoseProperties_->getLocalBounds().getPosition());
}

void Withdraw::createDecisionPlayerScreen() {
	std::shared_ptr<Button> buttonResign = createDefaultButton("Resign", 120, 50);
	buttonResign->setPosition(RESIGN_DECISION_BUTTON_POSITION);
	resignDecisionButton_ = buttonResign;
	addButton(buttonResign);

	std::shared_ptr<Button> buttonAccpet = createDefaultButton("Accept", 120, 50);
	buttonAccpet->setPosition(ACCEPT_DECISION_BUTTON_POSITION);
	acceptDecisionButton_ = buttonAccpet;
	addButton(buttonAccpet);

	int ystep = 50;

	std::shared_ptr<sf::Text> playerInfoText(new sf::Text("", getFont(), getFontSize() - 2));
	playerInfoText->setPosition(sf::Vector2f(DECISION_TEXT_POSITION.x, DECISION_TEXT_POSITION.y + ystep * 0));
	playerInfoText->setFillColor(sf::Color::Transparent);
	playerInfoText_ = playerInfoText;
	addText(playerInfoText);

	std::shared_ptr<sf::Text> playerInfoMoney(new sf::Text("", getFont(), getFontSize() - 2));
	playerInfoMoney->setPosition(sf::Vector2f(DECISION_TEXT_POSITION.x, DECISION_TEXT_POSITION.y + ystep * 1));
	playerInfoMoney->setFillColor(sf::Color::Transparent);
	playerInfoMoney->setOrigin(
		playerInfoMoney->getGlobalBounds().getSize() / 2.f + playerInfoMoney->getLocalBounds().getPosition());
	playerInfoMoney_ = playerInfoMoney;
	addText(playerInfoMoney);

	std::shared_ptr<sf::Text> playerInfoGetProperties(new sf::Text("", getFont(), getFontSize() - 2));
	playerInfoGetProperties->setPosition(sf::Vector2f(DECISION_TEXT_POSITION.x, DECISION_TEXT_POSITION.y + ystep * 2));
	playerInfoGetProperties->setFillColor(sf::Color::Transparent);
	playerInfoGetProperties_ = playerInfoGetProperties;
	addText(playerInfoGetProperties);

	std::shared_ptr<sf::Text> playerInfoLoseProperties(new sf::Text("", getFont(), getFontSize() - 2));
	playerInfoLoseProperties->setPosition(sf::Vector2f(DECISION_TEXT_POSITION.x, DECISION_TEXT_POSITION.y + ystep * 3));
	playerInfoLoseProperties->setFillColor(sf::Color::Transparent);
	playerInfoLoseProperties_ = playerInfoLoseProperties;
	addText(playerInfoLoseProperties);
}

void Withdraw::createValuePlayerScreen() {
	std::shared_ptr<Button> buttonResign = createDefaultButton("Resign", 120, 50);
	buttonResign->setPosition(RESIGN_VALUE_BUTTON_POSITION);
	resignValueButton_ = buttonResign;
	addButton(buttonResign);

	std::shared_ptr<Button> buttonSubmit = createDefaultButton("Submit", 120, 50);
	buttonSubmit->setPosition(sf::Vector2f(RESIGN_VALUE_BUTTON_POSITION.x + 200, RESIGN_VALUE_BUTTON_POSITION.y));
	submitValueButton_ = buttonSubmit;
	addButton(buttonSubmit);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	std::shared_ptr<sf::Text> choosePlayer1NameText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlayer1NameText->setPosition(COLUMN_1_POSITION);
	choosePlayer1NameText->setFillColor(sf::Color::Transparent);
	choosePlayer1NameText->setOrigin(choosePlayer1NameText->getGlobalBounds().getSize() / 2.f +
									 choosePlayer1NameText->getLocalBounds().getPosition());
	player1Text_ = choosePlayer1NameText;
	addText(choosePlayer1NameText);

	std::shared_ptr<sf::Text> choosePlaye1MoneyText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlaye1MoneyText->setPosition(sf::Vector2f(COLUMN_1_POSITION.x, COLUMN_1_POSITION.y + 30));
	choosePlaye1MoneyText->setFillColor(sf::Color::Transparent);
	choosePlaye1MoneyText->setOrigin(choosePlaye1MoneyText->getGlobalBounds().getSize() / 2.f +
									 choosePlaye1MoneyText->getLocalBounds().getPosition());
	player1Money_ = choosePlaye1MoneyText;
	addText(choosePlaye1MoneyText);

	player1Properties_.clear();

	///////////////////////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<sf::Text> choosePlayer2NameText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlayer2NameText->setPosition(COLUMN_4_POSITION);
	choosePlayer2NameText->setFillColor(sf::Color::Transparent);
	choosePlayer2NameText->setOrigin(choosePlayer2NameText->getGlobalBounds().getSize() / 2.f +
									 choosePlayer2NameText->getLocalBounds().getPosition());
	player2Text_ = choosePlayer2NameText;
	addText(choosePlayer2NameText);

	std::shared_ptr<sf::Text> choosePlaye2MoneyText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlaye2MoneyText->setPosition(sf::Vector2f(COLUMN_4_POSITION.x, COLUMN_4_POSITION.y + 30));
	choosePlaye2MoneyText->setFillColor(sf::Color::Transparent);
	choosePlaye2MoneyText->setOrigin(choosePlaye2MoneyText->getGlobalBounds().getSize() / 2.f +
									 choosePlaye2MoneyText->getLocalBounds().getPosition());
	player2Money_ = choosePlaye2MoneyText;
	addText(choosePlaye2MoneyText);

	player2Properties_.clear();

	///////////////////////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<sf::Text> choosePlayer1NameIndexText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlayer1NameIndexText->setPosition(COLUMN_2_POSITION);
	choosePlayer1NameIndexText->setFillColor(sf::Color::Transparent);
	choosePlayer1NameIndexText->setOrigin(choosePlayer1NameIndexText->getGlobalBounds().getSize() / 2.f +
										  choosePlayer1NameIndexText->getLocalBounds().getPosition());
	player1IndexText_ = choosePlayer1NameIndexText;
	addText(choosePlayer1NameIndexText);

	std::shared_ptr<sf::Text> choosePlaye1MoneyIndexText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlaye1MoneyIndexText->setPosition(sf::Vector2f(COLUMN_2_POSITION.x, COLUMN_2_POSITION.y + 30));
	choosePlaye1MoneyIndexText->setFillColor(sf::Color::Transparent);
	choosePlaye1MoneyIndexText->setOrigin(choosePlaye1MoneyIndexText->getGlobalBounds().getSize() / 2.f +
										  choosePlaye1MoneyIndexText->getLocalBounds().getPosition());
	player1IndexMoney_ = choosePlaye1MoneyIndexText;
	addText(choosePlaye1MoneyIndexText);

	player1IndexProperties_.clear();

	std::shared_ptr<sf::Text> choosePlayer2NameIndexText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlayer2NameIndexText->setPosition(COLUMN_3_POSITION);
	choosePlayer2NameIndexText->setFillColor(sf::Color::Transparent);
	choosePlayer2NameIndexText->setOrigin(choosePlayer2NameIndexText->getGlobalBounds().getSize() / 2.f +
										  choosePlayer2NameIndexText->getLocalBounds().getPosition());
	player2IndexText_ = choosePlayer2NameIndexText;
	addText(choosePlayer2NameIndexText);

	std::shared_ptr<sf::Text> choosePlaye2MoneyIndexText(new sf::Text("", getFont(), getFontSize() - 2));
	choosePlaye2MoneyIndexText->setPosition(sf::Vector2f(COLUMN_3_POSITION.x, COLUMN_3_POSITION.y + 30));
	choosePlaye2MoneyIndexText->setFillColor(sf::Color::Transparent);
	choosePlaye2MoneyIndexText->setOrigin(choosePlaye2MoneyIndexText->getGlobalBounds().getSize() / 2.f +
										  choosePlaye2MoneyIndexText->getLocalBounds().getPosition());
	player2IndexMoney_ = choosePlaye2MoneyIndexText;
	addText(choosePlaye2MoneyIndexText);

	player2IndexProperties_.clear();

	///////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<Button> buttonPlayer1minus1 = createDefaultButton("-1", 60, 50);
	buttonPlayer1minus1->setPosition(sf::Vector2f(COLUMN_1_POSITION.x, COLUMN_1_POSITION.y + 100));
	minus1Player1Button_ = buttonPlayer1minus1;
	addButton(buttonPlayer1minus1);

	std::shared_ptr<Button> buttonPlayer1minus10 = createDefaultButton("-10", 60, 50);
	buttonPlayer1minus10->setPosition(sf::Vector2f(COLUMN_1_POSITION.x + 70, COLUMN_1_POSITION.y + 100));
	minus10Player1Button_ = buttonPlayer1minus10;
	addButton(buttonPlayer1minus10);

	std::shared_ptr<Button> buttonPlayer1minus100 = createDefaultButton("-100", 60, 50);
	buttonPlayer1minus100->setPosition(sf::Vector2f(COLUMN_1_POSITION.x + 140, COLUMN_1_POSITION.y + 100));
	minus100Player1Button_ = buttonPlayer1minus100;
	addButton(buttonPlayer1minus100);

	std::shared_ptr<Button> buttonPlayer1plus1 = createDefaultButton("+1", 60, 50);
	buttonPlayer1plus1->setPosition(sf::Vector2f(COLUMN_1_POSITION.x, COLUMN_1_POSITION.y + 160));
	plus1Player1Button_ = buttonPlayer1plus1;
	addButton(buttonPlayer1plus1);

	std::shared_ptr<Button> buttonPlayer1plus10 = createDefaultButton("+10", 60, 50);
	buttonPlayer1plus10->setPosition(sf::Vector2f(COLUMN_1_POSITION.x + 70, COLUMN_1_POSITION.y + 160));
	plus10Player1Button_ = buttonPlayer1plus10;
	addButton(buttonPlayer1plus10);

	std::shared_ptr<Button> buttonPlayer1plus100 = createDefaultButton("+100", 60, 50);
	buttonPlayer1plus100->setPosition(sf::Vector2f(COLUMN_1_POSITION.x + 140, COLUMN_1_POSITION.y + 160));
	plus100Player1Button_ = buttonPlayer1plus100;
	addButton(buttonPlayer1plus100);

	std::shared_ptr<Button> buttonPlayer2minus1 = createDefaultButton("-1", 60, 50);
	buttonPlayer2minus1->setPosition(sf::Vector2f(COLUMN_4_POSITION.x, COLUMN_4_POSITION.y + 100));
	minus1Player2Button_ = buttonPlayer2minus1;
	addButton(buttonPlayer2minus1);

	std::shared_ptr<Button> buttonPlayer2minus10 = createDefaultButton("-10", 60, 50);
	buttonPlayer2minus10->setPosition(sf::Vector2f(COLUMN_4_POSITION.x + 70, COLUMN_4_POSITION.y + 100));
	minus10Player2Button_ = buttonPlayer2minus10;
	addButton(buttonPlayer2minus10);

	std::shared_ptr<Button> buttonPlayer2minus100 = createDefaultButton("-100", 60, 50);
	buttonPlayer2minus100->setPosition(sf::Vector2f(COLUMN_4_POSITION.x + 140, COLUMN_4_POSITION.y + 100));
	minus100Player2Button_ = buttonPlayer2minus100;
	addButton(buttonPlayer2minus100);

	std::shared_ptr<Button> buttonPlayer2plus1 = createDefaultButton("+1", 60, 50);
	buttonPlayer2plus1->setPosition(sf::Vector2f(COLUMN_4_POSITION.x, COLUMN_4_POSITION.y + 160));
	plus1Player2Button_ = buttonPlayer2plus1;
	addButton(buttonPlayer2plus1);

	std::shared_ptr<Button> buttonPlayer2plus10 = createDefaultButton("+10", 60, 50);
	buttonPlayer2plus10->setPosition(sf::Vector2f(COLUMN_4_POSITION.x + 70, COLUMN_4_POSITION.y + 160));
	plus10Player2Button_ = buttonPlayer2plus10;
	addButton(buttonPlayer2plus10);

	std::shared_ptr<Button> buttonPlayer2plus100 = createDefaultButton("+100", 60, 50);
	buttonPlayer2plus100->setPosition(sf::Vector2f(COLUMN_4_POSITION.x + 140, COLUMN_4_POSITION.y + 160));
	plus100Player2Button_ = buttonPlayer2plus100;
	addButton(buttonPlayer2plus100);

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<Button> buttonNext = createDefaultButton("Next", 120, 50);
	buttonNext->setPosition(NEXT_PROPERTY_PLAYER1_BUTTON_POSITION);
	nextPropertyPlayer1Button_ = buttonNext;
	addButton(buttonNext);

	std::shared_ptr<Button> buttonPrev = createDefaultButton("Previous", 120, 50);
	buttonPrev->setPosition(PREVIOUS_PROPERTY_PLAYER1_BUTTON_POSITION);
	previousPropertyPlayer1Button_ = buttonPrev;
	addButton(buttonPrev);

	std::shared_ptr<Button> buttonNext2 = createDefaultButton("Next", 120, 50);
	buttonNext2->setPosition(NEXT_PROPERTY_PLAYER2_BUTTON_POSITION);
	nextPropertyPlayer2Button_ = buttonNext2;
	addButton(buttonNext2);

	std::shared_ptr<Button> buttonPrev2 = createDefaultButton("Previous", 120, 50);
	buttonPrev2->setPosition(PREVIOUS_PROPERTY_PLAYER2_BUTTON_POSITION);
	previousPropertyPlayer2Button_ = buttonPrev2;
	addButton(buttonPrev2);

	std::shared_ptr<Button> buttonNextIndex = createDefaultButton("Next", 120, 50);
	buttonNextIndex->setPosition(NEXT_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION);
	nextPropertyPlayer1IndexButton_ = buttonNextIndex;
	addButton(buttonNextIndex);

	std::shared_ptr<Button> buttonPrevIndex = createDefaultButton("Previous", 120, 50);
	buttonPrevIndex->setPosition(PREVIOUS_PROPERTY_PLAYER1_INDEX_BUTTON_POSITION);
	previousPropertyPlayer1IndexButton_ = buttonPrevIndex;
	addButton(buttonPrevIndex);

	std::shared_ptr<Button> buttonNextIndex2 = createDefaultButton("Next", 120, 50);
	buttonNextIndex2->setPosition(NEXT_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION);
	nextPropertyPlayer2IndexButton_ = buttonNextIndex2;
	addButton(buttonNextIndex2);

	std::shared_ptr<Button> buttonPrevIndex2 = createDefaultButton("Previous", 120, 50);
	buttonPrevIndex2->setPosition(PREVIOUS_PROPERTY_PLAYER2_INDEX_BUTTON_POSITION);
	previousPropertyPlayer2IndexButton_ = buttonPrevIndex2;
	addButton(buttonPrevIndex2);

	std::shared_ptr<Button> buttoAdd1 = createDefaultButton(">>>", 120, 50);
	buttoAdd1->setPosition(ADD_PROPERTY_PLAYER1_BUTTON_POSITION);
	addPropertyPlayer1Button_ = buttoAdd1;
	addButton(buttoAdd1);

	std::shared_ptr<Button> buttonRemove1 = createDefaultButton("<<<", 120, 50);
	buttonRemove1->setPosition(REMOVE_PROPERTY_PLAYER1_BUTTON_POSITION);
	removePropertyPlayer1Button_ = buttonRemove1;
	addButton(buttonRemove1);

	std::shared_ptr<Button> buttoAdd2 = createDefaultButton("<<<", 120, 50);
	buttoAdd2->setPosition(ADD_PROPERTY_PLAYER2_BUTTON_POSITION);
	addPropertyPlayer2Button_ = buttoAdd2;
	addButton(buttoAdd2);

	std::shared_ptr<Button> buttonRemove2 = createDefaultButton(">>>", 120, 50);
	buttonRemove2->setPosition(REMOVE_PROPERTY_PLAYER2_BUTTON_POSITION);
	removePropertyPlayer2Button_ = buttonRemove2;
	addButton(buttonRemove2);
}

std::shared_ptr<Button> Withdraw::createDefaultButton(std::string text, unsigned int width, unsigned int height) {
	sf::Vector2f buttonSize = sf::Vector2f(width, height);
	sf::Color activeButtonBackColor = sf::Color::Green;
	sf::Color inActiveButtonBackColor = sf::Color(192, 192, 192);  // GREY
	sf::Color FocusButtonBackColor = sf::Color::Black;
	sf::Color activeButtonTextColor = sf::Color::Black;
	sf::Color inActiveButtonTextColor = sf::Color::Black;
	sf::Color FocusButtonTextColor = sf::Color::Green;
	std::shared_ptr<Button> buttonDefault(new Button(Idle, text, buttonSize, getFontSize()));
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
	std::shared_ptr<sf::Text> choosePlayerText(
		new sf::Text("Choose Player to withdraw with", getFont(), getFontSize() - 2));
	choosePlayerText->setPosition(CHOOSE_PLAYER_TEXT_POSITION);
	choosePlayerText->setFillColor(sf::Color::Transparent);
	choosePlayerText->setOrigin(
		choosePlayerText->getGlobalBounds().getSize() / 2.f + choosePlayerText->getLocalBounds().getPosition());
	choosePlayerText_ = choosePlayerText;
	addText(choosePlayerText);

	std::shared_ptr<Button> buttonChoosePlayer1 = createDefaultButton("Player 1", 120, 50);
	buttonChoosePlayer1->setPosition(sf::Vector2f(CHOOSE_PLAYER_TEXT_POSITION.x, CHOOSE_PLAYER_TEXT_POSITION.y + 100));
	choosePlayer1Button_ = buttonChoosePlayer1;
	addButton(buttonChoosePlayer1);

	std::shared_ptr<Button> buttonChoosePlayer2 = createDefaultButton("Player 2", 120, 50);
	buttonChoosePlayer2->setPosition(sf::Vector2f(CHOOSE_PLAYER_TEXT_POSITION.x, CHOOSE_PLAYER_TEXT_POSITION.y + 200));
	choosePlayer2Button_ = buttonChoosePlayer2;
	addButton(buttonChoosePlayer2);

	std::shared_ptr<Button> buttonChoosePlayer3 = createDefaultButton("Player 3", 120, 50);
	buttonChoosePlayer3->setPosition(sf::Vector2f(CHOOSE_PLAYER_TEXT_POSITION.x, CHOOSE_PLAYER_TEXT_POSITION.y + 300));
	choosePlayer3Button_ = buttonChoosePlayer3;
	addButton(buttonChoosePlayer3);

	std::shared_ptr<Button> buttonChoosePlayer4 = createDefaultButton("Player 4", 120, 50);
	buttonChoosePlayer4->setPosition(sf::Vector2f(CHOOSE_PLAYER_TEXT_POSITION.x, CHOOSE_PLAYER_TEXT_POSITION.y + 400));
	choosePlayer4Button_ = buttonChoosePlayer4;
	addButton(buttonChoosePlayer4);

	std::shared_ptr<Button> buttonResign = createDefaultButton("Resign", 120, 50);
	buttonResign->setPosition(RESIGN_BUTTON_POSITION);
	resignButton_ = buttonResign;
	addButton(buttonResign);
}

void Withdraw::setBoard(std::shared_ptr<Board> board_ptr) {
	gameboard_ = board_ptr;
}

void Withdraw::setTurnState(TurnState state) {
	turnStatePrev_ = state;
}

TurnState Withdraw::getTurnState() {
	return turnStatePrev_;
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

void Withdraw::addButton(std::shared_ptr<Button> buttonTmp) {
	std::shared_ptr<Button> button = buttonTmp;
	buttons_.push_back(button);
}

void Withdraw::addText(std::shared_ptr<sf::Text> textTmp) {
	std::shared_ptr<sf::Text> text = textTmp;
	texts_.push_back(text);
}

std::vector<std::shared_ptr<Button>>& Withdraw::getButtons() {
	return buttons_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTexts() {
	return texts_;
}

std::shared_ptr<Button> Withdraw::getPlayer1Button() {
	return choosePlayer1Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2Button() {
	return choosePlayer2Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer3Button() {
	return choosePlayer3Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer4Button() {
	return choosePlayer4Button_;
}

std::shared_ptr<Button> Withdraw::getResignButton() {
	return resignButton_;
}

void Withdraw::setPlayer1ToWithdraw(std::shared_ptr<Player> player_ptr) {
	player1ToWithDraw_ = player_ptr;
}

void Withdraw::setPlayer2ToWithdraw(std::shared_ptr<Player> player_ptr) {
	player2ToWithDraw_ = player_ptr;
}

std::shared_ptr<Button> Withdraw::getResignValueButton() {
	return resignValueButton_;
}

std::shared_ptr<Button> Withdraw::getSubmitValueButton() {
	return submitValueButton_;
}

std::shared_ptr<Player> Withdraw::getPlayer1ToWithdraw() {
	return player1ToWithDraw_;
}

std::shared_ptr<Player> Withdraw::getPlayer2ToWithdraw() {
	return player2ToWithDraw_;
}

std::shared_ptr<Button> Withdraw::getPlayer1minus1() {
	return minus1Player1Button_;
}
std::shared_ptr<Button> Withdraw::getPlayer1minus10() {
	return minus10Player1Button_;
}
std::shared_ptr<Button> Withdraw::getPlayer1minus100() {
	return minus100Player1Button_;
}
std::shared_ptr<Button> Withdraw::getPlayer1plus1() {
	return plus1Player1Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1plus10() {
	return plus10Player1Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1plus100() {
	return plus100Player1Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2minus1() {
	return minus1Player2Button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2minus10() {
	return minus10Player2Button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2minus100() {
	return minus100Player2Button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2plus1() {
	return plus1Player2Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2plus10() {
	return plus10Player2Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2plus100() {
	return plus100Player2Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1NextButton() {
	return nextPropertyPlayer1Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1PreviousButton() {
	return previousPropertyPlayer1Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2NextButton() {
	return nextPropertyPlayer2Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer2PreviousButton() {
	return previousPropertyPlayer2Button_;
}

std::shared_ptr<Button> Withdraw::getPlayer1IndexNextButton() {
	return nextPropertyPlayer1IndexButton_;
}
std::shared_ptr<Button> Withdraw::getPlayer1IndexPreviousButton() {
	return previousPropertyPlayer1IndexButton_;
}
std::shared_ptr<Button> Withdraw::getPlayer2IndexNextButton() {
	return nextPropertyPlayer2IndexButton_;
}
std::shared_ptr<Button> Withdraw::getPlayer2IndexPreviousButton() {
	return previousPropertyPlayer2IndexButton_;
}

std::shared_ptr<Button> Withdraw::getPlayer1AddButton() {
	return addPropertyPlayer1Button_;
}
std::shared_ptr<Button> Withdraw::getPlayer1RemoveButton() {
	return removePropertyPlayer1Button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2AddButton() {
	return addPropertyPlayer2Button_;
}
std::shared_ptr<Button> Withdraw::getPlayer2RemoveButton() {
	return removePropertyPlayer2Button_;
}

void Withdraw::moneyTransferIndex(unsigned int playerNum, int money) {
	if (playerNum == 1) {
		if (money > 0 && money > player1MoneyBuffer_) {
			return;
		} else if (money < 0 && (money * -1) > player1MoneyIndexBuffer_) {
			return;
		}
		player1MoneyBuffer_ -= money;
		player1MoneyIndexBuffer_ += money;
	} else if (playerNum == 2) {
		if (money > 0 && money > player2MoneyBuffer_) {
			return;
		} else if (money < 0 && (money * -1) > player2MoneyIndexBuffer_) {
			return;
		}
		player2MoneyBuffer_ -= money;
		player2MoneyIndexBuffer_ += money;
	}
}

void Withdraw::moneyTextUpdate() {
	player1Money_->setString("Money: " + std::to_string(player1MoneyBuffer_));
	player2Money_->setString("Money: " + std::to_string(player2MoneyBuffer_));
	player1IndexMoney_->setString("Money: " + std::to_string(player1MoneyIndexBuffer_));
	player2IndexMoney_->setString("Money: " + std::to_string(player2MoneyIndexBuffer_));
}

void Withdraw::showProperty(int column) {
	unsigned int pos = 0;
	std::vector<std::shared_ptr<sf::Text>> propertyPlayerTexts;
	sf::Vector2f PROPERTY_POSITION;
	sf::Sprite propertySprite;
	std::shared_ptr<sf::Texture> propertyTexture = std::make_shared<sf::Texture>();
	bool isEmpty = true;

	auto it = find(player1Properties_.begin(), player1Properties_.end(), currentPropertyPlayer1Showed_);
	int index;
	switch (column) {
		case 1:
			it = find(player1Properties_.begin(), player1Properties_.end(), currentPropertyPlayer1Showed_);
			index = it - player1Properties_.begin();
			isEmpty = player1Properties_.size() == 0;
			pos = (isEmpty) ? 0 : player1Properties_[index];
			currentPropertyPlayer1Showed_ = pos;
			PROPERTY_POSITION = PROPERTY_PLAYER1_POSITION;
			break;

		case 2:
			it = find(
				player1IndexProperties_.begin(), player1IndexProperties_.end(), currentPropertyPlayer1IndexShowed_);
			index = it - player1IndexProperties_.begin();
			isEmpty = player1IndexProperties_.size() == 0;
			pos = (isEmpty) ? 0 : player1IndexProperties_[index];
			currentPropertyPlayer1IndexShowed_ = pos;
			PROPERTY_POSITION = PROPERTY_PLAYER1_INDEX_POSITION;
			break;

		case 3:
			it = find(
				player2IndexProperties_.begin(), player2IndexProperties_.end(), currentPropertyPlayer2IndexShowed_);
			index = it - player2IndexProperties_.begin();
			isEmpty = player2IndexProperties_.size() == 0;
			pos = (isEmpty) ? 0 : player2IndexProperties_[index];
			currentPropertyPlayer2IndexShowed_ = pos;
			PROPERTY_POSITION = PROPERTY_PLAYER2_INDEX_POSITION;
			break;

		case 4:
			it = find(player2Properties_.begin(), player2Properties_.end(), currentPropertyPlayer2Showed_);
			index = it - player2Properties_.begin();
			isEmpty = player2Properties_.size() == 0;
			pos = (isEmpty) ? 0 : player2Properties_[index];
			currentPropertyPlayer2Showed_ = pos;
			PROPERTY_POSITION = PROPERTY_PLAYER2_POSITION;
			break;

		default:
			return;
	}

	if (!isEmpty) {
		FieldType fieldType = std::visit([](Field& field) { return field.getType(); }, gameboard_->getFieldById(pos));
		unsigned int price;
		unsigned int Mortgage;
		unsigned int housePrice;
		unsigned int hotelPrice;
		unsigned int rents[7];
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
			new sf::Text("Price: " + std::to_string(price), getFont(), getFontSize() - 2));
		propertyPrice->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset));
		propertyPrice->setFillColor(sf::Color::Black);

		std::shared_ptr<sf::Text> propertyMortgage(new sf::Text("Mortgage:", getFont(), getFontSize() - 2));
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
			std::shared_ptr<sf::Text> propertyRent1(new sf::Text("Rent: ", getFont(), getFontSize() - 2));
			propertyRent1->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 1));
			propertyRent1->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent2(new sf::Text("  with color set:", getFont(), getFontSize() - 2));
			propertyRent2->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 2));
			propertyRent2->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent3(new sf::Text("  with 1 house:", getFont(), getFontSize() - 2));
			propertyRent3->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 3));
			propertyRent3->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent4(new sf::Text("  with 2 houses:", getFont(), getFontSize() - 2));
			propertyRent4->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 4));
			propertyRent4->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent5(new sf::Text("  with 3 houses:", getFont(), getFontSize() - 2));
			propertyRent5->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 5));
			propertyRent5->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent6(new sf::Text("  with 4 houses:", getFont(), getFontSize() - 2));
			propertyRent6->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 6));
			propertyRent6->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyRent7(new sf::Text("  with hotel:", getFont(), getFontSize() - 2));
			propertyRent7->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 7));
			propertyRent7->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyHouseCost(new sf::Text("Houses cost:", getFont(), getFontSize() - 2));
			propertyHouseCost->setPosition(sf::Vector2f(dataPos.x + 20, dataPos.y + yOffset + yOffset_step * 9));
			propertyHouseCost->setFillColor(sf::Color::Black);

			std::shared_ptr<sf::Text> propertyHotelCost(new sf::Text("Hotel cost:", getFont(), getFontSize() - 2));
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
			std::shared_ptr<sf::Text> propertyRent1(new sf::Text("Rent: ", getFont(), getFontSize() - 2));
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
			std::shared_ptr<sf::Text> propertyRent(new sf::Text("Rent:", getFont(), getFontSize() - 2));
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

		const std::string streetName = "No property";
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
			propertyPlayer1Texts_.clear();
			propertyPlayer1Texts_ = propertyPlayerTexts;
			propertyPlayer1Sprite_ = propertySprite;
			propertyPlayer1Texture_ = propertyTexture;
			break;

		case 2:
			propertyPlayer1IndexTexts_.clear();
			propertyPlayer1IndexTexts_ = propertyPlayerTexts;
			propertyPlayer1IndexSprite_ = propertySprite;
			propertyPlayer1IndexTexture_ = propertyTexture;
			break;

		case 3:
			propertyPlayer2IndexTexts_.clear();
			propertyPlayer2IndexTexts_ = propertyPlayerTexts;
			propertyPlayer2IndexSprite_ = propertySprite;
			propertyPlayer2IndexTexture_ = propertyTexture;
			break;

		case 4:
			propertyPlayer2Texts_.clear();
			propertyPlayer2Texts_ = propertyPlayerTexts;
			propertyPlayer2Sprite_ = propertySprite;
			propertyPlayer2Texture_ = propertyTexture;
			break;
	}
}

sf::Sprite& Withdraw::getSpritePropertyPlayer1() {
	return propertyPlayer1Sprite_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTextsPropertyPlayer1() {
	return propertyPlayer1Texts_;
}

sf::Sprite& Withdraw::getSpritePropertyPlayer1Index() {
	return propertyPlayer1IndexSprite_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTextsPropertyPlayer1Index() {
	return propertyPlayer1IndexTexts_;
}

sf::Sprite& Withdraw::getSpritePropertyPlayer2() {
	return propertyPlayer2Sprite_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTextsPropertyPlayer2() {
	return propertyPlayer2Texts_;
}

sf::Sprite& Withdraw::getSpritePropertyPlayer2Index() {
	return propertyPlayer2IndexSprite_;
}

std::vector<std::shared_ptr<sf::Text>>& Withdraw::getTextsPropertyPlayer2Index() {
	return propertyPlayer2IndexTexts_;
}

std::shared_ptr<sf::Texture> Withdraw::getTexturePropertyPlayer1() {
	return propertyPlayer1Texture_;
}

std::shared_ptr<sf::Texture> Withdraw::getTexturePropertyPlayer1Index() {
	return propertyPlayer1IndexTexture_;
}

std::shared_ptr<sf::Texture> Withdraw::getTexturePropertyPlayer2() {
	return propertyPlayer2Texture_;
}

std::shared_ptr<sf::Texture> Withdraw::getTexturePropertyPlayer2Index() {
	return propertyPlayer2IndexTexture_;
}

void Withdraw::addPropertyPlayerShowed(int i, unsigned int col) {
	std::vector<unsigned int> properties;
	unsigned int currentProperty;

	switch (col) {
		case 1:
			properties = player1Properties_;
			currentProperty = currentPropertyPlayer1Showed_;
			break;
		case 2:
			properties = player1IndexProperties_;
			currentProperty = currentPropertyPlayer1IndexShowed_;
			break;
		case 3:
			properties = player2IndexProperties_;
			currentProperty = currentPropertyPlayer2IndexShowed_;
			break;
		case 4:
			properties = player2Properties_;
			currentProperty = currentPropertyPlayer2Showed_;
			break;
		default:
			return;
	}

	auto it = find(properties.begin(), properties.end(), currentProperty);
	int index = it - properties.begin();

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
			currentPropertyPlayer1Showed_ = properties[index];
			break;
		case 2:
			currentPropertyPlayer1IndexShowed_ = properties[index];
			break;
		case 3:
			currentPropertyPlayer2IndexShowed_ = properties[index];
			break;
		case 4:
			currentPropertyPlayer2Showed_ = properties[index];
			break;
	}
}

void Withdraw::propertyPlayerMoveIndex(int dir, unsigned int plrNum) {
	std::vector<unsigned int> properties;
	unsigned int currentProperty;

	std::vector<unsigned int> propertiesIndex;
	unsigned int currentPropertyIndex;

	switch (plrNum) {
		case 1:
			properties = player1Properties_;
			currentProperty = currentPropertyPlayer1Showed_;
			propertiesIndex = player1IndexProperties_;
			currentPropertyIndex = currentPropertyPlayer1IndexShowed_;
			break;

		case 2:
			properties = player2Properties_;
			currentProperty = currentPropertyPlayer2Showed_;
			propertiesIndex = player2IndexProperties_;
			currentPropertyIndex = currentPropertyPlayer2IndexShowed_;
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
		defualt:
			return;
	}

	switch (plrNum) {
		case 1:
			player1Properties_ = properties;
			player1IndexProperties_ = propertiesIndex;
			if (std::find(properties.begin(), properties.end(), currentProperty) == properties.end()) {
				if (properties.size() != 0) {
					currentPropertyPlayer1Showed_ = properties[0];
				} else {
					currentPropertyPlayer1Showed_ = 0;
				}
			} else {
				currentPropertyPlayer1Showed_ = currentProperty;
			}
			if (std::find(propertiesIndex.begin(), propertiesIndex.end(), currentPropertyIndex) ==
				propertiesIndex.end()) {
				if (propertiesIndex.size() != 0) {
					currentPropertyPlayer1IndexShowed_ = propertiesIndex[0];
				} else {
					currentPropertyPlayer1IndexShowed_ = 0;
				}
			} else {
				currentPropertyPlayer1IndexShowed_ = currentPropertyIndex;
			}
			break;

		case 2:
			player2Properties_ = properties;
			player2IndexProperties_ = propertiesIndex;
			if (std::find(properties.begin(), properties.end(), currentProperty) == properties.end()) {
				if (properties.size() != 0) {
					currentPropertyPlayer2Showed_ = properties[0];
				} else {
					currentPropertyPlayer2Showed_ = 0;
				}
			} else {
				currentPropertyPlayer2Showed_ = currentProperty;
			}
			if (std::find(propertiesIndex.begin(), propertiesIndex.end(), currentPropertyIndex) ==
				propertiesIndex.end()) {
				if (propertiesIndex.size() != 0) {
					currentPropertyPlayer2IndexShowed_ = propertiesIndex[0];
				} else {
					currentPropertyPlayer2IndexShowed_ = 0;
				}
			} else {
				currentPropertyPlayer2IndexShowed_ = currentPropertyIndex;
			}
			break;
		default:
			return;
	}
}

std::shared_ptr<Button> Withdraw::getResignDecisionButton() {
	return resignDecisionButton_;
}

std::shared_ptr<Button> Withdraw::getAcceptDecisionButton() {
	return acceptDecisionButton_;
}

void Withdraw::makeWithdraw() {
	// money flow
	player1ToWithDraw_->addMoney(player2MoneyIndexBuffer_);
	player2ToWithDraw_->addMoney(player1MoneyIndexBuffer_);
	player1ToWithDraw_->substractMoney(player1MoneyIndexBuffer_);
	player2ToWithDraw_->substractMoney(player2MoneyIndexBuffer_);

	// properties flow
	auto it = find(player1IndexProperties_.begin(), player1IndexProperties_.end(), 0);
	int index;

	// 1 -> 2
	for (auto property : player1IndexProperties_) {
		it = find(player1IndexProperties_.begin(), player1IndexProperties_.end(), property);
		index = it - player1IndexProperties_.begin();
		player2ToWithDraw_->addFieldOwnedId(property);
		player1ToWithDraw_->removeFiledOwnedId(property);
		FieldType type = std::visit(
			[](Field& visited_field) { return visited_field.getType(); }, gameboard_->getFieldById(property));
		switch (type) {
			case STREET: {
				StreetField& visited_field = std::get<StreetField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player2ToWithDraw_);
				break;
			}

			case UTILITY: {
				UtilityField& visited_field = std::get<UtilityField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player2ToWithDraw_);
				break;
			}

			case STATION: {
				StationField& visited_field = std::get<StationField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player2ToWithDraw_);
				break;
			}
		}
	}
	// 2 -> 1
	for (auto property : player2IndexProperties_) {
		it = find(player2IndexProperties_.begin(), player2IndexProperties_.end(), property);
		index = it - player2IndexProperties_.begin();
		player1ToWithDraw_->addFieldOwnedId(property);
		player2ToWithDraw_->removeFiledOwnedId(property);
		FieldType type = std::visit(
			[](Field& visited_field) { return visited_field.getType(); }, gameboard_->getFieldById(property));
		switch (type) {
			case STREET: {
				StreetField& visited_field = std::get<StreetField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player1ToWithDraw_);
				break;
			}

			case UTILITY: {
				UtilityField& visited_field = std::get<UtilityField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player1ToWithDraw_);
				break;
			}

			case STATION: {
				StationField& visited_field = std::get<StationField>(gameboard_->getFieldById(property));
				visited_field.setOwner(player1ToWithDraw_);
				break;
			}
		}
	}
}

bool Withdraw::isNonZeroValue() {
	if (ALLOW_ONE_SIDE_BENEFIT_WITHDRAW) {
		if (player1MoneyIndexBuffer_ != 0) {
			return true;
		} else if (player2MoneyIndexBuffer_ != 0) {
			return true;
		} else if (player1IndexProperties_.size() != 0) {
			return true;
		} else if (player2IndexProperties_.size() != 0) {
			return true;
		}
	} else {
		if ((player1MoneyIndexBuffer_ != 0 || player1IndexProperties_.size() != 0) &&
			(player2MoneyIndexBuffer_ != 0 || player2IndexProperties_.size() != 0)) {
			return true;
		}
	}

	return false;
}