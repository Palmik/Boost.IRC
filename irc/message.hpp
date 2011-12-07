#ifndef IRC_MESSSAGE_HPP
#define IRC_MESSSAGE_HPP

#include <boost/regex.hpp>

namespace irc { namespace message
{

namespace utility
{
    std::string first_match(std::string const& expr_str, std::string const& text)
    {
        boost::regex expr(expr_str);
        std::string result;
        boost::match_results<std::string::const_iterator> what;
        if (boost::regex_match(text, what, expr))
        {
            result = what[1];
        }

        return result;
    }
}

inline std::string make_authenticate_command(std::string const& nickname,
                                             std::string const& realname = std::string(),
                                             std::string const& password = std::string())
{

    std::string msg("NICK " + nickname + "\r\n"
                    "USER " + nickname + " 0 * :" + realname + "\r\n");

    if (!password.empty()) {
        msg = "PASS" + password + "\r\n" + msg;
    }

    return msg;
}

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

inline std::string make_kick_command(std::string const& channel,
                                     std::string const& nickname,
                                     std::string const& reason = std::string())
{
    return "KICK " + channel + ' ' + nickname + " :" + reason + "\r\n";
}

inline std::string make_pong_message(std::string const& msg)
{
    return "PING :" + msg + "\r\n";
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
    return "PRIVMSG " + receiver + " :" + char(1) + "ACTION " + msg + char(1) + "\r\n";
}

inline std::string get_sender_nickname(std::string const& message)
{
    return utility::first_match(":([^!]*)!.*", message);
}

inline std::string get_sender_channel(std::string const& message)
{
    return utility::first_match("[^ ]*(?: NOTICE | PRIVMSG )(#[^ ]*) :.*", message);
}

inline std::string get_sender_mask(std::string const& message)
{
    return utility::first_match("[^~]*~([^ ]*) .*", message);
}
    
}}

#endif