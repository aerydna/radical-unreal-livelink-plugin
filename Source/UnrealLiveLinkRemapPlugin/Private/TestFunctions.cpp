// Fill out your copyright notice in the Description page of Project Settings.

#include "TestFunctions.h"
#include <memory>
#include "RadicalLiveClient.h"
#include "SIOConnectionManagerAdapter.h"


using namespace rad::live_client;
void UTestFunctions::testConnectionAndData()
{
	static int i = 0;
	static Audience audience(
		"wss://room-handler.live-prod.live.k8s.getrad.co",
		//"wss://demo.piesocket.com/v3/channel_123?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self",
		//"http://localhost:3000",
		"746017b3-1013-43dd-aee4-e9d23928b79f",
		"4ad7b61c-ffe3-42a5-bcfd-9cb4406e96fa",
		"unity", std::shared_ptr<IConnectionManager>(new SIOConnectionManagerAdapter));
	if (i % 2 == 1)
	{
		audience.Disconnect();
		return;
	}
	i++;
	audience.OnConnected(
	[]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected"));
	});

	audience.OnDisconnected(
	[]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Disconnected")); 
	});

	audience.OnError(
	[](sio::message::ptr message)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error"));
	});
	
	audience.OnData(
	[](sio::message::ptr data)
	{
		UE_LOG(LogTemp, Warning, TEXT("Data recieved"));
	});

	UE_LOG(LogTemp, Warning, TEXT("Attempting connection"));
	audience.Connect();
}
