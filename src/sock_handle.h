#ifndef _REACTOR_SOCK_HANDLE_H_
#define _REACTOR_SOCK_HANDLE_H_

class SockHandle : public EventHandler
{
public:
	SockHandle(Handle fd);
	virtual ~SockHandle() override;

	virtual Handle get_handle() const override;
	virtual void handle_read() override;
	virtual void handle_write() override;
	virtual void handle_error() override;

private:
	Handle sock_fd;
	char* buf;
	static const int MAX_SIZE = 1024;
};

#endif
