#ifndef CONTEXT_WINDOW_H
#define CONTEXT_WINDOW_H

#include <SFML/Graphics.hpp>

class ContextWindow {
   private:
	ContextWindow();
	static ContextWindow* contextWindow_;

   public:
	ContextWindow(ContextWindow& other) = delete;
	void operator=(const ContextWindow&) = delete;
	static ContextWindow* GetInstance();

	sf::RenderWindow window_;
	sf::View view_;

	void display();
	void clear();
	bool isOpen();

	sf::RenderWindow& getWindow();
	sf::View& getView();
};

#endif