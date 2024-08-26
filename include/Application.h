#pragma once
#include "Broker.h"
#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"
#include "quickfix/Mutex.h"

#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "quickfix/fix42/MarketDataRequest.h"
#include "quickfix/fix43/MarketDataRequest.h"

class Application : FIX::Application, FIX::MessageCracker
{
    // Application overloads
    virtual ~Application() {};
    virtual void onCreate( const FIX::SessionID& ){}
    virtual void onLogon( const FIX::SessionID& );
    virtual void onLogout( const FIX::SessionID& );
    virtual void toAdmin( FIX::Message&, const FIX::SessionID& ){}
    virtual void toApp( FIX::Message&, const FIX::SessionID& )
        throw( FIX::DoNotSend ){}
    virtual void fromAdmin( const FIX::Message&, const FIX::SessionID& )
        throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ){}
    virtual void fromApp( const FIX::Message&, const FIX::SessionID& )
        throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

    void onMessage( const FIX42::MarketDataRequest&, const FIX::SessionID& );
    void onMessage( const FIX43::MarketDataRequest&, const FIX::SessionID& );

    Broker broker_;

public:
    const Broker& Broker() { return broker_; }
};
