#ifndef _D_ICLONABLE
#define _D_ICLONABLE

#include <memory>

template <
  typename Derived,
  typename Base,
  typename SafelyInheritedFrom = Derived
  >
class IClonableWithBase : public Base
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
    friend SafelyInheritedFrom;
};

template <typename Derived, typename SafelyInheritedFrom = Derived>
class IClonable
{
  public:
    template <typename Base>
      using WithBase = IClonableWithBase<Derived, Base, SafelyInheritedFrom>;

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
    friend SafelyInheritedFrom;
};

#endif
