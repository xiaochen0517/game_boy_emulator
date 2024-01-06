#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static emu_context ctx;

emu_context *emu_get_context()
{
  return &ctx;
}

void delay(u32 ms)
{
  SDL_Delay(ms);
}

int emu_run(int argc, char **argv)
{
  // check argments
  if (argc < 2)
  {
    printf("Usage: %s <rom>\n", argv[0]);
    return 1;
  }

  if (!cart_load(argv[1]))
  {
    printf("Failed to load ROM.\n");
    return 1;
  }

  printf("ROM loaded.\n");

  SDL_Init(SDL_INIT_VIDEO);
  printf("SDL initialized.\n");
  TTF_Init();
  printf("TTF initialized.\n");

  cpu_init();

  ctx.running = true;
  ctx.paused = false;
  ctx.ticks = 0;

  while (ctx.running)
  {
    if (ctx.paused)
    {
      delay(100);
      continue;
    }
    if (!cpu_step())
    {
      printf("CPU dead.\n");
      ctx.running = false;
      return -3;
    }
    ctx.ticks++;
  }

  return 0;
}