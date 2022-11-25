#include <iostream>
#include "raylib.h"
#include <ctime>

using namespace std;

class Player
{
    public:
    float x = 250;
    float y = 350;
    double jumpPower = 20;
    int velocity = 0;
    int gravity = 1;
    float width = 50;
    float height = 50;
    bool playJumpWav = false;

    void move()
    {
        if(IsKeyDown(KEY_SPACE) && y == 350 /* is on ground */)
        {
            y -= jumpPower;
            velocity -= jumpPower;
            playJumpWav = true;
        }

    }

    void update()
    {
        if(y < 350){
            velocity += gravity;
        }
        if(y > 350){
            velocity = 0;
            y = 350;
        }
        y += velocity;

        move();
    }
};

class Obstacle
{
    public:
    float x = 800; //800
    float y = 360; //400
    double speed = 8;
    double maxspeed = 15;
    float width = 40;
    float height = 40;

    void update()
    {
        x -= speed;

        if(x < -50)
        {
            x = 800;
        }
    }
};
/*
bool isColliding(Player object1, Obstacle object2)
{   //(object1.bottom_y >= object2.top_y) && (object1.top_y <= object2.bottom_y) && (object1.right_x >= object2.left_x) && (object1.left_x <= object2.right_x)
    if ((object1.bottom_y >= object2.top_y) && (object1.top_y <= object2.bottom_y) && (object1.right_x >= object2.left_x)){
        return true;
    }
    return false;
} */

int main()
{
    int window_width = 800;
    int window_height = 400;
    
    Player player;
    Obstacle obstacle1;

    int speed_starttime = time(NULL);
    int score_starttime = time(NULL);
    
    InitWindow(window_width, window_height, "GAME");
    InitAudioDevice();      

    //void SetWindowIcon(Image image);

    bool GameOver = false;

    int score = 0;

    Sound jumpwav = LoadSound("sfx/jump.wav");
    Sound deathwav = LoadSound("sfx/death.wav");

    bool deathAudioPlayed = false;


    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        if(!GameOver){
            DrawRectangle(player.x, player.y, player.width, player.height, GREEN);
            DrawRectangle(obstacle1.x, obstacle1.y, 40, 40, RED);
        }

        Rectangle p = {player.x, player.y, player.width, player.height};
        Rectangle e = {obstacle1.x, obstacle1.y, obstacle1.width, obstacle1.height};

        if(CheckCollisionRecs(p, e) || GameOver){
            obstacle1.speed = 0;
            GameOver = true;
            DrawText("Game Over", window_width/2 - (MeasureText("Game Over", 50)/2), 100, 50, RED);
        }

        if(GameOver && !deathAudioPlayed){
            PlaySound(deathwav);
            deathAudioPlayed = true;
        }
        if(player.playJumpWav){
            PlaySound(jumpwav);
            player.playJumpWav = false;
        }

        player.update();
        obstacle1.update();

        int speed_currenttime = time(NULL);
        int score_currectime = time(NULL);

        if(speed_currenttime > speed_starttime + 10 && obstacle1.speed <= obstacle1.maxspeed){
            obstacle1.speed += 1;
            speed_starttime = speed_currenttime;
        }
        if(score_currectime > score_starttime + 1 && !GameOver){
            score += 100;
            score_starttime = score_currectime;
        }

        //Score
        const char *scoretext = to_string(score).c_str();
        DrawText(scoretext, window_width/2 - (MeasureText(scoretext, 30)/2), 30, 30, RED);

        //cout << player.velocity;
        //cout << obstacle1.speed;
        cout << speed_currenttime - speed_starttime;

        EndDrawing();
    }

    UnloadSound(jumpwav);
    UnloadSound(deathwav);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}