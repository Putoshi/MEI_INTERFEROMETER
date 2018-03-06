#pragma once

#include <string>

class EventDispatcher;
class Event {
  friend EventDispatcher;

public:
  static std::string ECHO_DETECT;

private:
  const std::string _type;
  void *_target;
  void *_listener;

public:
  Event(const std::string &type);
  ~Event();

  virtual Event clone();

  const std::string type();
  void *target();
  void *listener();

private:
};

typedef void(*eventFunctionPtr)(Event &event);