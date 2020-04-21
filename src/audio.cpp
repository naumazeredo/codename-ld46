#include "audio.h"

#include <cassert>
#include <string>

#include <SDL_mixer.h>

#include "externs.h"

AudioInfo audio_info;

namespace audio {

void debug_window() {
  if (ImGui::TreeNode("Audio")) {
    ImGui::Text("Music: (%s)", is_music_playing() ? (is_music_paused() ? "paused" : "playing") : "not playing" );

    if (is_music_playing()) {
      ImGui::SameLine();

      if (is_music_paused()) {
        if (ImGui::Button("resume")) Mix_ResumeMusic();
      } else {
        if (ImGui::Button("pause")) Mix_PauseMusic();
      }

      ImGui::SameLine();
      if (ImGui::Button("stop")) Mix_HaltMusic();
    }

    int music_volume = get_music_volume();
    if (ImGui::SliderInt("Music Volume", &music_volume, 0, MIX_MAX_VOLUME)) {
      set_music_volume(music_volume);
    }

    for (u32 i = 0; i < audio_info.music.size(); i++) {
      std::string music_name = "music " + std::to_string(i);

      if (i > 0) ImGui::SameLine();
      if (ImGui::Button(music_name.c_str())) {
        audio::play_music(i, 0);
      }
    }

    ImGui::Text("SFX:");

    int sfx_volume = get_sfx_volume();
    if (ImGui::SliderInt("SFX Volume", &sfx_volume, 0, MIX_MAX_VOLUME)) {
      set_sfx_volume(sfx_volume);
    }

    for (u32 i = 0; i < audio_info.sfx.size(); i++) {
      std::string sfx_name = "sfx " + std::to_string(i);

      if (i > 0) ImGui::SameLine();
      if (ImGui::Button(sfx_name.c_str())) {
        audio::play_sfx(i);
      }
    }

    ImGui::TreePop();
  }
}

void setup() {
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    printf("Could not start mixer: %s\n", Mix_GetError());
    exit(1);
  }

  load_music("assets/sfx/MetalTheme.ogg");
  set_music_volume(4);

  load_sfx("assets/sfx/coin.wav"); //  COIN,
  load_sfx("assets/sfx/shop.wav"); //SHOP_CREATE
  load_sfx("assets/sfx/shot.wav");
  load_sfx("assets/sfx/duck.wav"); 
  load_sfx("assets/sfx/shop-produce.wav"); //SHOP_PRODUCE

  set_sfx_volume(16);

  play_music(0, -1);
}

void cleanup() {
  for (auto& m : audio_info.music) Mix_FreeMusic(m);
  for (auto& s : audio_info.sfx) Mix_FreeChunk(s);

  audio_info.music.clear();
  audio_info.sfx.clear();

  Mix_CloseAudio();
}

u32 load_music(const char* filename) {
  Mix_Music* music;

  music = Mix_LoadMUS(filename);
  if (!music) {
    printf("Could not load music: %s\n", Mix_GetError());
    return -1;
  }

  audio_info.music.push_back(music);
  return audio_info.music.size() - 1;
}

u32 load_sfx(const char* filename) {
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
