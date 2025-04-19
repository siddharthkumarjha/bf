#pragma once
#include <iterator>
#include <fstream>

typedef std::char_traits<char> traits_type;
typedef std::basic_istream<char, traits_type> istream_type;

class istream_iterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = char;
    using pointer           = char *;
    using reference         = char &;
    using const_pointer     = const char *;
    using const_reference   = const char &;

private:
    istream_type *m_stream;
    value_type m_value;

public:
    ///  Construct end of input stream iterator.
    constexpr istream_iterator() : m_stream(0), m_value() {}

    ///  Construct start of input stream iterator.
    istream_iterator(istream_type &lIstrmStart);

    constexpr istream_iterator(const istream_iterator &lObjToCopy)
        : m_stream(lObjToCopy.m_stream), m_value(lObjToCopy.m_value)
    {
    }

    istream_iterator &operator=(const istream_iterator &) = default;
    ~istream_iterator()                                   = default;

    const_reference operator*() const;
    const_pointer operator->() const;

    istream_iterator &operator++();
    istream_iterator operator++(int);

private:
    bool equal(const istream_iterator &lOtherObj) const;
    void read();

    /// Return true if the iterators refer to the same stream,
    /// or are both at end-of-stream.
    friend bool operator==(const istream_iterator &lLhsVal,
                           const istream_iterator &lRhsVal)
    {
        return lLhsVal.equal(lRhsVal);
    }
    friend bool operator!=(const istream_iterator &lLhsVal,
                           const istream_iterator &lRhsVal)
    {
        return !lLhsVal.equal(lRhsVal);
    }
};
