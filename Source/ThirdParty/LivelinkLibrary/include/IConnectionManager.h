#pragma once
#include <sio_message.h>
#include <functional>
namespace rad::live_client
{

enum class CloseReason
{
    normal,
    drop
};

using ConListener = std::function<void(void)>;
using CloseListener = std::function<void(const CloseReason& reason)>;
using EventAction = std::function<void(const std::string& name, sio::message::ptr const& message, bool need_ack, sio::message::list& ack_message)>;
using ErrorAction = std::function<void(sio::message::ptr const& message)>;
//using reconnectListener = std::function<void(unsigned, unsigned)>;
//using socketListener = std::function<void(const std::string& nsp)>;
class IConnectionManager
{
public:
    virtual void createSocket() = 0;
    virtual void clearConnectionListeners() = 0;
    virtual void clearSocketListeners() = 0;
    virtual bool isOpened() = 0;
    virtual void setOnOpen(const ConListener& cb) = 0;
    virtual void setOnFail(const ConListener& cb) = 0;
    virtual void setOnClose(const CloseListener& cb) = 0;
    virtual void connect(const std::string& uri) = 0;
    virtual void close() = 0;
    virtual void syncClose() = 0;
    virtual void onEvent(const std::string& eventId, const EventAction& cb) = 0;
    virtual void offEvent(const std::string& eventId) = 0;
    virtual void offAllEvents() = 0;
    virtual void onSocketError(const ErrorAction& cb) = 0;
    virtual void emit(std::string const& name, const sio::message::list& msglist = nullptr) = 0;
    virtual bool isActive() = 0;

    //virtual void setOnReconnecting(const conListener& cb) = 0;
    //virtual void setOnReconnect(const reconnectListener& cb) = 0;
    //virtual void setOnSocketOpen(const socketListener& cb) = 0;
    //virtual void setOnSocketClose(const socketListener& cb) = 0;
    
};

} // namespace rad::live_client