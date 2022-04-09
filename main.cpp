#include "clifx.h"

int main(){
    uint8_t target[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    clifx cl;
    cl.setColor(120, 1, 1, 0, 0, target);
    cl.~clifx();
    return 1;
}
