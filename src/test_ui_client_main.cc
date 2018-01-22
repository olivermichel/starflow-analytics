
#include "modules/ui_client.h"

int main(int argc, char** argv)
{
	starflow::modules::UIClient client("127.0.0.1:23202");

	if (client.say_hello())
		std::cout << "hello success" << std::endl;
	else
		std::cerr << "hello failed" << std::endl;

	return 0;
}
