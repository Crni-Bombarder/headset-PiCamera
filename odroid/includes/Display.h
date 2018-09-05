#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include <stdio.h>

#include "Frame.h"
#include "SDL.h"

/* *****************************************************************
    Display.h
    Using the SDL, allow to display in a window a Frame object
***************************************************************** */

// ------ Define -------

// Format of the frame to display
// See https://wiki.libsdl.org/SDL_PixelFormatEnum for all available format
#define SDL_FORMAT SDL_PIXELFORMAT_BGR24

// ----- Functions -----

/*
    Arguments
        int x, y    : Size of the window
        int xt, yt  : Size of the frame to display

    Initialize the window, displayFrame(Frame* frame) can then be called
    to display a frame.

*/
void createWindow(int x, int y, int xt, int yt);

/*
    Arguments
        Frame* frame    : Frame to display

    Display in the window the frame
    Have to call createWindow first with xt and yt the size of the frame

*/
void displayFrame(Frame* frame);

/*
    Arguments
        x, y    : Size of the window

    Resize the window
    The window have first to be created with createWindow

*/
void resizeWindow(int x, int y);

/*
    Arguments
        xt, yt  : Size of the window

    Reallocate the internal texture of the display
    The displayed has then to be of size x y

*/
void resizeTexture(int xt, int yt);

/*

    Close the window, and free all the internal buffer

*/
void destroyWindow(void);

#endif
