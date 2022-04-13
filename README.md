# clifx

**description**

clifx is an implementation of the [lifx LAN protocol](https://lan.developer.lifx.com/docs) written in C++ for unix systems. Clifx has been only tested on 
arch linux, support for other operating systems is a plan for the future.

**Functionality**

- Setting the power state of lifx lights on the local network
- Setting color, brightness, saturation, and kelvin level of lights as well as the duration of change.
- A command line application capible of using the above functionality

**Future Plans**

- Implementation of other supported functionality such as setting waveforms, infared, and HEV cycles in supported lights.
- The ability to get information from the lights such as power level, color, brightness, and saturation.
- A GUI application for use on linux systems.
- Cross platform implemenation of this entire library as well as GUI and CLI applications.

**available functions**

`void setLightPower(bool onOff, uint32_t duration, uint8_t target[8])`
- onOff -- a boolian value that determines the power state of the device. <1 -- on, 0 -- off>
- duration -- the time in miliseconds to turn on. 
- target -- device to be targeted (see example for more information)

`void setCOlor(uint16_t hue, float saturation, float brightness, uint16_t kelvin, uint32_t duration, uint8_t target[8])`
- hue -- the color to set the light in HSBK <int 0 - 360>
- saturation -- the saturation of the color <float 0.0 - 1.0>
- brightness -- the brightness of the divice <float 0.0 - 1.0>
- kelvin -- the kevlin level of the light (if saturation is greater than 0 the value of this does not matter) <int 1500 - 9000>
- duration -- the time in miliseconds to change colors.
- target -- device to be targeted (see example for more information)

**Examples**

```
//example.cpp
//program to turn on all lights and set the color to green at 100% saturation and 50% brightness.


int main(){
  uint8_t target[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // The target specifies which lights to change based on its mac address
                                                                       // ex. AB:CD:EF:12:34:56:78:90 -> {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90}
                                                                        // A target with all zeros will change all ligths
  clifx lightController;  // decalre a clifx object
  lightController.setLightPower(1, 0, target);  // turn the power on with no delay
  lightController.setColor(120, 1, .5, 0, 0, target); //sets the color to bright green (120deg) with a sautration of 100%, a brightness of 50%, 
                                                      //kelvin value of 0 (since we are using a saturation of >0% kelvin can be set to anything),
                                                      //and a duration of 0.
  lightController.~clifx(); //deconstructor, closes our udp socket created by the constructor.
  exit(SUCCESS);
}
```
