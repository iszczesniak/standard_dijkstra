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

  // The set of indexes that serves as the priority queue.  We need
  // the multiset, because there can be labels that compare equal with
  // the < operator (i.e., < doesn't hold between them): equal labels
  // for different vertexes.
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
    // The index of the label.
    const auto &i = get_index(l);
    // There should be no label for index i.
    assert(!base_type::operator[](i));
    // Emplace (copy or move construct) the value.
    auto &r = base_type::operator[](i).emplace(std::forward<T>(l));
    // Insert the index into the priority queue.
    m_pq.insert(i);

    return r;
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
    // Get the index at the top.
    auto i = *m_pq.begin();
    // Pop the element.
    m_pq.erase(m_pq.begin());

    // This is the label we return.
    auto l = std::move(*base_type::operator[](i));
    // We remove the label from the vector.
    base_type::operator[](i).reset();

    return l;
  }
};

template <typename Label>
bool
has_better_or_equal(const standard_tentative<Label> &C,
                    const Label &j)
{
  // The optional label i.
  const auto &oi = C[get_index(j)];

  if (oi)
    {
      const auto &i = *oi;
      // Make sure the indexes are the same.
      assert(get_index(i) == get_index(j));
      // Here we use the <= operator we define.
      return i <= j;
    }

  return false;
}

template <typename Label>
void
purge_worse(standard_tentative<Label> &T,
            const Label &j)
{
  // The optional tentative label for the vertex of index j.
  auto &oi = T[get_index(j)];
  // There must be no tentative label, and if there is one, it cannot
  // be better or equal to j.
  assert(!oi || !(*oi <= j));
  // And so we get here only to release the tentative label.
  oi.reset();
}

#endif // STANDARD_TENTATIVE_HPP
