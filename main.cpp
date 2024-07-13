#include <iostream>
#include <raylib.h>

using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball {
    public:
    float x, y;
    int speedx, speedy;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        x += speedx;
        y += speedy;

        if(y + radius >= GetScreenHeight() || y - radius <= 0) {
            speedy *= -1;
        }

        if(x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }
        
        if(x - radius <= 0) {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
        
        int speed_choices[2] = {-1, 1};
        speedx *= speed_choices[GetRandomValue(0, 1)];
        speedy *= speed_choices[GetRandomValue(0, 1)];
    }
};


class Paddle {
    protected:
    void LimitMovement() {
        if(y <= 0) {
            y = 0;
        }

        if(y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

    public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update() {
        if(IsKeyDown(KEY_UP)) {
            y = y - speed;
        }

        if(IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }

        LimitMovement();

    }
};


class CPUPaddle: public Paddle {
    public:
    void Update(int bally) {
        if(y + height/2 > bally) {
            y = y - speed;
        }

        if(y + height/2 <= bally) {
            y = y + speed;
        }

        LimitMovement();
    }
};


Ball ball;
Paddle player;
CPUPaddle cpu;

int main () {

    cout << "Starting the game..." << endl;

    // Initialize the game window
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speedx = 7;
    ball.speedy = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2; 
    player.speed = 6;

    cpu.height = 120;
    cpu. width = 25;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;

    while(WindowShouldClose() == false) {

        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speedx *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speedx *= -1;
        }

        BeginDrawing();

        // Clear the screen
        ClearBackground(BLACK);

        // Draw a line in the center that seperates the players
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);

        // Draw the ball
        ball.Draw();

        // Draw the CPU paddle
        cpu.Draw();

        // Draw the player paddle
        player.Draw();

        // Draw the scores
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width/4 - 20, 20, 80, WHITE);

        EndDrawing();

    }

    //Close the game window
    CloseWindow();
    return 0;
}