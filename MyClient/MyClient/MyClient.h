#include <string>

class MyClient
{
	public:
		void start();
		MyClient(std::string address);

	private:
		MyClient();

		std::string m_address;
};