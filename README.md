# Minigin

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

# Goal

Minigin can/may be used as a start project for the exam assignment in the course [Programming 4](https://youtu.be/j96Oh6vzhmg) at DAE. In that assignment students need to recreate a popular 80's arcade game with a game engine they need to program themselves. During the course we discuss several game programming patterns, using the book '[Game Programming Patterns](https://gameprogrammingpatterns.com/)' by [Robert Nystrom](https://github.com/munificent) as reading material. 

# Disclaimer

Minigin is, despite perhaps the suggestion in its name, **not** a game engine. It is just a very simple sdl2 ready project with some of the scaffolding in place to get started. None of the patterns discussed in the course are used yet (except singleton which use we challenge during the course). It is up to the students to implement their own vision for their engine, apply patterns as they see fit, create their game as efficient as possible.

# Use

Either download the latest release of this project and compile/run in visual studio or, since students need to have their work on github too, they can use this repository as a template (see the "Use this template" button at the top right corner). There is no point in forking this project.

# Info About Project
This project is a modern C++ remake of the classic arcade game *Digger*, built on a self-developed 2D engine using the Minigin template. It was developed as part of the semester work for the Programming 4 course in Howest, inspired by all the design patterns and C++ best practices that we have seen within this course.

## Game Features
- Three fully playable levels with increasing difficulty
- Single Player, Co-op, and Versus game modes
- Gamepad and keyboard input support across gameplay
- Functional HUD with lives and score tracking
- Highscore system with persistent saving
- Full sound support: background music and sound effects
- Arcade-style highscore name entry

## Engine Architecture

The game engine was custom-built on top of the **Minigin** starter project, separating engine and game logic into distinct projects (static/dynamic library + executable). Key design patterns and architecture choices include:

### Design Patterns Used
- **Component Pattern**: All game entities use modular components (`LivesComponent`, `ScoreComponent`, `TileTrackerComponent`)
- **Command Pattern**: Input handling via command bindings for both keyboard and gamepad (single and multiplayer)
- **Observer/Event Queue**: Components notify others of changes using a decoupled observer system
- **State Pattern**: AI behaviors and object states (`ChasingState`, `BreakingState`, `IdleState`) use finite state machines
- **Game Loop and Update Method**: Centralized loop handles component updates and rendering, as well as possible reloads needed to be performed
- **Service Locator**: Audio services are managed via a locator pattern with thread-safe backends
- **Double Buffering**: For safe state transitions (AI state changes during update cycles)

### Threading
The engine includes multithreaded sound service execution using a `SoundQueueService`, decoupling sound logic from the main game loop to maintain performance.

### File I/O
- **Level loading**: Levels are described in external binary files and parsed at runtime
- **Highscore saving**: Persistent highscores are serialized and loaded between sessions, this uses txt files

## Design Considerations

- **Modular Architecture**: The engine was designed with extensibility in mind. Most functionality is encapsulated in reusable components.
- **Minimal Dependencies**: Where possible, STL and custom utilities were favored over heavy frameworks.
- **Testing-First Development**: Logic was written iteratively and tested in stages, using Git versioning and feature branches.

## Source Repository

[GitHub Repository](https://github.com/MikhailLukach/ExamProjectProgramming4)  