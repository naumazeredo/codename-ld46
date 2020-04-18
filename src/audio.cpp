#include "audio.h"

#include <cassert>

#include <SDL_mixer.h>

AudioContainer audio_container;
u8 audio_music_volume;
u8 audio_sfx_volume;

void audio_setup() {
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    printf("Could not start mixer: %s\n", Mix_GetError());
    exit(1);
  }
}

void audio_cleanup() {
  for (auto& m : audio_container.music) Mix_FreeMusic(m);
  for (auto& s : audio_container.sfx) Mix_FreeChunk(s);

  audio_container.music.clear();
  audio_container.sfx.clear();

  Mix_CloseAudio();
}

s32 audio_load_music(const char* filename) {
  Mix_Music* music;

  music = Mix_LoadMUS(filename);
  if (!music) {
    printf("Could not load music: %s\n", Mix_GetError());
    return -1;
  }

  audio_container.music.push_back(music);
  return audio_container.music.size() - 1;
}

s32 audio_load_sfx(const char* filename) {
  Mix_Chunk* sample;

  sample = Mix_LoadWAV(filename);
  if (!sample) {
    printf("Could not load sfx: %s\n", Mix_GetError());
    return -1;
  }

  audio_container.sfx.push_back(sample);
  return audio_container.sfx.size() - 1;
}

void audio_play_music(u32 music_id, int loops) {
  assert(music_id < audio_container.music.size());
  assert(loops >= -1);

  auto error = Mix_PlayMusic(audio_container.music[music_id], loops);
  if (error == -1) {
    printf("Could not play music: %s\n", Mix_GetError());
  }

  // @TODO(naum): Fade out/in music
}

void audio_play_sfx(u32 sfx_id) {
  assert(sfx_id < audio_container.sfx.size());

  auto error = Mix_PlayChannel(-1, audio_container.sfx[sfx_id], 0);
  if (error == -1) {
    printf("Could not play sfx: %s\n", Mix_GetError());
  }
}
