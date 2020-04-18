#include "debug.h"

#include <string> // @TODO(naum): remove when possible

#include <GL/gl3w.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "types.h"
#include "render.h"
#include "audio.h"

extern RenderContainer render_container;
extern u32 tex;
extern int x;
extern int y;

extern AudioContainer audio_container;

void setup_debug(SDL_Window* window, SDL_GLContext gl_context) {
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

void cleanup_debug() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void handle_debug_input(SDL_Event* event) {
  ImGui_ImplSDL2_ProcessEvent(event);
}

void render_debug_window(SDL_Window* window) {
  ImGuiIO& io = ImGui::GetIO();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();

  {
    ImGui::Begin("Debug");                          // Create a window called "Hello, world!" and append into it.
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    //ImGui::ColorEdit3("Background color", (float*)&clear_color); // Edit 3 floats representing a color

    //ImGui::SliderFloat("frequency", &frequency, -5.0f, 5.0f);

    ImGui::Separator();

    ImGui::Text("Render");
    ImGui::Text("texture (%d):", tex);

    for (u32 i = 0; i < render_container.texture.size(); i++) {
      auto texture = render_container.texture[i];
      auto texture_w = render_container.texture_w[i];
      auto texture_h = render_container.texture_h[i];

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

    ImGui::Text("Music: (%s)", Mix_PlayingMusic() ? (Mix_PausedMusic() ? "paused" : "playing") : "not playing" );

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

    int music_volume = Mix_VolumeMusic(-1);
    if (ImGui::SliderInt("Music Volume", &music_volume, 0, MIX_MAX_VOLUME)) {
      Mix_VolumeMusic(music_volume);
    }

    for (u32 i = 0; i < audio_container.music.size(); i++) {
      std::string music_name = "music " + std::to_string(i);

      if (i > 0) ImGui::SameLine();
      if (ImGui::Button(music_name.c_str())) {
        audio_play_music(i, 0);
      }
    }

    ImGui::Text("SFX:");

    int sfx_volume = Mix_Volume(-1, -1);
    if (ImGui::SliderInt("SFX Volume", &sfx_volume, 0, MIX_MAX_VOLUME)) {
      Mix_Volume(-1, sfx_volume);
    }

    for (u32 i = 0; i < audio_container.sfx.size(); i++) {
      std::string sfx_name = "sfx " + std::to_string(i);

      if (i > 0) ImGui::SameLine();
      if (ImGui::Button(sfx_name.c_str())) {
        audio_play_sfx(i);
      }
    }


    //static float f = 0.0f;
    //static int counter = 0;
    //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //ImGui::Checkbox("Another Window", &show_another_window);
    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //  counter++;
    //ImGui::SameLine();
    //ImGui::Text("counter = %d", counter);

    ImGui::End();
  }

  /*
  if (show_another_window) {
    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
      show_another_window = false;
    ImGui::End();
  }
  */

  ImGui::Render();
  //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
