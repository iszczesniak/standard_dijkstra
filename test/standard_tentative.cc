#include "props.hpp"
#include "standard_tentative.hpp"

struct label: weight<unsigned>, index<unsigned>
{
  label(unsigned w, unsigned i): weight(w), index(i)
  {
  }

  bool operator < (const label &) const
  {
    return get_weight(*this) < get_weight(l);
  }
};

int main()
{
  label l1(1, 0), l2(0, 0), l3(0, 1);
  standard_tentative<label> t;
  t.push(l1);
  t.push(l2);
  t.push(l3);
}
