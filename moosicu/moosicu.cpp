#include<iostream>
#include<vector>
#include<string.h>
#include "raylib.h"
#include <cassert>

struct Frame{
    float left;
    float right;
};

Frame global_frames[4410*2] = {};
size_t global_frames_count = 0;

void callback(void* bufferData, unsigned int frames) {
    //static_cast<void>(bufferData);
    size_t capacity = std::size(global_frames);
    if (capacity - global_frames_count >= frames) {
        memcpy(global_frames+global_frames_count, bufferData, sizeof(Frame) * frames);
        global_frames_count += frames;
    
    }
    else if (frames <= capacity) {
        memmove(global_frames, global_frames +  frames, sizeof(Frame) * (capacity-frames));
        memcpy(global_frames + (capacity - frames), bufferData, sizeof(Frame) * frames);
    }
    else {
        memcpy(global_frames, bufferData, sizeof(Frame) * capacity);
        global_frames_count = capacity;
    }




    //if (frames > capacity) {
    //    frames = capacity ;
    //}
    //memcpy(global_frames, bufferData, sizeof(uint64_t) * frames );
    //global_frames_count = frames;
    //std::cout << "callback(frames = " << frames<<")\n";
}

int main()
{
    InitWindow(800, 600, "Moosix");
    SetTargetFPS(60);
    InitAudioDevice();
    Music music = LoadMusicStream("tranquillity.mp3");

    //Sample rate : 44100 Hz
    //Sample size : 32 bits
    //Channels:      2 (Stereo)
    //Total frames : 8471808
    
    assert(music.stream.sampleSize == 32);
    assert(music.stream.channels == 2);

    std::cout<<"music.frameCount = "<<music.frameCount<< ")\n";
    std::cout << "music.stream.sampleRate = " << music.stream.sampleRate << ")\n";
    std::cout << "music.stream.sampleSize = " << music.stream.sampleSize << ")\n";
    std::cout << "music.stream.channels = " << music.stream.channels << ")\n";

    PlayMusicStream(music);
    AttachAudioStreamProcessor(music.stream, callback);
    
    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        if (IsKeyPressed(KEY_SPACE)) {
            if (IsMusicStreamPlaying(music)) {
                PauseMusicStream(music);
            }
            else {
                ResumeMusicStream(music);
            }
        }

        int w = GetRenderWidth();
        int h = GetRenderHeight();

        BeginDrawing();
        
        ClearBackground(BLACK);
        float cell_width = (float)w / global_frames_count;
        for (size_t i = 0; i < global_frames_count;i++) {
            float t= global_frames[i].left;
            //std::cout << sample<<" ";
            
            if (t > 0) {
                
                int ypos1 = h / 2;

                int ypos2 = h / 2 - h / 2 * t;
                DrawRectangle(i * cell_width, ypos2, 1, h / 2 * t , RED);
            }
            else {
                
               
                int ypos1 = h / 2;

                int ypos2 = h / 2 - h / 2 * t;
                DrawRectangle(i * cell_width, ypos1, 1, h / 2 * t , RED);
            }
        }
        
        //if(global_frames_count> 0) exit(1);
        EndDrawing();
    }
    
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
