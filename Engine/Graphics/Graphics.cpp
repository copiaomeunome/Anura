#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /* Instances glew */
    if(glewInit()!=GLEW_OK){
        std::cout << "Error on init glew" << std::endl;
        return 1;
    }
    std::cout << "glew version: " << glGetString(GL_VERSION) << std::endl;

    /* generate buffer */
    unsigned int buffer;
    // (how many buffers do you want, ponter to buffer - it gives us the ID)
    glGenBuffers(1,&buffer);

    /* selecting the buffer */
    // (what is used for, ID of buffer)
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /* gives buffer a data and size, data can be passed later */
    double positions[6] = {
        -5.0, -5.0,
         0.0, 5.0,
         5.0, -5.0
    };
    // (what is buffer used for, size of data, data, what is used for)
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(double), positions, GL_STATIC_DRAW);

    /* setting vertex to identify what its data is */
    glEnableVertexAttribArray(0);
    // (index, size, normalize, size of vertex, offset to next attribute)
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, sizeof(double)*2, (const void*)0);
    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}