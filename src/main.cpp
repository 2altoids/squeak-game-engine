#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glad/glad.h>  //Including Glad before GLFW
#include <glfw/glfw3.h>


#define IMG_WIDTH 640
#define IMG_HEIGHT 480


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* initWindow() {
    GLFWwindow* window;
    if (!glfwInit()) {
        return NULL;
    }

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    window = glfwCreateWindow(IMG_WIDTH, IMG_HEIGHT, "Lab 2", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return NULL;
    }

    return window;
}

std::string shaderTypeName(GLenum shaderType) {
    switch(shaderType) {
        case GL_VERTEX_SHADER: return "VERTEX";
        case GL_FRAGMENT_SHADER: return "FRAGMENT";
        default: return "UNKNOWN";
    }
}

std::string readFile(const std::string& fileName) {
    std::ifstream stream(fileName);
    std::stringstream buffer;
    buffer << stream.rdbuf();

    std::string source = buffer.str();
    std::cout << "Source:" << std::endl;
    std::cout << source << std::endl;

    return source;
}


class Point {
public:
    float x, y, r, g, b;

    // Constructor
    Point(float xx, float yy, float rr, float gg, float bb) : x(xx), y(yy), r(rr), g(gg), b(bb){
        // nothing to do here as we've already initialized x, y, and z above
    }
    
    Point() {
        x = 0;
        y = 0;
        r = 0;
        g = 0;
        b = 0;
    }

    // Destructor - called when an object goes out of scope or is destroyed
    ~Point() {
        //Do nothing
    }
    
    
};


Point w2nd(Point input)
{
    float x = -1 + (input.x * (2.0f / float(IMG_WIDTH)));
    float y =  1 - (input.y * (2.0f / float(IMG_HEIGHT)));
    return Point(x, y, input.r, input.g, input.b);
}


GLuint createShader(const std::string& fileName, GLenum shaderType) {
    std::string source = readFile(fileName);
    const char* src_ptr = source.c_str();

    GLuint shader = 0;
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src_ptr, NULL);
    glCompileShader(shader);

    //error handling
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << shaderTypeName(shaderType)
            <<"::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = 0;
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    //error handling
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    return program;
}

int main(void) {
    Point points[3] = {
                        Point(300,400,1,0,0),
                        Point(450,20,0,1,0),
                        Point(600,400,0,0,1)
                    };
    Point* convertedPoints = new Point[3];
    for (int i=0; i<3; i++)
    {
        convertedPoints[i] = w2nd(points[i]);
    }

    // convert the triangle to an array of floats containing
    // normalized device coordinates and color components
    float triangle[15] = {};
    for (int i=0; i<3; i++)
    {
        triangle[5*i  ] = convertedPoints[i].x;
        triangle[5*i+1] = convertedPoints[i].y;
        triangle[5*i+2] = convertedPoints[i].r;
        triangle[5*i+3] = convertedPoints[i].g;
        triangle[5*i+4] = convertedPoints[i].b;
    }

    //create window
    GLFWwindow* window = initWindow();
    if (!window) {
        std::cout << "There was an error setting up the window" << std::endl;
        return 1;
    }
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    //make and populate VBO, VAO
    GLuint VBO[1], VAO[1];
    glGenBuffers(1, VBO);
    glGenVertexArrays(1, VAO);
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //create shaders, shader program
    GLuint vertexShader = createShader("res/shaders/vert.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader("res/shaders/frag.glsl", GL_FRAGMENT_SHADER);
    GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    //clean up the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //game loop
    while (!glfwWindowShouldClose(window)) {
        //Terminate on ESC key
        processInput(window);

        //clear the screen
        glClearColor(0.3f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //swap buffers
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
