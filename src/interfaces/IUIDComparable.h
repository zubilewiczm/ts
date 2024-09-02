#ifndef _D_IUIDCOMPARABLE
#define _D_IUIDCOMPARABLE

#include <string>

class IUIDComparable
{
  public:
    virtual std::string get_uid() const = 0;

    friend bool operator==(const IUIDComparable&, const IUIDComparable&);
    friend bool operator!=(const IUIDComparable&, const IUIDComparable&);
    friend bool operator<(const IUIDComparable&, const IUIDComparable&);
    friend bool operator<=(const IUIDComparable&, const IUIDComparable&);
    friend bool operator>(const IUIDComparable&, const IUIDComparable&);
    friend bool operator>=(const IUIDComparable&, const IUIDComparable&);
};

#endif
