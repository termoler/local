#ifndef SOUND_PROCESSOR_CONTROLLER_CONVERT_H
#define SOUND_PROCESSOR_CONTROLLER_CONVERT_H

#include <iostream>
#include <vector>
#include <array>
#include "complete_convert.h"
#include "wav_header.h"
#include "config_to_conditions_arr.h"

class controller_convert{
private:
    void prepare_header(std::ifstream& in, std::ofstream& out);

    void process_file(std::ifstream& in, std::ofstream& out,
                      const std::vector<std::pair<std::string, std::vector<int>>>& conditions,
                      const std::vector<std::string>& input_files);
public:
    void convert(const std::string &config_name, const std::string& out_filename,
                 const std::vector<std::string>& input_files);
};

#endif //SOUND_PROCESSOR_CONTROLLER_CONVERT_H
