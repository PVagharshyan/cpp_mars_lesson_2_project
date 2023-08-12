#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class Task;

typedef std::vector<std::string> dict;
typedef std::vector<Task> dump_load_type;

//def_func

dict sep_comma(const std::string& line) {
    std::string current_element = "";
    dict result = {};
    for (int i = 0;i < line.length();++i) {
        if (line[i] != ',') {
            current_element += line[i];
        }
        else {
            if (current_element != "") {
                result.push_back(current_element);
                current_element = ""; 
            }
        }
    }
    if (current_element != "") {
        result.push_back(current_element);
    }
    return result;
} 

bool check_element_completed(const std::string& str_element) {
    std::string str_completed = "";
    if (str_element[0] == '[' && str_element[str_element.length() - 1] == ']'){
        for (int i = 1;i < str_element.length() - 1;++i) {
            str_completed += str_element[i];
        }
        if (str_completed == "COMPLITED") {
            return true;
        }
        else if (str_completed == "") {
            return false;
        }
        else {            
            throw std::runtime_error("Error: There is an error in the relevant \"comlited\" section that you imported");
        }
    }
    else { 
        throw std::runtime_error("Error: Section imported in wrong format!");
    }
}

enum class Time {
    HOUR,
    MINUTE
};

const int ASSCI_0 = 48;
const int max_hour = 23;
const int max_minute = 59;

bool is_number(char c) {
    if (c - ASSCI_0 <= 9 && c + ASSCI_0 >= 0) {
        return true;
    }
    return false;
}

bool check_hour(std::string hour) {
    int result = 0;
    for (int i = 0;i < hour.length();++i) {
        result += hour[i] - ASSCI_0;
        result *= 10;
    }
    if (result/10 <= max_hour) {
        return true;
    }
    return false;
}

bool check_minute(std::string minute) {
    int result = 0;
    for (int i = 0;i < minute.length();++i) {
        result += minute[i] - ASSCI_0;
        result *= 10;
    }
    if (result/10 <= max_minute) {
        return true;
    }
    return false;
}

std::string check_element_time(const std::string& str_element) {
    const int position_endpoint = 2;
    const int currect_len_time = 5;
    if (str_element.length() != currect_len_time) {
        throw std::runtime_error("Error: Time imported in wrong format!");
    }
    if (str_element[position_endpoint] == ':') {
        std::map<Time, std::string> time;
        time[Time::HOUR] = "";
        time[Time::MINUTE] = "";
        for (int i = 0;i < position_endpoint;++i) {
            time[Time::HOUR] += str_element[i];
        }
        for (int i = position_endpoint + 1;i < str_element.length();++i) {
            time[Time::MINUTE] += str_element[i];
        }
        if (check_hour(time[Time::HOUR]) && check_minute(time[Time::MINUTE])) {
            return str_element;
        }
        else {
            throw std::runtime_error("Error: Time imported in wrong format!");
        }
    }
    else {
        throw std::runtime_error("Error: Time imported in wrong format!");
    }
}

std::string check_element_task(std::string str_element) {
    if (str_element[0] == '"' && str_element[str_element.length() - 1] == '"') {
        return str_element;
    }
    else {
        throw std::runtime_error("Error: Task imported in wrong format!");
    }
}

dict analysis_single_line(const std::string& line) {
    std::string str_line = "";
    dict result = {};
    if (line[0] == '{' && line[line.length() - 1] == '}') {
        for (int i = 1;i < line.length() - 1;++i) {
            str_line += line[i];
        }
        result = sep_comma(str_line);
        bool completed = check_element_completed(result[0]);
        if (completed) {
            result[0] = "True";
        }
        else {
            result[0] = "False";
        }
        result[1] = check_element_time(result[1]); 
        result[2] = check_element_task(result[2]);
        return result; 
    }
    else {
        throw std::runtime_error("Error: The file contains a line imported in the wrong format!");
    }
}

std::vector<char> events = {'c', 'u', 'd', 'p', 'e'};

bool check_event(std::string ev) {
    if (ev.length() == 1) {
        for (const auto& item: events) {
            if (item == ev[0]) {
                return true; 
            }
        }
    }
    return false;
}

bool check_event_completed(std::string ev) {
    if (ev == "[COMPLITED]" || ev == "[]") {
        return true;
    }
    return false;
}

bool check_id(std::string ev, int limit) {
    if (ev.length() == 1 && is_number(ev[0]) && (ev[0] - ASSCI_0) < limit) {
        return true; 
    }
    return false;
}

bool check_event_time(std::string ev) {
    try{
        check_element_time(ev);
        return true;
    }
    catch (const std::runtime_error& err){
        return false;
    }
}

class Task {
public:
    Task(bool, std::string, std::string);
    //void Update ();
    //void Delete ();
    void set_completed(bool turn_value);
    void set_time(std::string time);
    void set_task(std::string task);
    std::string get_completed() const;
    std::string get_time() const;
    std::string get_task() const; 
    void print_task() const;
public:
    std::string m_task;
    std::string m_time;
    bool m_is_deleted;
    bool m_is_completed;
};

class obj_db {
public:
    obj_db(std::string);
    dump_load_type dump() const;
    void load(const dump_load_type&) const; 
private:
    std::string m_file_name;
};

int main() {
    obj_db db{"Tasks.txt"};
    while (true){
        try {
            dump_load_type all_dump = db.dump();
            std::string str_event = "";
            char c_event = 0;
            do{
                std::cout << "create(c), update(u), delete(d), print(p), exit(e)" << std::endl;
                std::getline(std::cin, str_event);
            } while (!check_event(str_event));
            c_event = str_event[0];
            if (c_event == 'u') {
                int counter = 0;
                for (const auto& item: all_dump) {
                    std::cout << counter++ << " : ";
                    item.print_task(); 
                }
                std::string id_task = "";
                while (!check_id(id_task, all_dump.size())) {
                    std::cout << "id: ";
                    std::getline(std::cin, id_task);
                }
                //is completed
                std::string ev_completed = "";
                while (!check_event_completed(ev_completed)) {
                    std::cout << "completed or not([COMPLITED]/[]):";
                    std::getline(std::cin, ev_completed); 
                }
                bool u_com = false;
                if (ev_completed == "[COMPLITED]") {
                    u_com = true;
                }
                all_dump[id_task[0] - ASSCI_0].set_completed(u_com); 
                //time
                std::string ev_time = "";
                while (!check_event_time(ev_time)) {
                    std::cout << "time(hour:minute):";
                    std::getline(std::cin, ev_time); 
                }
                all_dump[id_task[0] - ASSCI_0].set_time(ev_time);
                //task
                std::string ev_task = "";
                std::cout << "task: ";
                std::getline(std::cin, ev_task);
                all_dump[id_task[0] - ASSCI_0].set_task("\""+ ev_task + "\"");
            }
            else if (c_event == 'c') {
                //is completed
                std::string ev_completed = "";
                while (!check_event_completed(ev_completed)) {
                    std::cout << "completed or not([COMPLITED]/[]):";
                    std::getline(std::cin, ev_completed); 
                }
                bool u_com = false;
                if (ev_completed == "[COMPLITED]") {
                    u_com = true;
                } 
                //time
                std::string ev_time = "";
                while (!check_event_time(ev_time)) {
                    std::cout << "time(hour:minute):";
                    std::getline(std::cin, ev_time); 
                }
                //task
                std::string ev_task = "";
                std::cout << "task: ";
                std::getline(std::cin, ev_task);
                all_dump.push_back(Task{u_com, ev_time, "\"" + ev_task + "\""});
            }
            else if (c_event == 'd') {
                int counter = 0;
                for (const auto& item: all_dump) {
                    std::cout << counter++ << " : ";
                    item.print_task(); 
                }
                std::string id_task = "";
                while (!check_id(id_task, all_dump.size())) {
                    std::cout << "id: ";
                    std::getline(std::cin, id_task);
                }
                all_dump.erase(all_dump.begin() + (id_task[0] - ASSCI_0));
            }
            else if (c_event == 'p') {
                int counter = 0;
                for (const auto& item: all_dump) {
                    std::cout << counter++ << " : ";
                    item.print_task(); 
                }
            }
            else {
                break;
            }
            db.load(all_dump);
        }
        catch (const std::runtime_error& err) {
            std::cerr << err.what() << std::endl;
        }
    }
    return 0;
}

//Task

Task::Task(bool completed, std::string time, std::string task) 
    :m_is_completed(completed), m_time(time), m_task(task), m_is_deleted(false)
{}

void Task::set_completed(bool turn_value) {
    m_is_completed = turn_value;
}

void Task::set_time(std::string time) {
    m_time = time;
}

void Task::set_task(std::string task) {
    m_task = task;
}

std::string Task::get_completed() const {
    if (m_is_completed == true) {
        return "[COMPLITED]";      
    }
    return "[]";
}

std::string Task::get_time() const {
    return m_time;
}

std::string Task::get_task() const {
    return m_task;
}

void Task::print_task() const {
    std::string completed = "";
    std::string time = m_time;
    std::string task = m_task;
    if (m_is_completed) {
        completed = "[COMPLITED]";
    }
    else {
        completed = "[         ]";
    }
    std::string current_task = completed + ",\t" + time + ",\t" + task;
    std::cout << current_task << std::endl;
}

//obj_db

obj_db::obj_db(std::string file_name) {
    std::ifstream file;
    file.open(file_name);
    if (file.is_open()) {
        std::cout << "File open successfully" << std::endl;
        m_file_name = file_name;
    }
    else {
        throw std::runtime_error("Error: File is not opened!");
    }
    file.close();
}

dump_load_type obj_db::dump() const {  
    std::ifstream file;
    file.open(m_file_name);
    dump_load_type result = {};
    if (file.is_open()) {
        std::string current_line = "";
        while (std::getline(file, current_line)) {
            dict current_line_dict = analysis_single_line(current_line);
            bool completed = false;
            if (current_line_dict[0] == "True") {
                completed = true;
            } 
            result.push_back(Task(
                        completed,
                        current_line_dict[1],
                        current_line_dict[2]
                        ));
        } 
    }
    else {
        throw std::runtime_error("Error: File is not opened!");
    }
    file.close();
    return result;
}

void obj_db::load(const dump_load_type& load_value) const {
    std::ofstream file;
    file.open(m_file_name);
    if (file.is_open()) {
        for (int i = 0;i < load_value.size();++i) {
            std::string completed = load_value[i].get_completed();
            std::string time = load_value[i].get_time();
            std::string task = load_value[i].get_task();
            std::string current_result_str = "{" + completed + "," + time + "," + task + "}";
            if (i != load_value.size() - 1)
                file << current_result_str << std::endl;
            else
                file << current_result_str;
        }
    }
    else {
        throw std::runtime_error("Error: File is not opened!");
    }
    file.close();
}

