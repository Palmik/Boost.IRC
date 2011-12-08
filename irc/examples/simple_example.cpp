#include <irc/connection.hpp>
#include <irc/message.hpp>
#include <irc/encoder.hpp>

#include <irc/bots/pong.hpp>
#include <irc/bots/slap_back.hpp>
#include <irc/bots/utility.hpp>

#include <iostream>

using namespace irc;

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
    std::cout << msg << std::endl;
}

void on_error(boost::system::error_code ec)
{
    std::cerr << "ERROR: " << ec << std::endl;
}

int main()
{
    // Setup connection
    connection irc(servername, port);
    irc.sig_connected().connect(boost::bind(on_connected, boost::ref(irc)));
    irc.sig_error().connect(on_error);
    irc.sig_received().connect(boost::bind(on_received, boost::ref(irc), _1));

    // Setup encoder
    encoder enc("utf8", "utf8", "utf8");
    
    // Setup pong bot
    bot::pong pong_bot;
    bot::attach_raw(pong_bot, irc);
    
    // Setup slap_back bot
    bot::slap_back slap_back_bot;
    slap_back_bot.protect("Palmik");
    slap_back_bot.protect(nickname);
    bot::attach_raw(slap_back_bot, irc);

    // Connect
    irc.connect();

    std::string msg;
    while (getline(std::cin, msg))
    {
        msg = message::make_message(channel, msg);
        irc.send(msg);
    }
}