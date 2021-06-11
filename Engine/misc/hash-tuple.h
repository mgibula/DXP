#pragma once

namespace std
{

template<typename... TTypes>
class hash<std::tuple<TTypes...>>
{
private:
    typedef std::tuple<TTypes...> Tuple;

    template<int N>
    size_t operator()(const Tuple& value) const { return 0; }

    template<int N, typename THead, typename... TTail>
    size_t operator()(const Tuple& value) const
    {
        constexpr int Index = N - sizeof...(TTail) - 1;
        return std::hash<typename std::decay<THead>::type>{}(std::get<Index>(value)) ^ (operator() < N, TTail... > (value) << N);
    }

public:
    size_t operator()(const Tuple& value) const
    {
        return operator() < sizeof...(TTypes), TTypes... > (value);
    }
};

}
