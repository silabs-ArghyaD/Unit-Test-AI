#ifndef CLOSURE_MANAGER_H
#define CLOSURE_MANAGER_H

#include <iostream>

class ClosureManager {
public:
    void open();
    void close();
    bool isOpen() const;

private:
    bool open_state = false;
};

#endif // CLOSURE_MANAGER_H
