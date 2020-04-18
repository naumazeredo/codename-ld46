#pragma once

#include <vector>

#include <SDL.h>
#include <SDL_mixer.h>

#include "types.h"

struct AudioContainer {
  std::vector<Mix_Music*> music;
  std::vector<Mix_Chunk*> sfx;
};

void audio_setup();
void audio_cleanup();

s32 audio_load_music(const char* filename);
s32 audio_load_sfx(const char* filename);

void audio_play_music(u32 music_id, int loops);
void audio_play_sfx(u32 sfx_id);

/*
inline int audio_get_music_volume() { return Mix_VolumeMusic(-1); }
inline void audio_set_music_volume(int volume) { Mix_VolumeMusic(volume); }

inline int audio_get_sfx_volume() { return Mix_Volume(-1, -1); }
inline void audio_set_sfx_volume(int volume) { Mix_VolumeMusic(-1, volume); }

inline void audio_pause_music() { return Mix_PauseMusic(); }
inline void audio_resume_music() { return Mix_ResumeMusic(); }
inline void audio_stop_music() { return Mix_StopMusic(); }

inline bool audio_is_music_playing() { return Mix_PlayingMusic(); }
inline bool audio_is_music_paused()  { return Mix_PlayingMusic() and Mix_PausedMusic(); }
*/
