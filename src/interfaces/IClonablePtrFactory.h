#ifndef _D_ICLONABLEPTRFACTORY
#define _D_ICLONABLEPTRFACTORY

#include "IClonable.h"
#include "IPtrFactory.h"

#include "../ArgList.h"

template <typename Derived, typename Base>
class IClonablePtrFactory :
  public IClonable<Derived, IClonablePtrFactory<Derived, Base>>
    ::template WithBase<Base>,
  public IPtrFactory<Derived>
{
  public:
    using IPtrFactory<Derived>::New;
    using IPtrFactory<Derived>::NewUnique;
};

#endif
