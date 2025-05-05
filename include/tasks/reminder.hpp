#pragma once

#include <iostream>
#include "tasks/task.hpp"

namespace cli_planner{

class Reminder final{
using string = std::string;
using ostream = std::ostream;
private:
    Task* task;
    string reminder_time;
    string notification_method;

public:
    Reminder(Task* task = nullptr, const string& time = "", const string& method = "console");
    void set_task(Task* task);
    void set_reminder(const string& time);
    void set_notification_method(const string& method);
    void cancel_reminder();
    string get_reminder_time() const;
    string get_notification_method() const;
    void display() const;
    Task* get_task() const;
};
}