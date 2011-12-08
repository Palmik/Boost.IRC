#ifndef IRC_BOTS_BRAINFUCK_INTERPRETER_HPP
#define IRC_BOTS_BRAINFUCK_INTERPRETER_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

#include <boost/timer/timer.hpp>

namespace brainfuck
{

class time_limit_exceeded : public std::runtime_error
{
public:
    explicit time_limit_exceeded(std::string const& what) : std::runtime_error(what) {}
};

class memory_limit_exceeded : public std::runtime_error
{
public:
    explicit memory_limit_exceeded(std::string const& what) : std::runtime_error(what) {}
};

class interpreter
{
public:
    interpreter(std::istream& in, std::ostream& out) :
        in_m(in),
        out_m(out),
        max_memory_m(0),
        max_miliseconds_m(0),
        program_pointer_m(0),
        memory_pointer_m(0),
        memory_m(1, 0)
    {
        timer_m.stop();
    }

    void interpret(std::string const& program, std::size_t max_memory, std::size_t max_miliseconds)
    {
        reset(program, max_memory, max_miliseconds);
        try
        {
            for (program_pointer_m = 0; program_pointer_m < program_m.size(); ++program_pointer_m)
            {
                switch (program_m.at(program_pointer_m))
                {
                case '>': inc_pointer(); break;
                case '<': dec_pointer(); break;
                case '+': inc_value(); break;
                case '-': dec_value(); break;
                case '[': skip_right(); break;
                case ']': skip_left(); break;
                case '.': print(); break;
                case ',': read(); break;
                default : throw std::invalid_argument("Ill formed program |" + program_m + "|");
                }

                check_limits();
            }
        }
        catch(std::invalid_argument&)
        {
            throw;
        }
        catch(time_limit_exceeded&)
        {
            throw;
        }
        catch(memory_limit_exceeded&)
        {
            throw;
        }
        catch(...)
        {
            throw std::runtime_error("Undefined behaviour detected.");
        }        
    }

    std::string result;
    
private:
    void reset(std::string program, std::size_t max_memory, std::size_t max_miliseconds);
    void check_limits();

    void inc_pointer();
    void dec_pointer();
    void inc_value();
    void dec_value();
    void skip_right();
    void skip_left();
    void print();
    void read();
    
    std::size_t pointer() { return memory_pointer_m; }
    
    std::istream& in_m;
    std::ostream& out_m;

    std::size_t max_memory_m;
    std::size_t max_miliseconds_m;

    std::string program_m;

    std::size_t program_pointer_m;
    std::size_t memory_pointer_m;

    std::vector<unsigned char> memory_m;

    boost::timer::cpu_timer timer_m;
};

void interpreter::inc_pointer()
{
    if (memory_pointer_m >= (memory_m.size() - 1)) {
        memory_m.push_back(0);
    }
    ++memory_pointer_m;
}

void interpreter::dec_pointer()
{
    if (memory_pointer_m == 0)
    {
        std::runtime_error("Undefined behaviour detected.");
    }
    --memory_pointer_m;
}

void interpreter::inc_value()
{
    ++(memory_m.at(pointer()));
}

void interpreter::dec_value()
{
    --(memory_m.at(pointer()));
}

void interpreter::skip_right()
{
    if (memory_m.at(pointer()) != 0) {
        return;
    }

    char next = program_m.at(++program_pointer_m);
    while (next != ']')
    {
        next = program_m.at(++program_pointer_m);
    }
}

void interpreter::skip_left()
{
    if (memory_m.at(pointer()) == 0) {
        return;
    }

    char next = program_m.at(--program_pointer_m);
    while (next != '[')
    {
        next = program_m.at(--program_pointer_m);
    }
}

void interpreter::print()
{
    out_m << ((char)memory_m.at(pointer()));
}

void interpreter::read()
{
    in_m >> (memory_m.at(pointer()));
}

void interpreter::reset(std::string program, std::size_t max_memory, std::size_t max_miliseconds)
{
    program_m = program;
    max_memory_m = max_memory;
    max_miliseconds_m = max_miliseconds;

    program_pointer_m = 0;
    memory_pointer_m = 0;

    std::fill(memory_m.begin(), memory_m.end(), 0);

    timer_m.stop();
    timer_m.start();
}

void interpreter::check_limits()
{    
    if (memory_pointer_m > max_memory_m)
    {
        throw memory_limit_exceeded("Interpreter exceeded maximum memory limit.");
    }

    if ((timer_m.elapsed().wall / 1000000) > max_miliseconds_m)
    {
        throw memory_limit_exceeded("Interpreter exceeded maximum time limit.");
    }
}

}

#endif