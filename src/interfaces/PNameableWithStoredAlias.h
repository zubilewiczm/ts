#ifndef _D_PNAMEABLEWITHSTOREDALIAS
#define _D_PNAMEABLEWITHSTOREDALIAS

#include "INameable.h"
#include <cstring>
#include <string>

class PNameableWithStoredAlias : virtual public INameable
{
  public:
    PNameableWithStoredAlias() : mAlias() {}
    PNameableWithStoredAlias(const std::string& s) : mAlias(s) {}

    std::string get_alias() const { return mAlias; }

    PNameableWithStoredAlias& set_alias(const std::string& s) {
      mAlias = s; return *this;
    };

  private:
    std::string mAlias;
};

#endif
