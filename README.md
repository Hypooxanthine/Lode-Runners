# Lode-Runners
This project is a university assignment. I'm planning to use threads, sockets in a replication-model implementation, IA-controled  entities, level editor and loader. I'll try to make a matchmaking system on a threaded server, with registered users and some highscores table.

EDIT : matchmaking server will take too much time. It could be a great improvement in the future.

## To-do list

1. Main features
  - [DONE] End-game checking system.
  - [DONE] Level end ladder spawn.
  - Back-to-lobby system (at the end of the game).
  - Scores saving system with leaderboard (another project/application ?).
  - Settings
    - Skin selector
2. Bug fixes
  - [DONE] Prevent people from connecting to the server if it is full. Currently clients can connect, but server doesn't register them. So server won't send anything to these unregistered clients, and they will just wait from something that will never happen.
  - Overlapping issue when an entity is standing above and between a solid and a transparent tile.
  - [DONE] Score replication has to be fixed.
  - Fix behaviour when server / a client leaves in GameState.
3. Improvements
  - [DONE] Implement a timestep in Networker threads to free some CPU time
  - Text in widgets should be resized with SFML scaling tools, not with setCharacterSize() (unaccurate method)

## Screenshots
![Alt text](/screenshots/MainMenu_0.png?raw=true)
![Alt text](/screenshots/LevelEditor_0.png?raw=true)
![Alt text](/screenshots/PreLobby_0.png?raw=true)
![Alt text](/screenshots/Game_0.png?raw=true)
