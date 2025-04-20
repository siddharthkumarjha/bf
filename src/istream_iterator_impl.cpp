#include "istream_iterator_impl.hpp"
#include "bf.hpp"

using namespace bf;

static bool is_bf_comment(const char c)
{
    switch (static_cast<INS>(c))
    {
    case MOV_DP_RIGHT:
    case MOV_DP_LEFT:
    case INC_BYTE:
    case DEC_BYTE:
    case OUT_BYTE:
    case ACCEPT_INPUT:
    case COND_JMP_START:
    case COND_JMP_END:
        return false;
    default:
        return true;
    }
}

istream_type &operator>>(istream_type &input_strm, char &store_char)
{
    typedef typename istream_type::int_type int_type;
    typename istream_type::sentry sentry_obj(input_strm, false);

    if (sentry_obj)
    {
        std::ios_base::iostate strm_err = std::ios_base::goodbit;
        try
        {
            int_type char_read;
            do
            {
                char_read = input_strm.rdbuf()->sbumpc();
            } while (
                !traits_type::eq_int_type(char_read, traits_type::eof()) and
                is_bf_comment(traits_type::to_char_type(char_read)));

            if (!traits_type::eq_int_type(char_read, traits_type::eof()))
                store_char = traits_type::to_char_type(char_read);
            else
                strm_err |= (std::ios_base::eofbit | std::ios_base::failbit);
        }
        catch (...)
        {
            input_strm.setstate(std::ios_base::badbit);
        }
        if (strm_err)
            input_strm.setstate(strm_err);
    }
    return input_strm;
}

///  Construct start of input stream iterator.
istream_iterator::istream_iterator(istream_type &istrm_start)
    : _stream(std::addressof(istrm_start))
{
    read();
}

istream_iterator::const_reference istream_iterator::operator*() const
{
    return _value;
}
istream_iterator::const_pointer istream_iterator::operator->() const
{
    return std::addressof((operator*()));
}

istream_iterator &istream_iterator::operator++()
{
    read();
    return *this;
}

istream_iterator istream_iterator::operator++(int)
{
    istream_iterator tmp = *this;
    read();
    return tmp;
}

bool istream_iterator::equal(const istream_iterator &other_istream_iterator) const
{
    return !_stream || (_stream == other_istream_iterator._stream);
}

void istream_iterator::read()
{
    if (_stream && !(*_stream >> _value))
    {
        _stream = 0;
    }
}
