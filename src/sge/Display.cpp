#include <iostream>

#include "Display.h"


//GLFW error callback
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


namespace sge{

Display::Display(bool isFullscreen, std::string windowTitle, int width, int height)
{
    mIsFullscreen = isFullscreen;
    mTitle = windowTitle;
    mWidth = width;
    mHeight = height;
    
    mIsDisplayCreated = false;
    mDelta = 0;
}


void Display::createDisplay()
{
    //Abort if the display is already created
    if (mIsDisplayCreated)
    {
        std::cerr << "Error: This display cannot be created more than once." << std::endl;
        return;
    }
    
    //Set flag so display is not created twice
    mIsDisplayCreated = true;
    
    //Write all GLFW errors to the standard error stream
    glfwSetErrorCallback(error_callback);

    //Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Error: GLFW could not be initialized." << std::endl;
    }
    
    //Specify OpenGL version and GLFW window options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    
    //Get primary monitor and video mode
    mPrimaryMonitor = glfwGetPrimaryMonitor();
    mVideoMode = glfwGetVideoMode(mPrimaryMonitor);

    //create display
    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), NULL, NULL);
    //Abort if the display does not exist
    if (!mWindow) {
        std::cerr << "Error: Failed to create window with GLFW." << std::endl;
        glfwTerminate();
        return;
    }
    //Center window on screen
    centerWindow();
    //If fullscreen, switch videomode and set monitor
    if (mIsFullscreen)
    {
        mVideoMode = glfwGetVideoMode(mPrimaryMonitor);
        glfwSetWindowMonitor(mWindow, mPrimaryMonitor, 0, 0, mVideoMode->width, mVideoMode->height, mVideoMode->refreshRate);
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(mWindow);
    
    // Enable v-sync to prevent screen tearing
    glfwSwapInterval(1);
    
    //Initialize Glad. OpenGL functions cannot be used until AFTER this.
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize glad."  << std::endl;
        return;
    }
    
    //Render over the whole display (windowed) or largest area that preserves the aspect ratio (fullscreen)
    adjustViewport();
    
    //Initialize time since last frame rendered to current time
    mLastFrameTime = glfwGetTime();
}


void Display::toggleDisplayMode()
{
    if (mIsFullscreen)
    {
        //Switch to windowed mode from fullscreen
        glfwSetWindowMonitor(mWindow, NULL, 0, 0, mWidth, mHeight, 0);
        centerWindow();
        mIsFullscreen = false;
    }
    else
    {
        //Switch to fullscreen from windowed mode
        mPrimaryMonitor = glfwGetPrimaryMonitor();
        if (mPrimaryMonitor != NULL)
        {
            mVideoMode = glfwGetVideoMode(mPrimaryMonitor);
            glfwSetWindowMonitor(mWindow, mPrimaryMonitor, 0, 0, mVideoMode->width, mVideoMode->height, mVideoMode->refreshRate);
        }
        mIsFullscreen = true;        
    }
    
    //Resize GL viewport to reflect new display dimensions
    adjustViewport();
}

void Display::updateDisplay()
{
    //TODO: move this code into a dedicated renderer
    /* this code should be handled in the future by a dedicated renderer */
        glfwSwapBuffers(mWindow);
        glClearColor(0, 0, 0.4f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* end of rendering code */    
    
    //TODO: Keyboard input should be handled in a better way.
    /*start of input code*/
        glfwPollEvents();
        if (glfwGetKey(mWindow, GLFW_KEY_A))
        {
            toggleDisplayMode();
        }
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
        }
    /* end of input code */
    
    //Update time variables
    double currentFrameTime = glfwGetTime();
    mDelta = currentFrameTime - mLastFrameTime; 
    mLastFrameTime = currentFrameTime;
}

void Display::closeDisplay()
{
    //GLFW display cleanup
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    //Set display created flag to false
    mIsDisplayCreated = false;
}

void Display::centerWindow()
{
    glfwSetWindowPos(mWindow, (mVideoMode->width - mWidth) / 2, (mVideoMode->height - mHeight) / 2);
}

void Display::adjustViewport()
{
    
    if (mIsFullscreen)
    {
        //Resize OpenGL viewport as large as possible without cropping
        //This may result in a letterboxing effect.
        
        int displayWidth = mVideoMode->width;
        int displayHeight = mVideoMode->height;
        float displayRatio = (float)displayWidth / (float)displayHeight;
        float contentRatio = (float)mWidth / (float)mHeight;
        
        if (displayRatio <= contentRatio)
        {
            //Display is taller than content. Bars on top and bottom,
            //colored with GL clear color (letterboxing);
            int proportionalHeight = (int) (displayWidth / contentRatio);
            glViewport(0, (displayHeight - proportionalHeight) / 2, displayWidth, proportionalHeight);
        } else
        {
            //Display is wider than content. Bars on left and right,
            //colored with GL clear color (pillarboxing);
            int proportionalWidth = (int) (displayHeight * contentRatio);
            glViewport((displayWidth - proportionalWidth) / 2, 0, proportionalWidth, displayHeight);
        }
    }
    else
    {
        //Resize OpenGL viewport to size of window
        glViewport(0, 0, mWidth, mHeight);    
    }
}

double Display::getFrameTimeSeconds()
{
    return mDelta;
}

int Display::getWidth()
{
    return mWidth;
}

int Display::getHeight()
{
    return mHeight;
}

std::string Display::getTitle()
{
    return mTitle;
}

GLFWwindow* Display::getWindow()
{
    return mWindow;
}

Display::~Display()
{
    //Close any open display
    if (mIsDisplayCreated)
    {
        closeDisplay();
    }
}

}