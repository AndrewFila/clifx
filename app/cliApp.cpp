#include "clifx.h"
#include <string.h>

int main(int argc, char** argv)
{
    clifx cl;
    uint8_t target[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int hue;
    int kelvin;
    float brightness;
    float saturation;
    bool transient;
    uint32_t period;
    float cycles;
    float skew;
    enum waveform wf;
    if (argc > 1)
    { //cli
        if (strcmp(argv[1], "-p") == 0){
            if (strcmp(argv[2], "on") == 0){
                cl.setLightPower(1, 0, target);
            }
            else if (strcmp(argv[2], "off") == 0){
                cl.setLightPower(0, 0, target);
            }
            else{
                std::cout << "Unrecognized command" << std::endl;
            }
        }
        else if(strcmp(argv[1],"-c") == 0){
            cl.setLightPower(1, 0, target);
            hue = atoi(argv[2]);
            saturation = atof(argv[3]);
            brightness = atof(argv[4]);
            cl.setColor(hue, saturation, brightness, 0, 0, target);
        }
        else if (strcmp(argv[1], "-w") == 0){
            cl.setLightPower(1, 0, target);
            kelvin = atoi(argv[2]);
            brightness = atof(argv[3]);
            cl.setColor(0, 0, brightness, kelvin, 0, target);
        }
        else if(strcmp(argv[1], "-wf") == 0){
            cl.setLightPower(1, 0, target);
            transient = atoi(argv[2]);
            hue = atoi(argv[3]);
            saturation = atof(argv[4]);
            brightness = atof(argv[5]);
            period = atoi(argv[6]);
            cycles = atof(argv[7]);
            skew = atof(argv[8]);
            switch (atoi(argv[9])){
                case 0:
                    wf = SAW;
                    break;
                case 1:
                    wf = SINE;
                    break;
                case 2:
                    wf = HALF_SINE;
                    break;
                case 3:
                    wf = TRIANGLE;
                    break;
                case 4:
                    wf = PULSE;
                    break;
            }
            cl.setWaveform(transient, hue, saturation, brightness, 0, period, cycles, skew, (uint8_t) wf, target);
        }
        else if(strcmp(argv[1], "-h") == 0){
            printf("usage: clifx [tag] [options]\n\t-p: set power\n\t\tclifx -p <on, off>\n\t-c: set color\n\t\tclifx -c <hue 0 - 360> <saturation 0.0 - 1.0> <brightness 0.0 - 1.0>\n\t-w: set white\n\t\tclifx -w <kelvin 1500 - 9000> <brightness 0.0 - 1.0>\n\t-h: show help\n");
        }
        else{
            std::cout << "Unrecognized command" << std::endl;
        }
    }
    else
    { //gui
        cl.setColor(0, 0, 1, 5000, 100, target);
    }
    cl.~clifx();
    exit(EXIT_SUCCESS);
}
