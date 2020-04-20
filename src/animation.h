#pragma once

#include <vector>
#include <map>
#include <functional>
#include <utility>

#include "types.h"
#include "geom.h"

using AnimationEndedCallback = std::function< void (u32,u32) >;

struct Frame {
  u32 texture_id;
  f32 duration;
};

struct Animation {
  std::vector<Frame> frames;
  u32 max_replays;
};

struct AnimationSet {
  std::vector<Animation> animations;
};

struct AnimationInstance {
  u32 id;
  u32 animation_set_id;
  u32 current_animation;
  u32 current_frame;
  u32 n_of_replays;
  f32 frame_time;

  s32 z;
  geom::Rect rect;

  bool is_looping;
  bool flip_horizontal;
};

struct AnimationInfo {
  u32 instance_count;
  std::map<u32, AnimationInstance> instances;
  std::vector<AnimationSet> animation_sets;
  std::vector<AnimationEndedCallback> on_animation_ended;
};

namespace animation {

void update();
void render();

Animation generate_animation_from_files(const char* prefix, u32 n_of_frames);

u32 add_animation_set(AnimationSet);
u32 add_animation_instance(u32, geom::Rect);

void destroy_instance(u32);

void force_play(u32 instance_id, u32 animation_id);
void set_animation_instance_pos(u32 set_id, f32 x, f32 y, f32 w, f32 h, s32 z, bool flip_horizontal = false);

};
