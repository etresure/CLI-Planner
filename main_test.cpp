#include <iostream>
#include <string>
#include <limits>

#include "tasks/task.hpp"
#include "tasks/urgent_collab.hpp"
#include "tasks/reminder.hpp" 
#include "model/project.hpp"
#include "model/user.hpp"

using std::cout, std::endl, std::cin, std::string, std::streamsize, std::numeric_limits;

void setColor(int code) {
    cout << "\033[" << code << "m";
}
void resetColor() {
    cout << "\033[0m";
}
int check_input(int p_count){
    int proj_num;
    while (true){
        if (!(cin >> proj_num) && !cin.eof()){
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "---invalid input, please enter a number---\n"; cout << "> "; continue;
        }
        if (proj_num < 1 || proj_num > p_count) {cout << "---invalid project number, try again---\n"; cout << "> "; continue;}
        break;
    }
    return proj_num;
}
void work(){
    using namespace cli_planner;
    User* users[MAX_USERS];
    int user_count   = 0;
    int current_user = -1;
    setColor(36);
    cout << "---starting the CLI Planner!---\n";
    string name1;
    int id1;
    while (true) {
        cout << "---input user name and id (example: Alice 1) or 'exit' to quite the program---\n";
        setColor(34);
        cout << "> ";
        setColor(36);
        cin >> name1; if (name1 == "exit") return; if(cin.eof()){ cout << endl; exit(0); }
        if (cin >> id1 ) break;
        cout << "--err: input user name (one word) and ID number separated by a space---\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    int p_count;
    users[user_count++] = new User(name1, id1);
    current_user = 0;
    cout << "---active user: " << users[current_user]->get_name() << " ---\n";
    cout << "---!for all task commands to work correctly, you need to create at least one project!---\n";
    cout << "---input 'add_proj' to create your first project---\n";
    cout << "---input 'help' to see all possible commands---\n";
    Project **proj;
    while(!cin.eof()){
        setColor(34);
        cout << "> ";
        setColor(36);
        int proj_num, t_num;
        string p_name, p_descr, cmd;
        cin >> cmd;
        if (cmd == "add_user"){
            string nm; int iid;
            cout << "---input user name and id---\n"; cout << "> ";
            cin >> nm >> iid; bool flag = true;
            if (user_count < MAX_USERS){
                for (int i = 0; i < user_count; i++)
                    if (users[i]->get_id() == iid)
                        flag = false;
                if (flag){
                    users[user_count] = new User(nm, iid);
                    cout << "---user #" << (user_count+1) << " added: " << nm << " ---\n";
                    user_count++; continue;
                }
                cout << "---user with id: " << iid << " is already exist, try again---\n";
            } 
            else
                cout << "---user limit reached---\n";
        }
        else if (cmd == "list_users"){
            for (int i = 0; i < user_count; ++i){
                cout << i+1 << ": " << users[i]->get_name() << " (ID=" << users[i]->get_id() << ")";
                if (i == current_user) 
                    cout << " [active]";
                cout << "\n";
            }
        }
        else if (cmd == "select_user") {
            cout << "---input user id---\n"; cout << "> ";
            int id;
            cin >> id;
            bool found = false;
            for (int i = 0; i < user_count; ++i) {
                if (users[i]->get_id() == id) {
                    current_user = i;
                    cout << "---switched active user to " << users[i]->get_name() << " ---\n";
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << "---user with this id not found---\n";
        }
        else if (cmd == "show_projects"){
            p_count = users[current_user]->get_project_count();
            proj = users[current_user]->get_projects();
            for (int i = 0; i < p_count; ++i){
                cout << "---project number: " << i + 1 << "---\n"; 
                proj[i]->display();
            }
            cout << endl;
        }
        else if (cmd == "add_proj"){
            cout << "---input project name (one word) and description separated by a space---\n"; cout << "> ";
            cin >> p_name;
            std::getline(cin, p_descr);
            users[current_user]->create_project(p_name, p_descr);
            cout << "---project " << p_name << " was created---" << endl;
            cout << endl;
            p_count = users[current_user]->get_project_count();
        }
        else if (cmd == "del_proj"){
            cout << "---input project number---\n"; cout << "> ";
            proj_num = check_input(p_count);
            proj = users[current_user]->get_projects();
            string name = proj[proj_num-1]->get_name();
            users[current_user]->delete_project(proj[proj_num-1]);
            cout << "---project" <<  name << " was deleted---" << endl;
            cout << endl;
            p_count = users[current_user]->get_project_count();
        }
        else if (cmd == "show_user"){
            users[current_user]->display();
        }
        else if (cmd == "proj_report"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            users[current_user]->generate_project_report(proj[proj_num-1]);
        }
        else if (cmd == "add_task"){
            string t_name, t_descr;
            cout << "---input task name and description (one word) separated by a space---\n"; cout << "> ";
            cin >> t_name;
            std::getline(cin, t_descr);
            cout << "---input project number to add the task---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            proj = users[current_user]->get_projects();
            Task *t1 = new Task(t_name, t_descr);
            (*proj[proj_num-1]) += t1;
            cout << "---new task was added to the project " <<  proj[proj_num-1]->get_name() << "---\n";
            cout << endl;
        }
        else if (cmd == "del_task"){
            cout << "---input project number to delete the task---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            Task *t1 = proj[proj_num-1]->get_task(t_num-1);
            if (t1 != nullptr){
                proj[proj_num-1]->remove_task(t1);
                cout << "---task was deleted from project " << proj[proj_num-1]->get_name() << "---\n";
            }
            else    
                cout << "---no task was found---\n";
        }
        else if(cmd == "get_proj_name"){ 
            cout << "---input project number---\n"; cout << "> ";
            int proj_num;
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            proj = users[current_user]->get_projects();
            cout << "---" << proj[proj_num - 1]->get_name() << "---\n";
            cout << endl;
        }
        else if(cmd == "get_comp_perc"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            proj = users[current_user]->get_projects();
            cout << "---completion percentage of " << proj[proj_num - 1]->get_name() << " is " << proj[proj_num - 1]->get_completion_percentage() << " ---\n";
            cout << endl;
        }
        else if(cmd == "get_task_count"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            proj = users[current_user]->get_projects();
            cout << "---task count of " << proj[proj_num - 1]->get_name() << " is " << proj[proj_num - 1]->get_task_count() << " ---\n";
            cout << endl;
        }
        else if(cmd == "get_project_count"){
            proj = users[current_user]->get_projects();
            cout << "---project count is " << (*proj)->get_project_count() << " ---\n";
            cout << endl;
        }
        else if(cmd == "full_proj_info"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            proj = users[current_user]->get_projects();
            cout << "---full information about " << proj[proj_num - 1]->get_name() << ":\n";
            cout << (*proj[proj_num - 1]);
        }
        else if (cmd == "new_task_title"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            string new_title;
            cout << "---input new task title--\n"; cout << "> ";
            cin >> new_title;
            Task* t = proj[proj_num - 1]->get_task(t_num - 1);
            if (t){
                t->set_title(new_title);
                std::cout << "---title was updated---\n";
            }
            else
                cout << "---no task was found---\n";
        }
        else if (cmd == "new_task_descr"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            string new_descr;
            cout << "---input new task decsription--\n"; cout << "> ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(cin, new_descr);
            Task* t = proj[proj_num - 1]->get_task(t_num - 1);
            if (t){
                t->set_description(new_descr);
                std::cout << "---decsription was updated---\n";
            }
            else
                cout << "---no task was found---\n";
        }
        else if (cmd == "task_due_date"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            string new_date;
            cout << "---input new task due date--\n"; cout << "> ";
            cin >> new_date;
            Task* t = proj[proj_num - 1]->get_task(t_num - 1);
            if (t){
                t->set_due_date(new_date);
                std::cout << "---due date was updated---\n";
            }
            else
                cout << "---no task was found---\n";
        }
        
        else if (cmd == "set_task_priority"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            string new_priority;
            cout << "---input new task priority: low, medium or high--\n"; cout << "> ";
            cin >> new_priority;
            Task* t = proj[proj_num - 1]->get_task(t_num - 1);
            if (t){
                try{
                    auto pr = parse_priority(new_priority);
                    t->update_priority(pr);
                    cout << "---priority was updated---\n";
                  }
                catch (const std::invalid_argument& e){
                    cout << "---invalid priority: " << e.what() << "---\n";
                }
            }
            else
                cout << "---no task was found---\n";
        }
        else if (cmd == "set_task_status"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            string new_status;
            cout << "---input new task status: pending, working or done--\n"; cout << "> ";
            cin >> new_status;
            Task* t = proj[proj_num - 1]->get_task(t_num - 1);
            if (t){
                try{
                    auto pr = parse_status(new_status);
                    t->update_status(pr);
                    cout << "---status was updated---\n";
                  }
                catch (const std::invalid_argument& e){
                    cout << "---invalid status: " << e.what() << "---\n";
                }
            }
            else
                cout << "---no task was found---\n";
        }
        else if (cmd == "show_task"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            Task* t = proj[proj_num - 1]->get_task(t_num - 1);
            if (t){
                t->display();
                cout << "\n";
            }
            else
                cout << "---no task was found---\n";
        }
        else if (cmd == "inc_task_prio"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            Task* t = proj[proj_num - 1]->get_task(t_num - 1);
            if (t){
                ++(*t);
                std::cout << "---priority was increased---\n";
            }
            else
                cout << "---no task was found---\n";
        }
        else if (cmd == "dec_task_prio"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            Task* t = proj[proj_num - 1]->get_task(t_num - 1);
            if (t){
                --(*t);
                std::cout << "---priority was decreased---\n";
            }
            else
                cout << "---no task was found---\n";
        }
        else if (cmd == "show_compl_tasks") {
            cout << "---input project number to list completed tasks---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            proj = users[current_user]->get_projects();
            Project* p = proj[proj_num - 1];
        
            int totalTasks = p->get_task_count();
            int doneCount = 0;
            for (int i = 0; i < totalTasks; ++i){
                if (p->get_task(i)->get_status() == Status::done)
                    ++doneCount;
            }
            Task** completed = p->get_completed_tasks();
            cout << "---completed tasks in project \"" 
                 << p->get_name() << "\" (" << doneCount << ")---\n";
            for (int i = 0; i < doneCount; ++i) {
                cout << "[Task " << (i+1) << "]\n";
                completed[i]->display();
                cout << endl;
            }
            delete[] completed;
        
            cout << endl;
        }
        else if (cmd == "set_rem") {
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            string when;
            cout << "---input time--\n"; cout << "> ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, when);
            string method;
            cout << "---input notification method or write 'skip' to stay console--\n"; cout << "> ";
            cin >> method;
                if(method == "skip")
                    method = "console";
            proj = users[current_user]->get_projects();
            if (users[current_user]->add_task_reminder(proj[proj_num-1], t_num-1, when, method))
                std::cout << "---reminder set---\n";
            else
                std::cout << "---failed to set reminder---\n";
        }
        else if (cmd == "cancel_rem") {
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            if (users[current_user]->cancel_task_reminder(proj[proj_num - 1], t_num - 1))
                std::cout << "---reminder canceled---\n";
            else
                std::cout << "---no such reminder---\n";
        }
        else if (cmd == "show_all_rem"){
            users[current_user]->show_reminders();
        }
        else if (cmd == "cancel_all_rem") {
            users[current_user]->cancel_all_reminders();
            std::cout << "---all reminders cleared---\n";
        }
        else if (cmd == "show_rem") {
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number--\n"; cout << "> ";
            cin >> t_num;
            proj = users[current_user]->get_projects();
            bool ok = users[current_user]->show_task_reminder(proj[proj_num-1], t_num-1);
            if (!ok)
                cout << "---no such reminder---\n";
        }
        else if (cmd == "add_urg_task"){
            string t_name, t_descr;
            cout << "---input task name and description (one word) separated by a space---\n"; cout << "> ";
            cin >> t_name;
            std::getline(cin, t_descr);
            cout << "---input urgency level (0 - 10)---\n"; cout << "> ";
            int urg_level;
            cin >> urg_level;
            UrgentTask *t1 = new UrgentTask();
            try{ t1->set_urgency_level(urg_level);}
            catch(int n){
                cout << "---wrong urgency level---\n";
                delete t1;
                continue;
            }
            cout << "---input project number to add the task---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            proj = users[current_user]->get_projects();
            t1->set_description(t_descr);
            t1->set_title(t_name);
            (*proj[proj_num-1]) += t1;
            cout << "---new urgent task was added to the project " <<  proj[proj_num-1]->get_name() << "---\n";
            cout << endl;
        }
        else if (cmd == "set_task_urg"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number---\n"; cout << "> ";
            int task_num; 
            cin >> task_num;
            cout << "---input new urgency level (0 - 10)---\n"; cout << "> ";
            int new_level; 
            cin >> new_level;
        
            proj = users[current_user]->get_projects();
            Task* base = proj[proj_num-1]->get_task(task_num-1);
            if (auto ut = dynamic_cast<UrgentTask*>(base)){
                try{ ut->set_urgency_level(new_level); }
                catch(int n){ cout << "---wrong urgency level---\n"; continue; }
                cout << "---urgency level updated to " << new_level << "---\n";
            } 
            else 
                cout << "---selected task is not urgent---\n";
        }
        else if (cmd == "get_task_urg") {
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number---\n"; cout << "> ";
            int task_num; 
            cin >> task_num;
            proj = users[current_user]->get_projects();
            Task* base = proj[proj_num-1]->get_task(task_num-1);
            if (auto ut = dynamic_cast<UrgentTask*>(base))
                cout << "---current urgency level: " << ut->get_urgency_level() << "---\n";
            else
                cout << "---selected task is not urgent---\n";
        }
        else if (cmd == "add_collab_task"){
            string t_name, t_descr;
            cout << "---input task name and description (one word) separated by a space---\n"; cout << "> ";
            cin >> t_name;
            std::getline(cin, t_descr);
            cout << "---input project number to add the task---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            proj = users[current_user]->get_projects();
            CollaborativeTask *t1 = new CollaborativeTask();
            t1->set_description(t_descr);
            t1->set_title(t_name);
            (*proj[proj_num-1]) += t1;
            cout << "---new collaborative task was added to the project " <<  proj[proj_num-1]->get_name() << "---\n";
            cout << endl;
        }
        else if (cmd == "assign_collab"){
            cout << "---input project number, task number and user number separated by a space to assign---\n"; cout << "> ";
            int proj_num, task_num, user_num;
            cin >> proj_num >> task_num >> user_num;
            User* owner = users[current_user];
            p_count = owner->get_project_count();
            proj = owner->get_projects();
            if (proj_num < 1 || proj_num > p_count){
                cout << "---invalid project number---\n";
                continue;
            }
            Task* base = proj[proj_num-1]->get_task(task_num-1);
            auto ct = dynamic_cast<CollaborativeTask*>(base);
            if (!ct){
                cout << "---selected task is not collaborative---\n";
                continue;
            }
            if (user_num < 1 || user_num > user_count){
                cout << "---invalid user number---\n";
                continue;
            }
            User* toAssign = users[user_num-1];
            ct->assign_user(toAssign);
            cout << "---user " << toAssign->get_name() << " assigned to task---\n";
        }
        else if (cmd == "remove_collab"){
            cout << "---input project number, task number and user number separated by a space to remove---\n"; cout << "> ";
            int proj_num, task_num, user_num;
            cin >> proj_num >> task_num >> user_num;
            User* owner = users[current_user];
            p_count = owner->get_project_count();
            proj = owner->get_projects();
            if (proj_num<1||proj_num>p_count){ 
                cout<<"---invalid project number---\n"; 
                continue; 
            }
            Task* base = proj[proj_num-1]->get_task(task_num-1);
            auto ct = dynamic_cast<CollaborativeTask*>(base);
            if (!ct){ 
                cout<<"---not a collaborative task---\n"; 
                continue; 
            }
            if (user_num<1||user_num>user_count){ 
                cout<<"---invalid user number---\n"; 
                continue; 
            }
            User* toRemove = users[user_num-1];
            ct->remove_user(toRemove);
            cout << "---user " << toRemove->get_name() << " removed from task---\n";
        }
        else if (cmd == "set_collab_progress"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            int task_num;
            proj_num = check_input(p_count);
            cout << "---input task number---\n"; cout << "> ";
            cin >> task_num;
            proj = users[current_user]->get_projects();
            cout << "---input new progress (0–100)%---\n"; cout << "> ";
            int prog; cin >> prog;
            Task* base = proj[proj_num-1]->get_task(task_num-1);
            auto ct = dynamic_cast<CollaborativeTask*>(base);
            if (!ct)
                cout << "---selected task is not collaborative---\n";
            else{
                ct->set_progress(prog);
                cout << "---progress set to " << ct->get_progress() << "%---\n";
            }
        }
        else if (cmd == "get_collab_progress"){
            cout << "---input project number---\n"; cout << "> ";
            p_count = users[current_user]->get_project_count();
            int task_num;
            proj_num = check_input(p_count);
            cout << "---input task number---\n"; cout << "> ";
            cin >> task_num;
            proj = users[current_user]->get_projects();
            Task* base = proj[proj_num-1]->get_task(task_num-1);
            auto ct = dynamic_cast<CollaborativeTask*>(base);
            if (!ct)
                cout << "---selected task is not collaborative---\n";
            else{
                cout << "---current progress: " << ct->get_progress() << "%---\n";
            }
        }
        else if (cmd == "show_collab") {
            cout << "---input project number---\n"; cout << "> ";
            int task_num;
            p_count = users[current_user]->get_project_count();
            proj_num = check_input(p_count);
            cout << "---input task number---\n"; cout << "> ";
            cin >> task_num;
            proj = users[current_user]->get_projects();
            Task* base = proj[proj_num-1]->get_task(task_num-1);
            auto ct = dynamic_cast<CollaborativeTask*>(base);
            if (!ct) {
                cout << "---selected task is not collaborative---\n";
            } 
            else{
                ct->display(); cout << endl;
            }
        }
        else if (cmd == "help"){
            std::cout <<
                "---available commands---\n"
                "  add_user              — Create a new user\n"
                "  list_users            — List all users (marks active)\n"
                "  select_user           — Switch active user by ID\n"
                "  show_projects         — List projects of active user\n"
                "  add_proj              — Create a new project\n"
                "  del_proj              — Delete a project\n"
                "  show_user             — Display active user’s info\n"
                "  proj_report           — Detailed report for one project\n"
                "  get_proj_name         — Print a project’s name\n"
                "  get_project_count     — Number of projects\n"
                "  full_proj_info        — Full info of one project\n"
                "  get_comp_perc         — % of completed tasks in a project\n"
                "  get_task_count        — Number of tasks in a project\n"
                "  add_task              — Add a Task to a project\n"
                "  del_task              — Delete a Task from a project\n"
                "  show_task             — Show one Task’s details\n"
                "  new_task_title        — Rename a Task\n"
                "  new_task_descr        — Change a Task’s description\n"
                "  task_due_date         — Set a Task’s due date\n"
                "  set_task_priority     — Change Task priority\n"
                "  inc_task_prio         — Increase Task priority\n"
                "  dec_task_prio         — Decrease Task priority\n"
                "  set_task_status       — Change Task status\n"
                "  show_compl_tasks      — List only completed tasks\n"
                "  set_rem               — Create a reminder on a Task\n"
                "  cancel_rem            — Cancel a Task’s reminder\n"
                "  show_rem              — Show reminder for one Task\n"
                "  show_all_rem          — List all reminders\n"
                "  cancel_all_rem        — Remove all reminders\n"
                "  add_urg_task          — Add an UrgentTask to a project\n"
                "  set_task_urg          — Change an UrgentTask’s urgency level\n"
                "  get_task_urg          — Show an UrgentTask’s urgency level\n"
                "  add_collab_task       — Add a CollaborativeTask to a project\n"
                "  assign_collab         — Assign a user to a CollaborativeTask\n"
                "  remove_collab         — Remove a user from a CollaborativeTask\n"
                "  set_collab_progress   — Set CollaborativeTask progress\n"
                "  get_collab_progress   — Show CollaborativeTask progress\n"
                "  show_collab           — Show full info of a CollaborativeTask\n"
                "  help                  — Show this list of commands\n"
                "  exit                  — Quit the application\n";
                if (p_count == 0) cout << "---input 'add_proj' to create your first project---\n";
        }
        else if (cmd == "exit")
            break;
        else{
            setColor(31); cout << "---wrong command! try again---\n"; resetColor();}
    }
    for (int i = 0; i < user_count; ++i)
        delete users[i];
}

int main(){
    using std::cerr, std::bad_alloc;
    try{
       work();    
    }
    catch (const bad_alloc& e){ 
        cerr << "---failed to allocate memory: " << e.what() << "---\n";
        return 1;
    }
    catch (...){
        cerr << "---unexpected error---\n";
        return 1;
    }

    cout << endl;
    return 0;
}
