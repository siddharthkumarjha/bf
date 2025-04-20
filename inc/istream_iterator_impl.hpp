#pragma once
#include <iterator>

namespace bf
{
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
        istream_type *_stream;
        value_type _value;

    public:
        ///  Construct end of input stream iterator.
        constexpr istream_iterator() : _stream(0), _value() {}

        ///  Construct start of input stream iterator.
        istream_iterator(istream_type &istrm_start);

        constexpr istream_iterator(const istream_iterator &obj_to_copy)
            : _stream(obj_to_copy._stream), _value(obj_to_copy._value)
        {
        }

        istream_iterator &operator=(const istream_iterator &) = default;
        ~istream_iterator()                                   = default;

        const_reference operator*() const;
        const_pointer operator->() const;

        istream_iterator &operator++();
        istream_iterator operator++(int);

    private:
        bool equal(const istream_iterator &other_obj) const;
        void read();

        /// Return true if the iterators refer to the same stream,
        /// or are both at end-of-stream.
        friend bool operator==(const istream_iterator &lhs_val,
                               const istream_iterator &rhs_val)
        {
            return lhs_val.equal(rhs_val);
        }
        friend bool operator!=(const istream_iterator &lhs_val,
                               const istream_iterator &rhs_val)
        {
            return !lhs_val.equal(rhs_val);
        }
    };
} // namespace bf
