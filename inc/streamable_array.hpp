#pragma once
#include <array>
#include <ostream>

template <typename T>
concept streamable = requires(std::ostream &os, T &&t) {
    { os << std::forward<T>(t) } -> std::same_as<std::ostream &>;
};

namespace bf
{
    template <streamable Tp, std::size_t Nm>
    class array : public std::array<Tp, Nm>
    {
        friend std::ostream &operator<<(std::ostream &os,
                                        const array<Tp, Nm> &arr)
        {
            static constexpr std::string_view sep_arr[] = {"[", ", "};
            std::string_view sep                        = sep_arr[0];

            if constexpr (Nm == 0)
                os << sep;

            for (const auto &arr_elem : arr)
            {
                os << sep << arr_elem;
                sep = sep_arr[1];
            }

            os << "]";
            return os;
        }
    };

    template <class> struct is_ref_wrapper : std::false_type
    {
    };
    template <class T>
    struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type
    {
    };

    template <class T>
    concept not_ref_wrapper = std::negation_v<is_ref_wrapper<std::decay_t<T>>>;

    template <class T>
    concept valid_array_element = streamable<T> && not_ref_wrapper<T>;

    template <class D, valid_array_element... Types>
    using array_type = array<
        std::conditional_t<std::is_void_v<D>, std::common_type_t<Types...>, D>,
        sizeof...(Types)>;

    template <class D = void, valid_array_element... Types>
    inline constexpr array_type<D, Types...> make_array(Types &&...t)
    {
        return {std::forward<Types>(t)...};
    }
} // namespace bf
