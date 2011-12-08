#ifndef IRC_BOTS_UTILITY_HPP
#define IRC_BOTS_UTILITY_HPP

namespace irc { namespace bot
{

template <typename B, typename C>
inline void attach_raw(B& bot, C& con)
{
    con.sig_received().connect(boost::bind(&B::process, &bot, _1));
    bot.sig_responded().connect(boost::bind(&C::send, &con,   _1));
}

template <typename B, typename C, typename E>
inline void attach(B& bot, C& con, E& enc)
{
    con.sig_received().connect(boost::bind(&B::process, &bot,
                                           boost::bind(&E::encode_read, &enc, _1)));
    
    bot.sig_responded().connect(boost::bind(&C::send,   &con,
                                           boost::bind(&E::encode_send, &enc, _1)));
}
    
}}

#endif