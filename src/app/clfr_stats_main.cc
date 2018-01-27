
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "../modules/clfr_file_reader.h"

int main(int argc, char** argv)
{
	namespace sf = starflow;


	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <data.clfr>" << std::endl;
		return 1;
	}

	try {

		unsigned long clfr_count = 0;
		sf::types::CLFR  first{}, last{}, current{};

		sf::modules::CLFRFileReader clfr_reader(argv[1]);

		while(!clfr_reader.end()) {

			clfr_reader.read_clfr(current);

			if (!clfr_count)
				first = current;

			clfr_count++;
		}

		last = current;
		std::time_t first_evict_time = (first.evict_ts().count() /1000000);
		std::time_t last_evict_time = (last.evict_ts().count() /1000000);

		std::cout << "File name:                 " << argv[1] << std::endl;
		std::cout << "Number of CLFRs:           " << clfr_count << std::endl;
		std::cout << "First eviction:            "
				  << std::put_time(std::localtime(&first_evict_time), "%F %H:%M:%S%z") << std::endl;
		std::cout << "Last eviction:             "
				  << std::put_time(std::localtime(&last_evict_time), "%F %H:%M:%S%z") << std::endl;

	} catch (std::runtime_error& e) {
		std::cerr << "Error reading CLFR File: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}