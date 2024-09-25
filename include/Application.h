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
    virtual void onCreate( const FIX::SessionID& ){}
    virtual void onLogon( const FIX::SessionID& );
    virtual void onLogout( const FIX::SessionID& );
    virtual void toAdmin( FIX::Message&, const FIX::SessionID& ){}
    virtual void toApp( FIX::Message&, const FIX::SessionID& )
        EXCEPT( FIX::DoNotSend ){}
    virtual void fromAdmin( const FIX::Message&, const FIX::SessionID& )
        EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ){}
    virtual void fromApp( const FIX::Message&, const FIX::SessionID& )
        EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

    void onMessage( const FIX42::MarketDataRequest&, const FIX::SessionID& );
    void onMessage( const FIX43::MarketDataRequest&, const FIX::SessionID& );

    void queryEnterOrder();
    void queryCancelOrder();
    void queryReplaceOrder();
    void queryMarketDataRequest();

    FIX42::NewOrderSingle queryNewOrderSingle42();
    FIX42::OrderCancelRequest queryOrderCancelRequest42();
    FIX42::OrderCancelReplaceRequest queryCancelReplaceRequest42();
    

    void queryHeader(FIX::Header& header);
    char queryAction();
    int queryVersion();
    bool queryConfirm(const std::string& query);

    FIX::SenderCompID querySenderCompID();
    FIX::TargetCompID queryTargetCompID();
    FIX::TargetSubID queryTargetSubID();
    FIX::ClOrdID queryClOrdID();
    FIX::OrigClOrdID queryOrigClOrdID();
    FIX::Symbol querySymbol();
    FIX::Side querySide();
    FIX::OrderQty queryOrderQty();
    FIX::OrdType queryOrdType();
    FIX::Price queryPrice();
    FIX::StopPx queryStopPx();
    FIX::TimeInForce queryTimeInForce();

    public:
    void run();

};
