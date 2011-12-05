#include <connection/socket/tcp.hpp>
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

int main()
{
    tcp::socket c;
    c.sig_connected().connect(handle_connected);
    c.sig_disconnected().connect(handle_disconnected);
    c.connect("ntp.cesnet.cz", "daytime");
}