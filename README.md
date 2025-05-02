# ENGG1340 - Battleship Game
This project is a console-based implementation of the classic Battleship game with multiple features, created for HKU's ENGG1340. Players can play against an AI opponent with different difficulty levels, challenge another player in local multiplayer mode, track scores on a leaderboard, and register as users to save their progress. The game follows standard Battleship rules where players place ships on a grid and take turns guessing coordinates to sink their opponent's ships. The first player to sink all of their opponent's ships wins.

## Team Members
- Ilyas - Interface Design (Main menu and animations)
- Aleksandr Vaskin - Multiplayer implementation
- Yerassyl Taisarinov - AI/Computer player algorithm and gameplay mechanics
- Altynai - AI/Computer player algorithm and main gameplay structure
- Milan - Authentication, Scoreboard

## How to play?

### Compilation
You can compile and run the game using the included Makefile:

```bash
make start
```

In case of an eror, run

```bash
make clean
```

Then run the first instruction again.

### Options
From the main menu, select an option (1-5):

![Main Menu Screenshot](main_menu.png)

#### 1. Versus AI:
   - Select a difficulty level
   - Place your ships on the board
   - Take turns with the AI until one player **wins**

#### 2. Scoreboard:
  - Shows the score of each player that is saved in the database
  - Each player gets a starting score of 600

#### 3. Multiplayer mode:
   - Both players need to log in or register
   - Players take turns placing ships on their boards
   - Players take turns attacking each other's boards
   - You see **two boards** at the same time:
     - Your *target* board (where you are shooting)
     - Your *own* board (where your opponent is shooting)

> **IMPORTANT:** after placing your ships, you won't see them for the rest of the game. This is done because you are playing with a friend sitting next to you, and if we show your ships, your opponent will see them as well, which we obviously do not want :) 

#### 4. Registration
  - Register for an account which you will use to login during the multiplayer game mode


## Implementation

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