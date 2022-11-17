#include"border.h"

Border::Border(const int inType, const Vector2f& inNormal, const std::vector<Vector2f>& inX_corner){
    normal = inNormal;
    type = inType;
    X_corner = inX_corner;
}

void Border::Collision(const Vector2f& node_coordinates, Vector2f& node_velocity, std::vector<int>& collision, const int b){
    double distance = normal.dot(node_coordinates - X_corner[0]);
    if(type == 1 && distance < 0){
        node_velocity = Vector2f(0);
    }
    else{
        Vector2f next_position = node_coordinates + DT * node_velocity;
        double next_distance = normal.dot(next_position - X_corner[0]);
        double dist_c = next_distance - std::min(distance, double(0.0));

        if(((type == 2) && dist_c < 0)|| ((type == 3) && (distance < 0))){
            node_velocity -= dist_c * normal / DT;
            collision.push_back(b);
        }
    }
}

void Border::Friction(Vector2f& Vi_fri, const Vector2f& Vi_col, const Vector2f& Vi){
    Vector2f Vt = Vi_col - normal * (normal.dot(Vi_fri));
    if(Vt.norm() > 1e-7){
        Vector2f t = Vt / Vt.norm();
        Vi_fri -= std::min(Vt.norm(), CFRI* (Vi_col - Vi).norm())*t;
    }

}



// void Border::DrawBorder()
// {


//     unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//     glCompileShader(vertexShader);
//     // check for shader compile errors
//     int success;
//     char infoLog[512];
//     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//     if (!success)
//     {
//         glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//         std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//     }
//     // fragment shader
//     unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//     glCompileShader(fragmentShader);
//     // check for shader compile errors
//     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//     if (!success)
//     {
//         glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//         std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//     }
//     // link shaders
//     unsigned int shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);
//     // check for linking errors
//     glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//     if (!success) {
//         glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//         std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//     }
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     // set up vertex data (and buffer(s)) and configure vertex attributes
//     // ------------------------------------------------------------------
//     float vertices[] = {
//          (float)(2 * (X_GRID - CUB) - X_GRID)/ X_GRID, (float)(2 * CUB - Y_GRID)/ Y_GRID,
//          (float)(2 * (X_GRID - CUB) - X_GRID)/ X_GRID, (float)(2* (Y_GRID - CUB) - Y_GRID) / Y_GRID,
//          (float)(2 * CUB - X_GRID) / X_GRID, (float)(2* (Y_GRID - CUB) - Y_GRID) / Y_GRID,
//          (float)(2 * CUB - X_GRID) / X_GRID, (float)(2 * CUB - Y_GRID)/ Y_GRID
//     };
//     unsigned int VBO, VAO, EBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0); 

//     glBindVertexArray(0); 


// }

