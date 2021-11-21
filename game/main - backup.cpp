#include <SDL.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

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
    SDL_RenderSetLogicalSize(renderer, 640, 480);
    bool hasQuit = false;
    Uint32 tickStart;
    Uint32 tickTime = 0;
    const Uint32 tickMinTime = 34; // ~30 ticks/second

    // Init texture
    SDL_Surface* surf = SDL_LoadBMP("./Assets/Sprites/hyperion.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    SDL_Rect texRect = { 0, 0, 0, 0 };
    SDL_QueryTexture(texture, NULL, NULL, &texRect.w, &texRect.h);
    SDL_Rect dstRect = { 0, 0, texRect.w, texRect.h };

    int screenWidth = 640;
    int screenHeight = 480;
    float theta = 0.f;
    float yaw = 0.f;
    glm::mat4x4 lookAt = glm::mat4x4();
    glm::mat4x4 view = glm::mat4x4();
    glm::vec3 cameraRightWorld = glm::vec3();
    glm::vec3 cameraUpWorld = glm::vec3();
    glm::vec4 camera = glm::vec4();
    glm::vec4 lookDir = glm::vec4();
    float zNear = 0.1f;
    float zFar = 1000.f;
    float fov = 90.f;
    float aspectRatio = (float)screenHeight / (float)screenWidth;
    glm::mat4x4 proj = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);

    while (!hasQuit)
    {
        tickStart = SDL_GetTicks();
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
                        case SDLK_DOWN:
                            camera.z -= 0.5f;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        if (hasQuit)
            break;

        // TODO: Decouple rendering/drawing from ticks and use lerping
        // Do 3d stuff
        //theta += 0.01f; // speen
        yaw += 0.05f;
        glm::mat4x4 matRotZ = glm::mat4x4();
        float halfTheta = theta * 0.5f;
        matRotZ[0][0] = std::cosf(halfTheta);
        matRotZ[0][1] = std::sinf(halfTheta);
        matRotZ[1][0] = -std::sinf(halfTheta);
        matRotZ[1][1] = std::cosf(halfTheta);
        matRotZ[2][2] = 1.0f;
        matRotZ[3][3] = 1.0f;
        glm::mat4x4 matRotX = glm::mat4x4();
        matRotX[0][0] = 1.0f;
        matRotX[1][1] = cosf(theta);
        matRotX[1][2] = sinf(theta);
        matRotX[2][1] = -sinf(theta);
        matRotX[2][2] = cosf(theta);
        matRotX[3][3] = 1.0f;
        glm::mat4x4 matTrans = glm::mat4x4();
        matTrans[0][0] = 1.0f;
        matTrans[1][1] = 1.0f;
        matTrans[2][2] = 1.0f;
        matTrans[3][3] = 1.0f;
        matTrans[3][0] = 0.f;
        matTrans[3][1] = 0.f;
        matTrans[3][2] = 5.0f;
        glm::mat4x4 matWorld = glm::mat4x4(1); // identity matrix, immediately thrown away next line (???)
        matWorld = matRotZ * matRotX;
        matWorld *= matTrans;
        glm::vec4 target(0.f, 0.f, 1.f, 1.f);
        glm::mat4x4 matCameraRot = glm::mat4x4();
        matCameraRot[0][0] = cosf(yaw);
        matCameraRot[0][2] = sinf(yaw);
        matCameraRot[2][0] = -sinf(yaw);
        matCameraRot[1][1] = 1.0f;
        matCameraRot[2][2] = cosf(yaw);
        matCameraRot[3][3] = 1.0f;
        lookDir = matCameraRot * target;
        target = camera + lookDir;
        glm::vec4 up(0.f, 1.f, 0.f, 1.f);
        lookAt = glm::lookAt(glm::vec3(camera), glm::vec3(target), glm::vec3(up));
        view = glm::inverse(lookAt);

        // place the billboard in space and project it to 2D
        glm::vec4 hypCenter(0.f, 0.f, 0.f, 1.f);
        glm::vec4 hypTransformed = matWorld * hypCenter;
        glm::vec4 hypViewed = view * hypTransformed;
        glm::vec4 hypProjected = proj * hypViewed;
        hypProjected /= hypProjected.w; // Scale into view by normalizing vector
        hypProjected.x *= -1.f;
        hypProjected.y *= -1.f;
        glm::vec4 offsetView(1.f, 1.f, 0.f, 1.f);
        hypProjected += offsetView;
        
        hypProjected.x *= 0.5f * (float)screenWidth;
        hypProjected.y *= 0.5f * (float)screenHeight;
        
        dstRect.x = (int)hypProjected.x;
        dstRect.y = (int)hypProjected.y;

        // Draw to screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        if (dstRect.x > 0 && dstRect.y > 0)
            SDL_RenderDrawPoint(renderer, dstRect.x, dstRect.y);
        //SDL_RenderCopy(renderer, texture, &texRect, &dstRect);
        SDL_RenderPresent(renderer);
        // Ticks/second limiting
        tickTime = SDL_GetTicks() - tickStart;
        if (tickTime < tickMinTime)
            SDL_Delay(tickMinTime - tickTime);
        tickTime = SDL_GetTicks() - tickStart;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
