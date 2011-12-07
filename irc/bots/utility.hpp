#ifndef IRC_BOTS_UTILITY
#define IRC_BOTS_UTILITY

namespace irc { namespace bot
{

template <typename B, typename C>
inline void attach(B& bot, C& connection)
{
    connection.sig_received().connect(boost::bind(&B::process, &bot, _1));
    bot.sig_responded().connect(boost::bind(&C::send, &connection, _1));
}
    
}}

#endif