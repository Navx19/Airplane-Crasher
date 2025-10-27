#include<windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdbool>
#define PI 3.1416f

int triangleAmount = 1000;
GLfloat twicePi = 2.0f * PI;

// Animation variables
float _moveC = 0.0f;
float _obstacleY = 1.0f;  // Position of the obstacle planes

// Movement variables for fighter jet
float jetX = 0.0f;
float jetY = 0.0f;

// Game state
bool gameOver = false;

// Score variables
int score = 0;
int lastObstacleReset = 0;
//return again
int playagain = 0;
//exit
int x = 0;

// Lives system
int lives = 3;
bool invulnerable = false;
float invulnerableTimer = 0.0f;
const float INVULNERABLE_TIME = 2.0f; // 2 seconds of invulnerability after hit

// Fuel positions
float fuelX[2] = {0.5f, -0.4f};
float fuelY[2] = {-0.2f, 0.4f};
float fuelSpeed = 0.020f; // speed of falling fuel

// Enemy plane slowdown
int slowdown = 0;
bool enemySlowdown = false;
float enemySlowdownTimer = 0.0f;
const float ENEMY_SLOWDOWN_TIME = 2.0f; // 2 seconds slowdown
float obstacleSpeed = 0.01f; // default obstacle speed

// Speed-meter position and fall speed
float speedMeterY = 0.9f;      // start near the top
float speedMeterSpeed = 0.01f; // downward speed per frame

// --- Jet speed boost variables ---
bool speedBoost = false;              // are we currently boosted?
float speedBoostTimer = 0.0f;         // countdown timer for boost
const float SPEED_BOOST_TIME = 2.0f;  // 2 second duration
const float NORMAL_MOVE_AMOUNT = 0.05f;
const float BOOST_MOVE_AMOUNT  = 0.10f;   // faster movement when boosted

// Blink while speed boost is active
bool Blink = false;

// --- Mini Rocket variables ---
float rocketX = 0.4f;      // horizontal position
float rocketY = 1.0f;      // starts at the top
float rocketSpeed = 0.015f;
bool rocketVisible = true; // toggled for blinking
float rocketBlinkTimer = 0.0f;

void drawCircle() {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f);
        for (int i = 0; i <= triangleAmount; i++) {
            glVertex2f(
                (std::cos(i * twicePi / triangleAmount)),
                (std::sin(i * twicePi / triangleAmount))
            );
        }
    glEnd();
}

void drawCloud(float x, float y, float scale) {
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    // Cloud 1
    glPushMatrix(); glColor3ub(255, 255, 255); glTranslatef(-0.8f, 0.8f, 0.0f); glScalef(0.12f, 0.12f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glColor3ub(255, 255, 255); glTranslatef(-0.8f, 0.9f, 0.0f); glScalef(0.12f, 0.12f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glColor3ub(255, 255, 255); glTranslatef(-0.7f, 0.9f, 0.0f); glScalef(0.12f, 0.12f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glColor3ub(255, 255, 255); glTranslatef(-0.7f, 0.8f, 0.0f); glScalef(0.12f, 0.12f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glColor3ub(255, 255, 255); glTranslatef(-0.9f, 0.85f, 0.0f); glScalef(0.12f, 0.12f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glColor3ub(255, 255, 255); glTranslatef(-0.6f, 0.85f, 0.0f); glScalef(0.12f, 0.12f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();

    // Cloud 2
    glPushMatrix(); glTranslatef(-0.2f, 0.7f, 0.0f); glScalef(0.15f, 0.15f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.2f, 0.8f, 0.0f); glScalef(0.15f, 0.15f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.1f, 0.8f, 0.0f); glScalef(0.15f, 0.15f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.1f, 0.7f, 0.0f); glScalef(0.15f, 0.15f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.3f, 0.75f, 0.0f); glScalef(0.15f, 0.15f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 0.75f, 0.0f); glScalef(0.15f, 0.15f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();

    // Cloud 3
    glPushMatrix(); glTranslatef(0.4f, 0.65f, 0.0f); glScalef(0.13f, 0.13f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(0.4f, 0.75f, 0.0f); glScalef(0.13f, 0.13f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(0.5f, 0.75f, 0.0f); glScalef(0.13f, 0.13f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(0.5f, 0.65f, 0.0f); glScalef(0.13f, 0.13f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(0.3f, 0.7f, 0.0f); glScalef(0.13f, 0.13f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();
    glPushMatrix(); glTranslatef(0.6f, 0.7f, 0.0f); glScalef(0.13f, 0.13f, 1.0f); glutSolidSphere(0.7, 32, 32); glPopMatrix();

    glPopMatrix();
}

void drawSun(float x, float y, float scale) {
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(scale, scale, 1.0f);
        drawCircle();
    glPopMatrix();
}

void drawAirplane() {
    // If invulnerable, make the jet blink
    bool Blink = (invulnerable && (static_cast<int>(invulnerableTimer * 10) % 2 == 0)) ||
             (speedBoost  && (static_cast<int>(speedBoostTimer  * 10) % 2 == 0));

if (Blink) return;


    glBegin(GL_QUADS);
        glVertex2f(-0.03f, -0.85f);
        glVertex2f( 0.03f, -0.85f);
        glVertex2f( 0.03f, -0.65f);
        glVertex2f(-0.03f, -0.65f);
    glEnd();

    if (invulnerable && (static_cast<int>(invulnerableTimer * 10) % 2 == 0)) {
        glColor4f(0.1f, 0.1f, 0.3f, 0.5f);
    } else {
        glColor3f(0.1f, 0.2f, 0.3f);
    }
    glBegin(GL_QUADS);
        glVertex2f(-0.025f, -0.67f);
        glVertex2f( 0.025f, -0.67f);
        glVertex2f( 0.02f, -0.63f);
        glVertex2f(-0.02f, -0.63f);
    glEnd();

    if (invulnerable && (static_cast<int>(invulnerableTimer * 10) % 2 == 0)) {
        glColor4f(0.6f, 0.6f, 0.6f, 0.5f);
    } else {
        glColor3f(0.6f, 0.6f, 0.6f);
    }
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.03f, -0.65f);
        glVertex2f( 0.03f, -0.65f);
        glVertex2f( 0.0f, -0.60f);
    glEnd();

    if (invulnerable && (static_cast<int>(invulnerableTimer * 10) % 2 == 0)) {
        glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    } else {
        glColor3f(0.5f, 0.5f, 0.5f);
    }
    glBegin(GL_QUADS);
        glVertex2f(-0.15f, -0.75f);
        glVertex2f( 0.15f, -0.75f);
        glVertex2f( 0.08f, -0.70f);
        glVertex2f(-0.08f, -0.70f);
    glEnd();

    if (invulnerable && (static_cast<int>(invulnerableTimer * 10) % 2 == 0)) {
        glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
    } else {
        glColor3f(0.5f, 0.5f, 0.5f);
    }
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.02f, -0.75f);
        glVertex2f( 0.02f, -0.75f);
        glVertex2f( 0.0f, -0.95f);
    glEnd();

    if (invulnerable && (static_cast<int>(invulnerableTimer * 10) % 2 == 0)) {
        glColor4f(0.3f, 0.3f, 0.3f, 0.5f);
    } else {
        glColor3f(0.3f, 0.3f, 0.3f);
    }
    glBegin(GL_QUADS);
        glVertex2f(-0.02f, -0.85f);
        glVertex2f(-0.01f, -0.85f);
        glVertex2f(-0.01f, -0.83f);
        glVertex2f(-0.02f, -0.83f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(0.02f, -0.85f);
        glVertex2f(0.01f, -0.85f);
        glVertex2f(0.01f, -0.83f);
        glVertex2f(0.02f, -0.83f);
    glEnd();

    if (invulnerable && (static_cast<int>(invulnerableTimer * 10) % 2 == 0)) {
        glColor4f(1.0f, 0.5f, 0.0f, 0.5f);
    } else {
        glColor3f(1.0f, 0.5f, 0.0f);
    }
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.02f, -0.85f);
        glVertex2f(-0.01f, -0.85f);
        glVertex2f(-0.015f, -0.89f);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(0.02f, -0.85f);
        glVertex2f(0.01f, -0.85f);
        glVertex2f(0.015f, -0.89f);
    glEnd();

    if (invulnerable && (static_cast<int>(invulnerableTimer * 10) % 2 == 0)) {
        glColor4f(0.8f, 0.8f, 0.8f, 0.5f);
    } else {
        glColor3f(0.8f, 0.8f, 0.8f);
    }
    glBegin(GL_QUADS);
        glVertex2f(-0.12f, -0.76f);
        glVertex2f(-0.10f, -0.76f);
        glVertex2f(-0.10f, -0.74f);
        glVertex2f(-0.12f, -0.74f);

        glVertex2f(0.12f, -0.76f);
        glVertex2f(0.10f, -0.76f);
        glVertex2f(0.10f, -0.74f);
        glVertex2f(0.12f, -0.74f);
    glEnd();
}

void drawAirplaneMoved() {
    glPushMatrix();
    glTranslatef(jetX, jetY, 0.0f);
    drawAirplane();
    glPopMatrix();
}
void drawObstaclePlane(float xOffset, float phaseOffset) {
    glPushMatrix();

    float yPos = _obstacleY + phaseOffset;
    if (yPos < -1.5f) yPos += 2.5f;
    if (yPos > 1.0f) yPos -= 2.5f;

    glTranslatef(xOffset, yPos, 0.0f);

    // --- Enemy plane body ---

    if (enemySlowdown)
        glColor3f(0.4f, 0.4f, 0.4f); // or another slowed color
    else glColor3f(0.8f, 0.2f, 0.2f);

    glBegin(GL_QUADS);
        glVertex2f(-0.04f, 0.0f);
        glVertex2f( 0.04f, 0.0f);
        glVertex2f( 0.04f, -0.15f);
        glVertex2f(-0.04f, -0.15f);
    glEnd();


    glColor3f(0.1f, 0.1f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(-0.03f, -0.02f);
        glVertex2f( 0.03f, -0.02f);
        glVertex2f( 0.02f, -0.08f);
        glVertex2f(-0.02f, -0.08f);
    glEnd();

    if (enemySlowdown)
        glColor3f(0.4f, 0.4f, 0.2f); // or another slowed color
    else glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.04f, -0.15f);
        glVertex2f( 0.04f, -0.15f);
        glVertex2f( 0.0f, -0.22f);
    glEnd();
    if (enemySlowdown)
        glColor3f(0.4f, 0.4f, 0.1f); // or another slowed color
    else glColor3f(0.7f, 0.2f, 0.2f);

    glBegin(GL_QUADS);
        glVertex2f(-0.12f, -0.08f);
        glVertex2f( 0.12f, -0.08f);
        glVertex2f( 0.08f, -0.12f);
        glVertex2f(-0.08f, -0.12f);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(-0.02f, 0.0f);
        glVertex2f( 0.02f, 0.0f);
        glVertex2f( 0.0f, 0.08f);
    glEnd();

    // --- Single Fuel Tank beside the plane (right side, bigger, slower) ---

    glPopMatrix(); // end plane
}
// ---------------- Fuel Can ----------------
void drawFuelCan(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    // Body (red can)
    glColor3f(0.9f, 0.2f, 0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.2f, -0.3f);
        glVertex2f( 0.2f, -0.3f);
        glVertex2f( 0.2f,  0.2f);
        glVertex2f( 0.1f,  0.35f);
        glVertex2f(-0.2f,  0.35f);
    glEnd();



    // Handle (black rectangle cutout)
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(-0.05f, 0.15f);
        glVertex2f( 0.12f, 0.15f);
        glVertex2f( 0.12f, 0.25f);
        glVertex2f(-0.05f, 0.25f);
    glEnd();

    glPopMatrix();
}

//Speed Icon

void drawSpeedMeter(float x, float y, float scale)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    glColor3f(0.3f, 0.4f, 0.6f);   // bluish color

    // --- lower chevron ---
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.5f, -0.5f);
        glVertex2f( 0.0f,  0.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f,  0.0f);
        glVertex2f( 0.5f,  0.0f);
        glVertex2f( 0.0f,  0.5f);
    glEnd();

    // --- upper chevron (shifted up) ---
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, 0.5f);
        glVertex2f( 0.5f, 0.5f);
        glVertex2f( 0.0f, 1.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, 1.0f);
        glVertex2f( 0.5f, 1.0f);
        glVertex2f( 0.0f, 1.5f);
    glEnd();

    glPopMatrix();
}

void drawMiniRocket(float x, float y) {
     if (!rocketVisible) return;  // blink off

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(180.0f,0.0f,0.0f,1.0);
    glScalef(0.15f, 0.15f, 1.0f);   // a bit larger & taller

    // ==== Body (metallic silver/blue) ====
    glColor3f(0.7f, 0.8f, 0.9f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.12f, -0.45f);
        glVertex2f( 0.12f, -0.45f);
        glVertex2f( 0.12f,  0.45f);
        glVertex2f(-0.12f,  0.45f);
    glEnd();

    // ==== Nose cone (bright red) ====
    glColor3f(0.9f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.12f, 0.45f);
        glVertex2f( 0.12f, 0.45f);
        glVertex2f( 0.0f,  0.75f);
    glEnd();

    // ==== Side fins (orange) ====
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.12f,  0.05f);
        glVertex2f(-0.30f, -0.15f);
        glVertex2f(-0.12f, -0.25f);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f( 0.12f,  0.05f);
        glVertex2f( 0.30f, -0.15f);
        glVertex2f( 0.12f, -0.25f);
    glEnd();

    // ==== Tail section (dark steel) ====
    glColor3f(0.3f, 0.3f, 0.4f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.12f, -0.45f);
        glVertex2f( 0.12f, -0.45f);
        glVertex2f( 0.15f, -0.60f);
        glVertex2f(-0.15f, -0.60f);
    glEnd();

    // ==== Flame (bright yellow/orange) ====
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.06f, -0.60f);
        glVertex2f( 0.06f, -0.60f);
        glVertex2f( 0.0f,  -0.90f);
    glEnd();
    glColor3f(1.0f, 0.4f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.04f, -0.60f);
        glVertex2f( 0.04f, -0.60f);
        glVertex2f( 0.0f,  -1.05f);
    glEnd();

    glPopMatrix();
}

void resetGame() {
    // Reset game variables
    gameOver = false;
    score = 0;
    lives = 3;
    jetX = 0.0f;
    jetY = 0.0f;

    _moveC = 0.0f;
    _obstacleY = 1.0f;
    lastObstacleReset = 0;

    invulnerable = false;
    invulnerableTimer = 0.0f;

    speedBoost = false;
    speedBoostTimer = 0.0f;
    speedMeterY = 0.9f;

    enemySlowdown = false;
    enemySlowdownTimer = 0.0f;

    // reset fuel positions
    fuelX[0] = 0.5f;  fuelY[0] = -0.2f;
    fuelX[1] = -0.4f; fuelY[1] =  0.4f;

    // reset rocket
    rocketX = 0.4f;
    rocketY = 1.0f;
    rocketVisible = true;
    rocketBlinkTimer = 0.0f;
    glutPostRedisplay();
}


void sound()
{

    //PlaySound("a.wav", NULL, SND_ASYNC|SND_FILENAME);
    PlaySound("videoplayback", NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);

}



// --- Collision Detection ---
bool checkCollision(float jetX, float jetY, float obsX, float obsY) {
    float jetLeft = jetX - 0.15f;
    float jetRight = jetX + 0.15f;
    float jetTop = jetY - 0.6f;
    float jetBottom = jetY - 0.95f;

    float obsLeft = obsX - 0.12f;
    float obsRight = obsX + 0.12f;
    float obsTop = obsY;
    float obsBottom = obsY - 0.22f;

    if (jetRight < obsLeft || jetLeft > obsRight ||
        jetBottom > obsTop || jetTop < obsBottom) {
        return false;
    }

    return true;
}

void drawScore() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.9f, 0.9f);

    char scoreText[20];
    std::sprintf(scoreText, "Score: %d", score);
    for (int i = 0; scoreText[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    }
}

void drawLives() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(0.7f, 0.9f);

    char livesText[20];
    std::sprintf(livesText, "Lives: %d", lives);
    for (int i = 0; livesText[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, livesText[i]);
    }
}
void drawslowdown() {
    if (enemySlowdown) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.2f, 0.9f);

        char slowdownText[30];
        // Display remaining time with 2 decimal places
        std::sprintf(slowdownText, "Slowdown: %.05f s", enemySlowdownTimer);
        for (int i = 0; slowdownText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, slowdownText[i]);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.15f, 0.0f);
        const char* msg = "GAME OVER!";
        for (int i = 0; msg[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
        }

        // Display final score in game over screen
        glRasterPos2f(-0.15f, -0.1f);
        char finalScore[30];
        std::sprintf(finalScore, "FINAL SCORE: %d", score);

        for (int i = 0; finalScore[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, finalScore[i]);
        }
        //Press R or r to play again
        glRasterPos2f(-0.15f, -0.2f);
        char returnagain[30];
        std::sprintf(returnagain, "PRESS R OR r TO PLAY AGAIN", playagain);

        for (int i = 0; returnagain[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, returnagain[i]);
        }
        //Press R or r to play again
        glRasterPos2f(-0.15f, -0.3f);
        char exitgame[30];
        std::sprintf(exitgame, "PRESS X TO END THE GAME", x);

        for (int i = 0; exitgame[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, exitgame[i]);
        }

        glutSwapBuffers();
        return;
    }

    glColor3f(0.5f, 0.8f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
    glEnd();

    drawSun(0.8f, 0.8f, 0.15f);
    drawCloud(-0.2f + _moveC, 0.0f, -0.2f);
    drawAirplaneMoved();

    drawObstaclePlane(0.0f, 0.0f);
    drawObstaclePlane(-0.6f, 0.8f);
    drawObstaclePlane(0.7f, 1.5f);

    drawMiniRocket(rocketX, rocketY);

    for (int i = 0; i < 2; i++) {
    drawFuelCan(fuelX[i], fuelY[i], 0.2f);
}

drawSpeedMeter(0.25f, speedMeterY, 0.10f);



    // Draw the score and lives
    drawScore();
    drawLives();
    drawslowdown();

    glutSwapBuffers();
}

void update(int value) {

    glutTimerFunc(15, update, 0);
    if (gameOver) return;

    // Update invulnerability timer
    if (invulnerable) {
        invulnerableTimer -= 0.020f; // 15ms per frame
        if (invulnerableTimer <= 0.0f) {
            invulnerable = false;
        }
    }
    if (speedBoost) {
        speedBoostTimer -= 0.015f; // same frame time step as elsewhere
        if (speedBoostTimer <= 0.0f) {
            speedBoost = false;     // boost ends
        }
    }

    float y0 = _obstacleY;
    float y1 = _obstacleY + 0.8f;
    float y2 = _obstacleY + 1.5f;

    if (y1 < -1.5f) y1 += 2.5f;
    if (y2 < -1.5f) y2 += 2.5f;
    if (y1 > 1.0f) y1 -= 2.5f;
    if (y2 > 1.0f) y2 -= 2.5f;

    // Check for collision only if not invulnerable
    if (!invulnerable &&
        (checkCollision(jetX, jetY, 0.0f, y0) ||
         checkCollision(jetX, jetY, -0.6f, y1) ||
         checkCollision(jetX, jetY, 0.7f, y2))) {

        lives--; // Decrease lives

        if (lives <= 0) {
            gameOver = true;
        } else {
            // Make player invulnerable for a short time after hit
            invulnerable = true;
            invulnerableTimer = INVULNERABLE_TIME;
        }

        glutPostRedisplay();
    }

    // Increment score based on time
    score += 1;

    // Bonus points when obstacles complete a full cycle
    if (_obstacleY < -1.4f && lastObstacleReset >= 0) {
        score += 50;
        lastObstacleReset = -1;
    }

    if (_obstacleY > 0.9f) {
        lastObstacleReset = 1;
    }

    _moveC += 0.002f;
    if (_moveC > 1.0f) _moveC = -1.0f;

   // Update obstacle speed depending on slowdown
float currentObstacleSpeed = obstacleSpeed;
if (enemySlowdown) currentObstacleSpeed *= 0.5f; // slow down to half speed

_obstacleY -= currentObstacleSpeed;

// Update slowdown timer
if (enemySlowdown) {
    enemySlowdownTimer -= 0.015f;
    if (enemySlowdownTimer <= 0.0f) {
        enemySlowdown = false;
    }
}

    if (_obstacleY < -1.5f) _obstacleY = 1.0f;

   // Move speed-meter downward
speedMeterY -= speedMeterSpeed;

// When it goes off the bottom, reset to top
if (speedMeterY < -1.2f) {
    speedMeterY = 1.0f;   // or whatever top value you prefer
}
// --- Speed-meter collision ---
{
    float jetLeft   = jetX - 0.03f;
    float jetRight  = jetX + 0.03f;
    float jetTop    = jetY - 0.63f;
    float jetBottom = jetY - 0.85f;

    float smLeft   = 0.25f - 0.05f;          // speed-meter approx half-width
    float smRight  = 0.25f + 0.05f;
    float smTop    = speedMeterY + 0.15f;    // adjust to icon size
    float smBottom = speedMeterY - 0.15f;

    if (!speedBoost && !(jetRight < smLeft || jetLeft > smRight ||
                         jetBottom > smTop || jetTop < smBottom))
    {
        // Collision detected: start boost
        speedBoost = true;
        speedBoostTimer = SPEED_BOOST_TIME;

        // reset speed-meter back to the top so it doesn’t instantly collide again
        speedMeterY = 1.0f;
    }
}
glutPostRedisplay();


    // Move fuels down
for (int i = 0; i < 2; i++) {
    fuelY[i] -= fuelSpeed;
    if (fuelY[i] < -1.2f) {  // reset fuel to top when off screen
        fuelY[i] = 1.0f;
    }
}


// Check for collision with jet (precise)
for (int i = 0; i < 2; i++) {
    // Jet bounding box (same as obstacle collision)
    float jetLeft = jetX - 0.03f;
    float jetRight = jetX + 0.03f;
    float jetTop = jetY - 0.63f;
    float jetBottom = jetY - 0.85f;

    // Fuel bounding box (approximate fuel can size)
    float fuelLeft = fuelX[i] - 0.1f;
    float fuelRight = fuelX[i] + 0.1f;
    float fuelTop = fuelY[i] + 0.15f;
    float fuelBottom = fuelY[i] - 0.15f;

    if (!(jetRight < fuelLeft || jetLeft > fuelRight ||
          jetBottom > fuelTop || jetTop < fuelBottom)) {
        // Collision detected
        score += 100;  // bonus points
        fuelY[i] = 2.0f; // reset fuel

        // Activate enemy slowdown
        enemySlowdown = true;
        enemySlowdownTimer = ENEMY_SLOWDOWN_TIME;
    }
}
// --- Mini Rocket movement ---
rocketY -= rocketSpeed;
if (rocketY < -1.2f) {
    rocketY = 1.2f; // reset to top
    // randomize horizontal position for variety
    rocketX = ((rand() % 160) / 100.0f) - 0.8f; // roughly -0.8 to 0.8
}

// --- Blink logic: toggle visibility about every 0.2 sec ---
rocketBlinkTimer += 0.015f;
if (rocketBlinkTimer >= 0.2f) {
    rocketVisible = !rocketVisible;
    rocketBlinkTimer = 0.0f;
}
// Jet bounding box
float jetLeft   = jetX - 0.03f;
float jetRight  = jetX + 0.03f;
float jetTop    = jetY - 0.63f;
float jetBottom = jetY - 0.85f;

// Rocket bounding box (matching scale of drawMiniRocket)
float rLeft   = rocketX - 0.075f;
float rRight  = rocketX + 0.075f;
float rTop    = rocketY + 0.6f * 0.15f;   // scaled height
float rBottom = rocketY - 0.6f * 0.15f;

if (!invulnerable && !(jetRight < rLeft || jetLeft > rRight ||
                       jetBottom > rTop || jetTop < rBottom))
{
    lives--;
    if (lives <= 0) {
        gameOver = true;
    } else {
        invulnerable = true;
        invulnerableTimer = INVULNERABLE_TIME;
    }
}

}
void handleNormalKey(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        resetGame();
    }
    else if (key == 'x' || key == 'X') {
        // Clean exit from the application
        exit(0);
    }
}



void handleSpecialKey(int key, int x, int y) {

    if (gameOver) return;
    float moveAmount = speedBoost ? BOOST_MOVE_AMOUNT : NORMAL_MOVE_AMOUNT;


    switch (key) {
        case GLUT_KEY_LEFT: jetX -= moveAmount; break;
        case GLUT_KEY_RIGHT: jetX += moveAmount; break;
        case GLUT_KEY_UP: jetY += moveAmount; break;
        case GLUT_KEY_DOWN: jetY -= moveAmount; break;
    }

    if (jetX < -0.85f) jetX = -0.85f;
    if (jetX > 0.85f) jetX = 0.85f;
    if (jetY < -0.95f) jetY = -0.95f;
    if (jetY > 0.85f) jetY = 0.85f;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("AirPlane Crasher");
    glutDisplayFunc(display);
    glutSpecialFunc(handleSpecialKey);
    glutKeyboardFunc(handleNormalKey);
    sound();
    glutTimerFunc(15, update, 0);
    glutMainLoop();
    return 0;
}
