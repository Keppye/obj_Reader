#ifndef RENDER_H
#define RENDER_H

#include "structs.h"

void renderSettings();
void drawEverything(Obj object, Point2D rotation, SDL_Window* mainWindow, double zoom);

#endif // RENDER_H