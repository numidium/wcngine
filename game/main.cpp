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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer3d
{
private:
    glm::mat4x4 matProj; // Matrix that converts from view space to screen space
    glm::vec4 camera;    // Location of camera in world space
    glm::vec4 lookDir;
    float yaw;
    float theta;
    uint16_t screenWidth, screenHeight;
    SDL_Renderer* renderer;
    bool hasQuit;

    glm::vec4 Matrix_MultiplyVector(glm::mat4x4& m, glm::vec4& i)
    {
        glm::vec4 v;
        v.x = i.x * m[0][0] + i.y * m[1][0] + i.z * m[2][0] + i.w * m[3][0];
        v.y = i.x * m[0][1] + i.y * m[1][1] + i.z * m[2][1] + i.w * m[3][1];
        v.z = i.x * m[0][2] + i.y * m[1][2] + i.z * m[2][2] + i.w * m[3][2];
        v.w = i.x * m[0][3] + i.y * m[1][3] + i.z * m[2][3] + i.w * m[3][3];
        return v;
    }

    glm::mat4x4 Matrix_MakeRotationX(float fAngleRad)
    {
        glm::mat4x4 matrix = glm::mat4x4();
        matrix[0][0] = 1.0f;
        matrix[1][1] = cosf(fAngleRad);
        matrix[1][2] = sinf(fAngleRad);
        matrix[2][1] = -sinf(fAngleRad);
        matrix[2][2] = cosf(fAngleRad);
        matrix[3][3] = 1.0f;
        return matrix;
    }

    glm::mat4x4 Matrix_MakeRotationY(float fAngleRad)
    {
        glm::mat4x4 matrix = glm::mat4x4();
        matrix[0][0] = cosf(fAngleRad);
        matrix[0][2] = sinf(fAngleRad);
        matrix[2][0] = -sinf(fAngleRad);
        matrix[1][1] = 1.0f;
        matrix[2][2] = cosf(fAngleRad);
        matrix[3][3] = 1.0f;
        return matrix;
    }

    glm::mat4x4 Matrix_MakeRotationZ(float fAngleRad)
    {
        glm::mat4x4 matrix = glm::mat4x4();
        matrix[0][0] = cosf(fAngleRad);
        matrix[0][1] = sinf(fAngleRad);
        matrix[1][0] = -sinf(fAngleRad);
        matrix[1][1] = cosf(fAngleRad);
        matrix[2][2] = 1.0f;
        matrix[3][3] = 1.0f;
        return matrix;
    }

    glm::mat4x4 Matrix_MakeTranslation(float x, float y, float z)
    {
        glm::mat4x4 matrix = glm::mat4x4();
        matrix[0][0] = 1.0f;
        matrix[1][1] = 1.0f;
        matrix[2][2] = 1.0f;
        matrix[3][3] = 1.0f;
        matrix[3][0] = x;
        matrix[3][1] = y;
        matrix[3][2] = z;
        return matrix;
    }

    glm::mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
    {
        float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
        glm::mat4x4 matrix = glm::mat4x4();
        matrix[0][0] = fAspectRatio * fFovRad;
        matrix[1][1] = fFovRad;
        matrix[2][2] = fFar / (fFar - fNear);
        matrix[3][2] = (-fFar * fNear) / (fFar - fNear);
        matrix[2][3] = 1.0f;
        matrix[3][3] = 0.0f;
        return matrix;
    }

    glm::mat4x4 Matrix_PointAt(glm::vec4& pos, glm::vec4& target, glm::vec4& up)
    {
        // Calculate new forward direction
        glm::vec4 newForward = target - pos;
        newForward = glm::normalize(newForward);

        // Calculate new Up direction
        glm::vec4 a = newForward * glm::dot(up, newForward);
        glm::vec4 newUp = up - a;
        newUp = glm::normalize(newUp);

        // New Right direction is easy, its just cross product
        glm::vec3 r = glm::cross((glm::vec3)newUp, (glm::vec3)newForward);
        glm::vec4 newRight { r.x, r.y, r.z, 1.f };

        // Construct Dimensioning and Translation Matrix
        glm::mat4x4 matrix = glm::mat4x4();
        matrix[0][0] = newRight.x;      matrix[0][1] = newRight.y;    matrix[0][2] = newRight.z;    matrix[0][3] = 0.0f;
        matrix[1][0] = newUp.x;         matrix[1][1] = newUp.y;       matrix[1][2] = newUp.z;       matrix[1][3] = 0.0f;
        matrix[2][0] = newForward.x;    matrix[2][1] = newForward.y;  matrix[2][2] = newForward.z;  matrix[2][3] = 0.0f;
        matrix[3][0] = pos.x;           matrix[3][1] = pos.y;         matrix[3][2] = pos.z;         matrix[3][3] = 1.0f;

        return matrix;
    }

public:
    Renderer3d(SDL_Renderer* _renderer)
    {
        screenWidth = 640;
        screenHeight = 480;
        hasQuit = false;
        matProj = Matrix_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);
        renderer = _renderer;
    }

    ~Renderer3d()
    {
        SDL_DestroyRenderer(renderer);
    }

    uint16_t ScreenWidth() { return screenWidth; }

    uint16_t ScreenHeight() { return screenHeight; }

    bool HasQuit() { return hasQuit; }

    void Fill(uint32_t color)
    {
        SDL_SetRenderDrawColor(renderer, color << 24, color << 16, color << 8, color);
        SDL_RenderClear(renderer);
    }

    void Update()
    {
        const float MoveSpeed = .1f;
        glm::vec4 vForward = lookDir * MoveSpeed;
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
                        camera = camera + vForward;
                        break;
                    case SDLK_s:
                        camera = camera - vForward;
                        break;
                    case SDLK_a:
                        yaw -= 0.1f;
                        break;
                    case SDLK_d:
                        yaw += 0.1f;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
            }
        }

        // Create world transform by applying rotations and translation
        glm::mat4x4 matRotZ, matRotX;
        matRotZ = Matrix_MakeRotationZ(theta * 0.5f);
        matRotX = Matrix_MakeRotationX(theta);

        glm::mat4x4 matTranslate;
        matTranslate = Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);

        glm::mat4x4 matWorld;
        matWorld = matRotZ * matRotX;
        matWorld = matWorld * matTranslate;

        // Create "Point At" Matrix for camera
        glm::vec4 vUp = { 0, 1, 0, 1 };
        glm::vec4 vTarget = { 0, 0, 1, 1 };
        glm::mat4x4 matCameraRot = Matrix_MakeRotationY(yaw);
        lookDir = matCameraRot * vTarget;
        vTarget = camera + lookDir;
        glm::mat4x4 matCamera = Matrix_PointAt(camera, vTarget, vUp);
        // The below call doesn't yield the same results. I have no clue why.
        //glm::mat4x4 matCamera = glm::lookAt((glm::vec3)vCamera, (glm::vec3)vTarget, (glm::vec3)vUp);

        // Make view matrix from camera
        glm::mat4x4 matView = glm::inverse(matCamera);

        glm::vec4 testPoint(0.f, 0.f, 0.f, 1.f);
        glm::vec4 testPointTransformed = matWorld * testPoint;
        glm::vec4 testPointView = matView * testPointTransformed;
        glm::vec4 testPointProj = matProj * testPointView;
        testPointProj = testPointProj / testPointProj.w;
        testPointProj.x *= -1.f;
        testPointProj.y *= -1.f;
        glm::vec4 offsetView(1.0f, 1.f, 0.f, 1.f);
        testPointProj = testPointProj + offsetView;
        testPointProj.x *= 0.5f * (float)ScreenWidth();
        testPointProj.y *= 0.5f * (float)ScreenHeight();
        glm::vec4 pointClip = matProj * testPointView;
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
    }
};

int main(int argc, char* argv[])
{
    
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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Renderer3d renderer3d(renderer);
    SDL_RenderSetLogicalSize(renderer, renderer3d.ScreenWidth(), renderer3d.ScreenHeight());
    bool hasQuit = false;
    Uint32 tickStart;
    Uint32 tickTime = 0;
    const Uint32 tickMinTime = 34; // ~30 ticks/second

    while (!renderer3d.HasQuit())
    {
        tickStart = SDL_GetTicks();
        renderer3d.Update();

        if (hasQuit)
            break;

        // Draw to screen
        SDL_RenderPresent(renderer);
        // Ticks/second limiting
        tickTime = SDL_GetTicks() - tickStart;
        if (tickTime < tickMinTime)
            SDL_Delay(tickMinTime - tickTime);
        tickTime = SDL_GetTicks() - tickStart;
    }

    return 0;
}
