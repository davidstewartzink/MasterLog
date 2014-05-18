// Copyright (c) 2014, Kelp Heavy Weaponry
// MasterLog project -- see MasterLog licencing for details.

namespace MasterLog {

class String
{
public:
    explicit String(const char *);
    String();
    String(const String&);
    String& operator=(const String&);
    ~String();

    const char *value() const;
    unsigned length() const;

    void fromText(const char *p, int len);

    bool operator == (const String&) const;
    bool operator < (const String&) const;
    bool operator == (const char *) const;

private:
    struct I;
    I *mi;
};

} // String : MasterLog
