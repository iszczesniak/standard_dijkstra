#ifndef STANDARD_CONSTRAINED_LABEL_CREATOR_HPP
#define STANDARD_CONSTRAINED_LABEL_CREATOR_HPP

#include "standard_label_creator.hpp"

#include <type_traits>

template <typename Weight>
struct standard_constrained_label_creator:
  standard_label_creator
{
  using base_type = standard_label_creator;
  using weight_type = Weight;
  const weight_type m_max_weight;

  standard_constrained_label_creator(weight_type max_weight):
    m_max_weight(max_weight)
  {
  }

  // Create a new label, and check the weight.
  template <typename Label, typename Edge>
  auto
  operator()(const Label &l, const Edge &e) const
  {
    static_assert(std::is_same_v<weight_type,
                                 typename Label::weight_type>);

    // The candidate label.
    auto cl = base_type::operator()(l, e);

    if (get_weight(cl[0]) > m_max_weight)
      throw true;

    return cl;
  }
};

#endif // STANDARD_CONSTRAINED_LABEL_CREATOR_HPP
