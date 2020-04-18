#include "audio.h"

#include <cassert>

#include <SDL_mixer.h>

AudioInfo audio_info;

namespace audio {

void setup() {
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    printf("Could not start mixer: %s\n", Mix_GetError());
    exit(1);
  }
}

void cleanup() {
  for (auto& m : audio_info.music) Mix_FreeMusic(m);
  for (auto& s : audio_info.sfx) Mix_FreeChunk(s);

  audio_info.music.clear();
  audio_info.sfx.clear();

  Mix_CloseAudio();
}

s32 load_music(const char* filename) {
  Mix_Music* music;

  music = Mix_LoadMUS(filename);
  if (!music) {
    printf("Could not load music: %s\n", Mix_GetError());
    return -1;
  }

  audio_info.music.push_back(music);
  return audio_info.music.size() - 1;
}

s32 load_sfx(const char* filename) {
  Mix_Chunk* sample;

  sample = Mix_LoadWAV(filename);
  if (!sample) {
    printf("Could not load sfx: %s\n", Mix_GetError());
    return -1;
  }

  audio_info.sfx.push_back(sample);
  return audio_info.sfx.size() - 1;
}

void play_music(u32 music_id, int loops) {
  assert(music_id < audio_info.music.size());
  assert(loops >= -1);

  auto error = Mix_PlayMusic(audio_info.music[music_id], loops);
  if (error == -1) {
    printf("Could not play music: %s\n", Mix_GetError());
  }

  // @TODO(naum): Fade out/in music
}

void play_sfx(u32 sfx_id) {
  assert(sfx_id < audio_info.sfx.size());

  auto error = Mix_PlayChannel(-1, audio_info.sfx[sfx_id], 0);
  if (error == -1) {
    printf("Could not play sfx: %s\n", Mix_GetError());
  }
}

}
