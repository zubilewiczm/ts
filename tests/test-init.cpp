#include <UnitTest++/UnitTestPP.h>
#include <initializer_list>
#include <memory>

#include "../src/Exception.h"
#include "../src/Term.h"
#include "../src/Var.h"
#include "../src/Type.h"

template <typename T>
class CountCopies : public T
{
  public:
    template <typename... Ts>
      CountCopies(const Ts&... vars) : mCtr(new unsigned(0)), T(vars...) {}
    CountCopies(const CountCopies& c) : mCtr(c.mCtr), T(c) { incr(); }
    CountCopies(CountCopies&& c) : mCtr(), T(c) {std::swap(mCtr, c.mCtr);}
    CountCopies& operator=(CountCopies& c)
    {
      if (&c != this)
      {
        mCtr = c.mCtr;
        incr();
      }
    }
    CountCopies& operator=(CountCopies&& c) { std::swap(mCtr, c.mCtr); }

    unsigned get_copies() const { return *mCtr; }
    void incr() { *mCtr+= 1; }

  protected:
    CountCopies* clone_impl() const override { return new CountCopies(*this); }
    CountCopies* deepcopy_impl() const override { return clone_impl(); }

  private:
    std::shared_ptr<unsigned> mCtr;
};

SUITE(tsInit)
{
  TEST(Term_ByName) {
    Term t("name");
    CHECK_EQUAL("name", t.get_name());
    CHECK(&(t.get_type()) == &T);
    CHECK_THROW(t[0], type_exception);
  }
  TEST(Type_ByName) {
    Type t("name");
    CHECK_EQUAL("name", t.get_name());
    CHECK_THROW(t[0], type_exception);
  }
  TEST(Var_Top) {
    Var v("name", T);
    CHECK_EQUAL("name", v.get_name());
    CHECK(&(v.get_type()) == &T);
  }
  TEST(Var) {
    Type A("A");
    Var v("name", A);
    CHECK_EQUAL("name", v.get_name());
    CHECK(v.get_type() == A);
    CHECK(&(v.get_type()) != &A);
  }

  TEST(Term_ByNameType_Top) {
    Term t("name", T);
    CHECK_EQUAL("name", t.get_name());
    CHECK(&(t.get_type()) == &T);
    CHECK_THROW(t[0], type_exception);
  }
  TEST(Term_ByNameType) {
    Type A("A");
    Term t("name", A);
    CHECK_EQUAL("name", t.get_name());
    CHECK(t.get_type() == A);
    CHECK(&(t.get_type()) != &A);
    CHECK_THROW(t[0], type_exception);
  }
  TEST(Term_Args_InitList) {
    CountCopies<Var> v("A", T);
    CountCopies<Var> w("B", T);
    Term t("f", T, {v,w});
    CHECK_EQUAL("f(A,B)", t.get_name());
    CHECK(*t[0] == v);
    CHECK(*t[1] == w);
    CHECK_THROW(t[2], type_exception);
    CHECK(t.get_type() == T);
    CHECK(v.get_copies() == 1);
    CHECK(w.get_copies() == 1);
  }
  TEST(Term_Args_InitList2) {
    CountCopies<Var> v("A", T);
    CountCopies<Var> w("B", T);
    CountCopies<Type> P("P", ArgList({v,w}));
    CountCopies<Type> R("R", ArgList({w}));
    CountCopies<Term> Q("Q", R, ArgList({w}));
    Term t("f", P, {Q,v});
    CHECK(*t[0] == Q);
    CHECK(*t[1] == v);
    CHECK_THROW(t[2], type_exception);
    CHECK(t.get_type() == P);
    CHECK(v.get_copies() == 2);
    CHECK(w.get_copies() == 3);
  }
  TEST(Type_Args_InitList) {
    CountCopies<Var> v("A", T);
    CountCopies<Var> w("B", T);
    Type t("f", {v,w});
    CHECK(*t[0] == v);
    CHECK(*t[1] == w);
    CHECK_THROW(t[2], type_exception);
    CHECK(t.get_type() == T);
    CHECK(v.get_copies() == 1);
    CHECK(w.get_copies() == 1);
  }
  TEST(Type_Args_InitList2) {
    CountCopies<Var> v("A", T);
    CountCopies<Var> w("B", T);
    CountCopies<Var> u("C", T);
    CountCopies<Type> P("P", ArgList( {v,w} ));
    CountCopies<Type> R("R", ArgList( {w} ));
    CountCopies<Term> Q("Q", R, ArgList( {v,w,R} ));
    Type t("f", {v,P,Q,R});
    CHECK(*t[0] == v);
    CHECK(*t[1] == P);
    CHECK(*t[2] == Q);
    CHECK(*t[3] == R);
    CHECK_THROW(t[4], type_exception);
    CHECK(t.get_type() == T);
    CHECK(v.get_copies() == 3);
    CHECK(w.get_copies() == 3);
  }
  TEST(Term_Args_PtrArray) {
    auto v = std::make_shared<CountCopies<Var>>("A", T);
    auto w = Var::New("B", T);
    auto P = Type::New("P", ArgList({v,w}));
    auto R = Type::New("R", ArgList({w}));
    auto Q = Term::New("Q", R, ArgList({w}));
    Term t("f", P, ArgList({Q,v}));
    CHECK(*t[0] == *Q);
    CHECK(*t[1] == *v);
    CHECK_THROW(t[2], type_exception);
    CHECK(t.get_type() == *P);
    CHECK(v->get_copies() == 0);
  }
}
