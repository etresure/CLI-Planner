#pragma once

#include <iostream>

namespace cli_planner{

enum class Priority {
    low,
    medium,
    high
};

enum class Status {
    pending,      // ещё не начато
    working,   // в процессе
    done          // выполнено
};

inline std::string to_string(Priority p){
    switch (p){
    case Priority::low: return "low";
    case Priority::medium: return "medium";
    case Priority::high: return "high";
    }
    return {};
}
inline std::string to_string(Status s){
    switch (s){
    case Status::pending: return "pending";
    case Status::working: return "working";
    case Status::done: return "done";
    }
    return {};
}
inline Priority parse_priority(const std::string& str) {
    if (str == "low")    
        return Priority::low;
    if (str == "medium") 
        return Priority::medium;
    if (str == "high")   
        return Priority::high;
    throw std::invalid_argument("Invalid priority: " + str);
}
inline Status parse_status(const std::string& str) {
    if (str == "pending") 
        return Status::pending;
    if (str == "working")  
        return Status::working;
    if (str == "done")         
        return Status::done;
    throw std::invalid_argument("Invalid status: " + str);
}

class Task{
using string = std::string;
using ostream = std::ostream;
protected:
    string title;
    string description;
    string creation_date;
    string due_date;
    Priority priority{Priority::low};
    Status status{Status::pending};

public:
    Task();
    Task(const std::string& newTitle, const std::string& newDescription);
    virtual ~Task();
    
    void update_status(Status newStatus); //
    void update_priority(Priority newPriority); //
    void set_due_date(const string& date); //
    void set_title(const string& newTitle); //
    void set_description(const string& newDescription); //
    
    virtual void display() const; //
    //static void print_task_count();

    string get_title() const;
    string get_description() const;
    string get_creation_date() const;
    string get_due_date() const;
    Priority get_priority() const;
    Status get_status() const;
    //унарные операции для изменения приоритета

    Task& operator++();          // Унарный префиксный  //
    Task operator++(int);       // Унарный постфиксный //
    Task& operator--();   

    virtual void print(std::ostream& out) const;
    virtual Task* smart_copy_task() const;
        
};
}
std::ostream& operator<<(std::ostream& os, const cli_planner::Task& task);

inline std::ostream& operator<<(std::ostream& os, cli_planner::Priority p){
    return os << to_string(p);
}
inline std::ostream& operator<<(std::ostream& os, cli_planner::Status s){
    return os << to_string(s);
}