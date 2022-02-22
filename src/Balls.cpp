#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

float angleBwPoints(float x_one, float y_one, float x_two, float y_two)
{
    float length1 = sqrt(x_one * x_one + y_one * y_one);// calculate modulus of Vector V1 i.e. |V1| 
    float length2 = sqrt(x_two * x_two + y_two * y_two);
    // calculate modulus of Vector V2 i.e. |V2| 
    float dot = x_one * x_two + y_one * y_two; // calculate dot product between two vectors. 

    float a = dot / (length1 * length2);

    if (a >= 1.0)
        return 0.0;
    else if (a <= -1.0)
        return 10;// PI means  
    else
        return acos(a) * (180.0/3.1415); // 0..PI 
}

void drawLine(float x, float y, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x2, y2);
    glEnd();
}

//
void addVector(float xAdd, float yAdd, float&xtarget, float&ytarget) {
    xtarget = xtarget + xAdd;
    ytarget = ytarget + yAdd;
}

// Function that return 
// dot product of two vector array. 
int dotProduct(int vect_A[], int vect_B[])
{

    int product = 0;

    // Loop for calculate cot product 
    for (int i = 0; i < 2; i++)

        product = product + vect_A[i] * vect_B[i];
    return product;
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * 3.1415;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount;i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void drawCircle(GLfloat x, GLfloat y, GLfloat r) {
    static const double inc = 3.1415 / 12;
    static const double max = 2 * 3.1415;
    glBegin(GL_LINE_LOOP);
    for (double d = 0; d < max; d += inc) {
        glVertex2f(cos(d) * r + x, sin(d) * r + y);
    }
    glEnd();
}

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
}

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(900, 900, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    //gets openGL functions from GPU
    //We use the glew library to do this so the program can be 
    //independent of our machine and OS
    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
           -0.5f, -0.5f, //0
            0.5f, -0.5f, //1
            0.5f,  0.5f, //2
           -0.5f,  0.5f, //3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    //glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW); //size of list of vertices

    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); //size of list of triangles


    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");
    //_ASSERT(location != -1);
    glUniform4f(location, 0.3f, 0.3f, 0.8f, 1.0f);

    float r = 0.0f;
    float increment = 0.05f;

    float x_positions[] = {
        0.05, 0.8, 0.2
    };

    float y_positions[] = {
        -0.0, 0.6, 0.8
    };

    float x_velocity[] = {
        //change velocities here
        0.0005, -0.007, 0.0012
    };

    float y_velocity[] = {
        //and here
        0.006, 0.006, 0.00
    };

    //do not change, should be 0
    float record_x_velocity[] = {
        0.000, 0.000
    };

    //do not change, should be 0
    float record_y_velocity[] = {
        0.000, 0.00
    };

    int hasCollidedRecently = 0;
    bool needToPause = false;
    float last_x_position[] = {0.08, 0.08};

    int colisions_so_far = 0;
    bool unfinished_collision = false;
    bool new_collision = false;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*
        GLClearError();
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW); //size of list of vertices (number of vertices * 2 * sizeof(float))

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); //size of list of triangles (3 * number of triangles * sizeof(unsigned int))
        GLCheckError();

        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //number of vertices to be drawn (3 * number of triangles)
        */

        //managing multi-collisions
        if (unfinished_collision == false && (sqrt(pow((x_positions[0] - x_positions[1]), 2) + pow((y_positions[0] - y_positions[1]), 2)) < 2 * 0.1)) {
            colisions_so_far++;
            std::cout << "Collisions so far: " << colisions_so_far << std::endl;
            unfinished_collision = true;
            new_collision = true;
        }
        else if (unfinished_collision == true){
            if ((sqrt(pow((x_positions[0] - x_positions[1]), 2) + pow((y_positions[0] - y_positions[1]), 2)) < 2 * 0.1)) {
                std::cout << "Waiting" << std::endl;
            }
            else {
                unfinished_collision = false;
            }
        }

        //update positions of the two particles
        if (new_collision == true && (sqrt(pow((x_positions[0] - x_positions[1]), 2) + pow((y_positions[0] - y_positions[1]), 2)) < 2 * 0.1)) {
            std::cout << "Total Momentum: " << sqrt((x_velocity[0] * x_velocity[0]) + (y_velocity[0] * y_velocity[0])) + sqrt((x_velocity[1] * x_velocity[1]) + (y_velocity[1] * y_velocity[1])) << std::endl;
            std::cout << "ANGLE_TEST: " << angleBwPoints(0, -3, 0, 3) << std::endl;

            //manage pause system
            //needToPause = true;

            //manage multi-collisions
            new_collision = false;

            //remember the pre-collision velocities
            record_x_velocity[0] = x_velocity[0];
            record_x_velocity[1] = x_velocity[1];
            record_y_velocity[0] = y_velocity[0];
            record_y_velocity[1] = y_velocity[1];

            //update the x_velocity and y_velocity
            //main = 0; target = 1

            //calculate impact vector
            float x_impact = x_positions[0] - x_positions[1];
            float y_impact = y_positions[0] - y_positions[1];
            //project target_velocity(1) onto impact vector
            float x_projected_impact = (((record_x_velocity[1] * x_impact) + (record_y_velocity[1] * y_impact)) / ((pow(x_impact, 2)) + (pow(y_impact, 2)))) * x_impact;
            float y_projected_impact = (((record_x_velocity[1] * x_impact) + (record_y_velocity[1] * y_impact)) / ((pow(x_impact, 2)) + (pow(y_impact, 2)))) * y_impact;
            std::cout << "<" << record_x_velocity[1] << ", " << record_y_velocity[1] << "> projected onto <" << x_impact << ", " << y_impact << "> is: <" << x_projected_impact << ", " << y_projected_impact << ">" << std::endl;
            //calculate angle between projected_impact and impact
            float angle1 = angleBwPoints(x_projected_impact, y_projected_impact, x_impact, y_impact);
            std::cout << "Angle: " << angle1 << std::endl;
            if (abs(angle1 - 180) < 1) {
                x_projected_impact = 0;
                y_projected_impact = 0;
            }
            //add projected_impact onto main_velocity
            x_velocity[0] = x_velocity[0] + x_projected_impact;
            y_velocity[0] = y_velocity[0] + y_projected_impact;
            //subtract projected_impact from target_velocity
            x_velocity[1] = x_velocity[1] - x_projected_impact;
            y_velocity[1] = y_velocity[1] - y_projected_impact;

            //main = 1; target = 0

            //calculate impact vector
             x_impact = x_positions[1] - x_positions[0];
             y_impact = y_positions[1] - y_positions[0];
            //project target_velocity(1) onto impact vector
             x_projected_impact = (((record_x_velocity[0] * x_impact) + (record_y_velocity[0] * y_impact)) / ((pow(x_impact, 2)) + (pow(y_impact, 2)))) * x_impact;
             y_projected_impact = (((record_x_velocity[0] * x_impact) + (record_y_velocity[0] * y_impact)) / ((pow(x_impact, 2)) + (pow(y_impact, 2)))) * y_impact;
            std::cout << "<" << record_x_velocity[0] << ", " << record_y_velocity[0] << "> projected onto <" << x_impact << ", " << y_impact << "> is: <" << x_projected_impact << ", " << y_projected_impact << ">" << std::endl;
            //calculate angle between projected_impact and impact
             angle1 = angleBwPoints(x_projected_impact, y_projected_impact, x_impact, y_impact);
            std::cout << "Angle: " << angle1 << std::endl;
            if (abs(angle1 - 180) < 1) {
                x_projected_impact = 0;
                y_projected_impact = 0;
            }
            //add projected_impact onto main_velocity
            x_velocity[1] = x_velocity[1] + x_projected_impact;
            y_velocity[1] = y_velocity[1] + y_projected_impact;
            //subtract projected_impact from target_velocity
            x_velocity[0] = x_velocity[0] - x_projected_impact;
            y_velocity[0] = y_velocity[0] - y_projected_impact;

            std::cout << "Total Momentum: " << sqrt((x_velocity[0] * x_velocity[0])+(y_velocity[0] * y_velocity[0])) + sqrt((x_velocity[1] * x_velocity[1])+(y_velocity[1] * y_velocity[1])) << std::endl;
            std::cout << std::endl;
        }
        else {
            //first ball
            //x
            if (x_positions[0] + x_velocity[0] > (1 - 0.1) || x_positions[0] + x_velocity[0] < (-1 + 0.1)) {
                x_velocity[0] = x_velocity[0] * -1;
            }
            else {
                x_positions[0] = x_positions[0] + x_velocity[0];
            }

            //y
            if (y_positions[0] + y_velocity[0] > (1 - 0.1) || y_positions[0] + y_velocity[0] < (-1 + 0.1)) {
                y_velocity[0] = y_velocity[0] * -1 * 0.9;
            }
            else {
                y_positions[0] = y_positions[0] + y_velocity[0];
            }
            y_velocity[0] = y_velocity[0] - 0.0005;

            //second ball
            //x
            if (x_positions[1] + x_velocity[1] > (1 - 0.1) || x_positions[1] + x_velocity[1] < (-1 + 0.1)) {
                x_velocity[1] = x_velocity[1] * -1;
            }
            else {
                x_positions[1] = x_positions[1] + x_velocity[1];
            }

            //y
            if (y_positions[1] + y_velocity[1] > (1 - 0.1) || y_positions[1] + y_velocity[1] < (-1 + 0.1)) {
                y_velocity[1] = y_velocity[1] * -1 * 0.9;
            }
            else {
                y_positions[1] = y_positions[1] + y_velocity[1];
            }
            y_velocity[1] = y_velocity[1] - 0.0005;
        }
        
        //draw the two particles
        for (int i = 0; i < 2; i++) {
            drawCircle(x_positions[i], y_positions[i], 0.1);
            if (i == 1) {
                drawCircle(x_positions[i], y_positions[i], 0.05);
            }
        }

        /*
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        if (needToPause == true) {
            std::string input = "a";
            do {
                std::cout << "Enter 'c' to continue" << std::endl;
                std::cin >> input;
            } while (input != "c");
            needToPause = false;
        }

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}