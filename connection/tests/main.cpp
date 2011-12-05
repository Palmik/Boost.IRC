#include <connection/universal.hpp>

int main()
{
    connection::connection c;
    c.connect("nist1-ny.ustiming.org", "daytime");

    while(true)
    {
    }
}