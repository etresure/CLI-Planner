#include <iostream>
#include <ctime>
#include "tasks/task.hpp"

namespace cli_planner{

Task::Task(){
    title = "";
    description = "";
    time_t time_now = std::time(nullptr);
    std::tm* ltm = std::localtime(&time_now);
    char buff[11];
    std::strftime(buff, sizeof(buff), "%Y-%m-%d", ltm);
    creation_date = buff;
    due_date = "";
    priority = Priority::low;
    status = Status::pending;
}
Task::Task(const std::string& newTitle, const std::string& newDescription){
    title = newTitle;
    description = newDescription;
    time_t time_now = std::time(nullptr);
    std::tm* ltm = std::localtime(&time_now);
    char buff[11];
    std::strftime(buff, sizeof(buff), "%Y-%m-%d", ltm);
    creation_date = buff;
    due_date = "";
    priority = Priority::low;
    status = Status::pending;
}

Task::~Task(){}

void Task::update_status(Status newStatus){
    status = newStatus;
}

void Task::update_priority(Priority newPriority){
    priority = newPriority;
}

void Task::set_due_date(const std::string& date){
    due_date = date;
}

void Task::set_title(const std::string& newTitle){
    title = newTitle;
}
void Task::set_description(const std::string& newDescription){
    description = newDescription;
}

void Task::display() const{
    std::cout << "Title: " << title << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Created: " << creation_date << std::endl;
    std::cout << "Due: " << due_date << std::endl;
    std::cout << "Priority: " << priority << std::endl;
    std::cout << "Status: " << status << std::endl;
}

std::string Task::get_title() const{ 
    return title; 
}
std::string Task::get_description() const{ 
    return description; 
}
std::string Task::get_creation_date() const{
    return creation_date; 
}
std::string Task::get_due_date() const{ 
    return due_date; 
}
Priority Task::get_priority() const{ 
    return priority; 
}
Status Task::get_status() const{ 
    return status; 
}

Task& Task::operator++(){
    if (priority == Priority::low)       
        priority = Priority::medium;
    else if (priority == Priority::medium) 
        priority = Priority::high;
    return *this;
}
Task Task::operator++(int){
    Task tmp(*this);
    ++(*this);
    return tmp;
}
Task& Task::operator--(){
    if (priority == Priority::high)       
         priority = Priority::medium;
    else if (priority == Priority::medium) 
        priority = Priority::low;
    return *this;
}
void Task::print(std::ostream &out) const{
    out << "Title: " << title << "\n"
       << "Description: " << description << "\n"
       << "Created: " << creation_date << "\n"
       << "Due: " << due_date << "\n"
       << "Priority: " << priority << "\n"
       << "Status: " << status;
}
Task* Task::smart_copy_task() const { 
    return new Task{*this}; 
}
}
std::ostream& operator<<(std::ostream& out, const cli_planner::Task &task){
    task.print(out);
    return out;
}