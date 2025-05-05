#pragma once

#include <iostream>
#include "task.hpp" 
#include "model/user.hpp"

namespace cli_planner{
constexpr int MAX_USERS = 20;


class UrgentTask final: public Task{
private:
    int urgency_level;
public:
    UrgentTask();
    void display() const override;
    void set_urgency_level(int level);
    int get_urgency_level() const;
    UrgentTask* smart_copy_task() const override;
};

class CollaborativeTask final: public Task{
private:
    User* assigned_users[MAX_USERS];
    int progress;

public:
    CollaborativeTask();
    void assign_user(User* user);
    void remove_user(User* user);
    void set_progress(int new_progress);
    int get_progress() const;
    void display() const override;
    CollaborativeTask* smart_copy_task() const override;
};
}