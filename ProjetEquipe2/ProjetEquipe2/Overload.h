#ifndef OVERLOAD_H
#define OVERLOAD_H

#include <variant>
#include <utility>
#include <type_traits>

// constexpr here for compile time execution

//overload needed for static polymorphism with my visitation
template<class...Ts>
struct overloaded :Ts...{
	template<class ...Fs>
	constexpr overloaded(Fs&&...ts) :Ts{ std::forward<Fs>(ts) }...{}//add perfect forwarding to my overload 
	using Ts::operator()...;
};

template<class...Ts>
overloaded(Ts...)->overloaded<Ts...>;

template<typename ...Ts>
constexpr auto CreateOverload(Ts&&... ts)
{
	return overloaded<std::decay_t<Ts>...>{std::forward<Ts>(ts)...};//decay to conversion( like function to pointer etc...) and perfect forwarding.
}

//finally function for visitation
template<typename Variants, typename... Ts>
constexpr decltype(auto) match(Variants&& v, Ts&&...ts)
{
	return std::visit(
		CreateOverload(std::forward<Ts>(ts)...), std::forward<Variants>(v));
}

#endif
