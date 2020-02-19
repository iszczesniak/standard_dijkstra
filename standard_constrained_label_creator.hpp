#ifndef STANDARD_CONSTRAINED_LABEL_CREATOR_HPP
#define STANDARD_CONSTRAINED_LABEL_CREATOR_HPP

#include "standard_label_creator.hpp"

#include <array>
#include <tuple>

// Creates new labels.
template <typename Graph, typename Cost>
struct standard_constrained_label_creator:
  standard_label_creator<Graph, Cost>
{
  using Label = standard_label<Graph, Cost>;
  using base = standard_label_creator<Graph, Cost>;

  const Cost m_max_cost;

  standard_constrained_label_creator(const Graph &g, Cost max_cost):
    base(g), m_max_cost(max_cost)
  {
  }

  // Create a new label.  We are at the target node of label l, and we
  // try to take edge e.
  auto
  operator()(const Edge<Graph> &e, const Label &l) const
  {
    auto f = [&m_max_cost = m_max_cost](Cost c)
             {
               if (c > m_max_cost)
                 // If we went over the reach, throw true.
                 throw true;
             };

    return base::operator()(e, l, f);
  }
};

#endif // STANDARD_CONSTRAINED_LABEL_CREATOR_HPP
