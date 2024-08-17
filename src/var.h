#ifndef _D_VAR
#define _D_VAR

#include <exception>
#include <string>
#include <memory>

#include "type.h"

class Var : public Term
{
  public:
    Var(const std::string&, const Type&);
    Var* clone() const override;
    Var* deepcopy() const override;

    std::string get_name() const override;
    std::string get_uid() const override;
};

#endif
