#include "poststatus.h"

bool Poststatus::hatPost(int fach){
    if(fach < status_.length()){
        return status_.at(fach) == '0'; // total verqueere semantik
    }
    return false;
}
