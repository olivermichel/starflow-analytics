
#include <raft>

#include "../kernels/zip.h"

class A
{
public:
	unsigned id;
	A() = default;
	A(unsigned id) : id(id) { }
};

class B : public A
{
public:
	bool test;
	B() = default;
	B(unsigned id, bool test) : A(id), test(test) { }
};

class Source : public raft::kernel
{
public:
	Source()
	{
		output.add_port<A>("a");
		output.add_port<B>("b");
	}

	raft::kstatus run() override
	{
		for (unsigned i = 0; i < 1000; i++) {
			output["a"].push(A(i));
			output["b"].push(B(i, true));
		}

		return raft::stop;
	}
};

template <typename T>
class Sink : public raft::kernel
{
public:
	Sink()
	{
		input.template add_port<T>("in");
	}

	raft::kstatus run() override
	{
		T t;
		input["in"].pop(t);

		std::cout << t.first.id << "/" << t.second.id << std::endl;

		return raft::proceed;
	}
};



int main(int argc, char** argv)
{
	raft::map m;

	Source src;
	starflow::kernels::Zip<unsigned, A, B>
		zip([](const A& a) { return a.id; }, [](const B& b) { return b.id; });

	Sink<std::pair<A,B>> snk;

	m += src["a"] >> zip["in_1"];
	m += src["b"] >> zip["in_2"];
	m += zip >> snk;

	m.exe();


	return 0;
}