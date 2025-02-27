# Monopoly vs AI: A C++ AI-Powered Monopoly Game

## 🎮 Overview

**Monopoly vs AI** is a C++ implementation of the classic Monopoly board game, where a human player can face off against a strategic AI opponent or other human players. The game follows the traditional rules and includes an intelligent AI capable of making strategic decisions. This project aims to provide a fun and challenging experience while demonstrating proficiency in using **C++ with SFML library**.

---

![alt text](docs/gameplay_ss.jpg?raw=true)


## 🚀 Features

✔️ **Nearly all Monopoly Rules**: Classic Monopoly mechanics with buying, trading, and property management.

✔️ **AI Opponents**: A strategic AI that simulates intelligent gameplay.

✔️ **Human Opponents**: Local friend player that simulates a bit less intelligent gameplay.

✔️ **Customizable Settings**: Adjust AI difficulty and other gameplay settings.

---

## 🚀 Installation

### Prerequisites

- C++ compiler (e.g., g++, clang++)
- Make as build tool
- SFML v.2.6.0 - Graphic library used in games and multimedia projects
- Catch v.2 - Unit test framework used for testing our code

### Steps to Install

1. **Clone the repository**:
   ```bash
   git clone https://github.com/FRSH-0109/Monopoly-vs-AI-Game.git
   ```
2. **Navigate to the project directory:**
   ```bash
    cd Monopoly-vs-AI-Game
   ```
3. **Build the project:**
   ```bash
   make
   ```
4. **Start the game:**
   ```bash
   sh monopolyVsAI.sh
   ```

> **Note:** Ensure you have the SFML library (v2.6.0) installed on your machine!.

---

### Unit tests
Unit testing framework - Catch2.

1. **Tests framework build**:
   ```bash
   make testsConfig
   ```
2. **Unit tests build**
   ```bash
    make tests
   ```
3. **Run test**
   ```bash
   sh monopolyTests
   ```

---

## 🎮 How to Play

🎯 **Objective:** Guide your frog from the bottom to the top of the screen, avoiding obstacles and hazards.

- Players: From 1 to 4 human players faces off against the AI.
- Game Flow: Players roll dice, buy properties, and make strategic decisions to defeat other players.
- AI Difficulty: The game includes multiple difficulty levels for the AI.

---

## 🤝 Contributing

Contributions are welcome! To contribute:

1. **Fork** the repository.
2. **Create a new branch:**
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes and commit them:**
   ```bash
   git commit -m 'Add some feature'
   ```
4. **Push to the branch:**
   ```bash
   git push origin feature/your-feature-name
   ```
5. **Open a Pull Request.**

📌 Please ensure your code follows the project's coding standards and includes appropriate tests.

---

## 📜 License

This project is licensed under the **[MIT License](LICENSE)**.

---

## 🙌 Acknowledgements

🎮 Monopoly is an iconic game, and this project is inspired by the classic board game version.

---

🚀 **Enjoy the game and happy coding!** 🐸🎉


