#include "gameEngine.hpp"
#include "button.hpp"

GameEngine::GameEngine()
{
    
}

void GameEngine::createWindow()
{
    this->window_.create(sf::VideoMode(1000, 1000), "MonopolyVsAI", sf::Style::Default);
    view_ = this->window_.getDefaultView();
} 

void GameEngine::pollForEvents()
{
    sf::Event event;
    while (this->window_.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                this->window_.close();
            break;
            case sf::Event::Resized: 
                // resize my view
                this->view_.setSize({
                        static_cast<float>(event.size.width),
                        static_cast<float>(event.size.height)
                });
                this->window_.setView(this->view_);
                // and align shape
            break;
        }
    }
}

void GameEngine::menu()
{

}

//https://termspar.wordpress.com/2019/04/11/c-sfml-textbox-and-button-classes/

void GameEngine::worker()
{
    sf::Font font;
	if (!font.loadFromFile("HAPPYMASS.ttf"))
		std::cout << "Font not found!\n";

    Button btn1("Enter", { 200, 100 }, 30, sf::Color::Green, sf::Color::Black);
	btn1.setFont(font);
	btn1.setPosition({ 100, 300 });

    sf::CircleShape circle(200.f);
    circle.setFillColor(sf::Color::Green);

    while (this->window_.isOpen())
    {
        this->pollForEvents();

        this->window_.clear();

        btn1.drawTo(this->window_);
        //this->window_.draw(circle);

        this->window_.display();
    }
}
