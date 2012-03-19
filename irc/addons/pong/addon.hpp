#ifndef IRC_BOTS_PONG_HPP
#define IRC_BOTS_PONG_HPP

#include <boost/signals2/signal.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <irc/message/message.hpp>

namespace irc { namespace addon
{

class pong
{
public:
    void process(std::string const& msg)
    {
        if (boost::starts_with(msg, "PING")) {
            sig_responded()(message::make_pong_message("leave me alone!"));
        }
    }
    
    boost::signals2::signal<void (std::string)>& sig_responded() { return sig_responded_m; }

private:
    boost::signals2::signal<void (std::string)> sig_responded_m;
};

}}

#endif