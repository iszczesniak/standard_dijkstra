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

  // The set of indexes that serves as the priority queue.  We need
  // the multiset, because there can be labels that compare equal with
  // the < operator (i.e., < doesn't hold between them): equal labels
  // for different vertexes.
  std::multiset<size_type, bool(const size_type &, const size_type &)>
  m_pq{[](const size_type &a, const size_type &b)
  {return *base_type::operator[](a) < *base_type::operator[](b);}};

  standard_tentative(size_type count): base_type(count)
  {
  }

  // This function pushes a new label.
  template <typename T>
  void
  push(T &&l)
  {
    // The index of the label.
    const auto &i = get_index(l);
    // There should be no label for index i.
    assert(!base_type::operator[](i));
    // Set the label for index i.
    base_type::operator[](i) = std::forward<T>(l);
    // Insert the index into the priority queue.
    m_pq.insert(i);
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

#endif // STANDARD_TENTATIVE_HPP
