#ifndef IRC_CONNECTION_HPP
#define IRC_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/signals2/signal.hpp>

#include <iostream>
#include <string>

namespace irc
{
    
class connection
{
public:
    connection(std::string const& hostname, std::string const& servicename);
    ~connection();

    void connect();
    
    void send(std::string const& msg);

    boost::signals2::signal<void ()>&            sig_connected() { return sig_connected_m; }
    boost::signals2::signal<void (std::string)>& sig_received()  { return sig_received_m; }
    
private:
    void do_connect();
    void do_send(std::string const& msg);
    
    void do_start_read();
    void do_read();

    void handle_read(boost::system::error_code const& error,
                     std::size_t bytes_transferred);
    
    std::string hostname_m;    // e.g. irc.freenode.net
    std::string servicename_m; // e.g. 6667

    boost::asio::io_service      io_service_m;
    boost::asio::ip::tcp::socket socket_m;
    
    boost::thread                read_thread_m;
    boost::thread                send_thread_m;

    boost::asio::streambuf       read_buffer_m;

    // SIGNAL OBJECTS
    boost::signals2::signal<void ()>            sig_connected_m;
    boost::signals2::signal<void (std::string)> sig_received_m;
};

inline connection::connection(std::string const& hostname, std::string const& servicename) :
    hostname_m(hostname), servicename_m(servicename), socket_m(io_service_m)
{
}

inline connection::~connection()
{
    socket_m.close();
    
    read_thread_m.join();
    send_thread_m.join();
}

inline void connection::connect()
{
    do_connect();
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
    sig_connected()();
    
    do_start_read();
}

inline void connection::do_send(std::string const& msg)
{
    boost::asio::write(socket_m, boost::asio::buffer(msg.data(), msg.size()), boost::asio::transfer_all());
}

inline void connection::do_start_read()
{
    do_read();
    read_thread_m = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_m));
}

inline void connection::do_read()
{
    boost::asio::async_read_until(socket_m, read_buffer_m, "\r\n",
                                  boost::bind(&connection::handle_read, this, _1, _2));
}

inline void connection::handle_read(boost::system::error_code const& error,
                                    std::size_t bytes_transferred)
{
    std::istream is(&read_buffer_m);
    std::string line;
    
    while (std::getline(is, line)) {
        sig_received()(line);
    }

    do_read();
}

}

#endif