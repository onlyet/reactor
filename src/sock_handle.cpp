#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <map>

#include "event_handle.h"
#include "reactor.h"
#include "sock_handle.h"

SockHandle::SockHandle(Handle fd)
	:sock_fd(fd), buf(new char[MAX_SIZE])
{
	memset(buf, 0, MAX_SIZE);
}

SockHandle::~SockHandle()
{
	close(sock_fd);
	delete[] buf;
}

Handle SockHandle::get_handle() const
{
	return sock_fd;
}

void SockHandle::handle_read()
{
	int n;
	if (0 < (n = read(sock_fd, buf, MAX_SIZE)))
	{
		write(sock_fd, buf, n);
	}
	handle_error();	//读写一次就关闭连接，相当于短连接？
}

void SockHandle::handle_write()
{

}

void SockHandle::handle_error()
{
	Reactor& r = Reactor::get_instance();
	r.remove(this);
}

