#include <irc/connection.hpp>
#include <irc/message.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <iostream>

using namespace irc;
using namespace std;

void connected_slot(connection& irc)
{
    irc.send(message::make_authenticate_command("TestTestBoost", "jmeno"));
}

void received_slot(connection& irc, std::string msg)
{
    cout << msg << endl;
    if (boost::starts_with(msg, "PING")) {
        irc.send(message::make_pong_message("leave me alone!"));
    }
}

int main()
{
    connection irc("irc.rizon.net", "6667");
    irc.sig_connected().connect(boost::bind(connected_slot, boost::ref(irc)));
    irc.sig_received().connect(boost::bind(received_slot, boost::ref(irc), _1));    
    irc.connect();

    irc.send(message::make_join_channel_command("#boost-bot"));
    irc.send(message::make_action("Palmik", "lolol"));

    std::string msg;
    while (getline(std::cin, msg))
    {
        msg = message::make_message("#boost-bot", msg);
        msg = message::make_notice("#boost-bot", msg);
        irc.send(msg);
    }
}