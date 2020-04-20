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

  void update() {
    auto dt = game_time::get_frame_duration();
    for (u32 i = 0; i < animation_system.animations_sets.size(); i++) {
      auto& animation_set = animation_system.animations_sets[i];
      auto animation_id = animation_set.current_animation;
      auto& animation = animation_set.animations[animation_id];
      update_animation(animation, i, animation_id, dt);
    }
  }

  Animation generate_animation_from_files(const char* prefix, geom::Rect rect, u32 n_of_frames){
    std::vector<Frame> frames;

    u32 texture_id;
    for (u32 i = 0 ; i < n_of_frames; i++){
      char file_name[50];
      sprintf(file_name, "%s_%d.png", prefix, i+1);
      texture_id = render::load_image(file_name);

      frames.push_back({texture_id, 0.2});
    }

    Animation animation = {frames, rect, 0.0, 0, 0, 10,  true};
    return animation;
  }

  void debug_animation() {
    auto animation = generate_animation_from_files("assets/gfx/animations/goose", {0, 0, 84, 84}, 4);
    std::vector<Animation> animations{animation};
    AnimationSet set{animations, 0};

    add_animation_set(set);
  }

  void render() {
    for (auto& animation_set : animation_system.animations_sets) {
      auto animation_id = animation_set.current_animation;
      auto& animation = animation_set.animations[animation_id];

      render::add_to_render(
        animation.rect.x,
        animation.rect.y,
        animation.rect.w,
        animation.rect.h,
        animation.frames[animation.current_frame].texture_id
      );
    }
  }

  u32 add_animation_set(AnimationSet set) {
    animation_system.animations_sets.push_back(set);
    return animation_system.animations_sets.size() - 1;
  }
};
