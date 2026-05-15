# TaskFlow — C++ Console To-Do List

> A feature-rich **command-line To-Do application** built in C++ using Object-Oriented Programming.  
> Minor project demonstrating OOP, file I/O, STL, and clean console UI.

---

## Project Structure

```
TaskFlow/
├── todo.h        # Class & struct declarations
├── todo.cpp      # Core logic implementation
├── main.cpp      # Interactive CLI menu
└── tasks.txt     # Auto-generated persistent storage
```

---

## Features

| Feature | Description |
|---|---|
| Add Task | Title + priority (High / Medium / Low) |
| View All Tasks | Formatted table with ID, title, priority, status |
| Mark Done | Mark a task complete by ID |
| Delete Task | Remove a task by ID |
| Sort by Priority | Reorder tasks High → Medium → Low |
| Persistence | Tasks auto-saved and loaded from `tasks.txt` |

---

## Getting Started

### Prerequisites
- **g++** compiler with C++11 or higher
- Windows: [MinGW-w64](https://www.mingw-w64.org/)
- Linux/macOS: `g++` comes pre-installed

### Compile & Run

**Windows:**
```powershell
cd TaskFlow
g++ -std=c++17 -o todo.exe main.cpp todo.cpp
.\todo.exe
```

**Linux / macOS:**
```bash
cd TaskFlow
g++ -std=c++17 -o todo main.cpp todo.cpp
./todo
```

---

## How to Use

```
  ╔══════════════════════════════════╗
  ║       ADVANCED TO-DO LIST        ║
  ╚══════════════════════════════════╝

  ── MENU ──────────────────────────
  1. Add task
  2. View all tasks
  3. Mark task done
  4. Delete task
  5. Sort by priority
  0. Exit
  ─────────────────────────────────
  Choice:
```

### Adding a Task
```
  Title    : Buy groceries
  Priority [1=HIGH / 2=MEDIUM / 3=LOW]: 1
  [+] Task added (ID=1)
```

### Viewing Tasks
```
  ID    Title                     Priority  Status
  --------------------------------------------------
  1     Buy groceries             HIGH      [PENDING]
  2     Read book                 LOW       [DONE]
  3     Complete assignment       HIGH      [PENDING]
```

---

## C++ Concepts Used

| Concept | Where Used |
|---|---|
| `struct` | `Task` — stores id, title, priority, done |
| `class` | `TodoList` — encapsulates all task operations |
| `vector<Task>` | Stores all tasks in memory |
| `fstream` | Read/write `tasks.txt` for persistence |
| `enum class` | `Priority` — HIGH, MEDIUM, LOW |
| `std::sort` + lambda | Sort tasks by priority |
| `std::remove_if` | Delete task from vector |
| Serialization | Task ↔ text line for file storage |

---

## File Format (tasks.txt)

Tasks are stored as pipe-separated lines:

```
nextId
id|title|done|priority
```

Example:
```
4
1|Buy groceries|0|1
2|Read book|1|3
3|Complete assignment|0|1
```

- `done` → `0` = pending, `1` = done
- `priority` → `1` = HIGH, `2` = MEDIUM, `3` = LOW

---

## Code Overview

### `todo.h` — Declarations
```cpp
enum class Priority { HIGH = 1, MEDIUM = 2, LOW = 3 };

struct Task {
    int id;
    std::string title;
    bool done;
    int priority;
};

class TodoList {
    std::vector<Task> tasks;
    int nextId;
    std::string filename;
public:
    void add(const std::string& title, int priority);
    void remove(int id);
    void markDone(int id);
    void printAll() const;
    void sortByPriority();
    void saveToFile() const;
    void loadFromFile();
};
```

### `main.cpp` — Menu Loop
```cpp
while (true) {
    printMenu();
    int choice = getInt("Choice: ");
    switch (choice) {
        case 1: /* add task */    break;
        case 2: /* view tasks */  break;
        case 3: /* mark done */   break;
        case 4: /* delete */      break;
        case 5: /* sort */        break;
        case 0: return 0;
    }
}
```

---

## Author

**Minor Project — C++ Console Application**  
Built with: C++17 · STL · File I/O · OOP

---

## Future Improvements

- [ ] Add deadline/due date support
- [ ] Add categories/tags
- [ ] Search by keyword
- [ ] Filter by status (pending/done)
- [ ] Color-coded terminal output
- [ ] Web frontend integration
- [ ] REST API server
