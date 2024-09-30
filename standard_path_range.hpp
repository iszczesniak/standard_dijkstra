#ifndef STANDARD_PATH_RANGE_HPP
#define STANDARD_PATH_RANGE_HPP

#include "graph.hpp"

#include <iostream>

// Iterates over the labels of a path.
template <typename Permanent>
struct standard_path_iterator
{
  // That's the label type we're using.
  using label_type = typename Permanent::label_type;

  // The permanent labels.  In this we dig for the labels.
  const Permanent &m_P;
  // The label we currently point to.  The label is stored in m_P.
  const label_type *m_lp;

  standard_path_iterator(const Permanent &P, const label_type &l):
    m_P(P), m_lp(&l)
  {
  }

  const label_type &
  operator * ()
  {
    return *m_lp;
  }

  /**
   * Find the previous label of the path.
   */
  auto &
  operator ++ ()
  {
    // This is the label we point to now.
    const auto &l = *m_lp;
    // This is the edge of the label.
    const auto &e = get_edge(l);
    // The key of the source of the edge.
    const auto &k = get_key(get_source(e));

    // Find the previous optinal label.
    const auto &pol = m_P[k];
    // The optional label must have a label.
    assert(pol);
    // Get the previous label.
    const auto &pl = *pol;

    // The weight of the label.
    const auto &w = get_weight(l);
    // The edge weight.
    const auto &ew = get_weight(e);
    // Make sure we've got the weights right.
    assert(get_weight(pl) + ew == w);

    // Remember the address of the previous label.
    m_lp = &pl;

    return *this;
  }
};

template <typename Permanent>
bool
operator == (const standard_path_iterator<Permanent> &i1,
             const standard_path_iterator<Permanent> &i2)
{
  return *(i1.m_lp) == *(i2.m_lp);
}

template <typename Permanent>
struct standard_path_range
{
  // That's the label type we're using.
  using label_type = typename Permanent::label_type;

  // The permanent labels.  In this we dig for the labels.
  const Permanent &m_P;
  // The label we start with.
  const label_type &m_l;
  // The initial label of the search, at which we end the itaration.
  const label_type &m_initial;

  standard_path_range(const Permanent &P,
                      const label_type &l, const label_type &initial):
    m_P(P), m_l(l), m_initial(initial)
  {
  }

  auto
  begin() const
  {
    return standard_path_iterator(m_P, m_l);
  }

  auto
  end() const
  {
    return standard_path_iterator(m_P, m_initial);
  }
};

#endif // STANDARD_PATH_RANGE_HPP
