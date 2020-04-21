#pragma once

#include <vector>

#include <SDL.h>
#include <SDL_mixer.h>

#include "types.h"

enum class SfxEnum {
  COIN,
  SHOP_CREATE,
  SHOT,
  DUCK,
  SHOP_PRODUCE
};

struct AudioInfo {
  std::vector<Mix_Music*> music;
  std::vector<Mix_Chunk*> sfx;
};

namespace audio {

void setup();
void cleanup();
void debug_window();

u32 load_music(const char* filename);
u32 load_sfx(const char* filename);

void play_music(u32 music_id, int loops);
void play_sfx(u32 sfx_id);
inline void play_sfx(SfxEnum sfx_id) { play_sfx((u32)sfx_id); }

inline int get_music_volume() { return Mix_VolumeMusic(-1); }
inline void set_music_volume(int volume) { Mix_VolumeMusic(volume); }

inline int get_sfx_volume() { return Mix_Volume(-1, -1); }
inline void set_sfx_volume(int volume) { Mix_Volume(-1, volume); }
inline void set_sfx_volume(u32 sfx, int volume) { Mix_Volume(sfx, volume); }

inline void pause_music  () { Mix_PauseMusic();  }
inline void resume_music () { Mix_ResumeMusic(); }
inline void stop_music   () { Mix_HaltMusic();   }

inline bool is_music_playing () { return Mix_PlayingMusic(); }
inline bool is_music_paused  () { return Mix_PlayingMusic() and Mix_PausedMusic(); }


}
