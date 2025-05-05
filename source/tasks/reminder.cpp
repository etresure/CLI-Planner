#include <iostream>  

#include "tasks/task.hpp" 
#include "tasks/reminder.hpp" 

namespace cli_planner{

Reminder::Reminder(Task* newTask, const std::string& newTime, const std::string& newMethod)
: task(newTask), reminder_time(newTime), notification_method(newMethod){}
void Reminder::set_task(Task* newTask){
    task = newTask;
}
void Reminder::set_reminder(const std::string& time){
    reminder_time = time;
}
void Reminder::set_notification_method(const std::string& method){
    notification_method = method;
}
void Reminder::cancel_reminder(){
    reminder_time.clear();
    notification_method.clear();
}
std::string Reminder::get_reminder_time() const{
    return reminder_time;
}
std::string Reminder::get_notification_method() const{
    return notification_method;
}
void Reminder::display() const{
    std::cout << "Reminder";
    if (task){
        std::cout << " for task '" << task->get_title() << "'";
    }
    std::cout << ":";
    if (!reminder_time.empty()){
        std::cout << " at " << reminder_time;
    } 
    else{
        std::cout << " [no time set]";
    }
    std::cout << ", notification method: ";
    if (!notification_method.empty()) {
        std::cout << notification_method << std::endl;
    } 
    else{
        std::cout << "[no method set]\n";
    }
    std::cout << std::endl;
}
Task* Reminder::get_task() const{
    return task;
}
}