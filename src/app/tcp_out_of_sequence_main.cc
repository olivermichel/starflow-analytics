
#include <raft>

#include "../etc/format_helpers.h"
#include "../kernels/clfr_file_reader.h"
#include "../kernels/formatted_printer.h"
#include "../kernels/group_by.h"

template <typename key_t, typename argument_t, typename return_t>
class OOS
{
public:

	using state_t = std::pair<argument_t, return_t>;

	return_t operator()(key_t k, argument_t s) {

		auto i = _s.find(k);

		if (i == std::end(_s)) {
			_s.insert(std::make_pair(k, std::make_pair(s, 0)));
			return 0;
		} else {
			std::uint32_t last_seq = i->second.first;
		}

		return v;
	}

private:
	std::map<key_t, state_t> _s;
};


int main(int argc, char** argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <data.clfr>" << std::endl;
		return 1;
	}

	namespace sf = starflow;
	//	using oos_per_flow_t = std::pair<sf::types::Key, std::pair<std::uint32_t, unsigned>>;

	// key_t, argument_t, return_t
	OOS<sf::types::Key, std::pair<std::uint32_t, std::uint16_t>, uint16_t> oos;


	return 0;
}