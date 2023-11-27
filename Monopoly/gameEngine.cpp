#include "gameEngine.h"

GameEngine::GameEngine(double frameRateHz, uint WindowWidth, uint WindowHeight) {
  this->windowWidth_ = WindowWidth;
  this->windowHeight_ = WindowHeight;
  
  this->frameRateHz_ = frameRateHz;
  this->frameRateDelayMs_ = sf::milliseconds(1000.0 / this->frameRateHz_);
  this->activeScreen_ = MainMenu;

  this->contextWindow_ = ContextWindow::GetInstance();
  this->contextWindow_->window_.create(sf::VideoMode(WindowWidth, WindowHeight),
                                       "MonopolyVsAI", sf::Style::Default);
  this->contextWindow_->view_ = this->contextWindow_->window_.getDefaultView();
}

void GameEngine::clear() { this->contextWindow_->window_.clear(sf::Color::White); }

void GameEngine::display() {

  static sf::Clock clock_frames;
  static sf::Time timeElapsedFromLastFrame_ms = sf::milliseconds(0);

  timeElapsedFromLastFrame_ms = clock_frames.getElapsedTime();
  if (timeElapsedFromLastFrame_ms >= this->frameRateDelayMs_) {
    clock_frames.restart();
    this->contextWindow_->display();
  }
}

void GameEngine::pollForEvents(sf::Event &event) {
  switch (event.type) {
  case sf::Event::Closed:
    this->contextWindow_->window_.close();
    break;
  case sf::Event::Resized:
    // resize my view
    this->contextWindow_->view_.setSize(
        {static_cast<float>(event.size.width),
         static_cast<float>(event.size.height)});
    this->contextWindow_->window_.setView(this->contextWindow_->view_);
    // and align shape
    break;
  }
}

void GameEngine::worker() {

  this->menuCreate();

  while (this->contextWindow_->isOpen()) {

    this->clear();

    sf::Event event;
    while (this->contextWindow_->window_.pollEvent(event)) {

      this->pollForEvents(event);
      switch (this->activeScreen_) {
      case MainMenu:
        this->getMenu().pollForEvents(event);
        break;
      case Game:

        break;
      }
    }

    switch (this->activeScreen_) {
    case MainMenu:
      this->getMenu().draw();
      break;
    case Game:

      break;
    }
    this->display();
  }
}

void GameEngine::menuCreate() { this->mainMenu_.create(); }

Menu &GameEngine::getMenu() { return this->mainMenu_; }

uint GameEngine::getWindowWidth() const{
  return this->windowWidth_;
}
uint GameEngine::getWindowHeight() const{
  return this->windowHeight_;
}
