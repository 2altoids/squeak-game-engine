#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <Display.h>


#define IMG_WIDTH 640
#define IMG_HEIGHT 480


int main(void) {
    
    //create window
    sge::Display display = sge::Display(false, "foo", IMG_WIDTH, IMG_HEIGHT);
    display.createDisplay();

    //game loop
    while (!glfwWindowShouldClose(display.getWindow())) {
        //Process pending input
        //TODO: Keyboard input should be recorded each frame, and prevent multiple "presses" before release
        glfwPollEvents();
        if (glfwGetKey(display.getWindow(), GLFW_KEY_A))
        {
            display.toggleDisplayMode();
        }
        if (glfwGetKey(display.getWindow(), GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(display.getWindow(), GLFW_TRUE);
        }
        
        //Update simulation
        //TODO: write ECS code here
        
        //Render and display new frame
        display.updateDisplay();
        
    }
    
    display.closeDisplay();

    return 0;
}
