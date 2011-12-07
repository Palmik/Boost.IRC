#include <irc/connection.hpp>
#include <irc/message.hpp>

#include <irc/bots/slap_back.hpp>
#include <irc/bots/utility.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <iostream>

using namespace irc;
using namespace std;

static const std::string servername = "irc.rizon.net";
static const std::string port = "6667";

static const std::string nickname = "BoostBot";
static const std::string channel = "#soulwell";

void on_connected(connection& irc)
{
    irc.send(message::make_authenticate_command(nickname, nickname));
    irc.send(message::make_join_channel_command(channel));
}

void on_received(connection& irc, std::string msg)
{
    cout << msg << endl;
    if (boost::starts_with(msg, "PING")) {
        irc.send(message::make_pong_message("leave me alone!"));
    }
}

void on_error(boost::system::error_code ec)
{
    cerr << "ERROR: " << ec << endl;
}

int main()
{
    
    // Setup bot
    bot::slap_back bot1;
    bot1.protect("Palmik");
    bot1.protect(nickname);

    // Setup connection
    connection irc(servername, port);
    irc.sig_connected().connect(boost::bind(on_connected, boost::ref(irc)));
    irc.sig_error().connect(on_error);
    irc.sig_received().connect(boost::bind(on_received, boost::ref(irc), _1));

    // Attach bot to connection
    bot::attach(bot1, irc);
    
    irc.connect();

    std::string msg;
    while (getline(std::cin, msg))
    {
        msg = message::make_message(channel, msg);
        irc.send(msg);
    }
}