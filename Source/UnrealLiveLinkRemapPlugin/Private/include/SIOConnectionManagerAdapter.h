// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "sio_client.h"
#include "sio_socket.h"
#include "IConnectionManager.h"

class SIOConnectionManagerAdapter : public rad::live_client::IConnectionManager
{
	sio::client _client{ sio::client(true, false) };
	std::shared_ptr<sio::socket> _socket;
public:
	void createSocket() override
	{
		_socket = _client.socket();
	}

	void clearConnectionListeners() override
	{
		_client.clear_con_listeners();
	}

	void clearSocketListeners() override
	{
		_client.clear_socket_listeners();
	}

	bool isOpened() override
	{
		return _client.opened();
	}

	void setOnOpen(const rad::live_client::ConListener& cb) override
	{
		_client.set_open_listener(cb);
	}

	void setOnFail(const rad::live_client::ConListener& cb) override
	{
		_client.set_fail_listener(cb);
	}

	void setOnClose(const rad::live_client::CloseListener& cb) override
	{
		_client.set_close_listener([cb](sio::client::close_reason inReason)
			{
				auto reason = inReason == sio::client::close_reason::close_reason_normal ?
					rad::live_client::CloseReason::normal :
					rad::live_client::CloseReason::drop;

				cb(reason);
			});
	}

	void connect(const std::string& uri) override
	{
		/*_client.set_open_listener([]()
		{
			UE_LOG(LogTemp, Warning, TEXT("internal Connected"));
		});*/
		_client.connect(uri);
	}

	void close() override
	{
		_client.close();
	}

	void syncClose() override
	{
		_client.sync_close();
	}

	void onEvent(const std::string& eventId, const rad::live_client::EventAction& cb) override
	{
		_socket->on(eventId, cb);
	}

	void offEvent(const std::string& eventId) override
	{
		_socket->off(eventId);
	}

	void offAllEvents() override
	{
		_socket->off_all();
	}

	void onSocketError(const rad::live_client::ErrorAction& cb) override
	{
		_socket->on_error(cb);
	}

	void emit(std::string const& name, const sio::message::list& msglist = nullptr) override
	{
		_socket->emit(name, msglist);
	}

	bool isActive() override
	{
		return _socket ? true : false;
	}
};
