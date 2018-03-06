#pragma once

#include "ofMain.h"

#define TYPE_STAGE                      0
#define TYPE_OBJECT                     1
#define TYPE_EVENT_DISPATCHER           2
#define TYPE_DISPLAY_OBJECT             3
#define TYPE_INTERACTIVE_OBJECT         4
#define TYPE_DISPLAY_OBJECT_CONTAINER   5
#define TYPE_SPRITE                     6
#define TYPE_MOVIE_CLIP                 7
#define TYPE_SHAPE                      8
#define TYPE_BITMAP                     9
#define TYPE_TEXT_FIELD                10

class Object {

public:
  Object();
  ~Object();

  int typeID;
};