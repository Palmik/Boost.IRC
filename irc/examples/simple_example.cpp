#include <irc/connection.hpp>
#include <irc/message.hpp>
#include <irc/encoder.hpp>

#include <irc/addons/pong/addon.hpp>
#include <irc/addons/slap_back/addon.hpp>
#include <irc/addons/brainfuck/addon.hpp>
#include <irc/addons/utility.hpp>

#include <iostream>

using namespace irc;

static const std::string servername = "irc.rizon.net";
static const std::string port = "6667";

static const std::string nickname = "sad_bot";
static const std::string channel = "#bots";

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
    addon::pong pong_bot;
    addon::attach_raw(pong_bot, irc);
    
    // Setup slap_back bot
    addon::slap_back slap_back_bot(" around a bit with a chainsaw.");
    slap_back_bot.protect("Palmik");
    slap_back_bot.protect(nickname);
    addon::attach_raw(slap_back_bot, irc);

    // Setup brainfuck bot
    addon::brainfuck brainfuck_bot("@brainfuck");
    brainfuck_bot.ignore(nickname);
    addon::attach_raw(brainfuck_bot, irc);
    
    // Connect
    irc.connect();

    std::string msg;
    while (getline(std::cin, msg))
    {
        msg = message::make_message(channel, msg);
        irc.send(msg);
    }
}