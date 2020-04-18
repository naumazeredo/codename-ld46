@ECHO OFF

set OUT_EXE=ld46

set TARGET=x64

set FLAGS=/std:c++latest /FC

if "%1" == "release" (
  echo [Release build]
  set FLAGS=%FLAGS% /MD /EHsc /O2
) else (
  echo [Debug build]
  set FLAGS=%FLAGS% /Zi /MDd /EHsc /Od /permissive-
)

if "%1" == "run" (
  pushd build
  .\%OUT_EXE%.exe
  popd

  exit
)

set DEFS= ^
  /DIMGUI_IMPL_OPENGL_LOADER_GL3W ^
  /DSDL_MAIN_HANDLED ^
  /DSTBI_ONLY_PNG ^
  /DSTB_IMAGE_IMPLEMENTATION

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
  /libpath:%SDL2_LIB% ^
  SDL2.lib ^
  SDL2main.lib ^
  /libpath:%SDL2_MIXER_LIB% ^
  SDL2_mixer.lib ^
  opengl32.lib

set INCS=^
  /I%DEPS_INC% ^
  /I%IMGUI_INC% ^
  /I%SDL2_INC%

set DEP_SRCS=^
  %IMGUI_SRC%\imgui*.cpp ^
  %GL_SRC%\gl3w.c

set SRCS=%DEP_SRCS% ^
  src\main.cpp ^
  src\debug.cpp ^
  src\render.cpp ^
  src\input.cpp ^
  src\audio.cpp ^
  src\game.cpp

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

cl /nologo %FLAGS% %INCS% %SRCS% %DEFS% /Fe%OUT_DIR%\%OUT_EXE%.exe /Fo%OBJ_DIR%\ /Fd%OUT_DIR%\ /link %LIBS% /INCREMENTAL:NO /subsystem:console

pushd build
.\%OUT_EXE%.exe
popd
