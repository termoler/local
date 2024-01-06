#include "check_argv.h"

void call_help(int argc, char **argv){
    if(argc == 2 && std::string(argv[1]) == "-h"){
        std::cout << "mute in wav file from X to Y" << "\n" <<
                  "mix $F X - mix in wav file with other wav file under the number F from X to the end" << "\n" <<
                  "double X Y - double the sound from X to Y" << "\n";
        throw std::runtime_error("this is help");
    }
}
void check_argv(int argc, char **argv) {
    if (argc < 4) {
        throw std::invalid_argument("insufficient number of arguments");
    }

    if (std::string(argv[1]) != "-c") {
        throw std::invalid_argument("invalid option: " + std::string(argv[1]));
    }

    std::string config_str = argv[2];
    size_t ind_point = config_str.find('.');
    if (ind_point == std::string::npos || config_str.substr(ind_point) != ".txt") {
        throw std::invalid_argument("invalid configuration file: " + config_str);
    }

    for (size_t i = 3; i < argc; ++i) {
        std::string wav_str = argv[i];
        size_t ind_point_w = wav_str.find('.');
        if (ind_point_w == std::string::npos || wav_str.substr(ind_point_w) != ".wav") {
            throw std::invalid_argument("invalid WAV file: " + wav_str);
        }
    }
}