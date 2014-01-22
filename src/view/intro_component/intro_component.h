#ifndef INTRO_COMPONENT_H
#define INTRO_COMPONENT_H
/*
 * Game component that plays the intro
 *
 */
#include "view/i_component.h"

class IntroComponent : public IComponent {
 public:
    // start displaying this component
    void start(View & view);
};

#endif 


