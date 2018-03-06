#include "EventDispatcher.h"

//==============================================================
// CONSTRUCTOR / DESTRUCTOR
//==============================================================

//--------------------------------------------------------------
//
EventDispatcher::EventDispatcher() {
  typeID = TYPE_EVENT_DISPATCHER;
  _target = this;
}

//--------------------------------------------------------------
//
EventDispatcher::~EventDispatcher() {
  _target = NULL;
}

//==============================================================
// PUBLIC METHOD
//==============================================================

//--------------------------------------------------------------
//
void EventDispatcher::dispatchEvent(Event *event) {
  if (!hasEventListener(event->type())) return;

  event->_target = _target;

  std::map<int, std::vector<Data> > &list = _eventHandlerList[event->type()];

  for (std::map<int, std::vector<Data> >::reverse_iterator i = list.rbegin(); i != list.rend(); ++i) {
    std::vector<Data> &funcList = i->second;
    for (std::vector<Data>::iterator f = funcList.begin(); f != funcList.end(); ++f) {
      event->_listener = f->listener;
      (*f->handler)(*event);
    }
  }

  delete event;
}

//--------------------------------------------------------------
//
void EventDispatcher::addEventListener(const std::string &type, void* listener, eventFunctionPtr handler, bool useCapture, int priority, bool useWeakReference) {

  Data data;
  data.listener = listener;
  data.handler = handler;

  _eventHandlerList[type][priority].push_back(data);
}

//--------------------------------------------------------------
//
void EventDispatcher::removeEventListener(const std::string &type, eventFunctionPtr handler) {
  if (!hasEventListener(type)) return;

  std::map<int, std::vector<Data> > &allFunctions = _eventHandlerList[type];
  std::map<int, std::vector<Data> >::iterator it;

  it = allFunctions.begin();

  while (it != allFunctions.end()) {
    allFunctions.erase(it);
    //it->second.remove(handler);
    if (it->second.empty()) {
      allFunctions.erase(it++);
    }
    else {
      ++it;
    }
  }

  if (allFunctions.empty()) _eventHandlerList.erase(type);
}

//--------------------------------------------------------------
//
bool EventDispatcher::hasEventListener(const std::string &type) {
  return (_eventHandlerList.find(type) != _eventHandlerList.end());
}

//==============================================================
// PRIVATE METHOD
//==============================================================