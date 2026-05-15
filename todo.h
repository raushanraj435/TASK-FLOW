#pragma once
#include <string>
#include <vector>

enum class Priority { HIGH = 1, MEDIUM = 2, LOW = 3 };
enum class Filter   { ALL, PENDING, DONE, OVERDUE };
enum class SortBy   { ID, PRIORITY, DEADLINE, TITLE };

struct Task {
    int         id;
    std::string title;
    std::string category;
    std::string deadline;   // "YYYY-MM-DD" or ""
    Priority    priority;
    bool        done;

    Task(int id, const std::string& title,
         const std::string& category,
         const std::string& deadline,
         Priority priority);

    std::string priorityStr()  const;
    std::string statusStr()    const;
    bool        isOverdue()    const;
    std::string serialize()    const;
    static Task deserialize(const std::string& line);
};

struct Stats {
    int total, done, pending, overdue;
    double completionPct;
};

class TodoList {
    std::vector<Task> tasks;
    int               nextId;
    std::string       filename;

    void saveToFile()  const;
    void loadFromFile();

public:
    explicit TodoList(const std::string& file = "tasks.json");

    void add   (const std::string& title, const std::string& category,
                const std::string& deadline, Priority priority);
    void remove(int id);
    void edit  (int id, const std::string& title, const std::string& category,
                const std::string& deadline, Priority priority);
    void markDone   (int id);
    void markPending(int id);

    void display(Filter filter = Filter::ALL, SortBy sort = SortBy::ID) const;
    void search (const std::string& keyword) const;

    Stats getStats() const;
    void  printStats() const;
    void  printCategoryBreakdown() const;
};
