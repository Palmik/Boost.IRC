#include <connection/socket/tcp.hpp>
#include <boost/array.hpp>
#include <iostream>
using namespace std;
using namespace connection;

void handle_connected()
{
    cout << "Connected, yay!" << endl;
}

void handle_disconnected()
{
    cout << "Disconnected, noo!" << endl;
}

void handle_read(tcp::socket& s)
{
    cout << "Read ready!" << endl;
}

void test(boost::array<char, 128>& buf)
{
std::cout.write(buf.data(), 128);
}

int main()
{
    tcp::socket c;
    c.sig_connected().connect(handle_connected);
    c.sig_disconnected().connect(handle_disconnected);
    c.sig_read_ready().connect(boost::bind(handle_read, boost::ref(c)));
    c.open("ntp.cesnet.cz", "daytime");


//       boost::array<char, 128> buf;
//       boost::system::error_code error;
// 
//       c.async_read_some(boost::asio::buffer(buf), boost::bind(test, ref(buf)));

}