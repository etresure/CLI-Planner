#include <iostream>  

#include "model/user.hpp"
#include "tasks/task.hpp" 
#include "tasks/urgent_collab.hpp"

using namespace cli_planner;

UrgentTask::UrgentTask() : Task(), urgency_level(0){}
void UrgentTask::set_urgency_level(int level){
    if (level < 0 || level > 10)
        throw level;
    urgency_level = level;
}
int UrgentTask::get_urgency_level() const {
    return urgency_level;
}
void UrgentTask::display() const { 
    Task::display();
    std::cout << "Urgency Level: " << urgency_level << std::endl;
}
UrgentTask* UrgentTask::smart_copy_task() const { 
    return new UrgentTask{*this}; 
}
CollaborativeTask::CollaborativeTask() : Task(), progress(0){
    for (int i = 0; i < MAX_USERS; ++i) {
        assigned_users[i] = nullptr;
    }
}
void CollaborativeTask::assign_user(User* user){
    if (!user) return;
    for (int i = 0; i < MAX_USERS; ++i){
        if (assigned_users[i] == user){
            return;
        }
    }
    for (int i = 0; i < MAX_USERS; ++i){
        if (assigned_users[i] == nullptr){
            assigned_users[i] = user;
            return;
        }
    }
}
CollaborativeTask* CollaborativeTask::smart_copy_task() const { 
    return new CollaborativeTask{*this}; 
}
void CollaborativeTask::remove_user(User* user){
    for (int i = 0; i < MAX_USERS; ++i){
        if (assigned_users[i] == user){
            for (int j = i; j < MAX_USERS - 1; ++j){ // удаляем и сдвигаем массив
                assigned_users[j] = assigned_users[j + 1];
            }
            assigned_users[MAX_USERS - 1] = nullptr;
            return;
        }
    }
}
void CollaborativeTask::set_progress(int new_progress){
    if (new_progress < 0) new_progress = 0;
    if (new_progress > 100) new_progress = 100;
    progress = new_progress;
}
int CollaborativeTask::get_progress() const{
    return progress;
}
void CollaborativeTask::display() const{
    Task::display();
    std::cout << "Progress: " << progress << "%" << std::endl;
    std::cout << "Assigned Users: ";
    bool first = true;
    for (int i = 0; i < MAX_USERS; ++i){
        if (assigned_users[i]){
            if (!first) std::cout << ", ";
            std::cout << assigned_users[i]->get_name();
            first = false;
        }
    }
    if (first) std::cout << "None";
    std::cout << std::endl;
}