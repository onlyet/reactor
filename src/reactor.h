#ifndef REACTOR_REACTOR_H_
#define REACTOR_REACTOR_H_

#include "event.h"
#include "event_handle.h"
#include "event_demultiplexer.h"
#include "reactor_impl.h"

class ReactorImplementation;

class Reactor
{
public:
    static Reactor& get_instance();

    int register_(EventHandler * handler, Event evt);
    void remove(EventHandler * handler);
    void event_loop(int timeout = 0);
	
private:
    Reactor();
    ~Reactor();
	Reactor(const Reactor &) = delete;
	Reactor & operator=(const Reactor &) = delete;

private:
    ReactorImplementation* impl;
    //static Reactor reactor;
};

#endif
