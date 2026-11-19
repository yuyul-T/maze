#include <raylib.h>
#include <format>
#include<format>
#include "maze.h"
#include "player.h"
#include"pathfind.h"

bool showPath=false;
std::vector<Point> path;

enum GameState
{
    STATE_START,//开始界面
    STATE_PLAY,//游戏中
    STATE_WIN//胜利界面
};

int main(){
    InitWindow(1280, 1280, "HBUT 程序设计综合实践项目：maze");
    Maze maze("../assets/maze0.txt");

    GameState currentState=STATE_START;

    Player player(maze);

    while (!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch(currentState)
        {
            case STATE_START://游戏开始界面
            {
                DrawText("MAZE GAME",450,400,80,DARKGRAY);
                DrawText("Press SPACE to start",420,520,36,BLACK);

                if(IsKeyPressed(KEY_SPACE))
                {
                    //按下空格->进入游玩状态
                    currentState=STATE_PLAY;
                    showPath=false;
                    path.clear();
                }
            }
            break;

            case STATE_PLAY:
            {
                //游戏游玩逻辑
                player.update();

                if(IsKeyPressed(KEY_SPACE))
                {
                    showPath=!showPath;
                    if(showPath)
                    {
                        Coordinate playerPos=player.get_curr_coord();
                        Coordinate endPos=maze.get_end_coord();
                        path=BFSFindPath(maze,Point(playerPos.y,playerPos.x),Point(endPos.y,endPos.x));
                    }
                }

                maze.draw();
                player.draw();

                //绘制路径：画小圆点标记路径
                if(showPath)
                {
                    for(Point& pt:path)
                    {
                        float px=pt.c*TILE_WIDTH+TILE_WIDTH/2.0f;
                        float py=pt.r*TILE_HEIGHT+TILE_HEIGHT/2.0f;
                        DrawCircle(px,py,12,BLACK);
                    }
                }
            
                DrawText(std::format("FPS: {}", GetFPS()).c_str(), 5, 5, 20, WHITE);
                DrawText("SPACE:Toggle Path",5,30,20,BLACK);
                //DrawText("Use arrow keys to move", 1300, 600, 24, BLACK);

                Coordinate pCoord = player.get_curr_coord();
                Coordinate eCoord = maze.get_end_coord();
                if(pCoord.x==eCoord.x && pCoord.y==eCoord.y)
                {
                    currentState=STATE_WIN;
                }
            }
            break;

            case STATE_WIN:
            {
                //胜利界面
                DrawText("恭喜你通关！",400,420,72,GREEN);
                DrawText("按下空格键返回开始界面",410,530,36,BLACK);

                if(IsKeyPressed(KEY_SPACE))
                {
                    currentState=STATE_START;
                }
            }
            break;
        }

        EndDrawing();
    }
    CloseWindow(); 
    return 0;
}