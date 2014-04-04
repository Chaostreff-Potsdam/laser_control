#pragma once
#include "../Painter.h"

namespace laser { namespace holodeck {

template <std::size_t... Indices>
struct indices {
	using next = indices<Indices..., sizeof...(Indices)>;
};

template <std::size_t N>
struct build_indices {
	using type = typename build_indices<N-1>::type::next;
};
template <>
struct build_indices<0> {
	using type = indices<>;
};
template <std::size_t N>
using BuildIndices = typename build_indices<N>::type;

template <size_t num_args>
struct InstructionCaller
{
	template <typename FuncType>
	ObjectPtr operator () (FuncType f, const std::vector<Point> &args){
		assert(args.size() >= num_args);
		return call(f, args, BuildIndices<num_args>{});
	}

private:
	template <typename FuncType, size_t... I>
	ObjectPtr call(FuncType f, const std::vector<Point> &args, indices<I...>){
		return (*f)(args[I]...);
	}
};

}} // namespace laser::holodeck
