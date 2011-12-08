#ifndef IRC_BOT_SLAP_BACK_HPP
#define IRC_BOT_SLAP_BACK_HPP

#include <iostream>
#include <set>

#include <boost/signals2/signal.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <irc/message.hpp>

namespace irc { namespace bot
{

class slap_back
{
public:
    slap_back(std::string const& appendix) :
        appendix_m(appendix)
    {}
    
    void process(std::string const& msg);
    boost::signals2::signal<void (std::string)>& sig_responded() { return sig_responded_m; }
    
    void protect(std::string const& nickname);
    bool is_protected(std::string const& nickname);

private:
    boost::signals2::signal<void (std::string)> sig_responded_m;
    
    std::set<std::string> protected_nicknames_m;
    std::string appendix_m;
};

void slap_back::process(std::string const& msg)
{
    std::string channel = message::get_message_addressee(msg);
    std::string sender = message::get_sender_nickname(msg);

    boost::regex expr("[^ ]* PRIVMSG " + channel + " :\001ACTION slaps ([^ \001]*).*");
    std::string receiver = message::utility::first_match(expr, msg);

    if (!channel.empty() &&
        !sender.empty() &&
        !receiver.empty() &&
        is_protected(receiver) &&
        !is_protected(sender))
    {
        sig_responded()(message::make_action(channel, "slaps " + sender + appendix_m) +
                        message::make_notice(sender, "Stay the fuck away from " + receiver));
    }
}

void slap_back::protect(std::string const& nickname)
{
    protected_nicknames_m.insert(boost::to_upper_copy(nickname));
}

bool slap_back::is_protected(std::string const& nickname)
{
    return (protected_nicknames_m.find(boost::to_upper_copy(nickname)) != protected_nicknames_m.end());
}
    
}}

#endif