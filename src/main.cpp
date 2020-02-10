#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "sge/Display.h"


#define IMG_WIDTH 640
#define IMG_HEIGHT 480


int main(void) {
    
    //create window
    sge::Display display = sge::Display(false, "foo", IMG_WIDTH, IMG_HEIGHT);
    display.createDisplay();

    //game loop
    while (!glfwWindowShouldClose(display.getWindow())) {
        
        display.updateDisplay();
        
    }
    
    display.closeDisplay();

    return 0;
}
