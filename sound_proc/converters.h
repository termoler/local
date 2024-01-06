#ifndef SOUND_PROCESSOR_CONVERTERS_H
#define SOUND_PROCESSOR_CONVERTERS_H
#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <memory>
#include "IArguments_factory_converts.h"

class Product{
public:
    virtual void action() = 0;
    virtual ~Product() = default;
};

class Mix : public Product {
public:
    std::array<int16_t, 44100>& array_1;
    std::array<int16_t, 44100>& array_2;

    explicit Mix(std::array<int16_t, 44100>& a1, std::array<int16_t, 44100>& a2): array_1(a1), array_2(a2) {}

    void action() override;
};

class Mute : public Product {
public:
    std::array<int16_t, 44100>& array;
    explicit Mute(std::array<int16_t, 44100>& a1): array(a1) {}

    void action() override;
};

class Double : public Product {
public:
    std::array<int16_t, 44100>& array;

    explicit Double(std::array<int16_t, 44100>& a1): array(a1) {}

    void action() override;
};

template<class ID, class base>
class factory_converters {
private:
    base* create_object(const ID& id, IArguments& args) {
        if(id == "mute"){
            auto& muteArgs = dynamic_cast<MuteArguments&>(args);
            return new Mute(muteArgs.array);
        } else if(id == "mix"){
            auto& mixArgs = dynamic_cast<MixArguments&>(args);
            return new Mix(mixArgs.array_1, mixArgs.array_2);
        } else if(id == "double"){
            auto& doubleArgs = dynamic_cast<DoubleArguments&>(args);
            return new Double(doubleArgs.array);
        } else return nullptr;
    }
    std::map<ID, std::shared_ptr<base>> classes;
public:
    base* get(const ID& id, IArguments& args){
        auto it = classes.find(id);
        if(it != classes.end()){
            return it->second.get();
        } else{
            base* new_object = create_object(id, args);
            if(new_object) {
                classes[id] = std::shared_ptr<base>(new_object);
                return new_object;
            }
            return nullptr;
        }
    }
};


#endif //SOUND_PROCESSOR_CONVERTERS_H
