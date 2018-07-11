#include <map>
#include <vector>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include "stdio.h"

#include "event.h"
#include "event_handle.h"
#include "event_demultiplexer.h"
#include "epoll_demultiplexer.h"

#include <poll.h>
#include <stdlib.h>

EpollDemultiplexer::EpollDemultiplexer()
:max_fd(0)
{
    //epoll_fd = epoll_create( 1024 );
	epoll_fd = epoll_create1(EPOLL_CLOEXEC);
}

EpollDemultiplexer::~EpollDemultiplexer()
{
    close( epoll_fd );
}

int EpollDemultiplexer::wait_event(  std::map<Handle, EventHandler *>&  handlers,
                                int timeout )
{
    std::vector<struct epoll_event> evs(max_fd); 

    int num = epoll_wait( epoll_fd, &evs[0], max_fd, timeout );

    if ( num > 0 )
    {
        for ( int i = 0; i < num; ++i )
        {
            Handle handle =  evs[i].data.fd ;

            if ( (POLLERR | POLLNVAL) & evs[i].events )
            {
                (handlers[ handle ])->handle_error();
            }
            else
            {
                if ( (POLLIN | POLLPRI | POLLRDHUP) & evs[i].events )
                {
                    handlers[ handle ]->handle_read();
                }
                if ( EPOLLOUT & evs[i].events )
                {
                    handlers[ handle ]->handle_write();
                }
            }
        }
    }
    else if ( num < 0 )
    {
        printf("epoll_wait error %s\n", strerror(errno) ); 
    }

    return num;
}

int EpollDemultiplexer::register_(Handle handle, Event evt)
{
    struct epoll_event ev;


    if ( evt & ReadEvent )
    {
		ev.events = EPOLLIN;
        //ev.events |= EPOLLIN;	//fck you
		//ev.events = EPOLLIN | EPOLLERR;
    }
    if ( evt & WriteEvent )
    {
        ev.events = EPOLLOUT;    
    }
    ev.data.fd = handle;
    ev.events |= EPOLLET;

	int ret;
    if ( 0 != (ret = epoll_ctl( epoll_fd, EPOLL_CTL_ADD, handle, &ev )) )
    {
		printf("ret = %d\n", ret);
		perror("epoll_ctl");
		exit(-1);
		//ENOENT
		//op was EPOLL_CTL_MOD or EPOLL_CTL_DEL, and fd is not registered with this epoll instance.
        //if ( errno == ENOENT ) 
        //{
        //    if ( 0 != epoll_ctl( epoll_fd, EPOLL_CTL_ADD, handle, &ev ) )
        //    {
        //        printf("epoll_ctrl add error : %s\n", strerror(errno));
        //        return -errno;
        //    }
        //    ++max_fd;
        //}
    }
    else
        ++max_fd;

    return 0;
}

int EpollDemultiplexer::remove(Handle handle)
{
    struct epoll_event ev;

    if ( 0 != epoll_ctl( epoll_fd, EPOLL_CTL_DEL, handle, &ev ) )
    {
        printf("epoll_ctrl del error : %s\n", strerror(errno));
        return -errno;
    }

    --max_fd;
    return 0;
}

