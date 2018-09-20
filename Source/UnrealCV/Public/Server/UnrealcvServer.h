// Weichao Qiu @ 2016
#pragma once

#include "Runtime/Engine/Public/Tickable.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "TcpServer.h"
#include "ServerConfig.h"

class FCommandDispatcher;
class FCommandHandler;

class FAsyncRecord
{
public:
	bool bIsCompleted;
	static TArray<FAsyncRecord*> Records; // Tracking all async task in the system
	static FAsyncRecord* Create()
	{
		FAsyncRecord* Record = new FAsyncRecord();
		return Record;
	}
	void Destory()
	{
		delete this;
	}
};

class FRequest
{
public:
	FRequest() {}
	FRequest(FString InMessage, uint32 InRequestId) : Message(InMessage), RequestId(InRequestId) {}
	FString Message;
	uint32 RequestId;
};

/**
 * UnrealCV server to interact with external programs
 */
class UNREALCV_API FUnrealcvServer : public FTickableGameObject
{
public:
	~FUnrealcvServer();

	/** Send a string message to connected clients */
	void SendClientMessage(FString Message);


	/** Get the singleton */
	static FUnrealcvServer& Get();

	/** The CommandDispatcher to handle a pending request */
	TSharedPtr<FCommandDispatcher> CommandDispatcher;

	/** Return the Pawn of this game */
	APawn* GetPawn();

	/** Implement ticking function of UnrealcvServer itself */
	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const{
		return bIsTicking;
	}

	virtual bool IsTickableWhenPaused() const
	{
		return bIsTicking; // Need to keep processing commands when game is paused
	}

	virtual TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT( FUnrealcvServer, STATGROUP_Tickables );
	}

	void RegisterCommandHandlers();

	/** Make sure UnrealcvServer correctly initialized itself in the GameWorld */
	// bool InitWorld();

	/** Return the GameWorld of the editor or of the game */
	UWorld* GetGameWorld();

	/** Update input mode */
	// void UpdateInput(bool Enable);

	/** Open new level */
	// void OpenLevel(FName LevelName);

	/** The config of UnrealcvServer */
	FServerConfig Config;

	/** The underlying class to handle network connection, ip and port are configured here */
	UTcpServer* TcpServer;

	/** A controller to control the UE4 world */
	TWeakObjectPtr<class AUnrealcvWorldController> WorldController;

	/** InitWorldController */
	void InitWorldController();

private:
	/** Handlers for UnrealCV commands */
	TArray<FCommandHandler*> CommandHandlers;

	/** Process pending requests in a tick */
	void ProcessPendingRequest();

	/** The Pawn of the Game */
	APawn* Pawn;

	bool bIsTicking = true;

	/** Construct a server */
	FUnrealcvServer();

	/** Store pending requests, A new request will be stored here and be processed in the next tick of GameThread */
	TQueue<FRequest, EQueueMode::Spsc> PendingRequest; // TQueue is a thread safe implementation

	/** Handle the raw message from TcpServer and parse raw message to a FRequest */
	void HandleRawMessage(const FString& RawMessage);

	/** Handle errors from TcpServer */
	void HandleError(const FString& ErrorMessage);

};
