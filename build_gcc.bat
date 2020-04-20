@ECHO OFF

set OUT_EXE=ld46

set TARGET=x64

set FLAGS=-std=c++17 -fpermissive

if "%1" == "release" (
  echo [Release build]
  set FLAGS=%FLAGS% -O2
) else (
  echo [Debug build]
  set FLAGS=%FLAGS% -g -O0
)

if "%1" == "run" (
  pushd build
  .\%OUT_EXE%.exe
  popd

  exit
)

set DEFS= ^
  -DIMGUI_IMPL_OPENGL_LOADER_GL3W ^
  -DSDL_MAIN_HANDLED ^
  -DSTBI_ONLY_PNG ^
  -DSTB_IMAGE_IMPLEMENTATION

set DEPS_LIB=deps\lib
set DEPS_INC=deps\inc
set DEPS_SRC=deps\src

@REM SDL2
set SDL2_LIB=%DEPS_LIB%\SDL2\%TARGET%
set SDL2_INC=%DEPS_INC%\SDL2
set SDL2_MIXER_LIB=%DEPS_LIB%\SDL2_mixer\%TARGET%

@REM GL GL3W
set GL_SRC=%DEPS_SRC%\GL

@REM ImGUI
set IMGUI_SRC=%DEPS_SRC%\imgui
set IMGUI_INC=%DEPS_INC%\imgui

set LIBS=^
  -L%SDL2_LIB% ^
  -lSDL2 ^
  -lSDL2main ^
  -L%SDL2_MIXER_LIB% ^
  -lSDL2_mixer ^
  -lopengl32

set INCS=^
  -I%DEPS_INC% ^
  -I%IMGUI_INC% ^
  -I%SDL2_INC%

set DEP_SRCS=^
  %IMGUI_SRC%\imgui.cpp ^
  %IMGUI_SRC%\imgui_draw.cpp ^
  %IMGUI_SRC%\imgui_impl_opengl3.cpp ^
  %IMGUI_SRC%\imgui_impl_sdl.cpp ^
  %IMGUI_SRC%\imgui_widgets.cpp ^
  %GL_SRC%\gl3w.c

set SRCS=%DEP_SRCS%
setlocal EnableDelayedExpansion
for /R src %%I in (*.cpp) do SET SRCS="%%I" !SRCS!
setlocal DisableDelayedExpansion

set FOLDERS=^
  assets

set FILES=^
  %SDL2_LIB%\SDL2.dll ^
  %SDL2_MIXER_LIB%\*.dll

set OUT_DIR=build
set OBJ_DIR=%OUT_DIR%\obj
set SYM_DIR=%OUT_DIR%\sym

rmdir /s /q %OUT_DIR%

mkdir %OUT_DIR%
mkdir %OBJ_DIR%

for %%I in (%FOLDERS%) do xcopy /sei %%I %OUT_DIR%\%%I > nul
for %%I in (%FILES%) do xcopy %%I %OUT_DIR% > nul

g++ %SRCS% %FLAGS% %INCS% %DEFS% -o %OUT_DIR%\%OUT_EXE%.exe %LIBS%

pushd build
.\%OUT_EXE%.exe
popd
