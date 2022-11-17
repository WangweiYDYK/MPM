#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "solver.h"
#include <iostream>

/* Declarations */
Solver *Simulation;
int t_count = 0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


void Initialization(){
    std::vector<Border> inBorders = Border::InitializeBorders();
    std::vector<Node> inNodes = Node::InitializeNodes();
    std::vector<Material> inParticles = Material::InitializeParticles();

    Simulation = new Solver(inBorders, inNodes, inParticles);
}

void Update(){
    Simulation->P2G();
    Simulation->UpdateNodes();
    Simulation->G2P();
    Simulation->UpdateParticles();
}

void AddParticles()										// Add particle during the simulation
{							
	// DT_ROB gives the rate of insertion. Maximum number of particles
	if (t_count % DT_ROB == 0 && Simulation->particles.size() < 2000)
	{
		std::vector<Material> new_p = Material::AddParticles();
		for (size_t p = 0, plen = new_p.size(); p < plen; p++)
			Simulation->particles.push_back(new_p[p]);
        Simulation->plen = Simulation->particles.size();
	}
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void program(const char *vertexShaderSource, const char* fragmentShaderSource){
    
}


int main()
{
    Initialization();
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
  while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        AddParticles();
        Update();
        
        if (t_count % (int)(DT_render / DT) == 0){            
            glm::vec2 *gl_position;
            gl_position = new glm::vec2[Simulation->plen];
            for(int i = 0; i < Simulation->plen; ++i){
                gl_position[i] = glm::vec2((2 * Simulation->particles[i].Xp[0] - X_GRID)/ X_GRID, (2 * Simulation->particles[i].Xp[1] - Y_GRID)/ Y_GRID);
            }
            glUseProgram(shaderProgram);

            // std::vector<Vector2f> gl_position;
            // for(int i = 0; i < Simulation->plen; ++i){
            //     gl_position.push_back(Vector2f(Simulation->particles[i].Xp[0] / X_GRID, Simulation->particles[i].Xp[1] / Y_GRID));
            // }


            glBufferData(GL_ARRAY_BUFFER, Simulation->plen * sizeof(glm::vec2), gl_position, GL_DYNAMIC_DRAW);      
            
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
            

            glPointSize(3);
            glDrawArrays(GL_POINTS, 0, Simulation->plen);  

            glfwSwapBuffers(window);
            glfwPollEvents();            

        }  

        Simulation->ResetGrid();
        t_count++;
        
    }           
    glDeleteProgram(shaderProgram); 
    glfwTerminate();
    return 0;
}





