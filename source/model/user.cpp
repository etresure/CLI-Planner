#include <iostream>  

#include "model/project.hpp"
#include "model/user.hpp"

namespace cli_planner{

User::User(const std::string& newName, int newId) : 
name(newName), id(newId), project_count(0), reminder_count(0){
    for(int i = 0; i < MAX_PROJECTS; ++i){
        projects[i] = nullptr;
    }
    for(int i = 0; i < MAX_REMINDERS; ++i){
        reminders[i]=nullptr;
    }
}
User::~User(){  // очищаем все созданные проекты
    for (int i = 0; i < project_count; ++i){
        delete projects[i];
        projects[i] = nullptr;
    }
    for(int i=0;i<reminder_count;++i){
        delete reminders[i];
        reminders[i] = nullptr;

    }
    project_count = 0;
    reminder_count = 0;
}

Project* User::create_project(const string& projName, const string& projDesc){
    if(project_count >= MAX_PROJECTS){
        std::cerr << "Cannot create project: maximum number of projects has reached." << std::endl;
        return nullptr;
    }
    Project *newProj = new Project(projName, projDesc);
    projects[project_count++] = newProj;
    return newProj;
}
void User::delete_project(Project* project){
    for(int i = 0; i < project_count; i++){
        if (projects[i] == project){
            delete projects[i];
            for (int j = i; j < project_count - 1; j++){
                projects[j] = projects[j + 1];
            }
            projects[--project_count] = nullptr;
            return;
        }
    }
    std::cerr << "Project not found, cannot delete." << std::endl;
}
void User::display() const{
    std::cout << "User: " << name << " (ID: " << id << ")" << std::endl;
    std::cout << "Projects (" << project_count << "):" << std::endl;
    for (int i = 0; i < project_count; ++i){
        std::cout << "--- Project " << (i + 1) << " ---" << std::endl;
        projects[i]->display();
        std::cout << std::endl;
    }
}
Project** User::get_projects(){
    return projects;
}
std::string User::get_name() const {
    return name;
}
int User::get_id() const {
    return id;
}
int User::get_project_count() const{
    return project_count;
}
void User::generate_project_report(Project *project) const{
    if(!project){
        std::cerr << "Cannot generate report: project is null." << std::endl;
        return;
    }
    std::cout << "=== Project Report for '" << project->get_name() << "' ===" << std::endl;
    std::cout << *project << std::endl;
}

bool User::add_task_reminder(Project* project, int task_index, const string& time, const string& method){
    if(reminder_count >= MAX_REMINDERS || !project) 
        return false;
    if(task_index < 0 || task_index >= project->get_task_count()) 
        return false;
    Task* t = project->get_task(task_index);
    Reminder* r = new Reminder(t, time, method);
    reminders[reminder_count++] = r;
    return true;
}
bool User::cancel_task_reminder(Project* project, int task_index){
    if(!project) return false;
    Task* t = project->get_task(task_index);
    for(int i=0;i<reminder_count;++i) {
        if(reminders[i]->get_task() == t) {
            delete reminders[i];
            // сдвигаем
            for(int j=i;j<reminder_count-1;++j)
                reminders[j] = reminders[j+1];
            reminders[--reminder_count] = nullptr;
            return true;
        }
    }
    return false;
}
void User::cancel_all_reminders(){
    for(int i=0;i<reminder_count;++i) 
        delete reminders[i];
    reminder_count = 0;
}

void User::show_reminders() const{
    if(reminder_count==0){
        std::cout<<"---no reminders---"<<std::endl;
        return;
    }
    std::cout<<"---your reminders:"<<std::endl;
    for(int i=0;i<reminder_count;++i) {
        std::cout<<"---reminder "<<(i+1)<<"---"<<std::endl;
        reminders[i]->display();
    }
}
bool User::show_task_reminder(Project* project, int task_index) const{
    if (!project || task_index < 0 || task_index >= project->get_task_count())
        return false;
    Task* t = project->get_task(task_index);
    for (int i = 0; i < reminder_count; ++i) {
        if (reminders[i]->get_task() == t) {
            reminders[i]->display();
            return true;
        }
    }
    return false;
}
User::User(const User& other) : name(other.name), id(other.id), project_count(0), reminder_count(0){
    for (int i = 0; i < MAX_PROJECTS; ++i) projects[i] = nullptr;
    for (int i = 0; i < MAX_REMINDERS; ++i) reminders[i] = nullptr;
    for (int i = 0; i < other.project_count; ++i){
        projects[i] = new Project(*other.projects[i]);
        ++project_count;
    }
    for (int i = 0; i < other.reminder_count; ++i){
        Task* orig = other.reminders[i]->get_task();
        for (int pj = 0; pj < project_count; ++pj){
            Project* oldP = other.projects[pj];
            Project* newP = projects[pj];
            for (int tk = 0; tk < oldP->get_task_count(); ++tk){
                if (oldP->get_task(tk) == orig)
                    reminders[reminder_count++] = new Reminder(newP->get_task(tk), other.reminders[i]->get_reminder_time(), other.reminders[i]->get_notification_method());
            }
        }
    }
}
User& User::operator=(const User& other){
    if (this == &other) return *this;
    for (int i = 0; i < project_count; ++i) 
        delete projects[i];
    for (int i = 0; i < reminder_count; ++i) 
        delete reminders[i];
    project_count = reminder_count = 0;
    name = other.name;
    id   = other.id;
    for (int i = 0; i < other.project_count; ++i)
        projects[i] = new Project(*other.projects[i]);
    project_count = other.project_count;
    
    for (int i = 0; i < other.reminder_count; ++i){
        Task* orig = other.reminders[i]->get_task();
        for (int pj = 0; pj < project_count; ++pj){
            Project* oldP = other.projects[pj];
            Project* newP = projects[pj];
            for (int tk = 0; tk < oldP->get_task_count(); ++tk){
                if (oldP->get_task(tk) == orig)
                    reminders[reminder_count++] = new Reminder(newP->get_task(tk), other.reminders[i]->get_reminder_time(), other.reminders[i]->get_notification_method());
            }
        }
    }
    return *this;
}
}