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

// =============================================================================
// ContextWindow Class Implementation
// =============================================================================

ContextWindow* ContextWindow::context_window_ = nullptr;

ContextWindow::ContextWindow(){};

ContextWindow* ContextWindow::GetInstance() {
	if (context_window_ == nullptr) {
		context_window_ = new ContextWindow();
	}
	return context_window_;
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