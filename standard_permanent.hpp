#ifndef STANDARD_PERMANENT_HPP
#define STANDARD_PERMANENT_HPP

#include <cassert>
#include <optional>
#include <utility>
#include <vector>

// The container type for storing permanent standard labels.  A vertex
// can have a label or not, and so we make the label optional.  We go
// for the std::optional instead of std::unique_ptr, because optional
// stores its value inside, and so the labels are allocated in-place
// and not in a remote part.
template <typename Label>
struct standard_permanent: std::vector<std::optional<Label>>
{
  // That's the label type we're using.
  using label_type = Label;
  // The type of data a vertex has.
  using vd_type = std::optional<label_type>;
  // The type of the vector of vertex data.
  using base_type = std::vector<vd_type>;
  // The size type of the base type.
  using size_type = typename base_type::size_type;

  standard_permanent(size_type count): base_type(count)
  {
  }

  // Pushes a label, and returns a reference to it.
  template <typename T>
  const label_type &
  push(T &&l)
  {
    // The index of the target vertex of the label.
    const auto &i = get_index(get_target(l));
    // There should be no label for the target vertex.
    assert(!base_type::operator[](i));
    // Assign the value, and return the reference to the label.  The *
    // operator returns the label of the optional without checking,
    // and no need to, because we just assigned it.
    return *(base_type::operator[](i) = std::forward<T>(l));
  }
};

#endif // STANDARD_PERMANENT_HPP
