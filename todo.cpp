#include "todo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <map>

// ═══════════════════════════════════════════════
//  Helpers
// ═══════════════════════════════════════════════
static std::string todayStr() {
    time_t t = time(nullptr);
    tm* tm_ = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", tm_);
    return buf;
}

static bool dateExpired(const std::string& d) {
    return !d.empty() && d < todayStr();
}

static std::string priorityToStr(Priority p) {
    if (p == Priority::HIGH)   return "HIGH";
    if (p == Priority::MEDIUM) return "MEDIUM";
    return "LOW";
}

static Priority strToPriority(const std::string& s) {
    if (s == "HIGH")   return Priority::HIGH;
    if (s == "MEDIUM") return Priority::MEDIUM;
    return Priority::LOW;
}

static std::string colorCode(Priority p) {
    if (p == Priority::HIGH)   return "\033[31m";  // red
    if (p == Priority::MEDIUM) return "\033[33m";  // yellow
    return "\033[32m";                              // green
}

static const std::string RESET  = "\033[0m";
static const std::string BOLD   = "\033[1m";
static const std::string DIM    = "\033[2m";
static const std::string CYAN   = "\033[36m";
static const std::string STRIKE = "\033[9m";

// ═══════════════════════════════════════════════
//  Task
// ═══════════════════════════════════════════════
Task::Task(int id, const std::string& title,
           const std::string& category,
           const std::string& deadline,
           Priority priority)
    : id(id), title(title), category(category),
      deadline(deadline), priority(priority), done(false) {}

std::string Task::priorityStr() const { return priorityToStr(priority); }

std::string Task::statusStr() const {
    if (done) return "DONE";
    if (isOverdue()) return "OVERDUE";
    return "PENDING";
}

bool Task::isOverdue() const {
    return !done && dateExpired(deadline);
}

// Format: id|title|category|deadline|priority|done
std::string Task::serialize() const {
    return std::to_string(id) + "|" + title + "|" + category + "|" +
           deadline + "|" + priorityToStr(priority) + "|" +
           std::to_string(done);
}

Task Task::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string tok;
    std::vector<std::string> p;
    while (std::getline(ss, tok, '|')) p.push_back(tok);
    if (p.size() < 6) throw std::runtime_error("bad line");
    Task t(std::stoi(p[0]), p[1], p[2], p[3], strToPriority(p[4]));
    t.done = (p[5] == "1");
    return t;
}

// ═══════════════════════════════════════════════
//  TodoList — private
// ═══════════════════════════════════════════════
void TodoList::saveToFile() const {
    std::ofstream f(filename);
    f << nextId << "\n";
    for (const auto& t : tasks) f << t.serialize() << "\n";
}

void TodoList::loadFromFile() {
    std::ifstream f(filename);
    if (!f.is_open()) return;
    f >> nextId; f.ignore();
    std::string line;
    while (std::getline(f, line))
        if (!line.empty()) {
            try { tasks.push_back(Task::deserialize(line)); }
            catch (...) {}
        }
}

// ═══════════════════════════════════════════════
//  TodoList — public
// ═══════════════════════════════════════════════
TodoList::TodoList(const std::string& file) : nextId(1), filename(file) {
    loadFromFile();
}

void TodoList::add(const std::string& title, const std::string& category,
                   const std::string& deadline, Priority priority) {
    tasks.emplace_back(nextId++, title, category, deadline, priority);
    saveToFile();
    std::cout << CYAN << "  [+] Task added (ID=" << nextId-1 << ")" << RESET << "\n";
}

void TodoList::remove(int id) {
    auto it = std::remove_if(tasks.begin(), tasks.end(),
        [id](const Task& t) { return t.id == id; });
    if (it == tasks.end()) { std::cout << "  [!] ID not found.\n"; return; }
    tasks.erase(it, tasks.end());
    saveToFile();
    std::cout << "\033[31m  [-] Task removed.\033[0m\n";
}

void TodoList::edit(int id, const std::string& title, const std::string& category,
                    const std::string& deadline, Priority priority) {
    for (auto& t : tasks) {
        if (t.id == id) {
            if (!title.empty())    t.title    = title;
            if (!category.empty()) t.category = category;
            if (!deadline.empty()) t.deadline = deadline;
            t.priority = priority;
            saveToFile();
            std::cout << CYAN << "  [✎] Task updated.\n" << RESET;
            return;
        }
    }
    std::cout << "  [!] ID not found.\n";
}

void TodoList::markDone(int id) {
    for (auto& t : tasks) {
        if (t.id == id) {
            t.done = true; saveToFile();
            std::cout << "\033[32m  [✓] Marked as done.\n\033[0m";
            return;
        }
    }
    std::cout << "  [!] ID not found.\n";
}

void TodoList::markPending(int id) {
    for (auto& t : tasks) {
        if (t.id == id) {
            t.done = false; saveToFile();
            std::cout << CYAN << "  [↺] Marked as pending.\n" << RESET;
            return;
        }
    }
    std::cout << "  [!] ID not found.\n";
}

void TodoList::display(Filter filter, SortBy sort) const {
    std::vector<Task> view;
    for (const auto& t : tasks) {
        if (filter == Filter::ALL)     view.push_back(t);
        else if (filter == Filter::PENDING && !t.done && !t.isOverdue()) view.push_back(t);
        else if (filter == Filter::DONE    &&  t.done)                   view.push_back(t);
        else if (filter == Filter::OVERDUE &&  t.isOverdue())            view.push_back(t);
    }

    std::sort(view.begin(), view.end(), [sort](const Task& a, const Task& b) {
        if (sort == SortBy::PRIORITY) return (int)a.priority < (int)b.priority;
        if (sort == SortBy::DEADLINE) return a.deadline < b.deadline;
        if (sort == SortBy::TITLE)    return a.title < b.title;
        return a.id < b.id;
    });

    if (view.empty()) { std::cout << "  No tasks found.\n"; return; }

    std::cout << "\n" << BOLD;
    std::cout << "  " << std::left
              << std::setw(5)  << "ID"
              << std::setw(26) << "Title"
              << std::setw(13) << "Category"
              << std::setw(12) << "Deadline"
              << std::setw(8)  << "Priority"
              << "Status\n" << RESET;
    std::cout << "  " << std::string(72, '-') << "\n";

    for (const auto& t : view) {
        std::string col = t.done ? DIM : (t.isOverdue() ? "\033[31m" : colorCode(t.priority));
        std::cout << col << "  "
                  << std::left
                  << std::setw(5)  << t.id
                  << std::setw(26) << (t.title.size() > 24 ? t.title.substr(0,23)+"…" : t.title)
                  << std::setw(13) << (t.category.empty() ? "-" : t.category)
                  << std::setw(12) << (t.deadline.empty() ? "-" : t.deadline)
                  << std::setw(8)  << t.priorityStr()
                  << t.statusStr()
                  << RESET << "\n";
    }
    std::cout << "  " << std::string(72, '-') << "\n\n";
}

void TodoList::search(const std::string& keyword) const {
    std::string kw = keyword;
    std::transform(kw.begin(), kw.end(), kw.begin(), ::tolower);

    std::cout << "\n  Search results for \"" << keyword << "\":\n";
    bool found = false;
    for (const auto& t : tasks) {
        std::string title = t.title;
        std::transform(title.begin(), title.end(), title.begin(), ::tolower);
        std::string cat = t.category;
        std::transform(cat.begin(), cat.end(), cat.begin(), ::tolower);
        if (title.find(kw) != std::string::npos ||
            cat.find(kw)   != std::string::npos) {
            std::cout << "  [" << t.id << "] " << t.title
                      << " (" << t.category << ") — " << t.statusStr() << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "  No matching tasks.\n";
    std::cout << "\n";
}

Stats TodoList::getStats() const {
    Stats s{};
    s.total = tasks.size();
    for (const auto& t : tasks) {
        if (t.done) s.done++;
        else if (t.isOverdue()) s.overdue++;
        else s.pending++;
    }
    s.completionPct = s.total ? (100.0 * s.done / s.total) : 0.0;
    return s;
}

void TodoList::printStats() const {
    auto s = getStats();
    int barLen = 30;
    int filled = (int)(barLen * s.completionPct / 100.0);

    std::cout << "\n" << BOLD << "  ── Statistics ──────────────────\n" << RESET;
    std::cout << "  Total   : " << s.total   << "\n";
    std::cout << "\033[32m  Done    : " << s.done    << "\033[0m\n";
    std::cout << "  Pending : " << s.pending << "\n";
    std::cout << "\033[31m  Overdue : " << s.overdue << "\033[0m\n";
    std::cout << "  Progress: [";
    std::cout << "\033[32m";
    for (int i = 0; i < filled; i++) std::cout << "#";
    std::cout << RESET;
    for (int i = 0; i < barLen - filled; i++) std::cout << ".";
    std::cout << "] " << std::fixed << std::setprecision(1) << s.completionPct << "%\n\n";
}

void TodoList::printCategoryBreakdown() const {
    std::map<std::string, std::pair<int,int>> cat; // category → {total, done}
    for (const auto& t : tasks) {
        std::string c = t.category.empty() ? "Uncategorized" : t.category;
        cat[c].first++;
        if (t.done) cat[c].second++;
    }
    std::cout << "\n" << BOLD << "  ── Category Breakdown ──────────\n" << RESET;
    for (const auto& [name, cnt] : cat) {
        double pct = cnt.first ? (100.0 * cnt.second / cnt.first) : 0.0;
        std::cout << "  " << std::left << std::setw(16) << name
                  << cnt.second << "/" << cnt.first
                  << "  (" << std::fixed << std::setprecision(0) << pct << "%)\n";
    }
    std::cout << "\n";
}
