#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


/* The Display class represents the window or full-screen display
 * provided by GLFW.  The display can be created, destroyed, updated,
 * and toggled between fullscreen and windowed modes. */

namespace sge{

class Display
{
    public:
        Display(bool isFullscreen, std::string windowTitle, int width, int height);
        void createDisplay();
        void toggleDisplayMode();
        void updateDisplay();
        void closeDisplay();
        void centerWindow();
        void adjustViewport();
        double getFrameTimeSeconds();
        int getWidth();
        int getHeight();
        std::string getTitle();
        GLFWwindow* getWindow();
        ~Display();
    private:
        int mWidth;
        int mHeight;
        std::string mTitle;
        double mLastFrameTime;
        double mDelta;
        GLFWwindow* mWindow;
        bool mIsFullscreen;
        GLFWmonitor* mPrimaryMonitor;
        const GLFWvidmode* mVideoMode;
        bool mIsDisplayCreated;
        
};

}


#endif // DISPLAY_H