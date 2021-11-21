/*
OneLoneCoder.com - 3D Graphics Part #3 - Cameras & Clipping
"Tredimensjonal Grafikk" - @Javidx9

License
~~~~~~~
One Lone Coder Console Game Engine  Copyright (C) 2018  Javidx9
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; See license for details.
Original works located at:
https://www.github.com/onelonecoder
https://www.onelonecoder.com
https://www.youtube.com/javidx9
GNU GPLv3
https://github.com/OneLoneCoder/videos/blob/master/LICENSE

From Javidx9 :)
~~~~~~~~~~~~~~~
Hello! Ultimately I don't care what you use this for. It's intended to be
educational, and perhaps to the oddly minded - a little bit of fun.
Please hack this, change it and use it in any way you see fit. You acknowledge
that I am not responsible for anything bad that happens as a result of
your actions. However this code is protected by GNU GPLv3, see the license in the
github repo. This means you must attribute me if you use it. You can view this
license here: https://github.com/OneLoneCoder/videos/blob/master/LICENSE
Cheers!

Background
~~~~~~~~~~
3D Graphics is an interesting, visually pleasing suite of algorithms. This is the
first video in a series that will demonstrate the fundamentals required to
build your own software based 3D graphics systems.

Video
~~~~~
https://youtu.be/ih20l3pJoeU
https://youtu.be/XgMWc6LumG4
https://youtu.be/HXSuNxpCzdM

Author
~~~~~~
Twitter: @javidx9
Blog: http://www.onelonecoder.com
Discord: https://discord.gg/WhwHUMV


Last Updated: 14/08/2018
*/

#include <fstream>
#include <algorithm>
#include <SDL.h>
#include <vector>
#include <strstream>
#include <list>
#include <iostream>

struct vec3d
{
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1; // Need a 4th term to perform sensible matrix vector multiplication
};

struct mat4x4
{
    float m[4][4] = { { 0 } };
};

class olcEngine3D
{
private:
    mat4x4 matProj;    // Matrix that converts from view space to screen space
    vec3d vCamera;    // Location of camera in world space
    vec3d vLookDir;    // Direction vector along the direction camera points
    float fYaw;        // FPS Camera rotation in XZ plane
    float fTheta;    // Spins World transform
    uint16_t screenWidth, screenHeight;
    SDL_Renderer* renderer;
    bool hasQuit;

    vec3d Matrix_MultiplyVector(mat4x4& m, vec3d& i)
    {
        vec3d v;
        v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
        v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
        v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
        v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
        return v;
    }

    mat4x4 Matrix_MakeIdentity()
    {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationX(float fAngleRad)
    {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = cosf(fAngleRad);
        matrix.m[1][2] = sinf(fAngleRad);
        matrix.m[2][1] = -sinf(fAngleRad);
        matrix.m[2][2] = cosf(fAngleRad);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationY(float fAngleRad)
    {
        mat4x4 matrix;
        matrix.m[0][0] = cosf(fAngleRad);
        matrix.m[0][2] = sinf(fAngleRad);
        matrix.m[2][0] = -sinf(fAngleRad);
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = cosf(fAngleRad);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationZ(float fAngleRad)
    {
        mat4x4 matrix;
        matrix.m[0][0] = cosf(fAngleRad);
        matrix.m[0][1] = sinf(fAngleRad);
        matrix.m[1][0] = -sinf(fAngleRad);
        matrix.m[1][1] = cosf(fAngleRad);
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeTranslation(float x, float y, float z)
    {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        matrix.m[3][0] = x;
        matrix.m[3][1] = y;
        matrix.m[3][2] = z;
        return matrix;
    }

    mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
    {
        float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
        mat4x4 matrix;
        matrix.m[0][0] = fAspectRatio * fFovRad;
        matrix.m[1][1] = fFovRad;
        matrix.m[2][2] = fFar / (fFar - fNear);
        matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
        matrix.m[2][3] = 1.0f;
        matrix.m[3][3] = 0.0f;
        return matrix;
    }

    mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2)
    {
        mat4x4 matrix;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
        return matrix;
    }

    mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up)
    {
        // Calculate new forward direction
        vec3d newForward = Vector_Sub(target, pos);
        newForward = Vector_Normalise(newForward);

        // Calculate new Up direction
        vec3d a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
        vec3d newUp = Vector_Sub(up, a);
        newUp = Vector_Normalise(newUp);

        // New Right direction is easy, its just cross product
        vec3d newRight = Vector_CrossProduct(newUp, newForward);

        // Construct Dimensioning and Translation Matrix    
        mat4x4 matrix;
        matrix.m[0][0] = newRight.x;    matrix.m[0][1] = newRight.y;    matrix.m[0][2] = newRight.z;    matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = newUp.x;        matrix.m[1][1] = newUp.y;        matrix.m[1][2] = newUp.z;        matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = newForward.x;    matrix.m[2][1] = newForward.y;    matrix.m[2][2] = newForward.z;    matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = pos.x;            matrix.m[3][1] = pos.y;            matrix.m[3][2] = pos.z;            matrix.m[3][3] = 1.0f;
        return matrix;

    }

    mat4x4 Matrix_QuickInverse(mat4x4& m) // Only for Rotation/Translation Matrices
    {
        mat4x4 matrix;
        matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
        matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
        matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    vec3d Vector_Add(vec3d& v1, vec3d& v2)
    {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    }

    vec3d Vector_Sub(vec3d& v1, vec3d& v2)
    {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    }

    vec3d Vector_Mul(vec3d& v1, float k)
    {
        return { v1.x * k, v1.y * k, v1.z * k };
    }

    vec3d Vector_Div(vec3d& v1, float k)
    {
        return { v1.x / k, v1.y / k, v1.z / k };
    }

    float Vector_DotProduct(vec3d& v1, vec3d& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    float Vector_Length(vec3d& v)
    {
        return sqrtf(Vector_DotProduct(v, v));
    }

    vec3d Vector_Normalise(vec3d& v)
    {
        float l = Vector_Length(v);
        return { v.x / l, v.y / l, v.z / l };
    }

    vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2)
    {
        vec3d v;
        v.x = v1.y * v2.z - v1.z * v2.y;
        v.y = v1.z * v2.x - v1.x * v2.z;
        v.z = v1.x * v2.y - v1.y * v2.x;
        return v;
    }

    vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd)
    {
        plane_n = Vector_Normalise(plane_n);
        float plane_d = -Vector_DotProduct(plane_n, plane_p);
        float ad = Vector_DotProduct(lineStart, plane_n);
        float bd = Vector_DotProduct(lineEnd, plane_n);
        float t = (-plane_d - ad) / (bd - ad);
        vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
        vec3d lineToIntersect = Vector_Mul(lineStartToEnd, t);
        return Vector_Add(lineStart, lineToIntersect);
    }

    uint32_t GetColor(float lum)
    {
        uint8_t brightness = (uint8_t)(lum * 255.f);

        return (brightness << 24) | (brightness << 16) | (brightness << 8) | brightness; // pack greyscale rbga values
    }

    void DrawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3)
    {
        SDL_Point points[3]{ {x1, y1}, {x2, y2}, {x3, y3} };
        SDL_RenderDrawLines(renderer, points, 3);
        SDL_RenderDrawLine(renderer, points[2].x, points[2].y, points[0].x, points[0].y);
    }

public:
    olcEngine3D()
    {
        screenWidth = 640;
        screenHeight = 480;
        hasQuit = false;
    }

    ~olcEngine3D()
    {
        SDL_DestroyRenderer(renderer);
    }

    uint16_t ScreenWidth() { return screenWidth; }

    uint16_t ScreenHeight() { return screenHeight; }

    bool HasQuit() { return hasQuit; }

    void SetRenderer(SDL_Renderer* _renderer) { renderer = _renderer; }

    bool OnUserCreate()
    {

        // Projection Matrix
        matProj = Matrix_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);
        return true;
    }

    void Fill(uint32_t color)
    {
        SDL_SetRenderDrawColor(renderer, color << 24, color << 16, color << 8, color);
        SDL_RenderClear(renderer);
    }

    bool OnUserUpdate()
    {
        const float MoveSpeed = .1f;
        vec3d vForward = Vector_Mul(vLookDir, MoveSpeed);
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                hasQuit = true;
                std::cout << "User has quit" << std::endl;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case SDLK_w:
                        vCamera = Vector_Add(vCamera, vForward);
                        break;
                    case SDLK_s:
                        vCamera = Vector_Sub(vCamera, vForward);
                        break;
                    case SDLK_a:
                        fYaw -= 0.1f;
                        break;
                    case SDLK_d:
                        fYaw += 0.1f;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
            }
        }

        // Set up "World Tranmsform" though not updating theta 
        // makes this a bit redundant
        mat4x4 matRotZ, matRotX;
        matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
        matRotX = Matrix_MakeRotationX(fTheta);

        mat4x4 matTrans;
        matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);

        mat4x4 matWorld;
        matWorld = Matrix_MakeIdentity();    // Form World Matrix
        matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX); // Transform by rotation
        matWorld = Matrix_MultiplyMatrix(matWorld, matTrans); // Transform by translation

        // Create "Point At" Matrix for camera
        vec3d vUp = { 0,1,0 };
        vec3d vTarget = { 0,0,1 };
        mat4x4 matCameraRot = Matrix_MakeRotationY(fYaw);
        vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);
        vTarget = Vector_Add(vCamera, vLookDir);
        mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

        // Make view matrix from camera
        mat4x4 matView = Matrix_QuickInverse(matCamera);

        vec3d testPoint = { 0.f, 0.f, 0.f };
        vec3d testPointTransformed = Matrix_MultiplyVector(matWorld, testPoint);
        vec3d testPointView = Matrix_MultiplyVector(matView, testPointTransformed);
        vec3d testPointProj = Matrix_MultiplyVector(matProj, testPointView);
        testPointProj = Vector_Div(testPointProj, testPointProj.w);
        testPointProj.x *= -1.f;
        testPointProj.y *= -1.f;
        vec3d offsetView = { 1.0f, 1.f, 0.f };
        testPointProj = Vector_Add(testPointProj, offsetView);
        testPointProj.x *= 0.5f * (float)ScreenWidth();
        testPointProj.y *= 0.5f * (float)ScreenHeight();
        vec3d pointClip = Matrix_MultiplyVector(matProj, testPointView);
        // TODO: apply an outside frustum tolerance so billboards don't get clipped when center is outside
        bool pointInFrustum = abs(pointClip.x) < pointClip.w && abs(pointClip.y) < pointClip.w && pointClip.z > .0f && pointClip.z < pointClip.w;

        // Clear Screen
        Fill(0);

        if (pointInFrustum)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            const float squareSize = 100.f;
            SDL_Rect r = { (int)testPointProj.x - (int)((squareSize / 2.f) / testPointView.z), (int)testPointProj.y - (int)((squareSize / 2.f) / testPointView.z), (int)(squareSize / testPointView.z), (int)(squareSize / testPointView.z) }; // square around center point
            SDL_RenderDrawRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, testPointProj.x, testPointProj.y);
        }

        return true;
    }

};

int main(int argc, char* argv[])
{
    olcEngine3D demo;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL failed to initialize: " << SDL_GetError() << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow("WCngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(r, 640, 480);
    demo.SetRenderer(r);
    bool hasQuit = false;
    Uint32 tickStart;
    Uint32 tickTime = 0;
    const Uint32 tickMinTime = 34; // ~30 ticks/second

    demo.OnUserCreate();
    while (!demo.HasQuit())
    {
        tickStart = SDL_GetTicks();
        demo.OnUserUpdate();

        if (hasQuit)
            break;

        // Draw to screen
        SDL_RenderPresent(r);
        // Ticks/second limiting
        tickTime = SDL_GetTicks() - tickStart;
        if (tickTime < tickMinTime)
            SDL_Delay(tickMinTime - tickTime);
        tickTime = SDL_GetTicks() - tickStart;
    }

    return 0;
}
