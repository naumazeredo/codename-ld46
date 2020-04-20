#pragma once

#include <vector>
#include <functional>
#include <utility>
//#include <GL/gl3w.h>

#include "types.h"
#include "geom.h"

using AnimationEndedCallback = std::function< void (u32,u32) >;

namespace animation {
  struct Frame {
    u32 texture_id;
    f32 duration;
  };

  struct Animation {
    std::vector<Frame> frames;

    f32 frame_time;
    u32 n_of_replays;
    u32 current_frame;
    u32 max_replays;
    bool is_looping;
  };

  struct AnimationSet {
    geom::Rect rect;
    std::vector<Animation> animations;
    u32 current_animation;
    bool flip_horizontal;
    bool is_disabled;
  };

  struct System {
    std::vector<AnimationSet> animations_sets;
    std::vector<AnimationEndedCallback> on_animation_ended;
  };


  // returns animation id
  u32 add_animation_set(AnimationSet);
  void force_play(u32 set_id,u32 animation_id);

  void render();
  void update();
  void set_animation_pos(u32 set_id, f32 x, f32 y, f32 w, f32 h, bool flip_horizontal = false);
  void set_is_animation_disabled(u32 set_id, bool is_disabled);
  void debug_animation();
  Animation generate_animation_from_files(const char* prefix, u32 n_of_frames);
};
