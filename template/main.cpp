#include "raylib.h"
#include "math.h"
// function for drawing the buttons
bool DrawButton(Rectangle rect, const char* text, Color baseColor) {
    Vector2 mousePos = GetMousePosition(); // gets the mouse position
    bool isHovered = CheckCollisionPointRec(mousePos, rect); // checks if the mouse is over the button
    bool isPressed = false;

    Color renderColor = baseColor; // setting button color
    if (isHovered) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            renderColor = ColorAlpha(baseColor, 0.6f);
            isPressed = true; // changes the color and sets ispressed to true
        }
        else {
            renderColor = ColorAlpha(baseColor, 0.8f); // adjusting color when hovered
        }
    }

    // drawing the plus and minus lines
    DrawRectangleRec(rect, renderColor);
    DrawRectangleLinesEx(rect, 1, WHITE);

    int fontSize = 16;
    int textWidth = MeasureText(text, fontSize);
    int textX = (int)(rect.x + (rect.width - textWidth) / 2);
    int textY = (int)(rect.y + (rect.height - fontSize) / 2);
    DrawText(text, textX, textY, fontSize, WHITE);

    return isPressed; // returns bool based on if button is pressed
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 500;
    InitWindow(screenWidth, screenHeight, "Hypotrochoid Drawing Tool"); // creating window


     // adjustable variables
    float R = 140.0f; // big circle radius
    float r = 88.0f; // little circle radius
    float d = 60.0f; // pen distance
    float speed = 0.05f; // speed of the drawing
    float hue = 180.0f; // changing the hue of the lines
    int multiplier = 1; // a multiplier for the speed, makes it even faster :3

    //remebering the settings when you change stuff to let it clear the screen
    float lastR = R, lastSmallr = r, lastD = d, lastSpeed = speed, lastHue = hue;
    int lastMultiplier = multiplier;

    // helps with drawing, and tracking circle size
    float theta = 0.0f;
    Vector2 center = { (screenWidth + 240) / 2.0f, screenHeight / 2.0f };
    Vector2 lastPenPos = { center.x + (R - r) + d, center.y };

    //allows for the drawing to be changed, and then cleared once changes are done
    RenderTexture2D canvas = LoadRenderTexture(screenWidth, screenHeight);
    BeginTextureMode(canvas);
    ClearBackground(BLACK);
    EndTextureMode();

    SetTargetFPS(60);

    // main loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        DrawTextureRec(canvas.texture, Rectangle{ 0, 0, (float)canvas.texture.width, (float)-canvas.texture.height }, Vector2{ 0, 0 }, WHITE);

        // drawing the settings part
        DrawRectangle(0, 0, 240, screenHeight, Fade(BLACK, 0.9f));
        DrawLine(240, 0, 240, screenHeight, GRAY);
        DrawText("SETTINGS", 15, 20, 20, WHITE);


        // all the text and buttons for the settings
        DrawText("Big Circle Radius (R)", 15, 60, 13, LIGHTGRAY);
        DrawText(TextFormat("%.1f", R), 15, 80, 16, WHITE);
        if (DrawButton(Rectangle{ 130, 75, 40, 26 }, "-", RED)) R -= 1.0f;
        if (DrawButton(Rectangle{ 180, 75, 40, 26 }, "+", GREEN))   R += 1.0f;

        DrawText("Small Circle Radius (r)", 15, 115, 13, LIGHTGRAY);
        DrawText(TextFormat("%.1f", r), 15, 135, 16, WHITE);
        if (DrawButton(Rectangle{ 130, 130, 40, 26 }, "-", RED)) r -= 1.0f;
        if (DrawButton(Rectangle{ 180, 130, 40, 26 }, "+", GREEN))   r += 1.0f;

        DrawText("Pen Offset Distance (d)", 15, 170, 13, LIGHTGRAY);
        DrawText(TextFormat("%.1f", d), 15, 190, 16, WHITE);
        if (DrawButton(Rectangle{ 130, 185, 40, 26 }, "-", RED)) d -= 1.0f;
        if (DrawButton(Rectangle{ 180, 185, 40, 26 }, "+", GREEN))   d += 1.0f;

        DrawText("Angle -> Going Up (Speed)", 15, 225, 13, LIGHTGRAY);
        DrawText(TextFormat("%.3f", speed), 15, 245, 14, WHITE);
        if (DrawButton(Rectangle{ 130, 240, 40, 26 }, "-", RED)) speed -= 0.002f;
        if (DrawButton(Rectangle{ 180, 240, 40, 26 }, "+", GREEN))   speed += 0.002f;

        DrawText("Speed Multiplier", 15, 280, 13, LIGHTGRAY);
        DrawText(TextFormat("%dx Fast", multiplier), 15, 300, 14, WHITE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // stops button from being held down
            if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ 130, 295, 40, 26 })) multiplier -= 1;
            if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ 180, 295, 40, 26 })) multiplier += 1;
        }
        DrawButton(Rectangle{ 130, 295, 40, 26 }, "-", RED);
        DrawButton(Rectangle{ 180, 295, 40, 26 }, "+", GREEN);

        DrawText("Color of Drawing (Hue)", 15, 335, 13, LIGHTGRAY);
        Color drawingColor = ColorFromHSV(hue, 1.0f, 1.0f);
        DrawRectangle(15, 355, 40, 26, drawingColor); // displays a box with the color being drawn
        DrawRectangleLines(15, 355, 40, 26, WHITE);
        if (DrawButton(Rectangle{ 130, 355, 40, 26 }, "-", RED)) hue -= 2.0f;
        if (DrawButton(Rectangle{ 180, 355, 40, 26 }, "+", GREEN))   hue += 2.0f;

        // some hard caps for stuff so it doesnt break
        if (R < 10.0f) R = 10.0f;
        if (r < 5.0f) r = 5.0f;
        if (d < 0.0f) d = 0.0f;
        if (speed < 0.001f) speed = 0.001f;
        if (multiplier < 1) multiplier = 1;
        if (multiplier > 20) multiplier = 20;
        if (hue < 0.0f) hue += 360.0f;
        if (hue > 360.0f) hue -= 360.0f;

        
        if (DrawButton(Rectangle{ 15, 410, 205, 30 }, "Clear Canvas", DARKGRAY) || IsKeyPressed(KEY_SPACE)) {
            lastR = -1.0f;
        }
        // clearing existing drawings to start new drawing
        if (R != lastR || r != lastSmallr || d != lastD || speed != lastSpeed || hue != lastHue || multiplier != lastMultiplier)
        {
            BeginTextureMode(canvas);
            ClearBackground(BLACK);
            EndTextureMode();

            theta = 0.0f;
            lastPenPos.x = center.x + (R - r) + d;
            lastPenPos.y = center.y;

            lastR = R;
            lastSmallr = r;
            lastD = d;
            lastSpeed = speed;
            lastHue = hue;
            lastMultiplier = multiplier;
        }

        float currentPenX = lastPenPos.x;
        float currentPenY = lastPenPos.y;
        // the actual drawing of the lines
        for (int i = 0; i < multiplier; i++) {
            theta += speed;
            float x = (R - r) * cosf(theta) + d * cosf(((R - r) / r) * theta);
            float y = (R - r) * sinf(theta) - d * sinf(((R - r) / r) * theta);
            Vector2 currentPenPos = { center.x + x, center.y + y };

            BeginTextureMode(canvas);
            DrawLineV(lastPenPos, currentPenPos, drawingColor);
            EndTextureMode();

            lastPenPos = currentPenPos;
            currentPenX = currentPenPos.x;
            currentPenY = currentPenPos.y;
        }

        DrawCircleLines((int)center.x, (int)center.y, (int)R, Fade(GRAY, 0.25f));
        float innerCenterX = center.x + (R - r) * cosf(theta);
        float innerCenterY = center.y + (R - r) * sinf(theta);
        DrawCircleLines((int)innerCenterX, (int)innerCenterY, (int)r, Fade(LIGHTGRAY, 0.12f));
        DrawLineV(Vector2{ innerCenterX, innerCenterY }, Vector2{ currentPenX, currentPenY }, RED);
        DrawCircle((int)currentPenX, (int)currentPenY, 3.0f, MAROON);

        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    CloseWindow();

    return 0;
}
