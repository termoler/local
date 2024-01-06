#include <iostream>
#include "check_argv.h"
#include "controller_convert.h"


int main(int argc, char **argv){
    try{
        call_help(argc, argv);
        check_argv(argc, argv);
        std::string _config = argv[2];
        std::string output_file = argv[3];
        std::vector<std::string> input_files;
        for(size_t i = 4; i < argc; i++){
            input_files.emplace_back(argv[i]);
        }
        controller_convert complete_convert;
        complete_convert.convert(_config, output_file, input_files);
        std::cout << "good";
    }
    catch(const std::exception &ex){
        std::cout << ex.what() << std::endl;
        throw;
    }
    return 0;
}
