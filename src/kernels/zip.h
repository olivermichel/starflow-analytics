
#ifndef STARFLOW_ANALYTICS_KERNELS_ZIP_H
#define STARFLOW_ANALYTICS_KERNELS_ZIP_H

#include <raft>

namespace starflow {
	namespace kernels {

		template<typename K, typename I1, typename I2 = I1, typename O = std::pair<I1, I2>>
		class Zip : public raft::kernel
		{
		public:

			Zip(std::function<K (const I1&)>&& k1, std::function<K (const I2&)>&& k2)
				: _key_i1(k1), _key_i2(k2)
			{
				input.template add_port<I1>("in_1");
				input.template add_port<I2>("in_2");
				output.template add_port<O>("out");
			}

			raft::kstatus run() override
			{
				if (input["in_1"].size()) {

					I1 i1 { };
					input["in_1"].pop(i1);
					auto i = _i2_map.find(_key_i1(i1));

					if (i != std::end(_i2_map)) {
						output["out"].push(std::make_pair(i1, *i));
						_i2_map.erase(i);
					} else {
						_i1_map.emplace(_key_i1(i1), i1);
					}
				}

				if (input["in_2"].size()) {

					I2 i2 { };
					input["in_2"].pop(i2);
					auto i = _i1_map.find(_key_i2(i2));

					if (i != std::end(_i1_map)) {
						output["out"].push(std::make_pair(*i, i2));
						_i1_map.erase(i);
					} else {
						_i2_map.emplace(_key_i2(i2), i2);
					}
				}

				return raft::proceed;
			}

		private:
			std::map<K, I1> _i1_map;
			std::map<K, I2> _i2_map;
			std::function<K (const I1&)> _key_i1;
			std::function<K (const I2&)> _key_i2;
		};
	}
}


#endif
