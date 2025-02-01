//
// Created by wdazhi on 2025/1/30.
//

#ifndef HTTPSERVER_CONNECTION_H
#define HTTPSERVER_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <fstream>
#include "httprequest.hpp"
#include "httpresponse.hpp"

class connection : public std::enable_shared_from_this<connection>{

public:

    connection(boost::asio::ssl::stream<boost::asio::ip::tcp::socket> & socket) : m_socket(std::move(socket))
    {
    }

    boost::asio::awaitable<void> start()
    {
        co_await m_socket.async_handshake(boost::asio::ssl::stream_base::server, boost::asio::use_awaitable);
        for(;;) {
            unsigned long len = 0;
            try {
                len = co_await m_socket.async_read_some(boost::asio::buffer(m_buff), boost::asio::use_awaitable);
            }
            catch (const boost::system::system_error & e)
            {
                if(e.code() == boost::asio::error::eof)
                {
                    break;
                }
                else
                {
                    std::cerr << "Error " << e.what() << std::endl;
                    break;
                }
            }
            http::request_parser::result_type result;
            std::cout << "read form client\n" << std::string(m_buff, len) << std::endl;
            std::tie(result, std::ignore) = m_parser.parse(
                    m_request, m_buff, m_buff + len);
            if (result == http::request_parser::good) {
                handle_request(m_request, m_response);
                auto write_byte = co_await boost::asio::async_write(m_socket, m_response.to_buffers(), boost::asio::use_awaitable);
                std::cout << "send to client\n" << m_response.to_string() << " " << write_byte << std::endl;
                bool keep_alive = is_keep_alive();
                if (!keep_alive) {
                    break;
                }

                m_request = {};
                m_response = {};
                m_parser.reset();
            } else if (result == http::request_parser::bad) {
                m_response = build_response(http::status_type::bad_request);
                co_await m_socket.async_write_some(m_response.to_buffers(), boost::asio::use_awaitable);
                break;
            }
        }

        std::cout << "disconnect from client " << std::endl;
    }

    void handle_request(const http::request &req, http::response &rep) {
        // Decode url to path.
        std::string request_path;
        if (!url_decode(req.uri, request_path)) {
            rep = build_response(http::status_type::bad_request);
            return;
        }

        // Request path must be absolute and not contain "..".
        if (request_path.empty() || request_path[0] != '/' ||
            request_path.find("..") != std::string::npos) {
            rep = build_response(http::status_type::bad_request);
            return;
        }

        // If path ends in slash (i.e. is a directory) then add "index.html".
        if (request_path[request_path.size() - 1] == '/') {
            rep = build_response(http::status_type::ok);
            return;
        }

        // Determine the file extension.
        std::size_t last_slash_pos = request_path.find_last_of("/");
        std::size_t last_dot_pos = request_path.find_last_of(".");
        std::string extension;
        if (last_dot_pos != std::string::npos &&
            last_dot_pos > last_slash_pos) {
            extension = request_path.substr(last_dot_pos + 1);
        }

        // Open the file to send back.
        std::string full_path = m_doc_root + request_path;
        std::cout << "request file path " << full_path << std::endl;
        std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
        if (!is) {
            rep = build_response(http::status_type::not_found);
            return;
        }

        // Fill out the response to be sent to the client.
        rep.status = http::status_type::ok;
        char buf[512];
        while (is.read(buf, sizeof(buf)).gcount() > 0)
            rep.content.append(buf, is.gcount());
        rep.headers.resize(2);
        rep.headers[0].name = "Content-Length";
        rep.headers[0].value = std::to_string(rep.content.size());
        rep.headers[1].name = "Content-Type";
        rep.headers[1].value = http::mime_types::extension_to_type(extension);
    }

    bool url_decode(const std::string &in, std::string &out) {
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.size(); ++i) {
            if (in[i] == '%') {
                if (i + 3 <= in.size()) {
                    int value = 0;
                    std::istringstream is(in.substr(i + 1, 2));
                    if (is >> std::hex >> value) {
                        out += static_cast<char>(value);
                        i += 2;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (in[i] == '+') {
                out += ' ';
            } else {
                out += in[i];
            }
        }
        return true;
    }

    bool is_keep_alive() {
        for (auto &[k, v] : m_request.headers) {
            if (k == "Connection" && v == "close") {
                return false;
                break;
            }
        }
        return true;
    }

private:

    char m_buff[1024 * 4];
    http::request_parser m_parser;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> m_socket;
    http::request m_request;
    http::response m_response;
    std::string m_doc_root = ".";
};


#endif //HTTPSERVER_CONNECTION_H
