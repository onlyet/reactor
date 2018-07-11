#ifndef _REACTOR_SELECT_H_
#define _REACTOR_SELECT_H_

#include <map>
#include "event_demultiplexer.h"
#include "event_handle.h"
#include "event.h"

class SelectDemultiplexer : public EventDemultiplexer
{
public:
	SelectDemultiplexer();
	virtual ~SelectDemultiplexer() override;

	virtual int wait_event(std::map<Handle, EventHandler *>&  handlers,
		int timeout = 0) override;
	virtual int register_(Handle handle, Event evt) override;
	virtual int remove(Handle handle) override;

private:
	fd_set read_set;
	fd_set write_set;
	fd_set err_set;
};

#endif
