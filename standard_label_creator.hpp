#ifndef STANDARD_LABEL_CREATOR_HPP
#define STANDARD_LABEL_CREATOR_HPP

#include <array>

// Creates new labels.
struct standard_label_creator
{
  // Create a new label: at the target node of label l, take edge e.
  template <typename Edge, typename Label,
            Callable<Label> F = EmptyCallable<Label>>
  auto
  operator()(const Edge &e, const Label &l, F f = {}) const
  {
    // Weight of the new label = weight of label l + weight of edge e.
    auto w = get_weight(e) + get_weight(l)

    f(c);

    // At cost c, and along edge e, we reach the target of e.
    return std::array<Label, 1>({Label(e, w)});
  }
};

#endif // STANDARD_LABEL_CREATOR_HPP
