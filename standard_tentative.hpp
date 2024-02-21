#ifndef STANDARD_TENTATIVE_HPP
#define STANDARD_TENTATIVE_HPP

#include <cassert>
#include <optional>
#include <set>
#include <vector>

// The container type for storing tentative labels for the standard
// Dijkstra algorithm.  A vertex can have a single label only, because
// we assume the ordering between labels is linear.
template <typename Label>
struct standard_tentative: std::vector<std::optional<Label>>
{
  // The label type.
  using label_type = Label;
  // The type of data a vertex has.
  using vd_type = std::optional<label_type>;
  // The type of the vector of vertex data.
  using base_type = std::vector<vd_type>;
  // The size type of the base type.
  using size_type = typename base_type::size_type;

  // The functor structure for comparing the elements of the queue.  I
  // tried to rewrite this as a lambda, but it failed to compile.
  struct cmp
  {
    base_type &m_r;

    cmp(base_type &r): m_r(r)
    {
    }

    bool operator()(const size_type &a, const size_type &b) const
    {
      return *m_r.operator[](a) < *m_r.operator[](b);
    }
  };

  // The set of keys that serves as the priority queue.  Even though
  // the keys in the queue are always different, we need the multiset,
  // because there can be labels that compare equivalent with the cmp
  // functor (i.e., cmp doesn't hold between them): equal labels for
  // different vertexes.
  std::multiset<size_type, cmp> m_pq;

  standard_tentative(size_type count):
    base_type(count), m_pq(cmp(*this))
  {
  }

  // This function pushes a new label.
  template <typename T>
  const label_type &
  push(T &&l)
  {
    // The key of the label to push.
    const auto &k = get_key(l);
    // The optional label i, the one that may already exist.
    auto &oi = base_type::operator[](k);

    // If there is a label already, then we first have to remove the
    // key from the queue.
    if (oi)
      {
        // Make sure the keys are the same.
        assert(get_key(*oi) == k);
        // Erase the key from the priority queueu.
        m_pq.erase(k);
      }

    // Either initialization or assignment by either copy or move.
    oi.emplace(std::forward<T>(l));
    // Insert the key into the priority queue.
    m_pq.insert(k);

    return *oi;
  }

  bool
  empty() const
  {
    return m_pq.empty();
  }

  auto
  pop()
  {
    // Make sure the queue is not empty.
    assert(!m_pq.empty());
    // Get the key at the top.
    auto k = *m_pq.begin();
    // Pop the element.
    m_pq.erase(m_pq.begin());

    // The optional label i, the one that may already exist.
    auto &oi = base_type::operator[](k);
    // The label must exist.
    assert (oi);
    auto l = std::move(*oi);
    // We remove the label from the vector.
    oi.reset();

    return l;
  }
};

template <typename Label>
bool
has_better_or_equal(const standard_tentative<Label> &C,
                    const Label &j)
{
  // The optional label i.
  const auto &oi = C[get_key(j)];

  if (oi)
    {
      const auto &i = *oi;
      // Make sure the keys are the same.
      assert(get_key(i) == get_key(j));
      // Here we use the <= operator for linear orderings.
      return i <= j;
    }

  return false;
}

#endif // STANDARD_TENTATIVE_HPP
