/*
 * Frames per second counter.
 */
#include <iostream>

#include "view/fps.h"


const unsigned int FRAME_RATE_PERIOD = 1000; // milliseconds


FPS::FPS() {

    // state variables used to calculate the current fps
    current_frame_count = 0;

    // current frames per second - updated every fps_period
    current_fps = 0.0f;

    // time we started counting frames in millisecs
    fps_start_time = 0;
}


void FPS::increment() {
    unsigned int time_now = SDL_GetTicks();
    unsigned int delta_time = time_now - fps_start_time;
        
    // increment the number of frames rendered
    current_frame_count += 1;

    // if it's time to update the fps count then 
    if (delta_time > FRAME_RATE_PERIOD) {

        // change the current_fps 
        current_fps = current_frame_count / FRAME_RATE_PERIOD;

        // FIXME: remove this!
        std::cout << "fps: " << get_fps() << std::endl;

        // and reset the fps counters            
        current_frame_count = 0;
        fps_start_time = time_now;
    }
}


float FPS::get_fps() {
    return current_frame_count;
}
