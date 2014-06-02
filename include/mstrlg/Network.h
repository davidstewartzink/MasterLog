// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

// Network access

namespace MasterLog {
namespace Network {

// Need to work out how comm works
extern void reschedule(Ticket*);
extern void schedule(Ticket*, int milliseconds);

struct ReadTicket : Ticket
{
    char *buffer;
    int bufferSize;
    int readLen;

    template<typename C>
    void set(C* obj, void(*f)(C*,ReadTicket*), char *b, int bs)
    {
        buffer = b;
        bufferSize = bs;
        Ticket::set(obj, f);
    }
};

struct WriteTicket : Ticket
{
    const char *buffer;
    int bufferLen;
    int writtenLen;

    template<typename C>
    void set(C* obj, void(*f)(C*,WriteTicket*), const char *b, int bs)
    {
        buffer = b;
        bufferLen = bs;
        Ticket::set(obj, f);
    }
};

class Socket
{
public:
    struct I;

    Socket(I*i = 0):mi(i){}
    Socket(Socket const&);
    Socket& operator=(Socket const&);
    ~Socket();

    int submit(ReadTicket* rt);
    int submit(WriteTicket* wr);

private:
    I* mi;
};

class Listener
{
public:
    struct I;

    Listener(I*i = 0):mi(i){}
    Listener(Listener const&);
    Listener& operator=(Listener const&);
    ~Listener();

    int bind(uint32_t ho, uint16_t po, int depth, void (*handler)(Socket &sock));

    void close();

private:
    I* mi;
};

int run(); // Network is the main loop for everything...for now

int configure(Configuration const& config);

}} // Network : MasterLog
