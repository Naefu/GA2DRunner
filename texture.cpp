#include "TextureBuilder.h"
#define GLUT_KEY_ESCAPE 27
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <glut.h>

GLuint texID;
int rep = 1;

float playerX = -0.5f;
float playerY = -0.4f;
bool isJumping = false;
bool isGrounded = true; 
bool isDucking = false;  
float jumpHeight = 0.05f;  
float jumpVelocity = 0.08f; 
float gravity = 0.015f;  
float fallSpeed = 0.0f;
bool whenG = false;
std::vector<float> obstacles;
const float obstacleWidth = 0.05f;
const float obstacleHeight = 0.1f;
float obstacleSpeed = 0.02f;
float obstacleSpawnTimer = 0.0f;
float spawnInterval = 1.0f;

std::vector<float> aerialObstacles;
const float aerialobstacleWidth = 0.05f;
const float aerialobstacleHeight = 0.1f;
float aerialobstacleSpeed = 0.02f;
float aerialobstacleSpawnTimer = 0.5f;
float aerialspawnInterval = 1.0f; 

bool gameOver = false;
float gameDuration = 30.0f;
float timeRemaining = gameDuration;
int score = 0;
int health = 50;


enum PowerUpType { NONE, SPEED_BOOST, HEALTH_UP };
struct PowerUp {
    float x, y;
    PowerUpType type;
    bool active; 
};

std::vector<PowerUp> powerups;
const float powerupWidth = 0.05f;
const float powerupHeight = 0.1f;
float powerupSpeed = 0.02f;
float powerupSpawnTimer = 0.5f; 
float powerupspawnInterval = 5.0f; 


void resetGame() {
    playerX = -0.5f;
    playerY = -0.4f;
    isJumping = false;
    isGrounded = true;
    isDucking = false;
    obstacles.clear();
    aerialObstacles.clear();
    obstacleSpeed = 0.02f;
    aerialobstacleSpeed = 0.02f;

    gameOver = false;
    timeRemaining = gameDuration;
    score = 0;
    health = 5;
}

void drawGround() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texID);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.5f); glTexCoord2f(0.0f, rep);
    glVertex2f(1.0f, -0.5f); glTexCoord2f(rep, rep);
    glVertex2f(1.0f, -1.0f); glTexCoord2f(rep, 0.0f);
    glVertex2f(-1.0f, -1.0f); glTexCoord2f(0.0f, 0.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

int testingattentionplease = 1.0;

void drawCharacter(float x, float y) {
    float playerWidth = 0.1f;
    float playerHeight = isDucking ? 0.1f : 0.2f;

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + playerWidth, y);
    glVertex2f(x + playerWidth, y + playerHeight); 
    glVertex2f(x, y + playerHeight);
    glEnd();

    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_QUADS);
    glVertex2f(x + 0.025f, y + playerHeight);
    glVertex2f(x + 0.075f, y + playerHeight);
    glVertex2f(x + 0.075f, y + playerHeight + 0.1f); 
    glVertex2f(x + 0.025f, y + playerHeight + 0.1f);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x + 0.035f, y + playerHeight + 0.05f); 
    glVertex2f(x + 0.045f, y + playerHeight + 0.05f);
    glVertex2f(x + 0.045f, y + playerHeight + 0.07f);
    glVertex2f(x + 0.035f, y + playerHeight + 0.07f);

    glVertex2f(x + 0.055f, y + playerHeight + 0.05f); 
    glVertex2f(x + 0.065f, y + playerHeight + 0.05f);
    glVertex2f(x + 0.065f, y + playerHeight + 0.07f);
    glVertex2f(x + 0.055f, y + playerHeight + 0.07f);
    glEnd();

    glColor3f(0.0f, 0.5f, 0.0f);
    float armHeight = 0.05f; 
    float armPlace = isDucking ? 0.03f : 0.1f;
    glBegin(GL_QUADS);
    glVertex2f(x - 0.02f, y + armPlace); 
    glVertex2f(x, y + armPlace);
    glVertex2f(x, y + armPlace + armHeight);
    glVertex2f(x - 0.02f, y + armPlace + armHeight);

    glVertex2f(x + playerWidth, y + armPlace); 
    glVertex2f(x + playerWidth + 0.02f, y + armPlace);
    glVertex2f(x + playerWidth + 0.02f, y + armPlace + armHeight);
    glVertex2f(x + playerWidth, y + armPlace + armHeight);
    glEnd();

    glColor3f(0.5f, 0.0f, 0.0f); 
    float legHeight = isGrounded ? 0.1f : 0.05f;
    float leftlegHeight = isGrounded ? testingattentionplease % 10 > 5 ? 0.05f : 0.0f : 0.0f;
    float rightlegHeight = isGrounded ? testingattentionplease % 10 > 5 ? 0.0f : 0.05f : 0.0f;

    glBegin(GL_QUADS);
    glVertex2f(x + 0.02f, y - legHeight + leftlegHeight); 
    glVertex2f(x + 0.04f, y - legHeight + leftlegHeight);
    glVertex2f(x + 0.04f, y); 
    glVertex2f(x + 0.02f, y); 
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x + 0.06f, y - legHeight + rightlegHeight); 
    glVertex2f(x + 0.08f, y - legHeight + rightlegHeight);
    glVertex2f(x + 0.08f, y); 
    glVertex2f(x + 0.06f, y); 
    glEnd();
}


void drawObstacles() {
    glColor3f(0.0f, 0.0f, 0.0f);
    for (float obstacleX : obstacles) {
        glBegin(GL_QUADS);
        glVertex2f(obstacleX, -0.4f); 
        glVertex2f(obstacleX + obstacleWidth, -0.4f); 
        glVertex2f(obstacleX + obstacleWidth, -0.4f + obstacleHeight); 
        glVertex2f(obstacleX, -0.4f + obstacleHeight);
        glEnd();
    }
}


void drawAerialObstacles() {
    glColor3f(0.0f, 0.0f, 0.0f);
    for (float aerialobstacleX : aerialObstacles) {
        glBegin(GL_QUADS);
        glVertex2f(aerialobstacleX, -0.15f); 
        glVertex2f(aerialobstacleX + aerialobstacleWidth, -0.15f);
        glVertex2f(aerialobstacleX + aerialobstacleWidth, -0.15f + aerialobstacleHeight);
        glVertex2f(aerialobstacleX, -0.15f + aerialobstacleHeight);
        glEnd();
    }
}


void drawTopBar() {
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(1.0f, 0.5f);
    glVertex2f(1.0f, 0.4f);
    glVertex2f(-1.0f, 0.4f);
    glEnd();
}


void drawHealth() {
    glColor3f(0.0f, 1.0f, 0.0f); 
    float healthBarWidth = 0.1f; 
    float healthSpacing = 0.02f; 

    for (int i = 0; i < health; i++) {
        glBegin(GL_QUADS);
        glVertex2f(-0.9f + (healthBarWidth + healthSpacing) * i, 0.45f); 
        glVertex2f(-0.9f + (healthBarWidth + healthSpacing) * i + healthBarWidth, 0.45f); 
        glVertex2f(-0.9f + (healthBarWidth + healthSpacing) * i + healthBarWidth, 0.45f + 0.03f); 
        glVertex2f(-0.9f + (healthBarWidth + healthSpacing) * i, 0.45f + 0.03f); 
        glEnd();
    }
}

void displayScore() {
    glColor3f(1.0f, 1.0f, 1.0f); 
    std::string scoreText = "Score: " + std::to_string(score);
    glRasterPos2f(0.1f, 0.45f); 
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}


void displayTime() {
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string timeText = "Time: " + std::to_string(static_cast<int>(timeRemaining));

    glRasterPos2f(0.6f, 0.45f);
    for (char c : timeText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}


void display() {

    glColor3f(0.6f, 0.9f, 1.0f); 
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.5f);
    glVertex2f(1.0f, 0.5f);
    glVertex2f(1.0f, -0.5f);
    glVertex2f(-1.0f, -0.5f);
    glEnd();

    drawGround();
    drawCharacter(playerX, playerY);
    drawObstacles();
	drawAerialObstacles();
    drawTopBar(); 
    drawHealth(); 
    displayScore(); 
    displayTime();

    
    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f); 
        glRasterPos2f(-0.1f, 0.0f);
        std::string gameOverText = (timeRemaining < 1) ? "You Win!" : "Game Over!";
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    glutSwapBuffers();
}


bool checkCollision(float obstacleX, float playerX, float playerY, float playerWidth, float playerHeight) {
    return !(obstacleX > playerX + playerWidth || obstacleX + obstacleWidth < playerX || -0.4f > playerY + playerHeight || -0.4f + obstacleHeight < playerY);
}

bool checkaerialCollision(float obstacleX, float playerX, float playerY, float playerWidth, float playerHeight) {
    return !(obstacleX > playerX + playerWidth || obstacleX + obstacleWidth < playerX || -0.2f > playerY + playerHeight || -0.2f + obstacleHeight < playerY);
}



void update(int value) {
    if (!gameOver) {
        
        if (isJumping) {
            playerY += jumpVelocity;
            if (playerY >= jumpHeight) {
                isJumping = false; 
                isGrounded = false; 
                fallSpeed = 0.0f; 
            }
        }
        else {
           
            fallSpeed += gravity; 
            playerY -= fallSpeed; 

            if (playerY < -0.4f) {
                playerY = -0.4f; 
                fallSpeed = 0.0f; 
                isGrounded = true; 
            }
        }




        for (size_t i = 0; i < obstacles.size();) {
            obstacles[i] -= obstacleSpeed;

            if (checkCollision(obstacles[i], playerX, playerY, 0.1f, isDucking ? 0.1f : 0.2f)) {
                health--;
                obstacles.erase(obstacles.begin() + i);
                if (health <= 0) {
                    gameOver = true; 
                }
                break;
            }
            else {
                ++i;
            }
        }
        float minObstacleX = 1.5f;
        float maxObstacleX = 2.0f;

        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](float x) { return x < -1.0f; }), obstacles.end());

        obstacleSpawnTimer += 0.016f;
        if (obstacleSpawnTimer >= spawnInterval) {
            float randomX = minObstacleX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxObstacleX - minObstacleX))); 
            obstacles.push_back(randomX);
            obstacleSpawnTimer = 0.0f; 

            spawnInterval = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.5f - 0.5f)));
        }








        for (size_t i = 0; i < aerialObstacles.size();) {
            aerialObstacles[i] -= aerialobstacleSpeed;

            if (checkaerialCollision(aerialObstacles[i], playerX, playerY, 0.1f, isDucking ? 0.1f : 0.2f)) {
                health--;
                aerialObstacles.erase(aerialObstacles.begin() + i); 
                if (health <= 0) {
                    gameOver = true; 
                }
                break; 
            }
            else {
                ++i;
            }
        }

        aerialObstacles.erase(std::remove_if(aerialObstacles.begin(), aerialObstacles.end(), [](float x) { return x < -1.0f; }), aerialObstacles.end());

        aerialobstacleSpawnTimer += 0.016f;
        if (aerialobstacleSpawnTimer >= aerialspawnInterval) {
            float randomX = minObstacleX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxObstacleX - minObstacleX)));
            aerialObstacles.push_back(randomX); 
            aerialobstacleSpawnTimer = 0.0f;

            aerialspawnInterval = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.5f - 0.5f)));
        }

        score++;
        if (score % 100 == 0) {
            obstacleSpeed += 0.005f; 
            aerialobstacleSpeed += 0.005f; 
        }

        timeRemaining -= 0.016f;
        if (timeRemaining <= 0) {
            gameOver = true;
        }

        glutPostRedisplay();
    }
	whenG = !whenG;
    testingattentionplease++;
    glutTimerFunc(16, update, 0);
}

void handleKeypress(unsigned char key, int x, int y) {
    if (key == ' ') {
        if (isGrounded) {
            isJumping = true; 
        }
    }
    else if (key == 'r') {
        resetGame();
    }
    else if (key == GLUT_KEY_ESCAPE) {
        exit(0);
    }
}

void handleSpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN) {
        isDucking = true; 
    }
}

void handleSpecialKeyUp(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN) {
        isDucking = false; 
    }
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0))); 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Jumping Game");

    glEnable(GL_TEXTURE_2D);

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeys);
    glutSpecialUpFunc(handleSpecialKeyUp);
    glutTimerFunc(0, update, 0);
    glutMainLoop();

    return 0;
}
