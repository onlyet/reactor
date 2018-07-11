#ifndef _REACTOR_EPOLL_H_
#define _REACTOR_EPOLL_H_

class EpollDemultiplexer : public EventDemultiplexer
{
public:
	EpollDemultiplexer();
	virtual ~EpollDemultiplexer() override;

	virtual int wait_event(std::map<Handle, EventHandler *>&  handlers,
		int timeout = 0) override;
	virtual int register_(Handle handle, Event evt) override;
	virtual int remove(Handle handle) override;

private:
	int max_fd;
	int epoll_fd;
};

#endif
