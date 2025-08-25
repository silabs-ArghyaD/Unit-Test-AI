#include "ClosureManager.h"

void ClosureManager::open() {
    open_state = true;
    std::cout << "Opening closure" << std::endl;
}

void ClosureManager::close() {
    open_state = false;
    std::cout << "Closing closure" << std::endl;
}

bool ClosureManager::isOpen() const { 
    return open_state; 
}