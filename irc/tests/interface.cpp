#include <irc/connection.hpp>
#include <irc/message.hpp>
#include <iostream>

using namespace irc;
using namespace std;

int main()
{
    connection irc("irc.rizon.net", "6667");
    irc.connect("TestTestBoost", "jmeno");

    irc.send(message::make_join_channel_command("#soulwell"));
    irc.send(message::make_action("Palmik", "lolol"));

    std::string msg;
    while (getline(std::cin, msg))
    {
        msg = message::make_message("#soulwell", msg);
        irc.send(msg);
    }
}