#pragma once

#include "ofMain.h"
#include "./event/EventDispatcher.h"

#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <cmath>
#include <string>
#include <string.h>
#include <iterator>

class EventManager {
private:
  EventManager() {
    //std::cerr << "EventManager" << std::endl;
  };
  EventDispatcher* dispatcher;

  

public:
  static EventManager& EventManager::getInstance() {
    static EventManager eventManager;
    return eventManager;
  }

  EventDispatcher* getEventDispatcher() {
    if (!dispatcher) {
      dispatcher = new EventDispatcher();
    }
    return dispatcher;
  }

  int lifetime;
};

