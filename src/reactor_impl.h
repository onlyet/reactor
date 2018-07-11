#ifndef _REACTOR_IMPL_H_
#define _REACTPR_IMPL_H_

class ReactorImplementation
{
public:
    ReactorImplementation();
    ~ReactorImplementation();

    typedef std::map<Handle, EventHandler *>::iterator iterator;

    int register_(EventHandler* handler, Event evt);
    void remove(EventHandler* handler);
    void event_loop(int timeout = 0 );

private:
    EventDemultiplexer* demultiplexer;	//构造的时候创建，析构的时候销毁
    std::map<Handle, EventHandler *>  handlers;    //register创建，remove销毁，析构时全销毁
};

#endif
