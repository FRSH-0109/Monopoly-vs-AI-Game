#include "contextWindow.h"

ContextWindow *ContextWindow::contextWindow_ = nullptr;

ContextWindow::ContextWindow(){};

ContextWindow *ContextWindow::GetInstance() {
  if (contextWindow_ == nullptr) {
    contextWindow_ = new ContextWindow();
  }
  return contextWindow_;
}

void ContextWindow::display() { this->window_.display(); }

void ContextWindow::clear() { this->window_.clear(); }

bool ContextWindow::isOpen() const { return this->window_.isOpen(); }