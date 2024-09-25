#include "../include/Application.h"
#include "quickfix/config.h"
#include "quickfix/Session.h"

#include "quickfix/fix42/ExecutionReport.h" // Not sure I need this include

void Application::onLogon( const FIX::SessionID& sessionID ) {
  std::cout << std::endl << "Logon: " << sessionID << std::endl;
}

void Application::onLogout( const FIX::SessionID& sessionID ) {
  std::cout << std::endl << "Logoff: " << sessionID << std::endl;
}

void Application::fromApp( const FIX::Message& message,const FIX::SessionID& sessionID )
                    EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue,
                     FIX::UnsupportedMessageType )
{
  crack( message, sessionID );
  std::cout << std::endl << "IN: " << message << std::endl;
}

void Application::toApp( FIX::Message& message, const FIX::SessionID& sessionID)
        EXCEPT( FIX::DoNotSend ){
  try{
    FIX::PossDupFlag possDupFlag;
    message.getHeader().getField(possDupFlag);
    if(possDupFlag){throw FIX::DoNotSend();}
  }catch(FIX::FieldNotFound&){}

  std::cout << std::endl << "OUT: " << message << std::endl;
}

void Application::onMessage( const FIX42::MarketDataRequest& message, const FIX::SessionID& )
{
  FIX::MDReqID mdReqID;
  FIX::SubscriptionRequestType subscriptionRequestType;
  FIX::MarketDepth marketDepth;
  FIX::NoRelatedSym noRelatedSym;
  FIX42::MarketDataRequest::NoRelatedSym noRelatedSymGroup;

  message.get( mdReqID );
  message.get( subscriptionRequestType );
  if (subscriptionRequestType != FIX::SubscriptionRequestType_SNAPSHOT)
      throw FIX::IncorrectTagValue(subscriptionRequestType.getTag());
  message.get( marketDepth );
  message.get( noRelatedSym );

  for ( int i = 1; i <= noRelatedSym; ++i )
  {
    FIX::Symbol symbol;
    message.getGroup( i, noRelatedSymGroup );
    noRelatedSymGroup.get( symbol );
  }
}

void Application::run(){
  while(true){
    try{
      char query_action = queryAction();

      switch (query_action)
      {
      case '1':
        /* code */
        // new order
        queryEnterOrder();
        break;
      
      case '2':
        // cancel order
        queryCancelOrder();
        break;
      
      case '3':
        // modify order
        queryReplaceOrder();
        break;

      case '4':
        // market data request
        queryMarketDataRequest();
        break;

      case '5':
        break;

      default:
        break;
      }
    }catch(std::exception& e){
      std::cout << "Error sending message: " << e.what() << std::endl;
    }
  }
}

void Application::queryEnterOrder(){

}

void Application::queryCancelOrder(){

}

void Application::queryReplaceOrder(){

}

void Application::queryMarketDataRequest(){

}

char Application::queryAction(){
    char action;
    std::cout << "1) New Order,\n2) Cancel Order,\n3) Modify Order,\n4)Market Data Request\n5) Quit,\nAction: ";
    std::cin >> action;

    switch(action){
      case '1': case '2': case '3': case '4': case '5': break; 
      default: throw std::exception();
    }
    return action;
}
