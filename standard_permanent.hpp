#ifndef STANDARD_PERMANENT_HPP
#define STANDARD_PERMANENT_HPP

#include <cassert>
#include <optional>
#include <utility>
#include <vector>

// We introduce myoptional only to have the empty function.
template <typename T>
struct myoptional: public std::optional<T>
{
  // Use the constructors of std::optional.
  using base = std::optional<T>;
  using base::base;

  bool
  empty() const
  {
    return !static_cast<bool>(*this);
  }
};

// The container type for storing permanent standard labels.  A vertex
// can have a label or not, and so we make the label optional.  We go
// for the std::optional instead of std::unique_ptr, because optional
// stores its value inside, and so the labels are allocated in-place
// and not in a remote part.
template <typename Label>
struct standard_permanent: std::vector<myoptional<Label>>
{
  // That's the label type we're using.
  using label_type = Label;
  // The type of data a vertex has.
  using vd_type = myoptional<label_type>;
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
    // The key of the label.
    const auto &i = get_key(l);
    // There should be no label for the target vertex.
    assert(!base_type::operator[](i));
    // Emplace (either copy or move construct) and return the
    // reference to the label.
    return base_type::operator[](i).emplace(std::forward<T>(l));
  }
};

template <typename Label>
bool
has_better_or_equal(const standard_permanent<Label> &C,
                    const Label &j)
{
  // The optional label i.
  const auto &oi = C[get_key(j)];

  if (oi)
    {
      const auto &i = *oi;
      // Make sure the keyes are the same.
      assert(get_key(i) == get_key(j));
      // Here we use the <= operator we define.
      return i <= j;
    }

  return false;
}

#endif // STANDARD_PERMANENT_HPP
