#include "props.hpp"
#include "standard_tentative.hpp"

struct label: weight<unsigned>, index<unsigned>
{
  label(unsigned w, unsigned i): weight(w), index(i)
  {
  }

  constexpr auto
  operator <=> (const label &l) const
  {
    if (get_weight(*this) < get_weight(l))
      return std::strong_ordering::less;
    if (get_weight(*this) > get_weight(l))
      return std::strong_ordering::greater;

    return std::strong_ordering::equal;
  }

  bool operator == (const label &) const = default;
};

int main()
{
  label l1(1, 0), l2(0, 0), l3(0, 1);
  standard_tentative<label> t(2);
  t.push(l1);
  t.push(l3);

  {
    auto l = t.pop();
    assert(l == l3);
  }
  
  {
    auto l = t.pop();
    assert(l == l1);
  }

  t.push(l2);
  {
    auto l = t.pop();
    assert(l == l2);
  }
}
