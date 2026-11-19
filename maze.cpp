#include "maze.h"
#include <fstream>
#include <iostream>
#include <utility>

/*
    将所有迷宫图片读入显存
*/
void Maze::load_textures(){
    Image end_image = LoadImage("../assets/end.png");//从磁盘中读取end.png，加载成image
    textures[END] = LoadTextureFromImage(end_image);
    SetTextureFilter(textures[END], TEXTURE_FILTER_POINT);
    SetTextureWrap(textures[END], TEXTURE_WRAP_CLAMP);//贴图超出范围时候拉伸边缘像素，不重复平铺
    UnloadImage(end_image);//Image已经转成Texture了，内存里的原图可以释放，节省内存

    Image start_image = LoadImage("../assets/start.png");
    textures[START] = LoadTextureFromImage(start_image);
    SetTextureFilter(textures[START] , TEXTURE_FILTER_POINT);
    SetTextureWrap(textures[START] , TEXTURE_WRAP_CLAMP);
    UnloadImage(start_image);

    Image floor_image = LoadImage("../assets/floor.png");
    textures[FLOOR] = LoadTextureFromImage(floor_image);
    SetTextureFilter(textures[FLOOR], TEXTURE_FILTER_POINT);
    SetTextureWrap(textures[FLOOR], TEXTURE_WRAP_CLAMP);
    UnloadImage(floor_image);

    Image wall_image = LoadImage("../assets/wall.png");
    textures[WALL] = LoadTextureFromImage(wall_image);
    SetTextureFilter(textures[WALL], TEXTURE_FILTER_POINT);
    SetTextureWrap(textures[WALL], TEXTURE_WRAP_CLAMP);
    UnloadImage(wall_image);

    Image grass_image = LoadImage("../assets/grass.png");
    textures[GRASS] = LoadTextureFromImage(grass_image);
    SetTextureFilter(textures[GRASS], TEXTURE_FILTER_POINT);
    SetTextureWrap(textures[GRASS], TEXTURE_WRAP_CLAMP);
    UnloadImage(grass_image);

    Image lava_image = LoadImage("../assets/lava.png");
    textures[LAVA] = LoadTextureFromImage(lava_image);
    SetTextureFilter(textures[LAVA], TEXTURE_FILTER_POINT);
    SetTextureWrap(textures[LAVA], TEXTURE_WRAP_CLAMP);
    UnloadImage(lava_image);
}

/*
    读取迷宫文件
    参数：maze_file：迷宫文本文件
*/
void Maze::load_maze(const std::string& maze_file){
    std::ifstream ifs(maze_file);
    if(!ifs.is_open()){
        std::cerr<<"Failed to open "<< maze_file <<std::endl;
        exit(1);
    }

    int row, column;
    ifs>>row>>column;//读取文件中迷宫一共多少行，多少列

    //先循环每一列，在循环每一行
    for(int r{0}; r<row; ++r){
        std::vector<Tile> tile_row;//保存这一行的所有格子
        for(int c{0}; c<column; ++c){
            int value;
            ifs>>value;
            TileType type{static_cast<TileType>(value)};
            tile_row.push_back({
                .type = type,
                .position = {c*TILE_WIDTH, r*TILE_HEIGHT}
            });
            if (type == START){
                start_coord = {c, r};
            }
            if (type == END){
                end_coord = {c, r};
            }
        }
        tiles.push_back(std::move(tile_row));
    }
}

Maze::Maze(const std::string& maze_file){
    load_textures();
    load_maze(maze_file);
}

Maze::~Maze(){
    //将所有迷宫图片从显存中删除
    for(std::pair<TileType,Texture2D> item: textures){
        UnloadTexture(item.second);
    }
}

/*
    在屏幕上绘制迷宫
*/
void Maze::draw() {
    for(const std::vector<Tile>& row : tiles){
        for(const Tile& tile : row){//遍历每一个格子
        std::unordered_map<TileType,Texture2D>::iterator texture_it = textures.find(tile.type);

            if(texture_it == textures.end()) continue;
            Texture2D tex = texture_it->second;

            Rectangle src = {
                0.5f,
                0.5f,
                tex.width - 1.0f,
                tex.height - 1.0f
            };
            Rectangle dest = {
                tile.position.x,
                tile.position.y,
                TILE_WIDTH,
                TILE_HEIGHT
            };
            DrawTexturePro(tex, src, dest, {0.0f,0.0f}, 0.0f, WHITE);
            //贴图绘制函数，参数分别为贴图、源截取矩形、目标位置矩形、旋转原点、旋转角度、颜色调色
        }
    }
}



//获得起点坐标
Coordinate Maze::get_start_coord() const{
    return start_coord;
}

/*
    获得地块类型
    参数coor：地块坐标
    返回值：coor位置地块的类型
*/
TileType Maze::get_tile_type(const Coordinate coor) const{
    return tiles[coor.y][coor.x].type;
}

/*
    获得地块的屏幕位置
    参数coor：地块坐标
    返回值：coor位置地块的屏幕显示位置
*/
Vector2 Maze::get_tile_position(const Coordinate coor) const{
    return tiles[coor.y][coor.x].position;
}