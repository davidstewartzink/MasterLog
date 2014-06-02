// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Network access

#include <mstrlg/Everything.h>

#include <sys/socket.h>
#include <arpa/inet.h>

namespace MasterLog { namespace Network {

static struct
{
    int maxListeners;
    int maxConnections;
} config =
{
    100,
    1000
};

struct Socket::I
{
    struct sockaddr sa;
    int fd;
    int mask;

    void handleRead();
    void handleWrite();
    void markClosed();
};

struct Listener::I : Socket::I
{
};

static const String network("network");
static const String maxListeners("maxListeners");
static const String maxConnections("maxConnections");
static Socket::I * sockets;
static int freeSocket = -1;
static int numSockets;

void Socket::I::handleRead()
{
}

void Socket::I::handleWrite()
{
}

void Socket::I::markClosed()
{
}

void dropSocket(Socket::I* s)
{
    int i = s - sockets;
    if (i == numSockets - 1)
        numSockets -= 1;
    else
    {
        s->fd = freeSocket;
        freeSocket = i;
    }
}

Socket::I* getSocket()
{
    if (freeSocket < 0)
    {
        if (numSockets == config.maxSockets)
            return NULL;
        return &sockets[numSockets++];
    }
    Socket::I* s = &sockets[freeSocket];
    freeSocket = s->fd;
    return s;
}

void Listener::bind(uint32_t ho, uint16_t po, int depth, void (*handler)(Socket &sock))
{
    Listener* thi = getListener();

    union
    {
        struct sockaddr sa;
        struct sockaddr_in sin;
    } u;

    ::memset(&u, 0, sizeof(u));

    int fd = ::socket(PF_INET, SOCK_STREAM, 0);

    u.sin.sin_len = sizeof(u.sin);
    u.sin.sin_family = PF_INET;
    u.sin.sin_addr.s_addr = htonl(host);
    u.sin.sin_port = htons(port);

    if (::bind(fd, &u.sa, sizeof(u.sin)))
    {
        ErrorLog("Failed to bind to %d.%d.%d.%d:%d", uint8_t(host >> 24), uint8_t(host >> 16), uint8_t(host >> 8), uint8_t(host), port);
        dropListener(thi);
    }
}

static int handleTimeout()
{
    return -1;
}

int setupPollers(Socket::I** socks, pollfd pfds[], int maxPfds)
{
    return 0;
}

int run()
{
    int r = 0;
    const int maxPfds = config.maxListeners + config.maxConnections;
    pollfd *pfds = new pollfd[maxPfds];
    Socket::I** socks = new Socket::I*[maxPfds];
    for (;;)
    {
        int timeo = handleTimeout();
        int npfds = setupPollers(socks, pfds, maxPfds);

        int nr = ::poll(pfds, npfds, timeo);

        for (int i = 0; i < nr; ++i)
        {
            int m = pfds[i].revents;
            if (m & POLLIN)
                socks[i]->handleRead();
            if (m & POLLOUT)
                socks[i]->handleWrite();
            if (m & POLLHUP)
                socks[i]->markClosed();
        }
        if (nr < 0)
        {
            switch(errno)
            {
            case EAGAIN: // not a good sign.
                continue;
            case EFAULT: // should be impossible.
            case EINVAL:
            default:
                ErrorLog("poll returns %d", int(errno));
                r = 1;
            case EINTR:
                break;
            }
            break;
        }
    }
    delete[] pfds;
    return r;
}

int configure(Configuration const& cfg)
{
    JSON_Config::Value const &netConfig(cfg.member(network));
    if (netConfig.isObject())
    {
        JSON_Config::Value tmp(netConfig.member(maxListeners));
        if (tmp.isInteger())
            config.maxListeners = tmp.integerValue();
        tmp = netConfig.member(maxConnections);
        if (tmp.isInteger())
            config.maxConnections = tmp.integerValue();
    }
    else if (!netConfig.isNull())
    {
        Error("Invalid \"network\" configuration: Exists but not an Object");
        return 1;
    }
    sockets = new Socket::I[config.maxListeners];
    return 0;
}

}} // Network : MasterLog
