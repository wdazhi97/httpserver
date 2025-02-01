//
// Created by wdazhi on 2025/1/30.
//

#ifndef HTTPSERVER_HTTPSERVER_H
#define HTTPSERVER_HTTPSERVER_H
#include <memory>
#include <boost/noncopyable.hpp>
#include "connection.h"
#include <boost/asio/ssl.hpp>

class httpserver : public boost::noncopyable {
public:
    typedef std::shared_ptr<boost::asio::ip::tcp::socket> sock_ptr;

    httpserver(std::string ip, unsigned port)
    {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(ip), port);
        m_ssl_context.reset(new boost::asio::ssl::context(boost::asio::ssl::context::sslv23));
        m_ssl_context->set_password_callback([](std::size_t size, boost::asio::ssl::context_base::password_purpose purpose)->std::string {return "test@123456";});
        m_ssl_context->use_certificate_chain_file("server.crt");
        m_ssl_context->use_private_key_file("server_private.pem", boost::asio::ssl::context::pem);
        m_context.reset(new boost::asio::io_context());
        m_acceptor.reset(new boost::asio::ip::tcp::acceptor(*m_context, endpoint));
    }

    boost::asio::awaitable<void> run()
    {
        std::cout << "start run" << std::endl;
        while(true) {
            auto sock = co_await m_acceptor->async_accept(boost::asio::use_awaitable);
            std::cout << "new connection" << std::endl;
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(std::move(sock), *m_ssl_context);
            std::shared_ptr<connection> conn = std::make_shared<connection>(socket);
            boost::asio::co_spawn(*m_context, [conn]()->boost::asio::awaitable<void> {
                co_await conn->start();
            }, boost::asio::detached);
        }
    };

    void start()
    {
        std::cout << "start call" << std::endl;
        boost::asio::co_spawn(*m_context, [this]()->boost::asio::awaitable<void>{
            std::cout << "start inner call" << std::endl;
            co_await this->run();
            },boost::asio::detached);
        m_context->run();
    }

private:

    std::unique_ptr<boost::asio::io_context> m_context;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
    std::unique_ptr<boost::asio::ssl::context> m_ssl_context;
};


#endif //HTTPSERVER_HTTPSERVER_H
