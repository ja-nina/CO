#include "street.h"

Street:: Street( int id, int start, int end, int length){
    this->id = id;
    this->start = start;
    this->end = end;
    this->length = length;
}

int Street::carEnter(){
        this->entered += 1;
        return this->entered - 1;
}

int Street::carLeave(){
        this->passed += 1;
        return this->passed;
}

bool Street::is_empty(){
    return this->entered == this->passed;
}

void Street::reset(){
    this->entered = 0;
    this->passed = 0;
}
