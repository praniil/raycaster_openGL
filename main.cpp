#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

//window dimensions
const int WIDTH = 800, HEIGHT = 600;

//players variable
float pos_x = 22.0, pos_y = 12.0f;
float player_dir_x = -1.0f, player_dir_y = 0.0f; //initial direction vector
float plane_x = 0.0f, plane_y = 0.9f; //2D raycaster version of camera

//World map
const int worldMap[24][24] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderScene();

int main() {
    std::cout << "in main" << std::endl;
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    //creating a GLFW window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Casting", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //set the opengl context of the current window for rendering
    glfwMakeContextCurrent(window);

    //for resizing the window in runtime, width and height is associated with the runtime window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Initialize GLEW
    if(glewInit()!= GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    //configure openGL
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        renderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    float move_speed = 0.05f;
    float rotation_speed = 0.03f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        std::cout << "w" << std::endl;
        if(worldMap[int(pos_x + player_dir_x * move_speed)][int(pos_y)] == 0) {
            pos_x += player_dir_x * move_speed;
        }
        if(worldMap[int(pos_x)][int(pos_y + player_dir_y * move_speed)] == 0) {
            pos_y += player_dir_y * move_speed;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        std::cout << "s" << std::endl;
        if(worldMap[int(pos_x - player_dir_x * move_speed)][int(pos_y)] == 0) pos_x -= player_dir_x * move_speed;
        if(worldMap[int(pos_x)][int(pos_y - player_dir_y * move_speed)] == 0) pos_y -= player_dir_y * move_speed;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        float oldDirX = player_dir_x;
        player_dir_x = player_dir_x * cos(rotation_speed) - player_dir_y * sin(rotation_speed);
        player_dir_y = oldDirX * sin(rotation_speed) + player_dir_y * cos(rotation_speed);
        float oldPlaneX = plane_x;
        plane_x = plane_x * cos(rotation_speed) - plane_y * sin(rotation_speed);
        plane_y = oldPlaneX * sin(rotation_speed) + plane_y * cos(rotation_speed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        float oldDirX = player_dir_x;
        player_dir_x = player_dir_x * cos(-rotation_speed) - player_dir_y * sin(-rotation_speed);
        player_dir_y = oldDirX * sin(-rotation_speed) + player_dir_y * cos(-rotation_speed);
        float oldPlaneX = plane_x;
        plane_x = plane_x * cos(-rotation_speed) - plane_y * sin(-rotation_speed);
        plane_y = oldPlaneX * sin(-rotation_speed) + plane_y * cos(-rotation_speed);
    }
}
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    for(int x = 0; x < WIDTH; x++) {
        float cameraX = 2 * x / float(WIDTH) - 1;
        float rayDirX = player_dir_x + plane_x * cameraX;
        float rayDirY = player_dir_y + plane_y * cameraX;

        //initial ray positioin and distance calculation
        //which box of the map we re in
        int map_x = int(pos_x);
        int map_y = int(pos_y);

        //length of ray from current position to next x or y side
        float sideDistX;
        float sideDistY;

        //length of ray from one x or y side to next x or y side
        float deltaDistX = std::abs(1 / rayDirX);
        float deltaDistY = std::abs(1 / rayDirY);
        float perpWallDist;

        int stepX;
        int stepY;

        int hit = 0;//was there a wall hit?
        int side;

        //calculate Initial Side Distance and steps
        if(rayDirX < 0) {
            stepX = -1;
            sideDistX = (pos_x - map_x) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (map_x + 1.0 -pos_x) * deltaDistX;
        }
        if(rayDirY < 0) {
            stepY = -1;
            sideDistY = (pos_y - map_y) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (map_y + 1.0 - pos_y) * deltaDistY;
        }
        while(hit == 0) {
            if(sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                map_x += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                map_y += stepY;
                side = 1;
            }

            if(worldMap[map_x][map_y] > 0) {
                hit = 1;
            }
        }
        if (side == 0) {
            perpWallDist = (map_x - pos_x + (1 - stepX) / 2) / rayDirX;
        } else {
            perpWallDist = (map_y - pos_y + (1 - stepY) / 2) / rayDirY;
        }

        //height of the line
        int lineHeight = (int)(HEIGHT / perpWallDist);

        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

        //choose wall color
        float color = 1.0;
        switch (worldMap[map_x][map_y]) 
        {
        case 1:
            color = 1.0;
            break;
        
        default:
            color = 1.0;
            break;
        }

        if (side == 1) {
            color = color / 2;
        }

        //render wall column
        glColor3f(color, color, color);
        glBegin(GL_LINES);
        glVertex2i(x, drawStart);
        glVertex2i(x, drawEnd);
        glEnd();
    }

}
