
#ifndef STARFLOW_ANALYTICS_KERNELS_REPLICATOR_H
#define STARFLOW_ANALYTICS_KERNELS_REPLICATOR_H

#include <raft>

namespace starflow {
	namespace kernels {
		template<typename T>
		class Replicator : public raft::kernel
		{
		public:
			explicit Replicator(unsigned n = 2)
				: _out_ports {}
				{
					input.template add_port<T>("in");

					for (unsigned i = 1; i <= n; i++) {
						std::string port_name = "out_" + std::to_string(i);
						output.template add_port<T>(port_name);
						_out_ports.push_back(port_name);
					}
				}

			raft::kstatus run() override
			{
				T i;
				input["in"].pop(i);

				for (auto& p : output) {
					p.push(i);
				}


				return raft::proceed;
			}

		private:
			std::vector<std::string> _out_ports;
		};
	}
}

#endif
