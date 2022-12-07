#include "props.hpp"
#include "standard_tentative.hpp"

struct label: weight<unsigned>, index<unsigned>
{
  label(unsigned w, unsigned i): weight(w), index(i)
  {
  }
};

bool
operator < (const label &l1, const label &l2)
{
  return get_weight(l1) < get_weight(l2);
}

int main()
{
  label l1(1, 0), l2(0, 0), l3(0, 1);
  standard_tentative<label> t(2);
  t.push(l1);
  t.push(l2);
  t.push(l3);
}
