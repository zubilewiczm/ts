#ifndef _D_IPTRFACTORY
#define _D_IPTRFACTORY

#include <initializer_list>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T>
class IPtrFactory
{
    template <typename... Args>
    using enable_if_constructible_t = typename
    std::enable_if_t<
      std::is_constructible<T, Args...>::value,
      bool
    >;

  public:

    template <
      typename... Args,
      enable_if_constructible_t<Args...> = true
    >
    static std::shared_ptr<T> New(Args&&... a)
    {
      return std::make_shared<T>(std::forward<Args>(a)...);
    }

    template <
      typename... Args,
      enable_if_constructible_t<Args...> = true
    >
    static std::unique_ptr<T> NewUnique(Args&&... a)
    {
      return std::make_unique<T>(std::forward<Args>(a)...);
    }
    
};  


#endif
