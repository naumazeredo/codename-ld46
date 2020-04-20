# To do

## Game

- [ ] Terrain
  - [x] Position king
  - [x] Position shop places
  - [ ] Position enemies spawn points
  - [ ] Position out-of-bounds
  - [ ] Position colliders
  - [x] Position king rock
  - [x] Position logs (bounds)

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
  - [x] Money
  - [x] Buy item
  - [x] Resolve collision on polygon
  - [x] Place shop
  - [~] (removed) Upgrade (which key? idea: dont have upgrades and always swap)

- [ ] Enemies
  - [ ] Types of enemies
  - [x] Take damage
  - [ ] AI
  - [ ] Drop money? Drop food? (idea)

- [ ] Items
  - [x] Traps
  - [x] Turret
  - [x] Money?
  - [ ] Barricade (idea)

- [ ] Items
  - [x] Item types
  - [ ] Show what it does

- [ ] Game Progress
  - [x] Waves
    - [x] Enemy spawns
    - [x] Timer

- [ ] Extra: Mercenary

### Graphics

- [ ] Player
  - [x] Idle
  - [x] Running
  - [ ] Holding item
  - [ ] Throwing
- [x] King
- [ ] Shops
- [ ] Enemies

- [ ] UI
  - [ ] (use ImGui?)
  - [ ] Money HUD
  - [ ] King hunger
  - [x] King health (maybe just use hunger)

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
  - [ ] Destruct animation on destroy item
  - [ ] Support multiple items with same animation set
