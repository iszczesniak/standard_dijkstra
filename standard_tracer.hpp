#ifndef STANDARD_TRACER_HPP
#define STANDARD_TRACER_HPP

#include "graph.hpp"

template <typename Permanent>
struct standard_tracer
{
  const Permanent &m_P;

  // That's the label type we're using.
  using label_type = typename Permanent::label_type;
  // The type of vertex data.
  using vd_type = typename Permanent::vd_type;
  // The path type.  The trace function needs it.
  using path_type = std::list<label_type>;
  // The size type of the permanent container.
  using size_type = Permanent::size_type;
  
  standard_tracer(const Permanent &P): m_P(P)
  {
  }

  /**
   * Initialize the tracing.
   */
  auto
  init(const vd_type &l)
  {
    assert(l);
    return &*l;
  }

  /**
   * Push the label to the path.
   */
  void
  push(path_type &p, const label_type *lp)
  {
    // This is the label we process.
    const auto &l = *lp;
    // The edge of the label.
    const auto &e = get_edge(l);
    // Add the label's edge to the path.
    p.push_front(e);
  }

  /**
   * We are given the iterator to the label that we process here.  We
   * require this is not the starting label.  The objective is to find
   * the previous label on the path.
   */
  const label_type *
  advance(const Permanent &P, const label_type *l_ptr)
  {
    // This is the label we process.
    const auto &l = *l_ptr;
    // This is the edge of the label.
    const auto &e = get_edge(l);
    // The source of the edge.
    const auto &s = get_source(e);

    // Find the previous element for vertex s.
    const auto &pe = P[s];
    // The element must have a label.
    assert(pe);
    // Get the label of the element.
    const auto &pl = *pe;

    // The weight of the label.
    const auto &c = get_weight(l);
    // The edge weight.
    const auto &ec = get_weight(e);
    // Make sure we've got the weights right.
    assert(get_weight(pl) + ec == c);

    // Return the address of the previous label.
    return &pl;
  }

  const auto &
  operator[](size_type index) const
  {
    return m_P[index];
  }
};

#endif // STANDARD_TRACER_HPP
