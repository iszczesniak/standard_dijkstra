#include "props.hpp"
#include "standard_tentative.hpp"

struct label: weight<unsigned>, key<unsigned>
{
  label(unsigned w, unsigned i): weight(w), key(i)
  {
  }

  // Ask for the default == explicitly, because <=> is not default.
  constexpr bool operator == (const label &l) const = default;

  // We delegate <=> to weight<unsigned>.  We do not want the default
  // implementation of <=> (i.e., lexicographic comparison), because
  // m_edge should not take part.
  constexpr auto operator <=> (const label &l) const
  {
    return weight<unsigned>::operator<=>(l);
  }
};

int main()
{
  label l1(1, 0), l2(0, 0), l3(0, 1);
  standard_tentative<label> t(2);

  // Labels l2 and l3 compare equal, because neither l2 < l3, nor l3 <
  // l2 hold.  The order between equal labels is stable, because the
  // insert function inserts an equal element at the upper bound
  // (i.e., after the last equal element), and the sorting is stable.
  t.push(l2);
  t.push(l3);

  {
    auto l = t.pop();
    assert(l == l2);
  }
  
  {
    auto l = t.pop();
    assert(l == l3);
  }

  t.push(l1);
  {
    auto l = t.pop();
    assert(l == l1);
  }
}
