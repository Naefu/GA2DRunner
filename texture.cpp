#include "TextureBuilder.h"
#define GLUT_KEY_ESCAPE 27
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <glut.h>
#include <playsoundapi.h>

float permatimer = 0.0f;
int permapermatimer = 0;
float songtimer = 0.0f;
int song1length = 1802;
int song2length = 1802;
int song3length = 1932;
int song4length = 1889;
int song5length = 1802;
int targetlength = -100;
bool startcounting = false;
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
bool randompower = false;
float invincibility = 0;
float jumpingjack = 0;

std::vector<float> cloudsforsomereason;
float cloudSpeed = 0.02f;
float cloudSpawnTimer = 0.0f;
float cloudspawnInterval = 1.0f;

std::vector<float> obstacles;
const float obstacleWidth = 0.1f;
const float obstacleHeight = 0.1f;
float obstacleSpeed = 0.02f;
float obstacleSpawnTimer = 0.0f;
float spawnInterval = 1.0f;

std::vector<float> aerialObstacles;
const float aerialobstacleWidth = 0.1f;
const float aerialobstacleHeight = 0.1f;
float aerialobstacleSpeed = 0.02f;
float aerialobstacleSpawnTimer = 0.5f;
float aerialspawnInterval = 1.0f;

std::vector<float> PowerUps;
const float PowerUpWidth = 0.05f;
const float PowerUpHeight = 0.05f;
float powerUpSpeed = 0.02f;
float PowerUpSpawnTimer = 0.5f;
float PowerUpspawnInterval = 10.0f;

std::vector<float> aerialPowerUps;
const float aerialpowerUpWidth = 0.05f;
const float aerialpowerUpHeight = 0.05f;
float aerialpowerUpSpeed = 0.02f;
float aerialpowerUpSpawnTimer = 0.5f;
float aerialpowerUpspawnInterval = 10.0f;

std::vector<float> Collectibles;
const float CollectibleWidth = 0.05f;
const float CollectibleHeight = 0.1f;
float CollectibleSpeed = 0.02f;
float CollectibleSpawnTimer = 0.5f;
float CollectiblespawnInterval = 5.0f;

std::vector<float> aerialCollectibles;
const float aerialcollectibleWidth = 0.05f;
const float aerialcollectibleHeight = 0.1f;
float aerialCollectibleSpeed = 0.02f;
float aerialCollectibleSpawnTimer = 0.5f;
float aerialCollectiblespawnInterval = 5.0f;

std::vector<float> allObstacles;


bool gameOver = false;
float gameDuration = 30.0f;
float timeRemaining = gameDuration;
int score = 0;
int health = 5;



void resetGame() {
    playerX = -0.5f;
    playerY = -0.4f;
    isJumping = false;
    isGrounded = true;
    isDucking = false;
    obstacles.clear();
    aerialObstacles.clear();
    PowerUps.clear();
    aerialPowerUps.clear();
    Collectibles.clear();
    aerialCollectibles.clear();
    allObstacles.clear();
    obstacleSpeed = 0.02f;
    aerialobstacleSpeed = 0.02f;
    powerUpSpeed = 0.02f;
    aerialpowerUpSpeed = 0.02f;
    CollectibleSpeed = 0.02f;
    aerialCollectibleSpeed = 0.02f;
    songtimer = 0;
	startcounting = false;
	targetlength = -100;
    gameOver = false;
    timeRemaining = gameDuration;
    invincibility = 0;
    jumpingjack = 0;
    score = 0;
    health = 5;
	PlaySound(TEXT("ror1.wav"), NULL, SND_FILENAME | SND_ASYNC);
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

void drawCircle(float x, float y, float radius, int numSegments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0f * 3.14159f * float(i) / float(numSegments);
        float dx = radius * cosf(angle);
        float dy = radius * sinf(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}


void drawCCircle(float x, float y, float radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * 3.14159f / 180; // Convert degrees to radians
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

void drawCloud() {
    glColor3f(1.0f, 1.0f, 1.0f); 
    for (float x : cloudsforsomereason) {
        drawCCircle(x, 0.1f, 0.1f);
        drawCCircle(x - 0.1f, 0.1f + 0.05f, 0.1f);
        drawCCircle(x + 0.1f, 0.1f + 0.05f, 0.1f);
        drawCCircle(x - 0.2f, 0.1f, 0.1f);
        drawCCircle(x + 0.2f, 0.1f, 0.1f);
    }
}

void drawCharacter(float x, float y) {
    float playerWidth = 0.1f;
    float playerHeight = isDucking ? 0.1f : 0.2f;
    float moron = (invincibility > 0 ? 0.5f : 0.0f);
    glColor3f(0.0f, moron, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + playerWidth, y);
    glVertex2f(x + playerWidth, y + playerHeight);
    glVertex2f(x, y + playerHeight);
    glEnd();


    glColor3f(1.0f, moron, 1.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x + (playerWidth / 2) - 0.04, y + 0.01);
    glVertex2f(x + (playerWidth / 2) - 0.04, y + playerHeight - 0.01);
    glVertex2f(x + (playerWidth / 2), y + 0.01);
    glVertex2f(x + (playerWidth / 2) + 0.04, y + playerHeight - 0.01);
    glVertex2f(x + (playerWidth / 2) + 0.04, y + 0.01);
    glEnd();

    glColor3f(1.0f, 0.8f, moron);
    glBegin(GL_QUADS);
    glVertex2f(x + 0.025f, y + playerHeight);
    glVertex2f(x + 0.075f, y + playerHeight);
    glVertex2f(x + 0.075f, y + playerHeight + 0.1f);
    glVertex2f(x + 0.025f, y + playerHeight + 0.1f);
    glEnd();


    glColor3f(0.0f, moron, 0.0f);
    drawCircle(x + (playerWidth / 2) + 0.01, y + playerHeight + 0.05f, 0.01, 16);
    drawCircle(x + (playerWidth / 2) - 0.01, y + playerHeight + 0.05f, 0.01, 16);

    glColor3f(moron, 0.5f, 0.0f);
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

    glColor3f(0.5f, 0.0f, moron);
    float legHeight = isGrounded ? 0.1f : 0.05f;
    float leftlegHeight = isGrounded ? testingattentionplease % 10 > 5 ? 0.05f : 0.0f : 0.0f;
    float rightlegHeight = isGrounded ? testingattentionplease % 10 > 5 ? 0.0f : 0.05f : 0.0f;

    glBegin(GL_TRIANGLES);
    glVertex2f(x + 0.02f, y - legHeight + leftlegHeight);
    glVertex2f(x + 0.04f, y - legHeight + leftlegHeight);
    glVertex2f(x + 0.03f, y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(x + 0.06f, y - legHeight + rightlegHeight);
    glVertex2f(x + 0.08f, y - legHeight + rightlegHeight);
    glVertex2f(x + 0.07f, y);
    glEnd();
}


void drawObstacles() {
    glColor3f(0.0f, 0.0f, 0.0f);
    for (float obstacleX : obstacles) {
        glPushMatrix();

        glTranslatef(obstacleX + obstacleWidth / 2, -0.5f + obstacleHeight / 2, 0);

        glRotated(permatimer, 0, 0, 1);

        glTranslatef(-(obstacleX + obstacleWidth / 2), -(-0.5f + obstacleHeight / 2), 0);

        glBegin(GL_QUADS);
        glVertex2f(obstacleX, -0.5f);
        glVertex2f(obstacleX + obstacleWidth, -0.5f);
        glVertex2f(obstacleX + obstacleWidth, -0.5f + obstacleHeight);
        glVertex2f(obstacleX, -0.5f + obstacleHeight);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(obstacleX, -0.5f + obstacleHeight); glVertex2f(obstacleX + obstacleWidth, -0.5f + obstacleHeight);
        glVertex2f(obstacleX + obstacleWidth / 2, -0.5f + obstacleHeight*2);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(obstacleX, -0.5f); glVertex2f(obstacleX + obstacleWidth, -0.5f);
        glVertex2f(obstacleX + obstacleWidth / 2, -0.5f - obstacleHeight);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(obstacleX, -0.5f); glVertex2f(obstacleX, -0.5f + obstacleHeight);
        glVertex2f(obstacleX - obstacleWidth, -0.5f + obstacleHeight / 2);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(obstacleX + obstacleWidth, -0.5f); glVertex2f(obstacleX + obstacleWidth, -0.5f + obstacleHeight);
        glVertex2f(obstacleX + obstacleWidth*2, -0.5f + obstacleHeight / 2);
        glEnd();


        glPopMatrix();
    }
}

void drawAerialObstacles() {
    glColor3f(0.0f, 0.0f, 0.0f);
    for (float aerialobstacleX : aerialObstacles) {
        
        glPushMatrix();

        glTranslatef(aerialobstacleX + obstacleWidth / 2, -0.15f + obstacleHeight / 2, 0);

        glRotated(permatimer, 0, 0, 1);

        glTranslatef(-(aerialobstacleX + obstacleWidth / 2), -(-0.15f + obstacleHeight / 2), 0);

        glBegin(GL_QUADS);
        glVertex2f(aerialobstacleX, -0.15f);
        glVertex2f(aerialobstacleX + obstacleWidth, -0.15f);
        glVertex2f(aerialobstacleX + obstacleWidth, -0.15f + obstacleHeight);
        glVertex2f(aerialobstacleX, -0.15f + obstacleHeight);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(aerialobstacleX, -0.15f + obstacleHeight); glVertex2f(aerialobstacleX + obstacleWidth, -0.15f + obstacleHeight);
        glVertex2f(aerialobstacleX + obstacleWidth / 2, -0.15f + obstacleHeight * 2);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(aerialobstacleX, -0.15f); glVertex2f(aerialobstacleX + obstacleWidth, -0.15f);
        glVertex2f(aerialobstacleX + obstacleWidth / 2, -0.15f - obstacleHeight);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(aerialobstacleX, -0.15f); glVertex2f(aerialobstacleX, -0.15f + obstacleHeight);
        glVertex2f(aerialobstacleX - obstacleWidth, -0.15f + obstacleHeight / 2);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(aerialobstacleX + obstacleWidth, -0.15f); glVertex2f(aerialobstacleX + obstacleWidth, -0.15f + obstacleHeight);
        glVertex2f(aerialobstacleX + obstacleWidth * 2, -0.15f + obstacleHeight / 2);
        glEnd();


        glPopMatrix();
    }
}

void drawPowerUps() {
    for (float powerUpX : PowerUps) {
        glPushMatrix();

        glTranslatef(powerUpX + PowerUpWidth / 2, -0.4f + PowerUpHeight / 2, 0);

        float permapermapermatimer = (permapermatimer % 100) / 100.0f;
        glScaled(1 + permapermapermatimer, 1 + permapermapermatimer, 1);

        glTranslatef(-(powerUpX + PowerUpWidth / 2), -(-0.4f + PowerUpHeight / 2), 0);

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(powerUpX, -0.4f);
        glVertex2f(powerUpX + PowerUpWidth / 3.0f, -0.4f);
        glVertex2f(powerUpX + PowerUpWidth / 3.0f, -0.4f + PowerUpHeight);
        glVertex2f(powerUpX, -0.4f + PowerUpHeight);
        glEnd();

        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(powerUpX + PowerUpWidth / 3.0f, -0.4f);
        glVertex2f(powerUpX + 2 * (PowerUpWidth / 3.0f), -0.4f);
        glVertex2f(powerUpX + 2 * (PowerUpWidth / 3.0f), -0.4f + PowerUpHeight);
        glVertex2f(powerUpX + PowerUpWidth / 3.0f, -0.4f + PowerUpHeight);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(powerUpX + 2 * (PowerUpWidth / 3.0f), -0.4f);
        glVertex2f(powerUpX + PowerUpWidth, -0.4f);
        glVertex2f(powerUpX + PowerUpWidth, -0.4f + PowerUpHeight);
        glVertex2f(powerUpX + 2 * (PowerUpWidth / 3.0f), -0.4f + PowerUpHeight);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(powerUpX, -0.4f);
        glVertex2f(powerUpX + PowerUpWidth, -0.4f);
        glVertex2f(powerUpX + PowerUpWidth / 2, -0.4f - PowerUpHeight);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(powerUpX + PowerUpWidth / 2, -0.4f + PowerUpHeight * 2);
        glVertex2f(powerUpX, -0.4f + PowerUpHeight);
        glVertex2f(powerUpX + PowerUpWidth, -0.4f + PowerUpHeight);
        glEnd();
        glPopMatrix();
    }
}

void drawAerialPowerUps() {
    for (float aerialPowerUpX : aerialPowerUps) {
        glPushMatrix();

        glTranslatef(aerialPowerUpX + aerialpowerUpWidth / 2, 0.15f + aerialpowerUpHeight / 2, 0);

        float permapermapermatimer = (permapermatimer % 100) / 100.0f;
        glScaled(1 + permapermapermatimer, 1 + permapermapermatimer, 1);

        glTranslatef(-(aerialPowerUpX + aerialpowerUpWidth / 2), -(0.15f + aerialpowerUpHeight / 2), 0);

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(aerialPowerUpX, 0.15f);
        glVertex2f(aerialPowerUpX + 2 * (aerialpowerUpWidth / 3.0f), 0.15f);
        glVertex2f(aerialPowerUpX + 2 * (aerialpowerUpWidth / 3.0f), 0.15f + aerialpowerUpHeight);
        glVertex2f(aerialPowerUpX, 0.15f + aerialpowerUpHeight);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(aerialPowerUpX + aerialpowerUpWidth / 3, 0.15f);
        glVertex2f(aerialPowerUpX + 2 * (aerialpowerUpWidth / 3.0f), 0.15f);
        glVertex2f(aerialPowerUpX + 2 * (aerialpowerUpWidth / 3.0f), 0.15f + aerialpowerUpHeight);
        glVertex2f(aerialPowerUpX + aerialpowerUpWidth / 3, 0.15f + aerialpowerUpHeight);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(aerialPowerUpX + 2 * (aerialpowerUpWidth / 3.0f), 0.15f);
        glVertex2f(aerialPowerUpX + aerialpowerUpWidth, 0.15f);
        glVertex2f(aerialPowerUpX + aerialpowerUpWidth, 0.15f + aerialpowerUpHeight);
        glVertex2f(aerialPowerUpX + 2 * (aerialpowerUpWidth / 3.0f), 0.15f + aerialpowerUpHeight);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(aerialPowerUpX, 0.15f);
        glVertex2f(aerialPowerUpX + PowerUpWidth, 0.15f);
        glVertex2f(aerialPowerUpX + PowerUpWidth / 2, 0.15f - PowerUpHeight);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(aerialPowerUpX + PowerUpWidth / 2, 0.15f + PowerUpHeight * 2);
        glVertex2f(aerialPowerUpX, 0.15f + PowerUpHeight);
        glVertex2f(aerialPowerUpX + PowerUpWidth, 0.15f + PowerUpHeight);
        glEnd();
		glPopMatrix();
    }
}

void drawCollectibles() {
    glColor3f(0.0f, 0.0f, 1.0f);
    for (float collectibleX : Collectibles) {
        glPushMatrix();
        glTranslatef(collectibleX + CollectibleWidth / 2, -0.4f + CollectibleHeight / 2, 0);

        glRotated(permatimer, 0, 1, 0);

        glTranslatef(-(collectibleX + CollectibleWidth / 2), -(-0.4f + CollectibleHeight / 2), 0);

        glBegin(GL_QUADS);
        glVertex2f(collectibleX, -0.4f);
        glVertex2f(collectibleX + CollectibleWidth, -0.4f);
        glVertex2f(collectibleX + CollectibleWidth, -0.4f + CollectibleHeight);
        glVertex2f(collectibleX, -0.4f + CollectibleHeight);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(collectibleX, -0.4f);
        glVertex2f(collectibleX, -0.4f + CollectibleHeight);
        glVertex2f(collectibleX - CollectibleWidth * 0.5, -0.4f + CollectibleHeight / 2);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(collectibleX + CollectibleWidth, -0.4f);
        glVertex2f(collectibleX + CollectibleWidth, -0.4f + CollectibleHeight);
        glVertex2f(collectibleX + CollectibleWidth * 1.5, -0.4f + CollectibleHeight / 2);
        glEnd();
        glPopMatrix();
    }
}

void drawAerialCollectibles() {
    glColor3f(0.0f, 0.0f, 1.0f);
    for (float aerialcollectibleX : aerialCollectibles) {
        glPushMatrix();
        glTranslatef(aerialcollectibleX + CollectibleWidth / 2, -0.4f + CollectibleHeight / 2, 0);

        glRotated(permatimer, 0, 1, 0);

        glTranslatef(-(aerialcollectibleX + CollectibleWidth / 2), -(-0.4f + CollectibleHeight / 2), 0);

        glBegin(GL_QUADS);
        glVertex2f(aerialcollectibleX, 0.1f);
        glVertex2f(aerialcollectibleX + CollectibleWidth, 0.1f);
        glVertex2f(aerialcollectibleX + CollectibleWidth, 0.1f + CollectibleHeight);
        glVertex2f(aerialcollectibleX, 0.1f + CollectibleHeight);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(aerialcollectibleX, 0.1f);
        glVertex2f(aerialcollectibleX, 0.1f + CollectibleHeight);
        glVertex2f(aerialcollectibleX - CollectibleWidth * 0.5, 0.1f + CollectibleHeight / 2);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(aerialcollectibleX + CollectibleWidth, 0.1f);
        glVertex2f(aerialcollectibleX + CollectibleWidth, 0.1f + CollectibleHeight);
        glVertex2f(aerialcollectibleX + CollectibleWidth * 1.5, 0.1f + CollectibleHeight / 2);
        glEnd();
        glPopMatrix();
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

void drawHeart(float xOffset, float yOffset, float scale) {

    glColor3f(1.0f, 1.0f, 1.0f);
    float radius = 0.05 * scale;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xOffset, yOffset);
    for (float angle = 0; angle <= 2 * 3.14; angle += 0.01) {
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(xOffset + x, yOffset + y);
    }
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xOffset, yOffset);

    for (float t = 0; t <= 2 * 3.14; t += 0.01) {
        float x = 16 * pow(sin(t), 3);
        float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);

        glVertex2f(xOffset + (x / 400.0f) * scale, yOffset + (y / 350.0f) * scale);
    }
    glEnd();

}

void drawHealth() {
    float healthBarWidth = 0.1f;
    float healthSpacing = 0.02f;

    for (int i = 0; i < health; i++) {
        drawHeart(-0.9f + (healthBarWidth + healthSpacing) * i, 0.45f, 1.0f);
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
    drawTopBar();
    drawHealth();
    drawCollectibles();
    drawAerialCollectibles();
    drawCloud();
    drawAerialObstacles();
    drawPowerUps();
    drawAerialPowerUps();
    displayScore();
    displayTime();


    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.1f, 0.0f);
        std::string gameOverText = (timeRemaining < 1) ? "You Win!" : "Game Over!";
        if (gameOverText == "You Win!") { 
            PlaySound(TEXT("lose.wav"), NULL, SND_FILENAME | SND_ASYNC); }
        else {
            PlaySound(TEXT("win.wav"), NULL, SND_FILENAME | SND_ASYNC);
        }
        glColor3f(0, 0, 0);
        glBegin(GL_QUADS);
		glVertex2f(-10.1f, 10.1f);
		glVertex2f(10.1f, 10.1f);
		glVertex2f(10.1f, -10.1f);
		glVertex2f(-10.1f, -10.1f);
		glEnd();

        glColor3f(1, 1, 1);
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    glutSwapBuffers();
}


bool checkCollision(float obstacleX, float playerX, float playerY, float playerWidth, float playerHeight) {
    return !(obstacleX > playerX + playerWidth || obstacleX + obstacleWidth < playerX
        || -0.4f > playerY + playerHeight || -0.4f + obstacleHeight < playerY);
}

bool checkaerialCollision(float obstacleX, float playerX, float playerY, float playerWidth, float playerHeight) {
    return !(obstacleX > playerX + playerWidth || obstacleX + obstacleWidth < playerX
        || -0.2f > playerY + playerHeight || -0.2f + obstacleHeight < playerY);
}

bool checkaerialspecialCollision(float obstacleX, float playerX, float playerY, float playerWidth, float playerHeight) {
    return !(obstacleX > playerX + playerWidth || obstacleX + obstacleWidth < playerX
        || 0.1f > playerY + playerHeight || 0.1f + obstacleHeight < playerY);
}

void die() {
    if(invincibility>0){
		return;
    }
    health--;
    if (PlaySound((std::to_wstring(5 - health) + L".wav").c_str(), NULL, SND_FILENAME | SND_ASYNC) == 0) {
        std::cerr << "Error playing sound 1: " << GetLastError() << std::endl;
    }
    startcounting = true;

    switch (5 - health) {
    case 1:
        targetlength = song1length;
        break;
    case 2:
        targetlength = song2length;
        break;
    case 3:
        targetlength = song3length;
        break;
    case 4:
        targetlength = song4length;
        break;
    case 5:
        targetlength = song5length;
        break;
    }

}
bool isWithinRange(float target, const std::vector<float>& allObstacles, float range = 0.4f) {
    for (float obstacle : allObstacles) {
        if (std::fabs(target - obstacle) <= range) {
            return true; // Return true if the target is within range of any element
        }
    }
    return false; // Return false if no elements are within range
}
void clearALL() {
    allObstacles.clear();
    allObstacles.insert(allObstacles.end(), obstacles.begin(), obstacles.end());
    allObstacles.insert(allObstacles.end(), aerialObstacles.begin(), aerialObstacles.end());
    allObstacles.insert(allObstacles.end(), PowerUps.begin(), PowerUps.end());
    allObstacles.insert(allObstacles.end(), aerialPowerUps.begin(), aerialPowerUps.end());
    allObstacles.insert(allObstacles.end(), Collectibles.begin(), Collectibles.end());
    allObstacles.insert(allObstacles.end(), aerialCollectibles.begin(), aerialCollectibles.end());
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
                die();
                score -= 100;
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
            clearALL();
            if (!isWithinRange(randomX, allObstacles)) {
            obstacles.push_back(randomX);
        }
            obstacleSpawnTimer = 0.0f; 
            spawnInterval = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.5f - 0.5f)));
        }


        for (size_t i = 0; i < aerialObstacles.size();) {
            aerialObstacles[i] -= aerialobstacleSpeed;

            if (checkaerialCollision(aerialObstacles[i], playerX, playerY, 0.1f, isDucking ? 0.1f : 0.2f)) {
                die();
                score -= 100;
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
            clearALL();
            if (!isWithinRange(randomX, allObstacles)) {
                aerialObstacles.push_back(randomX);
            }
            aerialobstacleSpawnTimer = 0.0f;
            aerialspawnInterval = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.5f - 0.5f)));
        }


        for (size_t i = 0; i < PowerUps.size();) {
            PowerUps[i] -= powerUpSpeed;

            if (checkCollision(PowerUps[i], playerX, playerY, 0.1f, isDucking ? 0.1f : 0.2f)) {
                if (randompower) {
                    invincibility += 5;
                }
                else {
                    jumpingjack += 5;
                }
                randompower = !randompower;
                PowerUps.erase(PowerUps.begin() + i);
                break;
            }
            else {
                ++i;
            }
        }
        float minPowerUpX = 5.0f;
        float maxPowerUpX = 10.0f;

        PowerUps.erase(std::remove_if(PowerUps.begin(), PowerUps.end(), [](float x) { return x < -1.0f; }), PowerUps.end());

        PowerUpSpawnTimer += 0.016f;
        if (PowerUpSpawnTimer >= PowerUpspawnInterval) {
            float randomX = minPowerUpX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxPowerUpX - minPowerUpX)));
            clearALL();
            if (!isWithinRange(randomX, allObstacles)) {
                PowerUps.push_back(randomX);
            }
            PowerUpSpawnTimer = 0.0f;

            PowerUpspawnInterval = 5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxPowerUpX - minPowerUpX)));
        }


        for (size_t i = 0; i < aerialPowerUps.size();) {
            aerialPowerUps[i] -= aerialpowerUpSpeed;

            if (checkaerialspecialCollision(aerialPowerUps[i], playerX, playerY, 0.1f, isDucking ? 0.1f : 0.2f)) {
                if (randompower) {
                    invincibility += 5;
                }
                else {
                    jumpingjack += 5;
                }
                randompower = !randompower;
                aerialPowerUps.erase(aerialPowerUps.begin() + i);
                break;
            }
            else {
                ++i;
            }
        }

        aerialPowerUps.erase(std::remove_if(aerialPowerUps.begin(), aerialPowerUps.end(), [](float x) { return x < -1.0f; }), aerialPowerUps.end());

        aerialpowerUpSpawnTimer += 0.016f;
        if (aerialpowerUpSpawnTimer >= aerialpowerUpspawnInterval) {
            float randomX = minPowerUpX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxPowerUpX - minPowerUpX)));
            clearALL();
            if (!isWithinRange(randomX, allObstacles)) {
                aerialPowerUps.push_back(randomX);
            }
            aerialpowerUpSpawnTimer = 0.0f;

            aerialpowerUpspawnInterval = 5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxPowerUpX - minPowerUpX)));
        }


        for (size_t i = 0; i < Collectibles.size();) {
            Collectibles[i] -= CollectibleSpeed;

            if (checkCollision(Collectibles[i], playerX, playerY, 0.1f, isDucking ? 0.1f : 0.2f)) {
                score += (jumpingjack > 0 ? 2000 : 1000);
                Collectibles.erase(Collectibles.begin() + i);
                break;
            }
            else {
                ++i;
            }
        }
        float minCollectibleX = 5.0f;
        float maxCollectibleX = 10.0f;

        Collectibles.erase(std::remove_if(Collectibles.begin(), Collectibles.end(), [](float x) { return x < -1.0f; }), Collectibles.end());

        CollectibleSpawnTimer += 0.016f;
        if (CollectibleSpawnTimer >= CollectiblespawnInterval) {
            float randomX = minCollectibleX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxCollectibleX - minCollectibleX)));
            clearALL();
            if (!isWithinRange(randomX, allObstacles)) {
                Collectibles.push_back(randomX);
            }
            CollectibleSpawnTimer = 0.0f;

            CollectiblespawnInterval = 0.5 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxCollectibleX - minCollectibleX)));
        }


        for (size_t i = 0; i < aerialCollectibles.size();) {
            aerialCollectibles[i] -= aerialCollectibleSpeed;

            if (checkaerialspecialCollision(aerialCollectibles[i], playerX, playerY, 0.1f, isDucking ? 0.1f : 0.2f)) {
                score += 1000;
                aerialCollectibles.erase(aerialCollectibles.begin() + i);
                break;
            }
            else {
                ++i;
            }
        }

        aerialCollectibles.erase(std::remove_if(aerialCollectibles.begin(), aerialCollectibles.end(), [](float x) { return x < -1.0f; }), aerialCollectibles.end());

        aerialCollectibleSpawnTimer += 0.016f;
        if (aerialCollectibleSpawnTimer >= aerialCollectiblespawnInterval) {
            float randomX = minCollectibleX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxCollectibleX - minCollectibleX)));

            clearALL();
            if (!isWithinRange(randomX, allObstacles)) {
                aerialCollectibles.push_back(randomX);
            }
            aerialCollectibleSpawnTimer = 0.0f;

            aerialCollectiblespawnInterval = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxCollectibleX - minCollectibleX)));
        }




        for (size_t i = 0; i < cloudsforsomereason.size();) {
            cloudsforsomereason[i] -= cloudSpeed;
            ++i;
        }

        cloudsforsomereason.erase(std::remove_if(cloudsforsomereason.begin(), cloudsforsomereason.end(), [](float x) { return x < -1.0f; }), cloudsforsomereason.end());

        cloudSpawnTimer += 0.016f;
        if (cloudSpawnTimer >= cloudspawnInterval) {
            float randomXY = 1.5 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2 - 1.5)));
            cloudsforsomereason.push_back(randomXY);
            cloudSpawnTimer = 0.0f;
            cloudspawnInterval = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.5f - 0.5f)));
        }



        score = (jumpingjack > 0 ? score + 2 : score + 1);
        if (score % 100 == 0) {
            obstacleSpeed += 0.005f;
            aerialobstacleSpeed += 0.005f;
            powerUpSpeed += 0.005f;
            aerialpowerUpSpeed += 0.005f;
            CollectibleSpeed += 0.005f;
            aerialCollectibleSpeed += 0.005f;
			cloudSpeed += 0.005f;
        }
        if (startcounting == true) { songtimer = songtimer + 21; }

        if (targetlength - 21 < songtimer) {
            if (songtimer < targetlength + 21) {
            startcounting = false;
            songtimer = 0;
            PlaySound(TEXT("ror1.wav"), NULL, SND_FILENAME | SND_ASYNC);
            targetlength = -100;
            }
        }

        timeRemaining -= 0.016f;
        permatimer += 12.6f;
        permapermatimer += 2;
        jumpingjack -= (jumpingjack > 0 ? 0.016f : 0.0f);
        jumpingjack = (jumpingjack < 0 ? 0 : jumpingjack);
        invincibility -= (invincibility > 0 ? 0.016f : 0.0f);
        invincibility = (invincibility < 0 ? 0 : invincibility);
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


    if (PlaySound(TEXT("ror1.wav"), NULL, SND_FILENAME | SND_ASYNC) == 0) {
        std::cerr << "Error playing sound 1: " << GetLastError() << std::endl;
    }

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
