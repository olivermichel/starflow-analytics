
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/formatted_printer.h"
#include "../kernels/group_by.h"

template <typename key_t, typename argument_t, typename return_t>
class FlowletDetector
{
	using _state_t = std::pair<std::uint64_t, return_t>;

public:
	FlowletDetector() = delete;
	explicit FlowletDetector(std::uint64_t alpha)
		: _alpha(alpha) {}

	return_t operator()(key_t k, argument_t s)
	{
		typename std::map<key_t, _state_t>::iterator i;

		if ((i = _s.find(k)) == std::end(_s))
			i = _s.emplace(k, std::make_pair(s.ts_in_us, std::vector<unsigned int>(1))).first;

		if ((s.ts_in_us - i->second.first) > _alpha)
			i->second.second.push_back(1);
		else
			i->second.second.back() += 1;

		i->second.first = s.ts_in_us;

		return i->second.second;
	}

private:
	std::uint64_t _alpha;
	std::map<key_t, _state_t> _s;
};


template <typename key_t, typename argument_t, typename return_t>
class FlowletSizeHistogram
{
	using _state_t = unsigned int;

public:
	return_t operator()(key_t k, argument_t s)
	{
		typename std::map<key_t, _state_t>::iterator i;

		if ((i = _s.find(k)) == std::end(_s))
			i = _s.emplace(k, 0).first;

		i->second += 1;

		return i->second;
	}

private:

	std::map<key_t, _state_t> _s;
};


int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <data.clfr>" << std::endl;
		return 1;
	}

	namespace sf = starflow;

	using fl_counts_per_flow_t = std::pair<sf::types::Key, std::vector<unsigned int>>;
	using fl_counts_per_fl_size_t = std::pair<unsigned int, unsigned int>;


	FlowletDetector<sf::types::Key, sf::types::Packet, std::vector<unsigned>> flowlet(1000000); // 1s
	FlowletSizeHistogram<unsigned int, unsigned int, unsigned int> fl_histogram;

	sf::kernels::CLFRFileReader clfr_file_reader(argv[1]);

	sf::kernels::GroupBy<sf::types::CLFR, std::vector<unsigned>> fl_group(
		[&flowlet](const sf::types::CLFR& clfr) {

			std::vector<unsigned int> counts = {};

			for (auto& packet : clfr.packets())
				counts = flowlet(clfr.key(), packet);

			return counts;
	});

	sf::kernels::GroupBy<std::vector<unsigned int>, std::vector<std::pair<unsigned, unsigned>>> fl_size_histogram(
		[&fl_histogram](std::vector<unsigned int> fl_sizes) {

			std::vector<std::pair<unsigned, unsigned>> hist;

			for (auto& l : fl_sizes)
				hist.emplace_back(l, fl_histogram(l, 1));

			return hist;
	});

	sf::kernels::FormattedPrinter<std::vector<std::pair<unsigned, unsigned>>> printer(
		[](std::ostream& os, const std::vector<std::pair<unsigned, unsigned>>& v) {

			for (auto& p : v)
				std::cout << p.first << " -> " << p.second << std::endl;
		});


	raft::map m;
	m += clfr_file_reader >> fl_group >>fl_size_histogram >> printer;
	m.exe();
	return 0;
}