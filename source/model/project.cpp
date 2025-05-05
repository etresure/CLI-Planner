#include "model/project.hpp"
#include "tasks/task.hpp"  

namespace cli_planner{

Project::Project() : name(""), description(""), task_count(0){
    for (int i = 0; i < MAX_TASKS; ++i){
        tasks[i] = nullptr;
    }
    project_count++;
}
Project::Project(const std::string& newName, const std::string& newDescription)
 : name(newName), description(newDescription), task_count(0){
    for (int i = 0; i < MAX_TASKS; ++i){
        tasks[i] = nullptr;
    }
    project_count++;
}
Project::Project(const Project& other) : name(other.name), description(other.description), task_count(other.task_count)
{
    ++project_count;
    for (int i = 0; i < task_count; ++i){
        tasks[i] = (*other.tasks[i]).smart_copy_task();
    }
    for (int i = task_count; i < MAX_TASKS; ++i) {
        tasks[i] = nullptr;
    }
}
Project& Project::operator=(const Project& other) {
    if (this == &other) 
        return *this;
    for (int i = 0; i < task_count; ++i){
        delete tasks[i];
        tasks[i] = nullptr;
    }
    name        = other.name;
    description = other.description;
    task_count  = other.task_count;
    for (int i = 0; i < task_count; ++i) 
        tasks[i] = (*other.tasks[i]).smart_copy_task();
    for (int i = task_count; i < MAX_TASKS; ++i)
        tasks[i] = nullptr;
    return *this;
}
Project::~Project(){  // очищаем все созданные задачи
    for (int i = 0; i < task_count; ++i){
        delete tasks[i];
        tasks[i] = nullptr;
    }
    task_count = 0;
    project_count--;
}
void Project::add_task(Task* task){ // добавление задачи по указателю!!!
    if (task_count < MAX_TASKS && task != nullptr){
        tasks[task_count++] = task;
    }
}
// добавление задачи по заголовку и описанию (создается новый объект Task)
void Project::add_task(const std::string& title, const std::string& description){
    Task* task = new Task(title, description);
    add_task(task);
}
// ищем и удаляем указатель, сдвигаем массив
void Project::remove_task(Task* task){
    for (int i = 0; i < task_count; ++i){
        if (tasks[i] == task){
            delete tasks[i];
            for (int j = i; j < task_count - 1; ++j){
                tasks[j] = tasks[j + 1];
            }
            tasks[--task_count] = nullptr;
            break;
        }
    }
}
// вывод информации о проекте и задачах
void Project::display() const{
    std::cout << "Project: " << name << "\n"
              << "Description: " << description << "\n"
              << "Tasks (" << task_count << "):\n";
    for (int i = 0; i < task_count; ++i){
        std::cout << "  [" << i + 1 << "] ";
        tasks[i]->display();
    }
}
const std::string& Project::get_name() const &{
    return name;
}
int Project::get_project_count(){
    return project_count;
}
int Project::get_task_count() const{
    return task_count;
}
double Project::get_completion_percentage() const{
    if (task_count == 0) return 0.0;
    int done = 0;
    for (int i = 0; i < task_count; ++i){
        if (tasks[i]->get_status() == Status::done){
            ++done;
        }
    }
    return (static_cast<double>(done) / task_count) * 100.0;
}
// возвращаем массив указателей на завершённые задачи!!!!! в main надо удалить!!!!
Task** Project::get_completed_tasks(){
    int done = 0;
    for (int i = 0; i < task_count; ++i){
        if (tasks[i]->get_status() == Status::done){
            ++done;
        }
    }
    Task** completed = new Task*[done];
    int idx = 0;
    for (int i = 0; i < task_count; ++i){
        if (tasks[i]->get_status() == Status::done){
            completed[idx++] = tasks[i];
        }
    }
    return completed;
}
Project& Project::operator+=(Task* task){
    add_task(task);
    return *this;
}
Task* Project::get_task(int idx) {
    return (idx >= 0 && idx < task_count) ? tasks[idx] : nullptr;
}
Task* Project::take_task(int idx) {
    if (idx < 0 || idx >= task_count) return nullptr;
    Task* t = tasks[idx];
    // сдвигаем оставшиеся
    for (int j = idx; j < task_count - 1; ++j)
        tasks[j] = tasks[j+1];
    tasks[--task_count] = nullptr;
    return t;
}
void cli_planner::Project::print(std::ostream &out) const{
    out << "Project: " << name << "\n"
       << "Description: " << description << "\n"
       << "Completion: " << get_completion_percentage() << "%\n"
       << "Tasks (" << task_count << "): \n";
    for (int i = 0; i < task_count; ++i){
        out << "  [" << i + 1 << "] " << (*tasks[i]) << "\n";
    }
}
int Project::project_count = 0;
}

std::ostream& operator<<(std::ostream& out, const cli_planner::Project &project){
    project.print(out);
    return out;
}