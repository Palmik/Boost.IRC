#include <irc/connection.hpp>
#include <irc/message.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <iostream>

using namespace irc;
using namespace std;

std::string servername = "irc.rizon.net";
std::string port = "6667";

std::string nickname = "TestBoostBot";
std::string channel = "#boost-bot";

void connected_slot(connection& irc)
{
    irc.send(message::make_authenticate_command(nickname, nickname));
    irc.send(message::make_join_channel_command(channel));
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
    connection irc(servername, port);
    
    irc.sig_connected().connect(boost::bind(connected_slot, boost::ref(irc)));
    irc.sig_received().connect(boost::bind(received_slot, boost::ref(irc), _1));    
    irc.connect();

    std::string msg;
    while (getline(std::cin, msg))
    {
        msg = message::make_message(channel, msg);
        irc.send(msg);
    }
}