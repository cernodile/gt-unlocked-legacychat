#pragma once

typedef void(__fastcall *SetFPSLimit_t)(void *, float);
extern SetFPSLimit_t SetFPSLimit_real;

void set_optimal_fps_limit(void);
void __fastcall SetFPSLimit_hook(void *arg, float fps);
