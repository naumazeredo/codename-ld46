#include "debug.h"

#include <string> // @TODO(naum): remove when possible

#include <GL/gl3w.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "types.h"
#include "render.h"
#include "audio.h"
#include "externs.h"

extern u32 tex;
extern int x;
extern int y;

namespace debug {

void setup(SDL_Window* window, SDL_GLContext gl_context) {
  const char* glsl_version = "#version 130";

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  //ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void handle_input(SDL_Event* event) {
  ImGui_ImplSDL2_ProcessEvent(event);
}

void render(SDL_Window* window) {
  ImGuiIO& io = ImGui::GetIO();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();

  {
    ImGui::Begin("Debug");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Separator();

    ImGui::Text("Render");
    ImGui::Text("texture (%d):", tex);

    for (u32 i = 0; i < render_info.texture.size(); i++) {
      auto texture = render_info.texture[i];
      auto texture_w = render_info.texture_w[i];
      auto texture_h = render_info.texture_h[i];

      if (i > 0) ImGui::SameLine();

      ImVec4 tint = ImVec4(1, 1, 1, 0.5);
      if (i == tex) tint = ImVec4(1, 1, 1, 1);

      if (ImGui::ImageButton((void*)(intptr_t)texture,
                             ImVec2(texture_w, texture_h),
                             ImVec2(0,0), ImVec2(1,1),
                             3,
                             ImVec4(0, 0, 0, 0),
                             tint)) {
        tex = i;
      }
    }

    ImGui::SliderInt("x", &x, 0, 1280);
    ImGui::SliderInt("y", &y, 0, 720);

    ImGui::Separator();

    ImGui::Text("Audio");

    ImGui::Text("Music: (%s)", audio::is_music_playing() ? (audio::is_music_paused() ? "paused" : "playing") : "not playing" );

    if (Mix_PlayingMusic()) {
      ImGui::SameLine();

      if (Mix_PausedMusic()) {
        if (ImGui::Button("resume")) Mix_ResumeMusic();
      } else {
        if (ImGui::Button("pause")) Mix_PauseMusic();
      }

      ImGui::SameLine();
      if (ImGui::Button("stop")) Mix_HaltMusic();
    }

    int music_volume = audio::get_music_volume();
    if (ImGui::SliderInt("Music Volume", &music_volume, 0, MIX_MAX_VOLUME)) {
      audio::set_music_volume(music_volume);
    }

    for (u32 i = 0; i < audio_info.music.size(); i++) {
      std::string music_name = "music " + std::to_string(i);

      if (i > 0) ImGui::SameLine();
      if (ImGui::Button(music_name.c_str())) {
        audio::play_music(i, 0);
      }
    }

    ImGui::Text("SFX:");

    int sfx_volume = audio::get_sfx_volume();
    if (ImGui::SliderInt("SFX Volume", &sfx_volume, 0, MIX_MAX_VOLUME)) {
      audio::set_sfx_volume(sfx_volume);
    }

    for (u32 i = 0; i < audio_info.sfx.size(); i++) {
      std::string sfx_name = "sfx " + std::to_string(i);

      if (i > 0) ImGui::SameLine();
      if (ImGui::Button(sfx_name.c_str())) {
        audio::play_sfx(i);
      }
    }

    ImGui::End();
  }

  ImGui::Render();
  //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
