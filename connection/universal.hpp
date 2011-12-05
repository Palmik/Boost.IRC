#ifndef BOOST_SOCKET_WRAPPER
#define BOOST_SOCKET_WRAPPER

#include <boost/asio.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <vector>

#include <iostream>

namespace connection
{

class connection : public boost::noncopyable
{

// PUBLIC TYPES
public:
    typedef boost::asio::ip::tcp::socket      socket_type;
    typedef boost::asio::ip::tcp::resolver    resolver_type;
    typedef std::vector<boost::uint8_t>       buffer_type;

    typedef boost::system::error_code         error_type;
    typedef resolver_type::iterator           iterator_type;

// PUBLIC INTERFACE
public:    
    connection();

    bool is_connected() const;
    
    void connect(char const* host, char const* service);
    void disconnect();

    void send(buffer_type const& buffer);

    // SIGNALS
    boost::signals2::signal<void ()>& sig_connected();
    boost::signals2::signal<void ()>& sig_disconnected();

// PRIVATE TYPES
private:
    typedef connection connection_type;

// PRIVATE INTERFACE
private:
    boost::asio::io_service&       io_service()       { return io_service_m; }
    boost::asio::io_service const& io_service() const { return io_service_m; }
    socket_type&       socket()       { return socket_m; }
    socket_type const& socket() const { return socket_m; }

    void resolve_handler(error_type const& error, iterator_type iterator);
    void connect_handler(error_type const& error, iterator_type iterator);

    void on_send();
    void on_received();    
    void on_error();

// PRIVATE VARIABLES
private:    
    // NETWORK OBJECTS.
    boost::asio::io_service      io_service_m;
    socket_type socket_m;
};

// IMPLEMENTATION

// PUBLIC INTERFACE
inline connection::connection() :
    socket_m(io_service_m)
{
}

inline bool connection::is_connected() const
{
    return socket().is_open();
}

inline void connection::connect(char const* host, char const* service)
{
    if (is_connected()) {
        return;
    }
    
    resolver_type        resolver(io_service());    
    resolver_type::query query(host, service);
    
    resolver.async_resolve(query, boost::bind(&connection_type::resolve_handler, this, _1, _2));
}

inline void connection::disconnect()
{
    socket().close();
    sig_disconnected()();
}

inline void connection::send(buffer_type const& buffer)
{
    // TODO: Implement asynchronous send.
}

// PUBLIC SIGNALS
inline boost::signals2::signal<void ()>& connection::connected()
{
    return sig_connected_m;
}

inline boost::signals2::signal<void ()>& connection::disconnected()
{
    return sig_disconnected_m;
}

// PRIVATE INTERFACE
inline void connection::resolve_handler(error_type const& error, iterator_type iterator)
{
    if (!error)
    {
        // Resolved successfully
        boost::asio::async_connect(socket(), iterator, boost::bind(&connection_type::connect_handler, this, _1, _2));
    }
    else
    {
        // TODO: Handle unsuccessful resolution (try again? emit error?).
    }
}

inline void connection::connect_handler(error_type const& error, iterator_type iterator)
{
    if (!error)
    {
        // Connected successfully
        sig_connected()();
        // TODO: 
    }
    else
    {
        // TODO: Handle unsuccessful connection (try again? emit error?)
    }
}

}

#endif 
