# ENGG1340 - Battleship Game
This project is a console-based implementation of the classic Battleship game with multiple features, created for HKU's ENGG1340. Players can play against an AI opponent with different difficulty levels, challenge another player in local multiplayer mode, track scores on a leaderboard, and register as users to save their progress. The game follows standard Battleship rules where players place ships on a grid and take turns guessing coordinates to sink their opponent's ships. The first player to sink all of their opponent's ships wins.

## Team Members
- Ilyas - Interface Design (Main menu and animations)
- Aleksandr Vaskin - Multiplayer implementation
- Yerassyl Taisarinov - AI/Computer player algorithm and gameplay mechanics
- Altynai - AI/Computer player algorithm and main gameplay structure
- Milan - Authentication, Scoreboard

## Features and Implementation

### 1. Generation of Random Events
- **Random Ship Placement**: The AI opponent randomly places ships on the board when starting a new game.
- **AI Move Generation**: At lower difficulty levels, the AI makes random moves when attacking the player's board.
- **Supported by**: Various randomized functions in `battleship.cpp`, such as `placeRandomShips()` which utilizes random number generation to place ships on the board.

### 2. Data Structures for Storing Data
- **2D Arrays**: The game uses 2D arrays to represent the game boards for both players.
- **User Information**: Hash maps (`unordered_map`) store user information including usernames, password hashes, and scores.
- **Leaderboard**: A vector of pointers to User objects maintains the leaderboard.
- **Attack Records**: A vector of custom structs tracks the AI's attack patterns at higher difficulty levels.
- **Supported by**: Data structures defined in `battleship.h`, `users.h`, and used throughout the codebase.

### 3. Dynamic Memory Management
- **User Management**: Pointers to User objects are used in the leaderboard and for logged-in players.
- **AI Algorithm**: The attack logic for harder difficulty levels uses dynamic memory allocation to track successful hits and plan subsequent moves.
- **Supported by**: Dynamic allocation in `multiplayer.cpp` and user management in `users.cpp`.

### 4. File Input/Output
- **User Database**: Player information is stored in and loaded from a database file, while the passwords are hashed for security reasons.
- **Saving Game State**: The game saves user scores and rankings to persistent storage.
- **Supported by**: File operations in `users.cpp`, particularly in the `buildUsers()` and `saveUsers()` functions.

### 5. Program Codes in Multiple Files
The project is organized into multiple directories and files:
- **Main Game Logic**: `battleship.cpp/h` in the 1340_Group_Project directory
- **User Interface**: `main_menu.cpp/h` at the root level
- **Data Handling**: `users.cpp/h` and `constants.h` in the DataHandling directory
- **Multiplayer**: `multiplayer.cpp/h` in the Multiplayer directory

### Multiple Difficulty Levels
The game features four difficulty levels for the AI opponent:
1. **Easy**: Random targeting with no strategy
2. **Normal**: Basic targeting with simple logic
3. **Medium**: Advanced targeting with ship-hunting algorithm
4. **Hard**: Sophisticated algorithm that uses probability maps to predict ship locations

## C/C++ Libraries
This project only uses standard C++ libraries, including:
- `<iostream>` - For input/output operations
- `<string>` - For string manipulation
- `<vector>` - For dynamic arrays
- `<unordered_map>` - For hash maps (user data)
- `<fstream>` - For file operations
- `<cstdlib>` - For random number generation and system commands
- `<ctime>` - For seeding random number generation
- `<algorithm>` - For sorting and other algorithms
- `<iomanip>` - For output formatting

## Compilation and Execution Instructions

### Compilation
You can compile the game using the included Makefile:

```bash
make start
```

Or compile manually with the following command:

```bash
g++ -pedantic-errors -std=c++11 main_menu.cpp Multiplayer/multiplayer.cpp DataHandling/users.cpp 1340_Group_Project/battleship.cpp -o main_menu
```

### Execution
To run the game after compilation:

```bash
./main_menu
```

### Gameplay Instructions
1. From the main menu, select an option (1-5):
   - 1: Play against AI (select difficulty level)
   - 2: View scoreboard
   - 3: Play multiplayer mode
   - 4: Register a new user
   - 5: Exit the game

2. For multiplayer mode:
   - Both players need to log in or register
   - Players take turns placing ships on their boards
   - Players take turns attacking each other's boards

3. For single-player mode:
   - Select a difficulty level
   - Place your ships on the board
   - Take turns with the AI until one player **wins**