#ifndef _REACTOR_LISTEN_HANDLE_H_
#define _REACTOR_LISTEN_HANDLE_H_

class ListenHandle : public EventHandler
{
public:
	ListenHandle(int fd);
	virtual ~ListenHandle() override;

	virtual Handle get_handle() const override { return listen_fd; }
	virtual void handle_read() override;
	virtual void handle_write() override {}
	virtual void handle_error() override {}

private:
	Handle listen_fd;
};

#endif
