#ifndef SOUND_PROCESSOR_IARGUMENTS_FACTORY_CONVERTS_H
#define SOUND_PROCESSOR_IARGUMENTS_FACTORY_CONVERTS_H
#include <iostream>
#include <array>

class IArguments {
public:
    virtual ~IArguments() = default;
};

class MixArguments : public IArguments{
public:
    std::array<int16_t, 44100>& array_1;
    std::array<int16_t, 44100>& array_2;

    explicit MixArguments(std::array<int16_t, 44100>& array_1, std::array<int16_t, 44100>& array_2)
            : array_1(array_1), array_2(array_2){}
};

class MuteArguments : public IArguments{
public:
    std::array<int16_t, 44100>& array;

    explicit MuteArguments(std::array<int16_t, 44100>& array): array(array) {}
};

class DoubleArguments : public IArguments{
public:
    std::array<int16_t, 44100>& array;

    explicit DoubleArguments(std::array<int16_t, 44100>& array): array(array) {}
};

#endif //SOUND_PROCESSOR_IARGUMENTS_FACTORY_CONVERTS_H
