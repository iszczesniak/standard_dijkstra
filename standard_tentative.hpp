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

  // The set of indexes that serves as the priority queue.
  std::multiset<size_type, bool(const size_type &, const size_type &)>
  m_pq{[](const size_type &a, const size_type &b)
  {return *base_type::operator[](a) < *base_type::operator[](b)}};

  standard_tentative(size_type count): base(count)
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
    assert(!base::operator[](i));
    // Set the label for index i.
    base::operator[](i) = std::forward<T>(l);
    // Push the index into the priority queue.
    m_pq.push(i);
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
    // Get a shared_ptr from the element on top.
    auto sp = m_pq.top().second.lock();
    // The top element must have a label, and so sp cannot be nullptr.
    // In the queue, there might be an element whose label was
    // discarded, because we found a label of a smaller cost.  But
    // then the label of the smaller cost will come first, and the
    // element of the queue with the discarded label would come later.
    assert(sp);
    // Pop the element.
    m_pq.pop();

    // Clean up the trailing elements, whose labels were discarded.
    while(!m_pq.empty() && m_pq.top().second.expired())
      m_pq.pop();

    // This is the label we return.
    const auto l = std::move(*sp);
    // Make sure we've got the label in the right place.
    assert(base::operator[](get_target(l)));
    // We remove the label from the vector.
    base::operator[](get_target(l)).reset();

    return l;
  }
};

#endif // STANDARD_TENTATIVE_HPP
