#ifndef _D_TYPE
#define _D_TYPE

#include "term.h"

extern const Type T;
extern const Type N;

class Type : public Term
{
  public:
    Type(const std::string&);
    Type(const std::string&, const Args&);
    Type(const Type&);
    Type* clone() const override;
    Type* deepcopy() const override;
    
    const Type& get_type() const override;
    std::string get_name() const override;
    std::string get_uid() const override;
    const Type& cons(const Term&);
};

#endif
