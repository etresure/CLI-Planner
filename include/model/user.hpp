#pragma once

#include <iostream>
#include "model/project.hpp"
#include "tasks/reminder.hpp"
namespace cli_planner{
    
constexpr int MAX_REMINDERS = 50;
constexpr int MAX_PROJECTS = 20;

class User final{
using string = std::string;
using ostream = std::ostream;
private:
    string name;
    int id;
    Project* projects[MAX_PROJECTS];
    int project_count;

    Reminder* reminders[MAX_REMINDERS];
    int reminder_count;

public:
    User(const string& newName, int newId);
    ~User();
    User(const User& other);
    User& operator=(const User& other);
    Project* create_project(const string& projName, const string& projDesc);
    Project* copy_project(const Project *project);
    void delete_project(Project* project); //
    void display() const; //

    string get_name() const; 
    int get_id() const;
    Project** get_projects(); //
    int get_project_count() const; 

    bool add_task_reminder(Project* project, int task_index, const string& time, const string& method = "console");
    bool cancel_task_reminder(Project* project, int task_index);
    void cancel_all_reminders();
    void show_reminders() const;
    bool show_task_reminder(Project* project, int task_index) const;

    void generate_project_report(Project* project) const; //
};
}