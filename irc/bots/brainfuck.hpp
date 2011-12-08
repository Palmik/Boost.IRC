#ifndef IRC_BOTS_BRAINFUCK_HPP
#define IRC_BOTS_BRAINFUCK_HPP

#include <string>
#include <iostream>

#include <boost/signals2/signal.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <irc/message.hpp>

#include <irc/bots/brainfuck/interpreter.hpp>

namespace irc { namespace bot
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
    
    void process(std::string const& msg)
    {
        clear();
        
        std::string content = message::get_message_content(msg);
        if (!boost::starts_with(content, command_m + ' '))
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

        sig_responded()(message::make_message(receiver, out_m.str()));
    }

    boost::signals2::signal<void (std::string)>& sig_responded() { return sig_responded_m; }

private:
    boost::signals2::signal<void (std::string)> sig_responded_m;
    
    void clear()
    {
        in_m.str("");
        out_m.str("");
        in_m.clear();
        out_m.clear();
    }
    
    std::string command_m;
    
    std::istringstream in_m;
    std::ostringstream out_m;
    ::brainfuck::interpreter interpreter_m;
};

}}

#endif