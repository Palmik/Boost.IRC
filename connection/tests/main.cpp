#include <connection/universal.hpp>

int main()
{
    connection::connection c;
    c.connect("127.0.0.1", "80");

    while(true)
    {
    }
}