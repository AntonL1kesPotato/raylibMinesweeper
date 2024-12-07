#include "raylib.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;
const int tileNumX = 16;
const int tileNumY = 16;
const int mineNum = 40;
int tileMap[tileNumY][tileNumX] = { 0 };
int clicks = 0;
int flags[tileNumX*tileNumY][2];
int flagNum;
double gameStart = 0;
double currGameTime;
void checkSurrounding(int (&plMap)[tileNumY][tileNumX], int tileMap[tileNumY][tileNumX], int chkX, int chkY){
    int mineCounter = 0;
    
    
    for(int x=-1; x<2; x++){
        
        for(int y=-1; y<2; y++){
            
            int ty = chkY+y;
            int tx = chkX+x;
            
            if ( !(ty < 0 || ty > tileNumY-1 || tx < 0 || tx > tileNumX-1)) {
                int scannedTileValue = tileMap[ty][tx];
                if (scannedTileValue == 1){
                    mineCounter++;
                }
            }


        }
    }
    if (mineCounter == 0){
        plMap[chkY][chkX] = -3; // -3 = verified empty
        for(int x=-1; x<2; x++) { for(int y=-1; y<2; y++){
            int ty = chkY+y;
            int tx = chkX+x;
            
            if ( !(ty < 0 || ty > tileNumY-1 || tx < 0 || tx > tileNumX-1) ) {
            if (plMap[ty][tx]==0 ){
                checkSurrounding(plMap,tileMap, tx, ty);
                }
            }
            } }
    }
    else {
        plMap[chkY][chkX] = mineCounter;
    }
    
}

float clip(float n, float lower, float upper) {
  return max(lower, min(n, upper));
}

void checkSurroundingFirstTime(int (&plMap)[tileNumY][tileNumX], int (&tileMap)[tileNumY][tileNumX], int chkX, int chkY, int (&minePos)[mineNum][2]){
    int mineCounter = 0;
    int randX;
    int randY;
    int mineIdx;
    
    for(int x=-1; x<2; x++){
        
        for(int y=-1; y<2; y++){
            
            int ty = chkY+y;
            int tx = chkX+x;
            
            if ( !(ty < 0 || ty > tileNumY-1 || tx < 0 || tx > tileNumX-1)) {
                int scannedTileValue = tileMap[ty][tx];
                if (scannedTileValue == 1){
                    
                    //find mine idx

                    for (int i=0; i<mineNum; i++){
                        if (minePos[i][0] == ty && minePos[i][1] == tx) {
                            mineIdx = i;
                            break;
                        }
                    }


                    tileMap[ty][tx] = 0;
                    do{
                        randX = rand()%tileNumX;
                        randY = rand()%tileNumY;
                    } while (tileMap[randY][randX] != 0);
                    tileMap[randY][randX] = 1;
                    minePos[mineIdx][0] = randY;
                    minePos[mineIdx][1] = randX;
                }
            }


        }
    }
    

    if (mineCounter == 0){
        plMap[chkY][chkX] = -3; // -3 = verified empty
        for(int x=-1; x<2; x++) { for(int y=-1; y<2; y++){
            int ty = chkY+y;
            int tx = chkX+x;
            
            if ( !(ty < 0 || ty > tileNumY-1 || tx < 0 || tx > tileNumX-1) ) {
            if (plMap[ty][tx]==0 ){
                checkSurrounding(plMap,tileMap, tx, ty);
                }
            }
            } }
    }
    
}

void setMines(int (&tileMap)[tileNumY][tileNumX], int (&minePos)[mineNum][2]){
    for (int i=0;i<mineNum;i++){
        int randX = rand()%tileNumX;
        int randY = rand()%tileNumY;

        do{
            randX = rand()%tileNumX;
            randY = rand()%tileNumY;
            //cout << i << (tileMap[randY][randX] != 0) << endl;
        }
        while(tileMap[randY][randX] == 1);
        
        tileMap[randY][randX] = 1;
        minePos[i][0] = randY;
        minePos[i][1] = randX;
        
    }
}
int main(void)
{
    bool gameReset = true;
    //Font fontTtf = LoadFontEx("gontserrat/Gontserrat-Regular.ttf", 32, 0, 250);
    const int tileSize = 32;
    tileMap[tileNumY][tileNumX] = { 0 };
    int playerMap[tileNumY][tileNumX] = { 0 };
    int minePos[mineNum][2] = {0};
    
    int gameState = 0; //0=unfinished, 1=win, -1=lose
    int HUDSize = tileSize*4;
    int bezel = tileSize;
    const int screenWidth  = tileNumX * tileSize + bezel*2;
    const int screenHeight = tileNumY * tileSize + HUDSize;
    srand(time(0)); 
    InitWindow(screenWidth, screenHeight, "raylib [audio] example - music playing (streaming)");
    SetTargetFPS(60);      
    int counter = 0;
    Texture2D flagImg = LoadTexture("img/flag.png");
    Texture2D mineImg = LoadTexture("img/mine.png");
    Texture2D xImg = LoadTexture("img/no.png");
    Texture2D faceded = LoadTexture("img/facedead.png");
    Texture2D facenormal = LoadTexture("img/facenormal.png");
    Texture2D facewin = LoadTexture("img/facewin.png");
    Texture2D faceclick = LoadTexture("img/faceclick.png");
    
    while (!WindowShouldClose())   
    {
        if (gameReset) {
            
            tileMap[tileNumY][tileNumX] = { 0 };
            //playerMap[tileNumY][tileNumX] = { 0 };
            memset(tileMap, 0, sizeof(tileMap));
            memset(playerMap, 0, sizeof(playerMap));
            memset(minePos, 0, sizeof(minePos));
            gameState = 0; //0=unfinished, 1=win, -1=lose
            memset(flags, 0, sizeof(flags));
            flagNum = 0;

            setMines(tileMap, minePos);
            clicks = 0;
            gameReset = false;
            cout << "RESET SUCCESSFUL" << endl;
            currGameTime = 0;

        }

        Vector2 mPos = (Vector2){int((GetMousePosition().x-bezel) / tileSize), int((GetMousePosition().y -HUDSize)/ tileSize)};
        //cout << sizeof(playerMap)/sizeof(playerMap[0][0]) << endl;
        bool validMouse = (mPos.x > -1 && mPos.x < tileNumX && mPos.y > -1 && mPos.y < tileNumY);
        if(gameState == 0 && validMouse){

            int x = int(mPos.x); int y = int(mPos.y);
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && playerMap[y][x] == 0){
                if (clicks){
                    if (tileMap[y][x] != 1){
                    checkSurrounding(playerMap, tileMap, x, y);
                    }
                    else {
                        
                        //Game over
                        playerMap[y][x] = -2;
                        for (int i = 1000; i < mineNum; i++){
                            int mX = minePos[i][1]; int mY = minePos[i][0];
                            playerMap[mY][mX] = -2;
                            
                        }
                        gameState = -1;
                    }
                }
                else {
                    checkSurroundingFirstTime(playerMap, tileMap, x, y, minePos);
                    gameStart = time(0);
                }
                clicks++;
            }
            

            else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                if (playerMap[y][x] == 0){
                    playerMap[y][x] = -1;
                    flags[flagNum][0] = y; flags[flagNum][1] = x;
                    flagNum++;
                    

                } else if (playerMap[y][x] == -1){
                    playerMap[y][x] = 0;

                    for (int flag=0;flag<flagNum;flag++){
                        if (flags[flag][1] == x && flags[flag][0] == y){
                            flags[flag][0] = 0; flags[flag][1] = 0;
                            for (int fRep = flag; fRep<flagNum; fRep++){
                                flags[fRep][1] = flags[fRep+1][1];
                                flags[fRep][0] = flags[fRep+1][0]; 
                                
                            }
                            break;
                        }
                    }

                    
                    flagNum--;
                    
                }
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {

                //check if game is won
                bool correctFlag;
                bool noUncheckedTiles = true;
                if (flagNum == mineNum){
                        for (int flagIdx=0; flagIdx<flagNum; flagIdx++){
                            
                            correctFlag = false;
                            for (int mineIdx=0; mineIdx<mineNum; mineIdx++) {
                                if (minePos[mineIdx][1] == flags[flagIdx][1] && minePos[mineIdx][0] == flags[flagIdx][0]) {
                                    correctFlag = true;
                                    //cout << correctFlag << endl;
                                    break;
                                }

                            }
                            if (correctFlag){
                                continue;
                            }
                            else {
                            //cout << correctFlag << endl;
                                break;
                            }
                            
                        }
                        for (int x=0;x<tileNumX;x++) {
                            if (!noUncheckedTiles) {
                                break;
                                }
                            for (int y=0;y<tileNumY;y++) { 
                                if(playerMap[y][x]==0) { 
                                    noUncheckedTiles = false;
                                    break;}
                                    }
                                }
                        if (correctFlag && noUncheckedTiles){
                            cout << "you won!!!!" << endl;
                            gameState = 1;
                        }

                    }
                    
                    //cout<<"FLAGS:"<<endl;for (int f=0;f<flagNum;f++){ cout<<flags[f][0]<<flags[f][1]<< endl;}cout<<endl;
                    
            }

            
            
        }
        

        BeginDrawing();
            

            ClearBackground(RAYWHITE);
            DrawRectangle(0, HUDSize, screenWidth, screenHeight-HUDSize, (Color){200,200,200,255});
            //draw settings

            
            //draw head and code
            int HeadWidth = 60;
            Vector2 FacePos = {screenWidth/2-HeadWidth/2, HUDSize/4};
            Color colors[3] = {RED, YELLOW, GREEN};
            Color txtColors[8] = {BLUE, DARKGREEN, RED, DARKBLUE, DARKBROWN, GRAY, DARKGRAY, BLACK };
            if (gameState==1) {DrawTexture(facewin, FacePos.x, FacePos.y, WHITE);}
            else if (gameState==-1){DrawTexture(faceded, FacePos.x, FacePos.y, WHITE);}
            else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && validMouse && playerMap[int(mPos.y)][int(mPos.x)] == 0) {DrawTexture(faceclick, FacePos.x, FacePos.y, WHITE);}
            else {DrawTexture(facenormal, FacePos.x, FacePos.y, WHITE);}
            //DrawRectangle(FacePos.x, FacePos.y, HeadWidth, HeadWidth, colors[gameState+1]);
            Vector2 mScrPos = GetMousePosition();
            if (mScrPos.x > FacePos.x && mScrPos.y > FacePos.y && mScrPos.x < FacePos.x+HeadWidth && mScrPos.y < FacePos.y+HeadWidth && IsMouseButtonPressed(0)){
                gameReset = true;
                gameReset;
                cout << "RESET" << endl;

            }
            if (clicks >0 && gameState == 0) {
                currGameTime = time(0) - gameStart;
            }
            
            DrawRectangle(0, HUDSize/4, tileSize*3, tileSize, BLACK);
            DrawText(TextFormat("%i", int(currGameTime)), 4, HUDSize/4+2, tileSize, RED);

            DrawRectangle(screenWidth-(tileSize*2), HUDSize/4, tileSize*2, tileSize, BLACK);
            DrawText(TextFormat("%i", mineNum-flagNum), screenWidth-(tileSize*2)+4, HUDSize/4+2, tileSize, RED);
            

            for(int y=0; y<tileNumY ; y++){
                for(int x=0; x<tileNumX ; x++){
                    int tX = x*tileSize+1+bezel; int tY = y*tileSize+1+HUDSize;

                    Color tilecolor = GRAY;
                    if (playerMap[y][x] > 0){
                        tilecolor = (Color){240,240,240,255};
                    }
                    else if (playerMap[y][x] == -2){
                        tilecolor = RED;
                    }else if (playerMap[y][x] == -3){
                        tilecolor = (Color){240,240,240,255};
                    }
                    
                    //highlight
                    if (mPos.x == x && mPos.y == y && playerMap[y][x] != -3 && playerMap[y][x] <= 0){
                        
                        tilecolor.r = clip(tilecolor.r - 50,0,255);
                        tilecolor.g = clip(tilecolor.g - 50,0,255);
                        tilecolor.b = clip(tilecolor.b - 50,0,255);
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && gameState == 0 && playerMap[y][x] != -1) {
                            tilecolor = (Color){240,240,240,255};
                            }
                        }
                    DrawRectangle(tX, tY, tileSize-2, tileSize-2, tilecolor);

                    if (playerMap[y][x] > 0) {
                            const char* txt = TextFormat("%i", playerMap[y][x]);
                            DrawText(txt, (tileSize/2 - (MeasureText(txt, tileSize))/2)+tileSize*x+bezel, tileSize*y+HUDSize, tileSize, txtColors[playerMap[y][x]-1]);

                    }
                    

                    if (playerMap[y][x] == -1){
                        DrawTexture(flagImg, tX-1,tY-1, (Color){255,255,255,255}); }

                }  

            }
            if (gameState == -1) {
                for (int i=0;i<mineNum;i++){
                    int x=minePos[i][1]; int y=minePos[i][0];
                    if (playerMap[y][x] != -1) {
                        int tX = x*tileSize+1+bezel; int tY = y*tileSize+1+HUDSize;
                        DrawTexture(mineImg, tX-1,tY-1, (Color){255,255,255,255});
                    }
                    
                }
                for (int i=0;i<flagNum;i++){
                    int x=flags[i][1]; int y=flags[i][0];
                    if (tileMap[y][x] == 0) {
                        int tX = x*tileSize+1+bezel; int tY = y*tileSize+1+HUDSize;
                        DrawTexture(mineImg, tX-1,tY-1, (Color){255,255,255,255});
                        DrawTexture(xImg, tX-1,tY-1, (Color){255,255,255,255});
                    }
                    
                }
            }

        EndDrawing();
    }

    CloseAudioDevice();        

    CloseWindow();             

    return 0;
}
