#pragma once
#include <SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class AudioManager {
private:
    Mix_Music* music = nullptr;  // 1 music 
    unordered_map<string, Mix_Chunk*> soundEffects;  // save sound effect
    bool allowSoundEffects = true;  // check sound effects
    vector<int> activeChannels;  // save effect list

public:
    AudioManager() {}

    bool init() {   //init SDL_Mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            cerr << "SDL_mixer Error: " << Mix_GetError() << endl;
            return false;
        }
        Mix_AllocateChannels(16); 
        return true;
    }

    bool loadMusic(const string& path) {    //upload music
        music = Mix_LoadMUS(path.c_str());
        if (!music) {
            cerr << "Failed to load music: " << Mix_GetError() << endl;
            return false;
        }
        return true;
    }

    bool loadSound(const string& name, const string& path) {    //upload sound effect
        Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
        if (!sound) {
            cerr << "Failed to load sound: " << Mix_GetError() << endl;
            return false;
        }
        soundEffects[name] = sound;
        return true;
    }

    void playMusic() {    //play music
        if (music) {
            Mix_PlayMusic(music, -1);
        }
    }

    void playSound(const string& name) { //play sound effect
        if (!allowSoundEffects) {
            cout << "Sound effects are disabled!" << endl;
            return; 
        }

        if (soundEffects.find(name) != soundEffects.end()) {
            int channel = Mix_PlayChannel(-1, soundEffects[name], 0);
            if (channel == -1) {
                cerr << "Failed to play sound: " << Mix_GetError() << endl;
            }
            else {
                activeChannels.push_back(channel); // if channel on, push in vector
            }
        }
        else {
            cerr << "Sound effect '" << name << "' not found!" << endl;
        }
    }

    int getPlayingChannels() {
        return Mix_Playing(-1); //return channel is playing
    }

    void stopMusic() { Mix_HaltMusic(); }
    void pauseMusic() { Mix_PauseMusic(); }
    void resumeMusic() { Mix_ResumeMusic(); }

    void stopAllSounds() {
        cout << "Stopping all sounds..." << endl;
        Mix_HaltChannel(-1); 
        activeChannels.clear();
        allowSoundEffects = false; 
        if (Mix_GetError()[0] != '\0') {
            cerr << "Mix_HaltChannel Error: " << Mix_GetError() << endl;
        }
    }

    void resumeSounds() {
        allowSoundEffects = true;
        Mix_Resume(-1);
    }


    void close() {
        if (music) {
            Mix_FreeMusic(music);
            music = nullptr;
        }
        for (auto& pair : soundEffects) {
            Mix_FreeChunk(pair.second);
        }
        soundEffects.clear();
        activeChannels.clear();
        Mix_CloseAudio();
    }

    ~AudioManager() {
        close();
    }
};