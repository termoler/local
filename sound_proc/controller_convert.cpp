#include "controller_convert.h"

void controller_convert::prepare_header(std::ifstream& in, std::ofstream& out){
    wav_header file{};
    in.read(reinterpret_cast<char*>(&file), sizeof(file));

    in.seekg(file.subchunk_2_size, std::ios::cur);
    in.read(file.subchunk_2_id, sizeof(file.subchunk_2_id));
    in.read(reinterpret_cast<char*>(&file.subchunk_2_size), sizeof(file.subchunk_2_size));

    out.write(reinterpret_cast<char*>(&file), sizeof(file));
}


void controller_convert::process_file(std::ifstream& in, std::ofstream& out,
                  const std::vector<std::pair<std::string, std::vector<int>>>& conditions,
                  const std::vector<std::string>& input_files){
    std::array<int16_t, 44100> array{};
    size_t i = 0;

    factory_complete_converts<std::string, complete> factory_pre_action;
    factory_converters<std::string, Product> factory_action;

    while (in.read(reinterpret_cast<char *>(&array), sizeof(array))) {
        for (const auto &condition: conditions) {
            factory_pre_action.get(condition.first)->complete_any(array, condition.second, input_files, i, factory_action);
        }
        out.write(reinterpret_cast<char *>(&array), sizeof(array));
        i++;
    }
    out.write(reinterpret_cast<char *>(&array), sizeof(array));
}

void controller_convert::convert(const std::string &config_name,
                                 const std::string& out_filename, const std::vector<std::string>& input_files) {
    try {
        config_to_conditions_arr c;
        auto conditions = c.get_conditions(config_name, input_files);

        std::ifstream in(input_files[0], std::ios::binary);
        if (!in) {
            std::cerr << "Failed to open file " << input_files[0] << std::endl;
            throw;
        }
        std::ofstream out(out_filename, std::ios::binary);
        if (!out) {
            std::cerr << "Failed to open file " << out_filename << std::endl;
            throw;
        }
        prepare_header(in, out);

        process_file(in, out, conditions, input_files);

        in.close();
        out.close();
    }
    catch (const std::exception &ex) {
        throw;
    }
}