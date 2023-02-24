// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyFunctions.h"
#include "RadicalLiveClient.h"

using namespace rad::live_client;


FString UDummyFunctions::testString(FString _text) {
	
	static Audience audience("wss://room-handler.live-prod.live.k8s.getrad.co", "746017b3-1013-43dd-aee4-e9d23928b79f", "4ad7b61c-ffe3-42a5-bcfd-9cb4406e96fa", "unity");

	audience.OnConnected(
		[]() {
			UE_LOG(LogTemp, Warning, TEXT("Connected"));
		});

	audience.OnDisconnected([]() {
			UE_LOG(LogTemp, Warning, TEXT("Disconnected")); 
		});

	audience.OnError([](sio::message::ptr message) {
			UE_LOG(LogTemp, Warning, TEXT("Error"));
		});
	
	audience.OnData([](sio::message::ptr data) {
			UE_LOG(LogTemp, Warning, TEXT("Data recieved"));
		});   

	
	audience.Connect([](std::string testoProva) {
			UE_LOG(LogTemp, Warning, TEXT("%s"), testoProva.c_str());
		});
	

	UE_LOG(LogTemp, Warning, TEXT("End Test"));

	return _text;
}
