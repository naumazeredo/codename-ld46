#pragma once

#include <vector>
#include <functional>
#include <utility>
//#include <GL/gl3w.h>

#include "types.h"
#include "geom.h"

using AnimationEndedCallback = std::function< std::pair<u32,u32> (void) >;

namespace animation {
  struct Frame {
    u32 texture_id;
    f32 duration;
  };

  struct Animation {
    std::vector<Frame> frames;
    geom::Rect rect;

    f32 frame_time;
    u32 n_of_replays;
    u32 current_frame;
    u32 max_replays;
    bool is_looping;
  };

  struct AnimationSet {
    std::vector<Animation> animations;
    u32 current_animation;
  };

  struct System {
    std::vector<AnimationSet> animations_sets;
    std::vector<AnimationEndedCallback> on_animation_ended;
  };


  // returns animation id
  u32 add_animation_set(AnimationSet);
  void force_play(u32 set_id,u32 animation_id, bool should_loop = true);

  void render();
  void update();
  void debug_animation();
};
