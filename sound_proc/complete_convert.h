#ifndef SOUND_PROCESSOR_COMPLETE_CONVERT_H
#define SOUND_PROCESSOR_COMPLETE_CONVERT_H
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include "converters.h"
#include <map>
#include <memory>

class complete{
public:
    virtual void complete_any(std::array<int16_t, 44100>& array, const std::vector<int>& data,
                              const std::vector<std::string>& input_files, size_t i,
                              factory_converters<std::string, Product>& factoryConverters) = 0;
    virtual ~complete() = default;
};

class complete_mute: public complete{
public:
    void complete_any(std::array<int16_t, 44100>& array, const std::vector<int>& data,
                      const std::vector<std::string>& input_files, size_t i,
                      factory_converters<std::string, Product>& factoryConverters) override;
};

class complete_mix: public complete{
    void move_place(std::ifstream& in);
public:
    void complete_any(std::array<int16_t, 44100>& array, const std::vector<int>& data,
                      const std::vector<std::string>& input_files, size_t i,
                      factory_converters<std::string, Product>& factoryConverters) override;
};
class complete_double: public complete{
public:
    void complete_any(std::array<int16_t, 44100>& array, const std::vector<int>& data,
                      const std::vector<std::string>& input_files, size_t i,
                      factory_converters<std::string, Product>& factoryConverters) override;
};


template<class ID, class base, class ... Args>
class factory_complete_converts {
private:
    base* create_object(const ID& id, Args... args) {
        if(id == "mute"){
            return new complete_mute(std::forward<Args>(args)...);
        }else if(id == "mix"){
            return new complete_mix(std::forward<Args>(args)...);
        }else if(id == "double"){
            return new complete_double(std::forward<Args>(args)...);
        }else return nullptr;
    }

    std::map<ID, std::shared_ptr<base>> classes;

public:
    base* get(const ID& id, Args... args){
        auto it = classes.find(id);
        if(it != classes.end()){
            return it->second.get();
        }else{
            base* new_object = create_object(id, std::forward<Args>(args)...);
            if(new_object) {
                classes[id] = std::shared_ptr<base>(new_object);
                return new_object;
            }
            return nullptr;
        }
    }
};
#endif //SOUND_PROCESSOR_COMPLETE_CONVERT_H
