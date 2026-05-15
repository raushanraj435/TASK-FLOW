#include "todo.h"
#include <iostream>
#include <string>
#include <limits>

static const std::string BOLD  = "\033[1m";
static const std::string CYAN  = "\033[36m";
static const std::string RESET = "\033[0m";
static const std::string DIM   = "\033[2m";

// ─── Input helpers ───────────────────────────────────
static int getInt(const std::string& prompt) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) { std::cin.ignore(); return v; }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Invalid. Try again.\n";
    }
}

static std::string getLine(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

static Priority getPriority() {
    std::cout << "  Priority  [1=HIGH / 2=MEDIUM / 3=LOW]: ";
    int p = getInt("");
    if (p == 1) return Priority::HIGH;
    if (p == 2) return Priority::MEDIUM;
    return Priority::LOW;
}

// ─── Banner ──────────────────────────────────────────
static void printBanner() {
    std::cout << "\n" << CYAN << BOLD;
    std::cout << "  ╔══════════════════════════════════╗\n";
    std::cout << "  ║       ADVANCED TO-DO LIST        ║\n";
    std::cout << "  ╚══════════════════════════════════╝\n" << RESET;
}

// ─── Main Menu ───────────────────────────────────────
static void printMenu() {
    std::cout << BOLD << "\n  ── MENU ──────────────────────────\n" << RESET;
    std::cout << "  1. Add task\n";
    std::cout << "  2. View all tasks\n";
    std::cout << "  3. Filter tasks\n";
    std::cout << "  4. Sort tasks\n";
    std::cout << "  5. Search tasks\n";
    std::cout << "  6. Mark done / pending\n";
    std::cout << "  7. Edit task\n";
    std::cout << "  8. Delete task\n";
    std::cout << "  9. Statistics\n";
    std::cout << "  10. Category breakdown\n";
    std::cout << "  0. Exit\n";
    std::cout << "  ─────────────────────────────────\n";
}

// ─── Main ────────────────────────────────────────────
int main() {
    TodoList todo("tasks.json");
    printBanner();

    while (true) {
        todo.printStats();
        printMenu();
        int choice = getInt("  Choice: ");

        switch (choice) {
        case 1: {
            std::string title    = getLine("  Title    : ");
            std::string category = getLine("  Category : ");
            std::string deadline = getLine("  Deadline (YYYY-MM-DD or blank): ");
            Priority p = getPriority();
            todo.add(title, category, deadline, p);
            break;
        }
        case 2:
            todo.display();
            break;

        case 3: {
            std::cout << "  Filter: [1=ALL / 2=PENDING / 3=DONE / 4=OVERDUE]: ";
            int f = getInt("");
            Filter filter = Filter::ALL;
            if (f == 2) filter = Filter::PENDING;
            else if (f == 3) filter = Filter::DONE;
            else if (f == 4) filter = Filter::OVERDUE;
            todo.display(filter);
            break;
        }
        case 4: {
            std::cout << "  Sort by: [1=ID / 2=PRIORITY / 3=DEADLINE / 4=TITLE]: ";
            int s = getInt("");
            SortBy sort = SortBy::ID;
            if (s == 2) sort = SortBy::PRIORITY;
            else if (s == 3) sort = SortBy::DEADLINE;
            else if (s == 4) sort = SortBy::TITLE;
            todo.display(Filter::ALL, sort);
            break;
        }
        case 5: {
            std::string kw = getLine("  Search keyword: ");
            todo.search(kw);
            break;
        }
        case 6: {
            int id = getInt("  Task ID: ");
            std::cout << "  [1=Mark Done / 2=Mark Pending]: ";
            int m = getInt("");
            if (m == 1) todo.markDone(id);
            else        todo.markPending(id);
            break;
        }
        case 7: {
            int id = getInt("  Task ID to edit: ");
            std::string title    = getLine("  New Title    (blank=keep): ");
            std::string category = getLine("  New Category (blank=keep): ");
            std::string deadline = getLine("  New Deadline (blank=keep): ");
            Priority p = getPriority();
            todo.edit(id, title, category, deadline, p);
            break;
        }
        case 8: {
            int id = getInt("  Task ID to delete: ");
            todo.remove(id);
            break;
        }
        case 9:
            todo.printStats();
            break;
        case 10:
            todo.printCategoryBreakdown();
            break;
        case 0:
            std::cout << "\n  Bye!\n\n";
            return 0;
        default:
            std::cout << "  Invalid choice.\n";
        }
    }
}
