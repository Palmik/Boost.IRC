#ifndef IRC_BOT_SLAP_BACK_HPP
#define IRC_BOT_SLAP_BACK_HPP

#include <boost/signals2/signal.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <irc/message.hpp>
#include <iostream>
#include <set>

namespace irc { namespace bot
{

class slap_back
{
public:
    template <typename Iterator>
    slap_back(Iterator begin, Iterator end);
    slap_back() {}
    
    void process(std::string const& msg);
    boost::signals2::signal<void (std::string)>& sig_responded() { return sig_responded_m; }
    
    void protect(std::string const& nickname);
    bool is_protected(std::string const& nickname);

private:
    boost::signals2::signal<void (std::string)> sig_responded_m;
    
    std::set<std::string> protected_nicknames_m;
};

template <typename Iterator>
slap_back::slap_back(Iterator begin, Iterator end) :
    protected_nicknames_m(begin, end)
{
}

void slap_back::process(std::string const& msg)
{
    std::string channel = message::get_sender_channel(msg);
    std::string sender = message::get_sender_nickname(msg);

    std::string expr("[^ ]* PRIVMSG " + channel + " :\001ACTION slaps ([^ \001]*).*");
    std::string receiver = message::utility::first_match(expr, msg);

    std::cerr << '|' << channel << '|' << sender << '|' << receiver << std::endl;

    if (!channel.empty() &&
        !sender.empty() &&
        !receiver.empty() &&
        is_protected(receiver) &&
        !is_protected(sender))
    {
        sig_responded()(message::make_action(channel, "slaps " + sender) +
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