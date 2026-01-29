#include <iostream>
#include "Sentinel.h"

struct TestObject{
    int x;
    double y;
};

int main(){
    std::cout << "Sentinel Memory Tracker Demo\n";

    // Correct Allocation
    int* a = static_cast<int*>(SENTINEL_NEW(sizeof(int)));
    *a = 42;
    SENTINEL_DELETE(a);

    // Proposital Leak
    TestObject* obj = static_cast<TestObject*>(
        SENTINEL_NEW(sizeof(TestObject))
    );
    obj->x = 90;
    obj->y = 3.14;
    
    // Not freeing obj

    Sentinel::reportLeaks();
    return 0;
}
