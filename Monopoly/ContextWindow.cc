/**
 * @file ContextWindow.cc
 *
 * @brief Source file for context Window class
 * It is Singleton class type used mainly for handling SFML window operations
 * between other classes
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include "ContextWindow.h"

ContextWindow* ContextWindow::contextWindow_ = nullptr;

ContextWindow::ContextWindow(){};

ContextWindow* ContextWindow::GetInstance() {
	if (contextWindow_ == nullptr) {
		contextWindow_ = new ContextWindow();
	}
	return contextWindow_;
}

void ContextWindow::display() {
	getWindow().display();
}

void ContextWindow::clear() {
	getWindow().clear();
}

bool ContextWindow::isOpen() {
	return getWindow().isOpen();
}

sf::RenderWindow& ContextWindow::getWindow() {
	return window_;
}

sf::View& ContextWindow::getView() {
	return view_;
}