#include "animation.h"
#include "time.h"
#include <stdio.h>
#include "render.h"

AnimationInfo animation_info;

namespace animation {

namespace { // internal

void update_instance(u32 id, f64 dt){
  if (!animation_info.instances.count(id)) {
    printf("[error] animation instance not found (update_instance)!\n");
    return;
  }

  AnimationInstance& instance = animation_info.instances[id];
  AnimationSet& animation_set = animation_info.animation_sets[instance.animation_set_id];
  Animation& animation = animation_set.animations[instance.current_animation];

  if (!instance.is_looping &&
      instance.n_of_replays >= animation.max_replays) return;

  auto& current_frame = animation.frames[instance.current_frame];

  instance.frame_time += dt;
  if (instance.frame_time > current_frame.duration) {
    instance.frame_time -= current_frame.duration;
    instance.current_frame = (instance.current_frame + 1) % animation.frames.size();
    instance.n_of_replays += 1;

    if (instance.n_of_replays == animation.max_replays) {
      for (auto callback : animation_info.on_animation_ended) {
        callback(instance.animation_set_id, instance.current_animation);
      }
    }
  }
}

}

void cleanup() {
  animation_info.animation_sets.clear();
  animation_info.instance_count = 0;
  animation_info.instances.clear();
}

void update() {
  auto dt = game_time::get_frame_duration();
  for (auto& [id, instance] : animation_info.instances) {
    update_instance(id, dt);
  }
}

void render() {
  for (auto& [id, instance] : animation_info.instances) {
    auto animation_set = animation_info.animation_sets[instance.animation_set_id];
    auto& animation = animation_set.animations[instance.current_animation];

    render::add_to_render(
      instance.rect.x - instance.rect.w/2,
      instance.rect.y,
      instance.rect.w,
      instance.rect.h,
      animation.frames[instance.current_frame].texture_id,
      instance.z,
      WHITE,
      instance.flip_horizontal
    );
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

  Animation animation;
  animation.frames = frames;
  animation.max_replays = 0;
  return animation;
}

u32 add_animation_set(AnimationSet set) {
  animation_info.animation_sets.push_back(set);
  return animation_info.animation_sets.size() - 1;
}

u32 add_animation_instance(u32 animation_set_id, geom::Rect rect) {
  u32 id = animation_info.instance_count;

  AnimationInstance instance;
  instance.id = id;
  instance.animation_set_id = animation_set_id;
  instance.current_animation = 0;
  instance.current_frame = 0;
  instance.n_of_replays = 0;
  instance.frame_time = 0;
  instance.z = 0;
  instance.rect = rect;
  instance.is_looping = true;
  instance.flip_horizontal = false;

  animation_info.instances[id] = instance;

  animation_info.instance_count++;
  return id;
}

void destroy_instance(u32 id) {
  if (!animation_info.instances.count(id)) {
    printf("[error] animation instance not found! (destroy_instance)\n");
    return;
  }

  animation_info.instances.erase(id);
}

void force_play(u32 instance_id, u32 animation_id){
  if (!animation_info.instances.count(instance_id)) {
    printf("[error] animation instance not found! (force_play)\n");
    return;
  }

  animation_info.instances[instance_id].current_animation = animation_id;
}

void set_animation_instance_pos(u32 id, f32 x, f32 y, f32 w, f32 h, s32 z, bool flip_horizontal) {
  animation_info.instances[id].rect.x = x;
  animation_info.instances[id].rect.y = y;
  animation_info.instances[id].rect.w = w;
  animation_info.instances[id].rect.h = h;
  animation_info.instances[id].z = z;
  animation_info.instances[id].flip_horizontal = flip_horizontal;
}

};
