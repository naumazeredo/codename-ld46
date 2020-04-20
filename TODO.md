# To do

## Game

### Logic

- [ ] Player controls
  - [x] Movement
  - [x] Item holding
  - [x] Item throwing (attacking = item throwing?)
  - [ ] Attack enemies on interaction (idea)

- [ ] King
  - [x] Basic structure
  - [x] Feed
  - [x] Die

- [ ] Shops / Factories
  - [ ] Money
  - [ ] Buy item
  - [ ] Resolve collision on polygon
  - [x] Place shop
  - [~] (removed) Upgrade (which key? idea: dont have upgrades and always swap)

- [ ] Enemies
  - [ ] Types of enemies
  - [ ] Take damage
  - [ ] AI
  - [ ] Drop money? Drop food? (idea)

- [ ] Items
  - [ ] Traps
  - [ ] Turret
  - [ ] Barricade (idea)

- [ ] Items
  - [x] Item types
  - [ ] Show what it does

- [ ] Extra: Mercenary

### Graphics

- [ ] Player
  - [ ] Idle
  - [ ] Running
  - [ ] Holding item
  - [ ] Throwing
- [ ] King
- [ ] Shops
- [ ] Enemies

- [ ] UI
  - [ ] (use ImGui?)
  - [ ] Money HUD
  - [ ] King hunger
  - [ ] King health (maybe just use hunger)

### Audio

## Engine

- Builds
  - [x] Windows
  - [x] Linux
  - [ ] WebGL (Emscripten)

- Libraries
  - [x] OpenGL (Vulkan?)
  - [x] stb_image
  - [x] SDL_mixer (OpenAL?)

- Systems
  - [x] Rendering system basics
  - [ ] Input system basics
  - [x] Audio system basics
  - [ ] Improved rendering
    - [ ] Render from back to front
    - [x] Animations
  - [ ] Logger
  - [ ] Debugger window
    - [ ] Show "object fields"
  - [ ] Game info
    - [x] Time

-  Refactor
  - [ ] Remove unused SDL_mixer dependencies
  - [ ] Build with DEBUG to add debug window
  - [x] Create debug window on each system

- Bugs
