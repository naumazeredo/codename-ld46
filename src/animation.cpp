#include "animation.h"
#include "time.h"
#include <stdio.h>
#include "render.h"

animation::System animation_system;

namespace animation {
  void update_animation(Animation& animation, u32 set_id, u32 animation_id,f32 dt){
    if (!animation.is_looping &&
        animation.n_of_replays >= animation.max_replays) return;

    auto& current_frame = animation.frames[animation.current_frame];

    animation.frame_time += dt;
    if (animation.frame_time > current_frame.duration) {
      animation.frame_time -= current_frame.duration;
      animation.current_frame = (animation.current_frame + 1) % animation.frames.size();
      animation.n_of_replays += 1;

      if (animation.n_of_replays == animation.max_replays) {
        for (auto callback : animation_system.on_animation_ended) {
          callback(set_id, animation_id);
        }
      }
    }
  }

  void set_animation_pos(u32 set_id, f32 x, f32 y, f32 w, f32 h, bool flip_horizontal) {
    animation_system.animations_sets[set_id].rect.x = x;
    animation_system.animations_sets[set_id].rect.y = y;
    animation_system.animations_sets[set_id].rect.w = w;
    animation_system.animations_sets[set_id].rect.h = h;
    animation_system.animations_sets[set_id].flip_horizontal = flip_horizontal;
  }

  void update() {
    auto dt = game_time::get_frame_duration();
    for (u32 i = 0; i < animation_system.animations_sets.size(); i++) {
      auto& animation_set = animation_system.animations_sets[i];
      auto animation_id = animation_set.current_animation;
      auto& animation = animation_set.animations[animation_id];
      update_animation(animation, i, animation_id, dt);
    }
  }

  Animation generate_animation_from_files(const char* prefix, u32 n_of_frames){
    std::vector<Frame> frames;

    u32 texture_id;
    for (u32 i = 0 ; i < n_of_frames; i++){
      char file_name[50];
      sprintf(file_name, "%s_%d.png", prefix, i+1);
      texture_id = render::load_image(file_name);

      frames.push_back({texture_id, 0.2});
    }

    Animation animation = {frames, 0.0, 0, 0, 0,  true};
    return animation;
  }

  void debug_animation() {
    auto rect = geom::Rect{0, 0, 84, 84};
    auto animation = generate_animation_from_files("assets/gfx/animations/goose", 4);
    std::vector<Animation> animations{animation};
    AnimationSet set{rect, animations, 0};

    add_animation_set(set);
  }

  void render() {
    for (auto& animation_set : animation_system.animations_sets) {
      auto animation_id = animation_set.current_animation;
      auto& animation = animation_set.animations[animation_id];

      render::add_to_render(
        animation_set.rect.x - animation_set.rect.w/2,
        animation_set.rect.y,
        animation_set.rect.w,
        animation_set.rect.h,
        animation.frames[animation.current_frame].texture_id,
        WHITE,
        animation_set.flip_horizontal
      );
    }
  }

  u32 add_animation_set(AnimationSet set) {
    animation_system.animations_sets.push_back(set);
    return animation_system.animations_sets.size() - 1;
  }

  void force_play(u32 set_id,u32 animation_id){
    animation_system.animations_sets[set_id].current_animation = animation_id;
  }
};
