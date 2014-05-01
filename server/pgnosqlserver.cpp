#include "pgnosqlserver.h"

PgnosqlServer::PgnosqlServer()
{
	//default logging options.
	logOption.fileLogging = false;
	logOption.consoleLogging = true;

	//configure LOGGING
	LOGGING_CONFIG
	logger = Logger::getLogger( "PgnosqlServer");
	pgConn = NULL;
}

PgnosqlServer::PgnosqlServer(std::map<std::string,std::string> config)
{
	logOption.fileLogging = (std::stoi(config["filelog"]) == 1);
	logOption.consoleLogging = (std::stoi(config["consolelog"]) == 1);

    connLimit = std::stoi(config["connection_limit"]);
	username = config["username"];
	password = config["password"];
	port = std::stoi(config["port"]);

	pgConn = new PGConnection(config);

	//configure LOGGING
	LOGGING_CONFIG
	logger = Logger::getLogger("PgnosqlServer");
}

int PgnosqlServer::init()
{
	pgConn->connect();
	LOG4CXX_INFO(logger, "Connected!");
}
int PgnosqlServer::setSocket(int sfd)
{
	int flags, s;

	flags = fcntl (sfd, F_GETFL, 0);
	if (flags == -1)
	{
		LOG4CXX_FATAL(logger, "setSocket Error fcntl F_GETFL");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl (sfd, F_SETFL, flags);
	if (s == -1)
	{
	  LOG4CXX_FATAL(logger, "setSocket Error fcntl F_GETFL");
	  return -1;
	}

	return 0;
}

int PgnosqlServer::bindSocket()
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;

	memset (&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
	hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
	hints.ai_flags = AI_PASSIVE;     /* All interfaces */

	s = getaddrinfo (NULL, std::to_string(port).c_str(), &hints, &result);
	if (s != 0)
	{
	  LOG4CXX_FATAL(logger,"bindSocket Error: "<< gai_strerror (s));
	  return -1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0)
		{
		  /* We managed to bind successfully! */
		  break;
		}

		close (sfd);
	}

	if (rp == NULL)
	{
	  LOG4CXX_FATAL(logger,"bindSocket Error could not bind.");
	  return -1;
	}

	freeaddrinfo (result);

	return sfd;
}

int PgnosqlServer::run()
{
	int sfd, s;
	int efd;
	struct epoll_event event;
	struct epoll_event *events;

	sfd = bindSocket();
	if (sfd == -1)
    abort ();

	s = setSocket(sfd);
	if (s == -1)
		abort ();

	s = listen (sfd, SOMAXCONN);
	if (s == -1)
    {
		LOG4CXX_FATAL(logger,"run Error, could not listen");
		abort ();
    }

	efd = epoll_create1 (0);
	if (efd == -1)
    {
		LOG4CXX_FATAL(logger,"run Error, epoll_create failed");
		abort ();
    }

	event.data.fd = sfd;
	event.events = EPOLLIN | EPOLLET;
	s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
	if (s == -1)
    {
		LOG4CXX_FATAL(logger,"run Error, epoll_ctl failed");
		abort ();
    }

	/* Buffer where events are returned */
	events = (epoll_event*) calloc (connLimit, sizeof event);

	/* The event loop */
	while (1)
    {
		int n, i;

		n = epoll_wait (efd, events, connLimit, -1);
		for (i = 0; i < n; i++)
		{
			if ((events[i].events & EPOLLERR) ||
				  (events[i].events & EPOLLHUP) ||
				  (!(events[i].events & EPOLLIN)))
			{
				/* An error has occured on this fd, or the socket is not
                ready for reading (why were we notified then?) */
				LOG4CXX_WARN (logger, "run event loop epoll error");
				close (events[i].data.fd);
				continue;
			 }
			else if (sfd == events[i].data.fd)
			{
				/* We have a notification on the listening socket, which
                means one or more incoming connections. */
				while (1)
                {
					struct sockaddr in_addr;
					socklen_t in_len;
					int infd;
					char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

					in_len = sizeof in_addr;
					infd = accept (sfd, &in_addr, &in_len);
					if (infd == -1)
                    {
						if ((errno == EAGAIN) ||
							(errno == EWOULDBLOCK))
                        {
							/* We have processed all incoming
                            connections. */
							break;
                        }
						else
                        {
							LOG4CXX_ERROR(logger,"run Error, accept error");
							break;
                        }
                    }
					s = getnameinfo (&in_addr, in_len,
						             hbuf, sizeof hbuf,
									 sbuf, sizeof sbuf,
									 NI_NUMERICHOST | NI_NUMERICSERV);
					if (s == 0)
                    {
						LOG4CXX_INFO (logger,
								"Accepted connection on descriptor=" << infd
								<<"host: "<<hbuf<<" port: "<<sbuf);

                    }

					/* Make the incoming socket non-blocking and add it to the
                    list of fds to monitor. */
					s = setSocket(infd);
					if (s == -1)
						abort ();

					event.data.fd = infd;
					event.events = EPOLLIN | EPOLLET;
					s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
					if (s == -1)
                    {
						LOG4CXX_FATAL(logger,"run Error, epoll_ctl aborting.");
						abort ();
                    }
                }
				continue;
            }
			else
            {
				/* We have data on the fd waiting to be read. Read and
                display it. We must read whatever data is available
                completely, as we are running in edge-triggered mode
                and won't get a notification again for the same
                data. */
				int done = 0;

				while (1)
                {
					ssize_t count;
					char buf[512];

					count = read (events[i].data.fd, buf, sizeof buf);
					if (count == -1)
                    {
						/* If errno == EAGAIN, that means we have read all
                        data. So go back to the main loop. */
						if (errno != EAGAIN)
                        {
							LOG4CXX_ERROR(logger,"run Error, read error");
							done = 1;
                        }
						break;
                    }
					else if (count == 0)
                    {
						/* End of file. The remote has closed the
                        connection. */
						done = 1;
						break;
                    }

					/* Write the buffer to standard output */
					s = write (1, buf, count);
					if (s == -1)
                    {
						LOG4CXX_FATAL(logger,"run Error, write error aborting");
						abort ();
                    }
                }

				if (done)
                {
					LOG4CXX_INFO(logger,"Closed connection on descriptor: "<<
							events[i].data.fd);

					/* Closing the descriptor will make epoll remove it
                    from the set of descriptors which are monitored. */
					close (events[i].data.fd);
                }
            }
        }
    }

	free (events);
	close (sfd);
	return EXIT_SUCCESS;
}
PgnosqlServer::~PgnosqlServer()
{
	if (pgConn)
	{
		delete pgConn;
		pgConn = NULL;
	}
}
