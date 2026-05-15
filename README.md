# TaskFlow — Advanced To-Do List

> A full-stack To-Do application with a **C++ backend** (CLI + HTTP server) and a **Web frontend** (HTML/CSS/JS).  
> Built as a minor project demonstrating OOP, file I/O, REST APIs, and modern web UI.

---

## Project Structure

```
TaskFlow/
├── backend/
│   ├── todo.h          # Class & struct declarations
│   ├── todo.cpp        # Core logic implementation
│   ├── main.cpp        # Interactive CLI menu
│   ├── server.cpp      # HTTP REST API server
│   └── tasks.json      # Auto-generated persistent storage
├── frontend/
│   └── index.html      # Standalone Web UI
└── README.md
```

---

## Features

### C++ CLI App
| Feature | Description |
|---|---|
| Add Task | Title, category, deadline, priority |
| Edit Task | Update any field by ID |
| Delete Task | Remove by ID |
| Mark Done / Pending | Toggle task status |
| Filter | All / Pending / Done / Overdue |
| Sort | By ID / Priority / Deadline / Title |
| Search | Keyword search in title & category |
| Statistics | Total, done, pending, overdue + progress bar |
| Category Breakdown | Per-category completion % |
| Persistence | Auto save/load via `tasks.json` |

### Web Frontend
- Dark mode UI with real-time search, filter & sort
- Category sidebar with task counts
- Stats cards + animated progress bar
- Add/Edit modal with keyboard support (`Enter` to save, `Esc` to close)
- Overdue badges appear automatically
- Auto-refresh every 5 seconds when connected to server

### HTTP REST API (server.cpp)
| Method | Endpoint | Action |
|---|---|---|
| GET | `/api/tasks` | Fetch all tasks |
| POST | `/api/tasks` | Create new task |
| PUT | `/api/tasks/:id` | Edit task |
| PATCH | `/api/tasks/:id/toggle` | Toggle done/pending |
| DELETE | `/api/tasks/:id` | Delete task |
| GET | `/api/stats` | Get statistics |

---

## Getting Started

### Prerequisites
- **g++** with C++17 support — [MinGW-w64](https://www.mingw-w64.org/) recommended for Windows
- A modern web browser (Chrome, Firefox, Edge)

### 1. Compile & Run CLI App

```powershell
# Navigate to backend folder
cd TaskFlow\backend

# Compile
g++ -std=c++17 -o todo.exe main.cpp todo.cpp

# Run
.\todo.exe
```

### 2. Run Web Frontend (Standalone)

Just double-click `frontend\index.html` — no server needed.  
Data is saved in browser `localStorage`.

### 3. Run Full Stack (C++ Server + Web UI)

**Windows:**
```powershell
cd TaskFlow\backend
g++ -std=c++17 -O2 -o server.exe server.cpp todo.cpp -lws2_32
.\server.exe
```

**Linux / macOS:**
```bash
cd TaskFlow/backend
g++ -std=c++17 -O2 -o server server.cpp todo.cpp -lpthread
./server
```

Then open `frontend/index.html` in your browser.  
The web app will connect to `http://localhost:8080` automatically.

---

## C++ Concepts Used

| Concept | Where Used |
|---|---|
| `struct` | `Task` — groups all task fields |
| `class` | `TodoList` — encapsulates data + operations |
| `vector<Task>` | Stores all tasks in memory |
| `fstream` | Read/write `tasks.json` |
| `enum class` | `Priority`, `Filter`, `SortBy` |
| `std::sort` + lambda | Sorting tasks |
| `std::remove_if` | Deleting tasks |
| Serialization | Converting Task ↔ text line for file storage |
| Raw Sockets | HTTP server using `Winsock2` / POSIX sockets |
| Multithreading | Each client connection handled in separate thread |

---

## How It Works

```
[ CLI App ]                   [ Web App ]
  main.cpp                    index.html
      │                           │
      ▼                           ▼
 TodoList                    JavaScript
      │                           │
      ▼                           ▼
 tasks.json  ←──  server.cpp ──→  fetch() API calls
              (connects both)
```

- **Without server:** CLI and Web work independently. CLI saves to `tasks.json`, Web saves to `localStorage`.
- **With server:** Web app talks to C++ server via HTTP. All data stored in `tasks.json`. Both CLI and Web share the same data.

---

## API Request Examples

```bash
# Get all tasks
curl http://localhost:8080/api/tasks

# Add a new task
curl -X POST http://localhost:8080/api/tasks \
  -H "Content-Type: application/json" \
  -d '{"title":"Buy groceries","category":"Personal","deadline":"2025-06-01","priority":"HIGH"}'

# Mark task 3 as done
curl -X PATCH http://localhost:8080/api/tasks/3/toggle

# Delete task 5
curl -X DELETE http://localhost:8080/api/tasks/5
```

---

## Screenshots

```
  ╔══════════════════════════════════╗
  ║       ADVANCED TO-DO LIST        ║
  ╚══════════════════════════════════╝

  ── Statistics ──────────────────
  Total   : 8
  Done    : 5
  Pending : 2
  Overdue : 1
  Progress: [##########################....] 62.5%

  ID    Title                     Category     Deadline    Priority  Status
  -----------------------------------------------------------------------
  1     Buy groceries             Personal     2025-06-01  HIGH      PENDING
  2     Complete assignment       Study        2025-05-20  HIGH      OVERDUE
  3     Read book                 Personal     -           LOW       DONE
```

---

## File Format (tasks.json)

Each task is stored as a pipe-separated line:

```
nextId
id|title|category|deadline|priority|done
```

Example:
```
4
1|Buy groceries|Personal|2025-06-01|HIGH|0
2|Complete assignment|Study|2025-05-20|HIGH|0
3|Read book|Personal||LOW|1
```

---

## Author

**Minor Project — C++ with Web Integration**  
Built with: C++17 · HTML5 · CSS3 · Vanilla JavaScript · Winsock2

---

## Future Improvements

- [ ] User authentication
- [ ] Task due-date notifications
- [ ] Export to CSV / PDF
- [ ] Dark/Light theme toggle
- [ ] Mobile responsive layout
- [ ] SQLite database instead of flat file
