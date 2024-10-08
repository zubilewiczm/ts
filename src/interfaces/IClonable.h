#ifndef _D_ICLONABLE
#define _D_ICLONABLE

#include <memory>

#include "IMakesNewShared.h"

template <typename Derived, typename Base>
class IClonableWithBase : public Base, public IMakesNewShared<Derived>
{
  public:
    std::unique_ptr<Derived> clone() const
    {
      return std::unique_ptr<Derived>(static_cast<Derived*>(clone_impl()));
    }
    std::unique_ptr<Derived> deepcopy() const
    {
      return std::unique_ptr<Derived>(static_cast<Derived*>(deepcopy_impl()));
    }

  protected:
    virtual IClonableWithBase* clone_impl() const = 0;
    virtual IClonableWithBase* deepcopy_impl() const = 0;

    template <typename... Ts>
      IClonableWithBase(const Ts&... vars) : Base(vars...) {}
      IClonableWithBase(const IClonableWithBase&) = default;

  private:
    IClonableWithBase() {}
    friend Derived;
};

template <typename Derived>
class IClonable : public IMakesNewShared<Derived>
{
  public:
    template <typename Base>
      using WithBase = IClonableWithBase<Derived, Base>;

  public:
    std::unique_ptr<Derived> clone() const
    {
      return std::unique_ptr<Derived>(static_cast<Derived*>(clone_impl()));
    }
    std::unique_ptr<Derived> deepcopy() const
    {
      return std::unique_ptr<Derived>(static_cast<Derived*>(deepcopy_impl()));
    }

  protected:
    virtual IClonable* clone_impl() const = 0;
    virtual IClonable* deepcopy_impl() const = 0;

  private:
    IClonable() {}
    friend Derived;
};

#endif
