#pragma once

#include <string>
#include <map>
#include <list>

#include "./core/Object.h"
#include "./event/Event.h"

class EventDispatcher :public Object {

public:

protected:
  void *_target;
private:
  typedef struct st_Data {
    void* listener;
    eventFunctionPtr handler;
  } Data;

  std::map<const std::string, std::map<int, std::vector<Data> > > _eventHandlerList;
public:
  EventDispatcher();
  ~EventDispatcher();

  void dispatchEvent(Event *event);
  void addEventListener(const std::string &type, void* listener, eventFunctionPtr handler, bool useCapture = false, int priority = 0, bool useWeakReference = false);
  void removeEventListener(const std::string &type, eventFunctionPtr handler);
  bool hasEventListener(const std::string &type);

protected:

private:

};