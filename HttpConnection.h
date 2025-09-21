#pragma once
#include "const.h"
#include <string>
#include <unordered_map>
class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
    friend class LogicSystem;
public:
    HttpConnection(tcp::socket socket);
    HttpConnection(boost::asio::io_context& ioc);
    void Start();
    void PreParseGetParam();
private:
    void CheckDeadline();
    void WriteResponse();
    void HandleReq();
    tcp::socket  _socket;
    // The buffer for performing reads.
    beast::flat_buffer  _buffer{ 8192 };

    // The request message.
    http::request<http::dynamic_body> _request;

    // The response message.
    http::response<http::dynamic_body> _response;
    std::string _get_url;
    std::unordered_map<std::string, std::string> _get_params;
    // The timer for putting a deadline on connection processing.
    net::steady_timer deadline_{
        _socket.get_executor(), std::chrono::seconds(60) };
    unsigned char ToHex(unsigned char x);
    unsigned char FromHex(unsigned char x);
    std::string UrlEncode(const std::string& str);
    std::string UrlDecode(const std::string& str);
};
