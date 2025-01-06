# CSC1103Assignment: Tic Tac Toe

## Overview

This project is a sophisticated implementation of the classic Tic Tac Toe game with multiple gameplay options and advanced features, such as:

- Single-player mode with AI.
- Two-player local mode.
- AI using the Minimax algorithm.
- Machine learning components for strategy testing.
- GUI built with GTK for a polished user experience.

The project is designed as a learning and experimentation platform for algorithm design and GUI programming.

---

## Features

### Game Modes

1. **Single-Player Mode**: Play against the computer with three difficulty levels:
   - Easy
   - Medium
   - Hard (uses Minimax with alpha-beta pruning).
2. **Two-Player Mode**: Play against another player locally.

### AI Implementation

- **Minimax Algorithm**:
  - Strategically evaluates all possible moves.
  - Implements alpha-beta pruning for efficiency.

### Machine Learning

- **Strategy Testing**:
  - Uses data-driven approaches to analyze and evaluate game outcomes.
- **Win Rate Testing**:
  - Benchmarks AI strategies by simulating games.

### GUI

- Built with GTK for a seamless and interactive user experience.
- Includes elements like buttons, labels, and a game grid.

---

## Installation and Setup

### Prerequisites

1. **GTK Development Libraries**:
   Ensure GTK is installed on your system. On Linux, you can install it using:
   ```sh
   sudo apt-get install libgtk-3-dev
   ```
2. **C Compiler**:
   - GCC or any compatible compiler.

### Steps

1. Clone the repository:
   ```sh
   git clone <repository-url>
   ```
2. Navigate to the project directory:
   ```sh
   cd CSC1103Assignment-main
   ```
3. Build the project using the `makefile`:
   ```sh
   make
   ```
4. Run the application:
   ```sh
   ./tictactoe
   ```

---

## Code Structure

### Files

- **`tictactoe_gameops.c`**: Handles core game logic and grid updates.
- **`tictactoe_gui.c`**: Manages the graphical interface.
- **`tictactoe_main.c`**: Initializes and coordinates the overall game flow.
- **`tictactoe_minimax.c`**: Implements the Minimax algorithm for AI.
- **`tictactoe_ml.c`**: Contains machine learning logic for strategy evaluation.
- **`tictactoe_twoplayers.c`**: Handles two-player game logic.
- **`tictactoe_winrate.c`**: Automates AI benchmarking and win-rate analysis.
- **`tictactoe.h`**: Header file with shared definitions and function prototypes.

### Key Components

1. **Game Logic**:
   - Resetting the game.
   - Handling player moves.
2. **AI**:
   - Intelligent decision-making with Minimax.
3. **Machine Learning**:
   - Data allocation and strategy comparison.
4. **GUI**:
   - Creating and managing interactive elements.

---

## Usage

1. **Launching the Game**:
   - Follow the installation steps and run `./tictactoe`.
2. **Gameplay**:
   - Select single or two-player mode.
   - For single-player, choose difficulty.
   - Play moves by clicking the grid.
3. **Testing AI**:
   - Modify and run `tictactoe_winrate.c` for custom benchmarks.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Acknowledgments

This project is part of the CSC1103 coursework, designed to enhance skills in algorithms, AI, and GUI programming.

