// class Button {
// public:
//     Button (sf::Image* normal,sf::Image* clicked,std::string,sf::Vector2f location);
//     void checkClick (sf::Vector2f);
//     void setState(bool);
//     void setText(std::string);
//     bool getVar();
//     sf::Sprite* getSprite();
//     sf::String * getText();
// private:
//     sf::Sprite normal;
//     sf::Sprite clicked;
//     sf::Sprite* currentSpr;
//     sf::String String;
//     bool current;
// };

// Button::Button(sf::Image* normal,sf::Image* clicked,std::string words,sf::Vector2f location) {
//     this->normal.SetImage(*normal);
//     this->clicked.SetImage(*clicked);
//     this->currentSpr=&this->normal;
//     current =false;
//     this->normal.SetPosition(location);
//     this->clicked.SetPosition(location);
//     String.SetText(words);
//     String.SetPosition(location.x+3,location.y+3);
//     String.SetSize(14);
// }
// void Button::checkClick (sf::Vector2f mousePos) {
//     if (mousePos.x>currentSpr->GetPosition().x && mousePos.x<(currentSpr->GetPosition().x + currentSpr->GetSize().x)) {
//         if(mousePos.y>currentSpr->GetPosition().y && mousePos.y<(currentSpr->GetPosition().y + currentSpr->GetSize().y)) {
//             setState(!current);
//         }
//     }
// }
// void Button::setState(bool which) {
//     current = which;
//     if (current) {
//         currentSpr=&clicked;
//         return;
//     }
//     currentSpr=&normal;
// }
// void Button::setText(std::string words) {
//     String.SetText(words);
// }
// bool Button::getVar() {
//     return current;
// }
// sf::Sprite* Button::getSprite() {
//     return currentSpr;
// }

// sf::String * Button::getText() {
//     return &String;
// }

#include "SFML/Graphics.hpp"

#include "gameEngine.hpp"

int main()
{
    GameEngine gameEngine;

    gameEngine.createWindow();

    gameEngine.worker();

    return 0;
}