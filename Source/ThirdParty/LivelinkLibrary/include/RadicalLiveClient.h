/// @brief Radical Live Client library.
///
/// @copyright "Copyright (C) 2021, Radical
/// All rights reserved.
/// CONFIDENTIAL INFORMATION - DO NOT DISTRIBUTE"

#pragma once

#include <string>
#include <memory>
#include <functional>
#include <IConnectionManager.h>
#include <sio_message.h>

namespace rad::live_client
{
using ConnectionMagerPtr = std::shared_ptr<IConnectionManager>;
class AudienceImpl;

/**
 * @brief Audience class implements an audience connection to a Radical Live Room
 */
class Audience
{
 public:
  /**
   * @brief Connects to a room with the given details and credentials as audience
   * @param uriString - Room handler URI to connect to
   * @param userAuthToken - User authentication token which will be used for authenticating the connecting user (you)
   * @param roomId - Room id of the room to connect to
   */
  Audience(std::string uriString, std::string userAuthToken, std::string roomId, std::string clientLabel = "", ConnectionMagerPtr connectionManager = ConnectionMagerPtr(nullptr));
  ~Audience();

  /**
   * @brief Tries to establish the connection to the room handler
   */
  void Connect();
  /**
   * @brief Disconnects from the room handler
   */
  void Disconnect();
  /**
   * @brief Disconnects from the room handler synchronously, waits until the connection is actually closed
   */
  void DisconnectSync();
  /**
   * @brief Sets the global callback to be called with each event received
   * @param event - Event name
   * @param callback
   */
  int On(std::string const &event, std::function<void(sio::message::ptr)> callback);
  /**
   * @brief Resets every callback that has registered
   */
  void Reset();
  /**
   * @brief Resets every callback that has registered for the event
   * @param event
   */
  void Reset(std::string const &event);
  /**
   * @brief Resets the callback with the given id
   * @param event
   * @param callbackId
   */
  void Reset(std::string const &event, int callbackId);
  /**
   * @brief Shortcut callback registration. Sets the callback to be called when animation data is received
   * @param callback
   */
  void OnData(std::function<void(sio::message::ptr)> callback);
  /**
   * @brief Shortcut callback registration. Sets the callback to be called when a command is received with
   * receive-command event
   * @param callback
   */
  void OnCommand(std::function<void(sio::message::ptr)> callback);
  /**
   * @brief Sets the callback to be called when the connection is successfully established
   * @param callback
   */
  void OnConnected(std::function<void()> callback);
  /**
   * @brief Sets the callback to be called when disconnected
   * @param callback
   */
  void OnDisconnected(std::function<void()> callback);
  /**
   * @brief Sets the callback to be called when there is an error
   * @param callback
   */
  void OnError(std::function<void(sio::message::ptr)> callback);

 private:
  std::unique_ptr<AudienceImpl> _audienceImpl;
};

}
