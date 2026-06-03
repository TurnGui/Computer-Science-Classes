# Concurrent Programming — Practical Project

**University of Minho — Licenciatura em Ciências da Computação**
**Course: Programação Concorrente (Concurrent Programming) — 2025/2026**

---

## Overview

A multiplayer 2D mini-game where players control avatars in a shared space, interacting with each other and with the environment. The server handles all physics simulation and game logic, while clients are responsible only for rendering and sending player input.

The project demonstrates concurrent programming concepts using two different paradigms:
- **Erlang server** — Actor model (message passing, no shared memory)
- **Java/Processing client** — Shared memory with explicit synchronization (`ReentrantLock`, `Condition`)

---

## Features

- User registration and login (persisted to `accounts.txt`)
- Matchmaking lobby with a queue system (3–4 players per match, up to 4 simultaneous matches)
- Real-time 2D physics simulation (velocity, acceleration, torque, friction, world boundaries)
- Edible and poisonous objects spawned in the game world
- Player collisions and captures
- Top scores leaderboard (shown in lobby while waiting)
- Match timer (2 minutes) with winner detection and tie handling
- Graphical client built with Processing

---

## Project Structure

```
ProjetoPC/
├── server/
│   └── server.erl          # Erlang server
└── cliente/
    └── ClienteJogo/
        ├── ClienteJogo.pde  # Main sketch — setup(), draw(), shared state
        ├── Client.pde       # Network thread (ConnectionServer)
        ├── Menus.pde        # Drawing functions for all screens
        └── Inputs.pde       # Keyboard and mouse input handling
```

---

## Concurrency Design

### Erlang Server — Actor Model
The server uses isolated processes that communicate exclusively through message passing. No shared memory means no race conditions and no need for locks.

| Process | Responsibility |
|---|---|
| `acceptor` | Accepts incoming TCP connections |
| `main_loop` | Handles registration and login |
| `lobby_loop` | Manages the matchmaking queue |
| `game_loop` | Runs physics ticks and broadcasts game state |
| `top_scores_loop` | Maintains the leaderboard |

### Java Client — Shared Memory with Locks
The client has two concurrent threads accessing shared state:
- **Main thread** — runs `draw()` at 60fps, reads game state to render
- **Network thread** (`ConnectionServer`) — reads messages from the server, writes game state

All shared variables are protected by a `ReentrantLock` with a `Condition` variable, following the patterns taught in the practical sessions (Guiões 3 and 5).

```java
final ReentrantLock lock = new ReentrantLock();
final Condition estadoMudou = lock.newCondition();

// Network thread writes:
lock.lock();
try {
    estadoAtual = 2;
    estadoMudou.signalAll();
} finally {
    lock.unlock();
}

// draw() reads:
lock.lock();
try {
    // render current state
} finally {
    lock.unlock();
}
```

---

## Protocol

Communication between client and server uses TCP with line-delimited text messages and `#` as a separator.

### Client → Server
```
register#username#password
login#username#password
join_lobby#username
key_press#username#LEFT|RIGHT|FORWARD
key_release#username#LEFT|RIGHT|FORWARD
```

### Server → Client
```
OK#LOGIN / OK#REGISTER
ERR#message
LOBBY#OK
TOP#user1:pts|user2:pts|...
GAME_START#username
STATE#time#user:x:y:mass:angle:score|user2:...
OBJS#id:x:y:radius:type|...
GAME_OVER#message
```

---

## How to Run

### Requirements
- Erlang/OTP 25+
- Processing 4.x (or exported application)

### Start the Server
```bash
cd server
erl
```
```erlang
c(server).
server:start(8080).
```

### Start the Client
**Option A — Processing IDE:**
Open `ClienteJogo.pde` in Processing and press ▶ Run.

**Option B — Exported application (multiple instances):**
```bash
cd cliente/ClienteJogo/linux-amd64
./ClienteJogo &
./ClienteJogo &
./ClienteJogo
```

### Simulate players with netcat (terminal only)
```bash
nc 127.0.0.1 8080
```
```
register#player1#pass
login#player1#pass
join_lobby#player1
```

---

## Controls

| Key | Action |
|---|---|
| `←` | Rotate left |
| `→` | Rotate right |
| `↑` | Accelerate forward |

