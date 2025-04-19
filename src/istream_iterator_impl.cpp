#include "istream_iterator_impl.hpp"
#include "bf.hpp"

bool is_bf_comment(const char c)
{
    switch (static_cast<INS>(c))
    {
    case MOV_IP_RIGHT:
    case MOV_IP_LEFT:
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

istream_type &operator>>(istream_type &lInputStrm, char &lStoreChar)
{
    typedef typename istream_type::int_type int_type;
    typename istream_type::sentry lSentryObj(lInputStrm, false);

    if (lSentryObj)
    {
        std::ios_base::iostate lStrmErr = std::ios_base::goodbit;
        try
        {
            int_type lCharRead;
            do
            {
                lCharRead = lInputStrm.rdbuf()->sbumpc();
            } while (
                !traits_type::eq_int_type(lCharRead, traits_type::eof()) and
                is_bf_comment(traits_type::to_char_type(lCharRead)));

            if (!traits_type::eq_int_type(lCharRead, traits_type::eof()))
                lStoreChar = traits_type::to_char_type(lCharRead);
            else
                lStrmErr |= (std::ios_base::eofbit | std::ios_base::failbit);
        }
        catch (...)
        {
            lInputStrm.setstate(std::ios_base::badbit);
        }
        if (lStrmErr)
            lInputStrm.setstate(lStrmErr);
    }
    return lInputStrm;
}

///  Construct start of input stream iterator.
istream_iterator::istream_iterator(istream_type &lIstrmStart)
    : m_stream(std::addressof(lIstrmStart))
{
    read();
}

istream_iterator::const_reference istream_iterator::operator*() const
{
    return m_value;
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
    istream_iterator lTmp = *this;
    read();
    return lTmp;
}

bool istream_iterator::equal(const istream_iterator &lOtherObj) const
{
    return !m_stream || (m_stream == lOtherObj.m_stream);
}

void istream_iterator::read()
{
    if (m_stream && !(*m_stream >> m_value))
    {
        m_stream = 0;
    }
}
