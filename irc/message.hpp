#ifndef IRC_MESSSAGE_HPP
#define IRC_MESSSAGE_HPP

namespace irc { namespace message
{

inline std::string make_join_channel_command(std::string const& channel)
{
    return "JOIN " + channel + "\r\n";
}

inline std::string make_leave_channel_command(std::string const& channel, std::string const& msg)
{
    return "PART " + channel + " :" + msg + "\r\n";
}

inline std::string make_quit_command(std::string const& msg)
{
    return "QUIT :" + msg + "\r\n";
}

inline std::string make_message(std::string const& receiver, std::string const& msg)
{
    return "PRIVMSG " + receiver + " :" + msg + "\r\n";
}

inline std::string make_notice(std::string const& receiver, std::string const& msg)
{
    return "NOTICE " + receiver + " :" + msg + "\r\n";
}

inline std::string make_action(std::string const& receiver, std::string const& msg)
{
    return "PRIVMSG " + receiver + " :" + char(1) + "ACTION " + msg + "\r\n";
}
    
}}

#endif