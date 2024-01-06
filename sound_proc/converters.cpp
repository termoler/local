#include "converters.h"

void Mix::action(){
    for (size_t k = 0; k < array_1.size(); k++) {
        array_1[k] = array_1[k]/2 + array_2[k]/2;
    }
}

void Mute::action(){
    array.fill(0x00);
}

void Double::action(){
    for (size_t k = 0; k < array.size(); k++) {
        if(array[k] * 2 > 32767) continue;
        else array[k] *= 2;
    }
}