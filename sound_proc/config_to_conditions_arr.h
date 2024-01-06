#ifndef SOUND_PROCESSOR_CONFIG_TO_CONDITIONS_ARR_H
#define SOUND_PROCESSOR_CONFIG_TO_CONDITIONS_ARR_H

#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>

class config_to_conditions_arr{
private:
    bool is_empty(const std::string &tmp);
    bool is_comment(const std::string &tmp);
    void check_line(const std::string &tmp);
    std::vector<int> get_data(const std::string &tmp);
    size_t get_size_sample(const std::string& name);
    void check_mute(const std::vector<std::string>& input_files, const std::vector<int>& data_from_config);
    void check_mix(const std::vector<std::string>& input_files, const std::vector<int>& data_from_config);
    void check_double(const std::vector<std::string>& input_files, const std::vector<int>& data_from_config);

public:
    std::vector<std::pair<std::string, std::vector<int>>> get_conditions(const std::string &config_name,
                                                                         const std::vector<std::string>& input_files);
};
#endif //SOUND_PROCESSOR_CONFIG_TO_CONDITIONS_ARR_H
