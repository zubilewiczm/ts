#include <exception>
#include <iostream>

#include "Term.h"
#include "Type.h"
#include "Var.h"

int main()
{
  try {
  std::cout << T.get_name() << std::endl;
  std::cout << T.get_type().get_name() << std::endl;
  std::cout << T.get_type().get_type().get_name() << std::endl;
  std::cout << N.get_name() << std::endl;
  std::cout << N.get_type().get_name() << std::endl;
  std::cout << N.get_type().get_type().get_name() << std::endl;
 
  Var a("A", T);
  Var A("A", T);
  Var b("B", T);
  Var c("C", T);
  Term x("δ", T, {a, b, c});

  std::cout << a.get_name() << std::endl;
  std::cout << A.get_name() << std::endl;
  std::cout << b.get_name() << std::endl;
  std::cout << c.get_name() << std::endl;
  std::cout << x.get_name() << std::endl;

  std::cout << a.get_uid() << std::endl;
  std::cout << A.get_uid() << std::endl;
  std::cout << b.get_uid() << std::endl;
  std::cout << c.get_uid() << std::endl;
  std::cout << x.get_uid() << std::endl;

  std::cout << (a==a) << std::endl;
  std::cout << (a==A) << std::endl;
  std::cout << (a==b) << std::endl;
  std::cout << (a==c) << std::endl;

  auto y = x.subs(b, x);
  std::cout << y->get_name() << std::endl;

  auto y2 = x.subs(Var("B", T), x);
  std::cout << y2->get_name() << std::endl;

  auto z = x.subs(Var("δ", T), x);
  std::cout << z->get_name() << std::endl;
  std::cout << (x == *z) << std::endl;

  auto w = y->subs(c, *y);
  std::cout << w->get_name() << std::endl;
  std::cout << w->get_uid() << std::endl;

//   Lambda F(T,T);
//   Func l = F.cons(a, Term("f", T, {&a,&b}), "f");
//   std::cout << F.get_name() << std::endl;
//   std::cout << F.get_uid() << std::endl;
//   std::cout << F.get_type().get_name() << std::endl;
//   std::cout << F.get_type().get_uid() << std::endl;
//   std::cout << l.get_name() << std::endl;
//   std::cout << l.get_uid() << std::endl;
//   std::cout << l.get_type().get_name() << std::endl;
//   std::cout << l.get_type().get_uid() << std::endl;
//   std::cout << l.get_name_and_type() << std::endl;
//   std::cout << F.apply(l,x)->get_name() << std::endl;
//   std::cout << F.apply(l,*F.apply(l,x))->get_name_and_type() << std::endl;
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
