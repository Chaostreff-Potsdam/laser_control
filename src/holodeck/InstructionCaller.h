#pragma once
#include "Instruction.h"
#include "../Painter.h"

#include <json/forwards.h>

#include <boost/config.hpp>

namespace laser { namespace holodeck {

#ifdef BOOST_NO_CXX11_VARIADIC_TEMPLATES

template <size_t num_args>
struct InstructionCaller;

template<>
struct InstructionCaller<1>
{
	template <typename FuncType>
	ObjectPtr operator() (FuncType f, const std::vector<Point> &ps) {
		return f(ps[0]);
	}
};

template<>
struct InstructionCaller<2>
{
	template <typename FuncType>
	ObjectPtr operator() (FuncType f, const std::vector<Point> &ps) {
		return f(ps[0], ps[1]);
	}
};

template<>
struct InstructionCaller<3>
{
	template <typename FuncType>
	ObjectPtr operator() (FuncType f, const std::vector<Point> &ps) {
		return f(ps[0], ps[1], ps[2]);
	}
};

template<>
struct InstructionCaller<4>
{
	template <typename FuncType>
	ObjectPtr operator() (FuncType f, const std::vector<Point> &ps) {
		return f(ps[0], ps[1], ps[2], ps[3]);
	}
};

template<>
struct InstructionCaller<5>
{
	template <typename FuncType>
	ObjectPtr operator() (FuncType f, const std::vector<Point> &ps) {
		return f(ps[0], ps[1], ps[2], ps[3], ps[4]);
	}
};

#else

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
	ObjectPtr operator () (FuncType f, Json::Value& root, const std::vector<Point> &args){
		assert(args.size() >= num_args);
		return call(f, root, args, BuildIndices<num_args>{});
	}

private:
	template <typename FuncType, size_t... I>
	ObjectPtr call(FuncType f, Json::Value& root, const std::vector<Point> &args, indices<I...>){

		InstructionPtr instruction = Instruction::construct(root);
		instruction->add((*f)(instruction, root, args[I]...));
		return instruction;

	}
};

#endif

}} // namespace laser::holodeck
