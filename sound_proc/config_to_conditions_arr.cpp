#include "config_to_conditions_arr.h"

bool config_to_conditions_arr::is_empty(const std::string &tmp){
    if(tmp.empty()) return true;
    return false;
}

bool config_to_conditions_arr::is_comment(const std::string &tmp){
    if(tmp.substr(0, 1) == "#") return true;
    return false;
}

void config_to_conditions_arr::check_line(const std::string &tmp){
    if(tmp.substr(0,4) != "mute" && tmp.substr(0,4) != "mix " &&
        tmp.substr(0,6) != "double" && !is_empty(tmp) && !is_comment(tmp)){
        throw std::runtime_error("incorrect name of convert in config");
    }
}
std::vector<int> config_to_conditions_arr::get_data(const std::string &tmp){
    std::string tmp1 = tmp;
    for (size_t k = tmp1.find('$'); k != std::string::npos;  k = tmp1.find('$', k)){
        tmp1.erase(k, 1);
    }
    std::istringstream iss(tmp1);
    std::vector<int> data_from_config {std::istream_iterator<int>(iss), std::istream_iterator<int>()};
    return data_from_config;
}

size_t config_to_conditions_arr::get_size_sample(const std::string& name){
    std::ifstream in(name, std::ios::binary | std::ios::ate);
    if (!in) {
        std::cerr << "Failed to open file " << name << std::endl;
        throw;
    }
    size_t size = in.tellg();
    in.seekg(40, std::ios::beg);
    unsigned long extra_info;
    in.read(reinterpret_cast<char *>(&extra_info), sizeof(extra_info));
    in.close();
    return (size_t)((size - extra_info - 40 - 12)/44100);
}

void config_to_conditions_arr::check_mute(const std::vector<std::string>& input_files, const std::vector<int>& data_from_config){
    size_t size_file_for_mute;
    try{
        size_file_for_mute = get_size_sample(input_files[0]);
    }
    catch(const std::exception &ex){
        throw;
    }
    if(data_from_config[0] < 0 || data_from_config[1] > size_file_for_mute ||
       data_from_config[0] > data_from_config[1]){
        throw std::runtime_error("you enter incorrectly value start seconds in a field of mute, you need to enter "
                                 " 0 <= value <= song duration. for reference : song duration = "
                                 + std::to_string(size_file_for_mute) + " seconds");
    }
}

void config_to_conditions_arr::check_mix(const std::vector<std::string>& input_files, const std::vector<int>& data_from_config){
    if(input_files.size() <= 1){
        throw std::runtime_error("not enough input files for mix. you need to enter more files");
    }
    size_t size_file_for_mix;
    try{
        size_file_for_mix = get_size_sample(input_files[0]);
    }
    catch(const std::exception &ex){
        throw;
    }
    if(data_from_config[1] < 0 || data_from_config[1] > size_file_for_mix){
        throw std::runtime_error("you enter incorrectly value start seconds in a field of mix, you need to enter "
                                 " 0 <= value <= song duration. for reference : song duration = "
                                 + std::to_string(size_file_for_mix) + " seconds");
    }
}

void config_to_conditions_arr::check_double(const std::vector<std::string>& input_files, const std::vector<int>& data_from_config){
    size_t size_file_for_double;
    try{
        size_file_for_double = get_size_sample(input_files[0]);
    }
    catch(const std::exception &ex){
        throw;
    }
    if(data_from_config[0] < 0 || data_from_config[1] > size_file_for_double ||
       data_from_config[0] > data_from_config[1]){
        throw std::runtime_error("you enter incorrectly value start seconds in a field of mute, you need to enter "
                                 " 0 <= value <= song duration. for reference : song duration = "
                                 + std::to_string(size_file_for_double) + " seconds");
    }
}

std::vector<std::pair<std::string, std::vector<int>>> config_to_conditions_arr::get_conditions(const std::string &config_name,
                                                                     const std::vector<std::string>& input_files){
    std::ifstream config(config_name);
    std::string tmp;
    std::vector<std::pair<std::string, std::vector<int>>> conditions;
    while(std::getline(config, tmp, '\n')) {
        try{
            check_line(tmp);
            if (tmp.substr(0,4) == "mute"){
                std::vector<int> data_from_config = get_data(tmp.substr(4));
                check_mute(input_files, data_from_config);
                conditions.emplace_back(tmp.substr(0,4), data_from_config);
            }
            else if (tmp.substr(0,3) == "mix"){
                std::vector<int> data_from_config = get_data(tmp.substr(3));
                check_mix(input_files, data_from_config);
                conditions.emplace_back(tmp.substr(0,3), data_from_config);
            }
            else if (tmp.substr(0,6) == "double"){
                std::vector<int> data_from_config = get_data(tmp.substr(6));
                check_double(input_files, data_from_config);
                conditions.emplace_back(tmp.substr(0,6), data_from_config);
            }
        }
        catch(const std::exception &ex){
            throw;
        }
    }
    config.close();
    return conditions;
}