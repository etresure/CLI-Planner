#pragma once

#include <iostream>
#include "tasks/task.hpp"

constexpr int MAX_TASKS = 20;

namespace cli_planner{

class Project final{
using string = std::string;
using ostream = std::ostream;
private:
    string name;
    string description;
    int task_count;
    static int project_count;
    Task* tasks[MAX_TASKS];

public:
    Project();
    ~Project();
    Project(const string& newName, const string& newDescription);
    Project(const Project& other);
    Project& operator=(const Project& other);

    void add_task(Task* task); //
    void add_task(const string& title, const string& description); //
    void remove_task(Task* task); //
    void display() const; //

    const string& get_name() const &; 
    double get_completion_percentage() const; //
    Task** get_completed_tasks(); //
    int get_task_count() const; // 
    static int get_project_count();  //

    Project& operator+=(Task* task);
    Task* get_task(int idx);
    Task* take_task(int idx);

    void print(std::ostream& out) const;
};
}
std::ostream& operator<<(std::ostream& os, const cli_planner::Project& project);