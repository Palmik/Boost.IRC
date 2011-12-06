#ifndef IRC_CONNECTION_HPP
#define IRC_CONNECTION_HPP

#include <boost/asio.hpp>
#include <string>
#include <irc/message.hpp> // I probably should not include whole header for just one function

namespace irc
{
    
class connection
{
public:
    connection(std::string const& hostname, std::string const& servicename);

    void connect();
    void connect(std::string const& nickname,
                 std::string const& realname = std::string(),
                 std::string const& password = std::string());

    void send(std::string const& msg);
    
private:
    void do_connect();
    void do_authenticate(std::string const& nickname,
                         std::string const& realname = std::string(),
                         std::string const& password = std::string());
    void do_send(std::string const& msg);
    
    std::string hostname_m;    // e.g. irc.freenode.net
    std::string servicename_m; // e.g. 6667

    boost::asio::io_service      io_service_m;
    boost::asio::ip::tcp::socket socket_m;
};

inline connection::connection(std::string const& hostname, std::string const& servicename) :
    hostname_m(hostname), servicename_m(servicename), socket_m(io_service_m)
{
}

inline void connection::connect()
{
    do_connect();
}

inline void connection::connect(std::string const& nickname,
                         std::string const& realname,
                         std::string const& password)
{
    do_connect();
    do_authenticate(nickname, realname, password);
}

inline void connection::send(std::string const& msg)
{
    do_send(msg);
}

inline void connection::do_connect()
{
    using namespace boost::asio::ip;

    boost::asio::ip::tcp::resolver           resolver(io_service_m);
    boost::asio::ip::tcp::resolver::query    query(hostname_m, servicename_m);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    boost::asio::connect(socket_m, endpoint_iterator);
}

inline void connection::do_authenticate(std::string const& nickname,
                                 std::string const& realname,
                                 std::string const& password)
{    
    do_send(message::make_authenticate_command(nickname, realname, password));
}

inline void connection::do_send(std::string const& msg)
{
    boost::asio::write(socket_m, boost::asio::buffer(msg.data(), msg.size()), boost::asio::transfer_all());
}

}

#endif