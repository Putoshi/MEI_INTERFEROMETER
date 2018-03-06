#include "Event.h"

std::string Event::ECHO_DETECT = "ECHO_DETECT";

//--------------------------------------------------------------
//
Event::Event(const std::string &type) :_type(type) {

}

//--------------------------------------------------------------
//
Event::~Event() {

}

//--------------------------------------------------------------
//
Event Event::clone() {
  Event event(type());
  return event;
}

//--------------------------------------------------------------
//
const std::string Event::type() { return _type; }

//--------------------------------------------------------------
//
void *Event::target() { return _target; }

//--------------------------------------------------------------
//
void *Event::listener() { return _listener; }
