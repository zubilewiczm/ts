#ifndef _D_EXCEPTION
#define _D_EXCEPTION

#include <exception>
#include <string>

class generic_exception : public std::exception {
  public:
    generic_exception(const char*);
    generic_exception(std::string);
    const char* what() const noexcept override;

  protected:
    const std::string mMsg;
};

class type_exception : public generic_exception {
  public:
    type_exception(const char* s) : generic_exception(s) {}
    type_exception(std::string s) : generic_exception(s) {}
};

class display_exception : public generic_exception {
  public:
    display_exception(const char* s) : generic_exception(s) {}
    display_exception(std::string s) : generic_exception(s) {}
};

#endif

