#include <SDL.h>
#include "structs.h"
#include <iostream>

void eventHandler(int &menu, double &zoom, Mouse &mouse, Obj obj){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT: menu = MENU_QUIT; break;
			case SDL_MOUSEBUTTONDOWN: 
				mouse.buttonPressed = true;
				if(menu == MENU_START){
					SDL_Rect rect = { 288, 2, 10, 10 };
					SDL_Point mousePos = {mouse.x, mouse.y};
					if(SDL_PointInRect(&mousePos, &rect)){
						menu = MENU_QUIT;
					}
				}
			break;
			case SDL_MOUSEBUTTONUP: mouse.buttonPressed = false; break;
			case SDL_MOUSEWHEEL:
				if(menu == MENU_GLDISPLAY){
					if(event.wheel.y > 0 && zoom > 1){
						zoom -= (obj.offset.y + 1) / 10;
					} else if(event.wheel.y < 0){
						zoom += (obj.offset.y + 1) / 10;
					}
				}
			break;
		}
	}
}

Point2D getRotationFromCursor(SDL_Window* window, Mouse &currentPos, Mouse oldPos, Point2D currentRot) {
	if(currentPos.buttonPressed){
		currentRot.x += (currentPos.y - oldPos.y) / 5;
		currentRot.y += (currentPos.x - oldPos.x) / 5;
	}
	return currentRot;
}
