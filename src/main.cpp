#include <raylib.h>

#include <iostream>

using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball {
   public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth()) {
            player_score++;
            ResetBall();
        }
        if (x - radius <= 0) {
            cpu_score++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
   protected:
    void LimitMovement() {
        // Collision detection
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
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
        // Movement
        if (IsKeyDown(KEY_UP)) {
            y -= speed;
        }

        if (IsKeyDown(KEY_DOWN)) {
            y += speed;
        }

        LimitMovement();
    }
};

class CpuPaddle : public Paddle {
   public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y) {
            y = y + speed;
        }

        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main() {
    cout << "Starting the game";
    const int screen_width = 640;
    const int screen_height = 400;

    InitWindow(screen_width, screen_height, "My Pong Game");
    SetTargetFPS(60);

    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.radius = screen_width / 100;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = screen_width / 51;
    player.height = screen_height / 6;
    player.x = 10;
    player.y = screen_height / 2 - 60;
    player.speed = 6;

    cpu.width = screen_width / 51;
    cpu.height = screen_height / 6;
    cpu.x = screen_width - 35;
    cpu.y = screen_height / 2 - 60;
    cpu.speed = 6;

    while (WindowShouldClose() == false) {
        BeginDrawing();

        // Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
        }
        // Drawing
        ClearBackground(RED);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}