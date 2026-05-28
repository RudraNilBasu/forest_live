#include "raylib.h"
#include <stdio.h>

struct position {
    int x;
    int y;
};

enum gold_status {
    NONE,
    NOT_COLLECTED,
    COLLECTED,
};

bool RectanglesOverlap(position A, position B, int Width, int Height)
{
    bool Result = false;
    // For this game: all entities are of same width (for now)
    if (A.x == B.x && A.y == B.y)
    {
        Result = true;
    }
    return(Result);
}

int main()
{

    InitWindow(480, 640, "Forest");

    int ScreenWidth = GetScreenWidth();
    int ScreenHeight = GetScreenHeight();
    int TilesPerWidth = 10;
    int TilesPerHeight = 12;
    int TileWidth = ScreenWidth / TilesPerWidth;
    int TileHeight = ScreenHeight / TilesPerHeight;

    position DoorP = {0, 7};

    position BlockP[] = {
        { 0,  0 }, { 0,  1 }, { 0,  2 }, { 0,  3 }, { 0,  4 }, { 0,  5 }, { 0,  6 }, { 0,  7 }, { 0,  8 }, { 0,  9 }, { 0,  10 }, { 0,  11 },
        { 9,  0 }, { 9,  1 }, { 9,  2 }, { 9,  3 }, { 9,  4 }, { 9,  5 }, { 9,  6 }, { 9,  7 }, { 9,  8 }, { 9,  9 }, { 9,  10 }, { 9,  11 },
        {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0},
        {1, 11}, {2, 11}, {3, 11}, {4, 11}, {5, 11}, {6, 11}, {7, 11}, {8, 11},
    };
    int BlockCount = sizeof(BlockP)/sizeof(BlockP[0]);

    position PlayerP = { 5, 9 };

    position GoldP[] = {
        { 2, 2 },
        { 7, 2 }
    };
    gold_status GoldStatus[] = {
        NOT_COLLECTED,
        NOT_COLLECTED,
    };
    int GoldCount = sizeof(GoldP) / sizeof(GoldP[0]);
    int GoldCollected = 0;
    char GoldScore[1024];

    bool LevelCompleted = false;
    char *LevelCompletedText = (char *)"GO THRU THE DOOR";

    bool GameOver = false;

    while (!WindowShouldClose())
    {
        ClearBackground(GRAY);
        BeginDrawing();

        // Physics
        int KeyPressed = GetKeyPressed();
        if (KeyPressed && !GameOver)
        {
            position NewPlayerP = PlayerP;
            if (KeyPressed == 87)
            {
                // Up
                NewPlayerP.y--;
            }
            if (KeyPressed == 83)
            {
                // Down
                NewPlayerP.y++;
            }
            if (KeyPressed == 65)
            {
                // Left
                NewPlayerP.x--;
            }
            if (KeyPressed == 68)
            {
                // Right
                NewPlayerP.x++;
            }

            bool Collided = false;
            for (int Index = 0; Index < BlockCount; ++Index)
            {
                if (LevelCompleted && (BlockP[Index].x == DoorP.x && BlockP[Index].y == DoorP.y))
                {
                    continue;
                }
                if (RectanglesOverlap(NewPlayerP, BlockP[Index], TileWidth, TileHeight))
                {
                    Collided = true;
                }
            }

            for (int Index = 0; Index < GoldCount; ++Index)
            {
                if (GoldStatus[Index] == NOT_COLLECTED &&
                    RectanglesOverlap(NewPlayerP, GoldP[Index], TileWidth, TileHeight))
                {
                    GoldCollected++;
                    GoldStatus[Index] = COLLECTED;
                }
            }

            bool GoldRemaining = false;
            for (int Index = 0; Index < GoldCount && !GoldRemaining; ++Index)
            {
                if (GoldStatus[Index] == NOT_COLLECTED)
                {
                    GoldRemaining = true;
                }
            }
            if (!GoldRemaining)
            {
                LevelCompleted = true;
            }

            if (!Collided)
            {
                PlayerP = NewPlayerP;
            }
        }

        if (PlayerP.x < 0 || PlayerP.x > ScreenWidth || PlayerP.y < 0 || PlayerP.y > ScreenHeight)
        {
            GameOver = true;
        }

        // Rendering
        for (int Index = 0; Index < BlockCount; ++Index)
        {
            if (LevelCompleted && (BlockP[Index].x == DoorP.x && BlockP[Index].y == DoorP.y))
            {
            }
            else
            {
                DrawRectangle((BlockP[Index].x * TileWidth), (BlockP[Index].y * TileHeight), TileWidth, TileHeight, BLACK);
            }
        }

        for (int Index = 0; Index < GoldCount; ++Index)
        {
            if (GoldStatus[Index] == NOT_COLLECTED)
            {
                DrawRectangle((GoldP[Index].x * TileWidth), (GoldP[Index].y * TileHeight), TileWidth, TileHeight, GOLD);
            }
        }

        DrawRectangle((PlayerP.x * TileWidth), (PlayerP.y * TileHeight), TileWidth, TileHeight, PURPLE);

        // Score
        snprintf(GoldScore, 1024, "Gold Collected: %d\n", GoldCollected);
        if (!GameOver)
        {
            DrawText(GoldScore, (5 * TileWidth), ((TilesPerHeight - 1) * TileHeight) + (0.2f*TileHeight), 24, GOLD);
        }

        if (!GameOver && LevelCompleted)
        {
            DrawText(LevelCompletedText, (1 * TileWidth),  (0.2f*TileHeight), 24, GOLD);
        }
        if (GameOver)
        {
            DrawText("ESCAPED", (3 * TileWidth),  (2 * TileHeight), 48, GOLD);
        }

        EndDrawing();
    }
    CloseWindow();
    return(0);
}
