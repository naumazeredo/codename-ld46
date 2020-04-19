#pragma once

#include "types.h"
#include <vector>

namespace shop {

enum State {CLOSED, OPEN};

void setup();
void update();

} // namespace shop

struct Shop {
    Point center;
    Rect collider, trigger;

    shop::State state; 

    u32 textures[2];

    Shop(Point p, Rect c, Rect t);

    void sell();
};

struct ShopsInfo {
    std::vector<Shop> shops;
};