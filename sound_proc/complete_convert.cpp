#include "complete_convert.h"
void complete_mute::complete_any(std::array<int16_t, 44100>& array, const std::vector<int>& data,
                                 const std::vector<std::string>& input_files, size_t i,
                                 factory_converters<std::string, Product>& factoryConverters) {
    if (data[0] <= i && data[1] >= i) {
        MuteArguments mute_arguments(array);
        factoryConverters.get("mute", mute_arguments)->action();
    }
    else return;
}

void complete_mix::move_place(std::ifstream& in){
    in.seekg(40, std::ios::beg);
    unsigned long extra_info;
    in.read(reinterpret_cast<char *>(&extra_info), sizeof(extra_info));
    in.seekg(extra_info + 8, std::ios::cur);
}
void complete_mix::complete_any(std::array<int16_t, 44100>& array, const std::vector<int>& data,
                                const std::vector<std::string>& input_files, size_t i,
                                factory_converters<std::string, Product>& factoryConverters) {
    if (i >= data[1]) {
        std::array<int16_t, 44100> array1{};
        std::ifstream in2(input_files[data[0] - 1], std::ios::binary);
        if (!in2) {
            std::cerr << "Failed to open file " << input_files[data[0] - 1] << std::endl;
            throw;
        }

        move_place(in2);
        in2.seekg(array.size() * (i - data[1]), std::ios::cur);
        if (in2.read(reinterpret_cast<char*>(&array1), sizeof(array1))) {
            MixArguments mix_arguments(array, array1);
            factoryConverters.get("mix", mix_arguments)->action();
        }
        in2.close();
    }
    else return;
}
void complete_double::complete_any(std::array<int16_t, 44100>& array, const std::vector<int>& data,
                                   const std::vector<std::string>& input_files, size_t i,
                                   factory_converters<std::string, Product>& factoryConverters) {
    if (i >= data[0] && data[1] >= i){
        DoubleArguments double_arguments(array);
        factoryConverters.get("double", double_arguments)->action();
    }
    else return;
}
