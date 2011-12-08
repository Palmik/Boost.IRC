#ifndef IRC_MESSSAGE_HPP
#define IRC_MESSSAGE_HPP

#include <boost/regex.hpp>

namespace irc { namespace message
{

namespace utility
{
    std::string first_match(boost::regex const& expr, std::string const& text)
    {
        std::string result;
        boost::match_results<std::string::const_iterator> what;
        if (boost::regex_match(text, what, expr))
        {
            result = what[1];
        }

        return result;
    }

    bool does_match(boost::regex const& expr, std::string const& text)
    {
        boost::match_results<std::string::const_iterator> what;
        return boost::regex_match(text, what, expr);
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

inline std::string get_message_addressee(std::string const& message)
{
    boost::regex expr("[^ ]*(?: NOTICE | PRIVMSG )(#[^ ]*) :.*");
    return utility::first_match(expr, message);
}

inline std::string get_sender_nickname(std::string const& message)
{
    boost::regex expr(":([^!]*)!.*");
    return utility::first_match(expr, message);
}

inline std::string get_sender_mask(std::string const& message)
{
    boost::regex expr("[^~]*~.*@([^ ]*).*");
    return utility::first_match(expr, message);
}

inline std::string get_sender_realname(std::string const& message)
{
    boost::regex expr("[^~]*~([^@]*)@.*");
    return utility::first_match(expr, message);
}

inline std::string get_message_content(std::string const& message)
{
    boost::regex expr(":.*!~.*@.* (?:PRIVMSG|NOTICE) .* :(?:\001ACTION )?(.*)");
    return utility::first_match(expr, message);
}

inline bool is_message(std::string const& message)
{
    boost::regex expr(":.*!~.*@.* PRIVMSG .* :.*");
    return utility::does_match(expr, message);
}

inline bool is_notice(std::string const& message)
{
    boost::regex expr(":.*!~.*@.* NOTICE .* :.*");
    return utility::does_match(expr, message);
}

inline bool is_action(std::string const& message)
{
    boost::regex expr(":.*!~.*@.* PRIVMSG .* :\001ACTION .*\001");
    return utility::does_match(expr, message);
}
    
}}

#endif