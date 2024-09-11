#ifndef _D_IMAKESNEWSHARED
#define _D_IMAKESNEWSHARED

#include <memory>

template <typename T>
class IMakesNewShared
{
  public:
    template <typename... Args>
      static std::shared_ptr<T> New(Args... a) {
        return std::make_shared<T>(a...);
      }
    template <typename... Args>
      static std::unique_ptr<T> NewUnique(Args... a) {
        return std::make_unique<T>(a...);
      }
};  

#endif
