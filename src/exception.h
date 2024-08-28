#ifndef _D_EXCEPTION
#define _D_EXCEPTION

#include <exception>
#include <string>

class type_exception : public std::exception {
  public:
    type_exception(const char*);
    type_exception(std::string);
    const char* what() const noexcept override;

  protected:
    const std::string mMsg;
};

#endif

