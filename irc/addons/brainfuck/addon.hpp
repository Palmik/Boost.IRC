#ifndef IRC_BOTS_BRAINFUCK_HPP
#define IRC_BOTS_BRAINFUCK_HPP

#include <string>
#include <iostream>
#include <set>

#include <boost/signals2/signal.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <irc/message/message.hpp>

#include <irc/addons/brainfuck/interpreter.hpp>

namespace irc { namespace addon
{

class brainfuck
{
public:
    brainfuck(std::string const& command) :
        command_m(command),
        in_m(""),
        out_m(""),
        interpreter_m(in_m, out_m)
    {}

    void ignore(std::string const& nickname)
    {
        ignored_nicknames_m.insert(boost::to_upper_copy(nickname));
    }

    bool is_ignored(std::string const& nickname) const
    {
        return ignored_nicknames_m.find(boost::to_upper_copy(nickname)) != ignored_nicknames_m.end();
    }
    
    void process(std::string const& msg)
    {
        clear();
        
        std::string content = message::get_message_content(msg);
        if (!boost::starts_with(content, command_m + ' ') ||
            is_ignored(message::get_sender_nickname(msg)))
        {
            return;
        }
        content = content.substr(command_m.size() + 1);
        boost::trim(content);

        std::string receiver = message::get_response_receiver(msg);

        boost::regex expr1("([^ ]*) ?.*");
        std::string program = message::utility::first_match(expr1, content);
        boost::regex expr2(".* < (.*)");
        std::string input = message::utility::first_match(expr2, content);

        in_m.str(input);

        try
        {
            interpreter_m.interpret(program, 500000, 3000);
        }
        catch(std::exception& ex)
        {
            std::cerr << "Exception: " << ex.what() << std::endl;
            
            sig_responded()(message::make_message(receiver, ex.what()));
            return;
        }

        if (out_m.str().empty())
        {
            sig_responded()(message::make_message(receiver, "No output."));
        }
        else if (out_m.str() == last_output_m)
        {
            sig_responded()(message::make_message(receiver, "No change in output."));
        }
        else
        {
            sig_responded()(message::make_message(receiver, out_m.str()));
        }
    }

    boost::signals2::signal<void (std::string)>& sig_responded() { return sig_responded_m; }

private:
    boost::signals2::signal<void (std::string)> sig_responded_m;
    
    void clear()
    {
        last_output_m = out_m.str();
        
        in_m.str("");
        out_m.str("");
        in_m.clear();
        out_m.clear();
    }

    std::string last_output_m;
    std::string command_m;
    
    std::istringstream in_m;
    std::ostringstream out_m;
    ::brainfuck::interpreter interpreter_m;

    std::set<std::string> ignored_nicknames_m;
};

}}

#endif