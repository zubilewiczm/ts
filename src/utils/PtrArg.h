#ifndef _D_PTRARG
#define _D_PTRARG

#include <memory>

template <typename T>
class CloneAllocator
{
  public:
    std::unique_ptr<T> operator()(const T& stack_obj) {
      return stack_obj.clone();
    }
};

template <typename T, typename Ptr = std::shared_ptr<T>, typename Allocator = CloneAllocator<T>>
class PtrArg
{
  public:
    PtrArg(const T& stack_obj, Allocator f = Allocator{}) : mPtr{f(stack_obj)} {};
    PtrArg(Ptr p) : mPtr(p) {};

    operator Ptr() const {
      return mPtr;
    }

    Ptr get() const {
      return mPtr;
    }

  private:
    Ptr mPtr;
};

template <typename T, typename Allocator>
class PtrArg<T, std::shared_ptr<T>, Allocator>
{
  using Ptr = std::shared_ptr<const T>;

  public:
    PtrArg(const T& stack_obj, Allocator f = Allocator{}) : mPtr{f(stack_obj)} {};

    template <typename U>
    PtrArg(std::shared_ptr<U> p) : mPtr(p) {};

    operator Ptr() const {
      return mPtr;
    }

    Ptr get() const {
      return mPtr;
    }

  private:
    Ptr mPtr;
};

#endif
