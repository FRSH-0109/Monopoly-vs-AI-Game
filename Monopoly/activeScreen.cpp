#include "activeScreen.h"

ActiveScreen::ActiveScreen() {}
ActiveScreen::~ActiveScreen() {}

void ActiveScreen::setFont(sf::Font font) {
	font_ = font;
}

sf::Font& ActiveScreen::getFont() {
	return font_;
}

void ActiveScreen::addButton(std::shared_ptr<Button> buttonTmp) {
	std::shared_ptr<Button> button = buttonTmp;
	buttons_.push_back(button);
}

void ActiveScreen::addText(std::shared_ptr<sf::Text> textTmp) {
	std::shared_ptr<sf::Text> text = textTmp;
	texts_.push_back(text);
}

void ActiveScreen::draw() {
	for (auto element : buttons_) {
		element->draw(getContextWindow()->getWindow());
	}

	for (auto element : texts_) {
		getContextWindow()->getWindow().draw(*element);
	}
}

void ActiveScreen::setContextWindow(ContextWindow* cw) {
	contextWindow_ = cw;
}

ContextWindow* ActiveScreen::getContextWindow() {
	return contextWindow_;
}

ActiveScreenType ActiveScreen::getScreenType() {
	return type_;
}

void ActiveScreen::setScreenType(ActiveScreenType type) {
	type_ = type;
}

MainMenuScreen::MainMenuScreen() {
	std::cout << "MMS constructor" << std::endl;
	setScreenType(MainMenu);
	mainMenuCreate();
}

MainMenuScreen::~MainMenuScreen() {
	std::cout << "MMS deconstructor" << std::endl;
}

void MainMenuScreen::mainMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonExit(new Button(Exit, "Exit", {200, 100}, 30));
	buttonExit->setFont(getFont());
	buttonExit->setPosition({getContextWindow()->getWindow().getSize().x / 2.0f, 300});
	buttonExit->setMainBackColor(sf::Color::Red);
	buttonExit->setMainTextColor(sf::Color::Black);
	buttonExit->setFocusBackColor(sf::Color::Black);
	buttonExit->setFocusTextColor(sf::Color::Red);

	std::shared_ptr<Button> buttonPlay(new Button(Play, "Play", {200, 100}, 30));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition({getContextWindow()->getWindow().getSize().x / 2.0f, 100});
	buttonPlay->setMainBackColor(sf::Color::Red);
	buttonPlay->setMainTextColor(sf::Color::Black);
	buttonPlay->setFocusBackColor(sf::Color::Black);
	buttonPlay->setFocusTextColor(sf::Color::Red);

	addButton(buttonExit);
	addButton(buttonPlay);

	for (auto element : getButtons()) {
		element->setIsClicked(false);
	}
}

ScreenEventType MainMenuScreen::worker() {
	ScreenEventType eventType = Idle;
	for (auto element : getButtons()) {
		if (element->getIsClicked()) {
			element->setIsClicked(false);
			return element->getEventType();
		}
	}
	return eventType;
}

ScreenEventType GameMenuScreen::worker() {
	ScreenEventType eventType = Idle;
	for (auto element : getButtons()) {
		if (element->getIsClicked()) {
			element->setIsClicked(false);
			return element->getEventType();
		}
	}
	return eventType;
}

GameMenuScreen::GameMenuScreen() {
	std::cout << "GMS constructor" << std::endl;
	setScreenType(GameMenu);
	gameMenuCreate();
}

GameMenuScreen::~GameMenuScreen() {
	std::cout << "GMS deconstructor" << std::endl;
}

void GameMenuScreen::gameMenuCreate() {
	setContextWindow(ContextWindow::GetInstance());

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	std::shared_ptr<Button> buttonPlay(new Button(StartGame, "Start game", {200, 100}, 30));
	buttonPlay->setFont(getFont());
	buttonPlay->setPosition({(getContextWindow()->getWindow().getSize().x / 2.0f), 100});
	buttonPlay->setMainBackColor(sf::Color::Red);
	buttonPlay->setMainTextColor(sf::Color::Black);
	buttonPlay->setFocusBackColor(sf::Color::Black);
	buttonPlay->setFocusTextColor(sf::Color::Red);

	std::shared_ptr<Button> buttonReturn(new Button(ReturnToMainMenu, "Return", {200, 100}, 30));
	buttonReturn->setFont(getFont());
	buttonReturn->setPosition({(getContextWindow()->getWindow().getSize().x / 2.0f), 700});
	buttonReturn->setMainBackColor(sf::Color::Red);
	buttonReturn->setMainTextColor(sf::Color::Black);
	buttonReturn->setFocusBackColor(sf::Color::Black);
	buttonReturn->setFocusTextColor(sf::Color::Red);

	addButton(buttonReturn);
	addButton(buttonPlay);

	for (auto element : getButtons()) {
		element->setIsClicked(false);
		element->setIsVisible(true);
	}

	sf::Vector2f col1(getContextWindow()->getWindow().getSize().x / 2.0f - 300, 200);
	sf::Vector2f col2(getContextWindow()->getWindow().getSize().x / 2.0f - 100, 200);
	sf::Vector2f col3(getContextWindow()->getWindow().getSize().x / 2.0f + 100, 200);
	sf::Vector2f col4(getContextWindow()->getWindow().getSize().x / 2.0f + 300, 200);
	int y_step = 100;

	std::shared_ptr<sf::Text> Player1Text(new sf::Text("Player 1", getFont(), 30));
	Player1Text->setOrigin(Player1Text->getGlobalBounds().getSize() / 2.f +
						   Player1Text->getLocalBounds().getPosition());  // text origin at center
	Player1Text->setPosition(col1);
	Player1Text->setColor(sf::Color::Black);
	addText(Player1Text);

	std::shared_ptr<Button> buttonPlayer1SetHuman(new Button(AddPlayer, "Human", {150, 100}, 30));
	buttonPlayer1SetHuman->setFont(getFont());
	buttonPlayer1SetHuman->setPosition(sf::Vector2f(col1.x, col1.y + y_step));
	buttonPlayer1SetHuman->setMainBackColor(sf::Color::Green);
	buttonPlayer1SetHuman->setMainTextColor(sf::Color::Black);
	buttonPlayer1SetHuman->setFocusBackColor(sf::Color::Black);
	buttonPlayer1SetHuman->setFocusTextColor(sf::Color::Green);
	addButton(buttonPlayer1SetHuman);

	std::shared_ptr<Button> buttonPlayer1SetAI(new Button(AddPlayer, "AI", {150, 100}, 30));
	buttonPlayer1SetAI->setFont(getFont());
	buttonPlayer1SetAI->setPosition(sf::Vector2f(col1.x, col1.y + y_step * 2));
	buttonPlayer1SetAI->setMainBackColor(sf::Color::White);
	buttonPlayer1SetAI->setMainTextColor(sf::Color::Black);
	buttonPlayer1SetAI->setFocusBackColor(sf::Color::Black);
	buttonPlayer1SetAI->setFocusTextColor(sf::Color::White);
	addButton(buttonPlayer1SetAI);

	std::shared_ptr<sf::Text> Player1TextAILevel(new sf::Text("AI level", getFont(), 30));
	Player1TextAILevel->setOrigin(Player1TextAILevel->getGlobalBounds().getSize() / 2.f +
								  Player1TextAILevel->getLocalBounds().getPosition());	// text origin at center
	Player1TextAILevel->setPosition(sf::Vector2f(col1.x, col1.y + y_step * 3));
	Player1TextAILevel->setColor(sf::Color::Black);
	addText(Player1TextAILevel);

	std::shared_ptr<Button> buttonPlayer1SetAILevel1(new Button(AddPlayer, "1", {50, 100}, 30));
	buttonPlayer1SetAILevel1->setFont(getFont());
	buttonPlayer1SetAILevel1->setPosition(sf::Vector2f(col1.x - 50, col1.y + y_step * 4));
	buttonPlayer1SetAILevel1->setMainBackColor(sf::Color::Green);
	buttonPlayer1SetAILevel1->setMainTextColor(sf::Color::Black);
	buttonPlayer1SetAILevel1->setFocusBackColor(sf::Color::Black);
	buttonPlayer1SetAILevel1->setFocusTextColor(sf::Color::Red);
	addButton(buttonPlayer1SetAILevel1);

	std::shared_ptr<Button> buttonPlayer1SetAILevel2(new Button(AddPlayer, "2", {50, 100}, 30));
	buttonPlayer1SetAILevel2->setFont(getFont());
	buttonPlayer1SetAILevel2->setPosition(sf::Vector2f(col1.x, col1.y + y_step * 4));
	buttonPlayer1SetAILevel2->setMainBackColor(sf::Color::White);
	buttonPlayer1SetAILevel2->setMainTextColor(sf::Color::Black);
	buttonPlayer1SetAILevel2->setFocusBackColor(sf::Color::Black);
	buttonPlayer1SetAILevel2->setFocusTextColor(sf::Color::Red);
	addButton(buttonPlayer1SetAILevel2);

	std::shared_ptr<Button> buttonPlayer1SetAILevel3(new Button(AddPlayer, "3", {50, 100}, 30));
	buttonPlayer1SetAILevel3->setFont(getFont());
	buttonPlayer1SetAILevel3->setPosition(sf::Vector2f(col1.x + 50, col1.y + y_step * 4));
	buttonPlayer1SetAILevel3->setMainBackColor(sf::Color::White);
	buttonPlayer1SetAILevel3->setMainTextColor(sf::Color::Black);
	buttonPlayer1SetAILevel3->setFocusBackColor(sf::Color::Black);
	buttonPlayer1SetAILevel3->setFocusTextColor(sf::Color::Red);
	addButton(buttonPlayer1SetAILevel3);

	std::shared_ptr<sf::Text> Player2Text(new sf::Text("Player 2", getFont(), 30));
	Player2Text->setOrigin(Player2Text->getGlobalBounds().getSize() / 2.f +
						   Player2Text->getLocalBounds().getPosition());  // text origin at center
	Player2Text->setPosition(col2);
	Player2Text->setColor(sf::Color::Black);
	addText(Player2Text);

	std::shared_ptr<Button> buttonPlayer2SetHuman(new Button(AddPlayer, "Human", {150, 100}, 30));
	buttonPlayer2SetHuman->setFont(getFont());
	buttonPlayer2SetHuman->setPosition(sf::Vector2f(col2.x, col2.y + y_step));
	buttonPlayer2SetHuman->setMainBackColor(sf::Color::Red);
	buttonPlayer2SetHuman->setMainTextColor(sf::Color::Black);
	buttonPlayer2SetHuman->setFocusBackColor(sf::Color::Black);
	buttonPlayer2SetHuman->setFocusTextColor(sf::Color::Red);
	addButton(buttonPlayer2SetHuman);

	std::shared_ptr<Button> buttonPlayer2SetAI(new Button(AddPlayer, "AI", {150, 100}, 30));
	buttonPlayer2SetAI->setFont(getFont());
	buttonPlayer2SetAI->setPosition(sf::Vector2f(col2.x, col2.y + y_step * 2));
	buttonPlayer2SetAI->setMainBackColor(sf::Color::White);
	buttonPlayer2SetAI->setMainTextColor(sf::Color::Black);
	buttonPlayer2SetAI->setFocusBackColor(sf::Color::Black);
	buttonPlayer2SetAI->setFocusTextColor(sf::Color::Red);
	addButton(buttonPlayer2SetAI);

	std::shared_ptr<sf::Text> Player3Text(new sf::Text("Player 3", getFont(), 30));
	Player3Text->setPosition((getContextWindow()->getWindow().getSize().x / 2.0f) + 100, 400);
	Player3Text->setColor(sf::Color::Black);
	addText(Player3Text);

	std::shared_ptr<sf::Text> Player4Text(new sf::Text("Player 4", getFont(), 30));
	Player4Text->setPosition((getContextWindow()->getWindow().getSize().x / 2.0f) + 300, 500);
	Player4Text->setColor(sf::Color::Black);
	addText(Player4Text);
}

std::vector<std::shared_ptr<Button>>& ActiveScreen::getButtons() {
	return buttons_;
}

void GameMenuScreen::pollForEvents(sf::Event& event) {
	switch (event.type) {
		case sf::Event::MouseMoved:
			for (auto element : getButtons()) {
				if (element->isMouseOver(getContextWindow()->getWindow())) {
					element->mouseIsOver();
				} else {
					element->mouseIsNotOver();
				}
			}
			break;
		case sf::Event::MouseButtonPressed:
			for (auto element : getButtons()) {
				if (element->isMouseOver(getContextWindow()->getWindow())) {
					element->setIsClicked(true);
				}
			}
			break;
	}
}

void MainMenuScreen::pollForEvents(sf::Event& event) {
	switch (event.type) {
		case sf::Event::MouseMoved:
			for (auto element : getButtons()) {
				if (element->isMouseOver(getContextWindow()->getWindow())) {
					element->mouseIsOver();
				} else {
					element->mouseIsNotOver();
				}
			}
			break;
		case sf::Event::MouseButtonPressed:
			for (auto element : getButtons()) {
				if (element->isMouseOver(getContextWindow()->getWindow())) {
					element->setIsClicked(true);
				}
			}
			break;
	}
}