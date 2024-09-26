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
  int version = queryVersion();
  std::cout << std::endl << "New Order Request" << std::endl;
  switch (version)
  {
  case 42:
    /* code */
    queryNewOrderSingle42();
    break;
  
  default:
    std::cerr << "FIX version not supported!" << std::endl;
    break;
  }
}

void Application::queryCancelOrder(){
  std::cout << std::endl << "Cancel Order Request" << std::endl;
  int version = queryVersion();
  FIX::Message cancel;
  switch (version)
  {
  case 42:
    /* code */
    cancel = queryOrderCancelRequest42();
    break;
  
  default:
    std::cerr << "FIX version not supported!" << std::endl;
    break;
  }

  if(queryConfirm("Send cancel")){
    FIX::Session::sendToTarget(cancel);
  }
}

void Application::queryReplaceOrder(){
  int version = queryVersion();
  std::cout << std::endl << "Modify Order Request" << std::endl;
  FIX::Message modify;
  switch (version)
  {
  case 42:
    /* code */
    modify = queryCancelReplaceRequest42();
    break;
  
  default:
    std::cerr << "FIX version not supported!" << std::endl;
    break;
  }

  if(queryConfirm("Send replace")){
    FIX::Session::sendToTarget(modify);
  }

}

void Application::queryMarketDataRequest(){
  // To Do
  int version = queryVersion();
  FIX::Message market_data;
  switch (version)
  {
  case 42:
    std::cerr << "Market Data Request not supported yet!" << std::endl;
    // market_data = queryMarketDataRequest();
    break;
  
  default:
    std::cerr << "FIX version not supported!" << std::endl;
    break;
  }
}

FIX42::NewOrderSingle Application::queryNewOrderSingle42(){
  // To Do
  FIX::OrdType order_type;
  FIX42::NewOrderSingle new_order_single(queryClOrdID(), FIX::HandlInst( '1' ), querySymbol(), querySide(),
                                         FIX::TransactTime(), order_type = queryOrdType());

  new_order_single.set(queryOrderQty());
  new_order_single.set(queryTimeInForce());

  if(order_type == FIX::OrdType_LIMIT || FIX::OrdType_STOP_LIMIT){
    new_order_single.set(queryPrice());
  }

  if(order_type == FIX::OrdType_STOP || order_type == FIX::OrdType_STOP_LIMIT){
    new_order_single.set(queryStopPx());
  }
  queryHeader(new_order_single.getHeader());
  return new_order_single;
}

FIX42::OrderCancelRequest Application::queryOrderCancelRequest42(){
  // To Do
  FIX42::OrderCancelRequest order_cancel_request(queryOrigClOrdID(), queryClOrdID(), querySymbol(), querySide(), FIX::TransactTime());
  order_cancel_request.set(queryOrderQty());
  queryHeader(order_cancel_request.getHeader());
  return order_cancel_request;
}

FIX42::OrderCancelReplaceRequest Application::queryCancelReplaceRequest42(){
  // To Do
  FIX42::OrderCancelReplaceRequest order_modify_request(queryOrigClOrdID(), queryClOrdID(), FIX::HandlInst( '1' ), 
                                                        querySymbol(), querySide(), FIX::TransactTime(), queryOrdType());
  if(queryConfirm("New price")){
    order_modify_request.set(queryPrice());
  }
  
  if(queryConfirm("New quantity")){
    order_modify_request.set(queryOrderQty());
  }

  queryHeader(order_modify_request.getHeader());
  return order_modify_request;
}


void queryHeader(FIX::Header& header){
  // To do
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

int queryVersion(){
  // To do
}

bool queryConfirm(const std::string& query){
  // To do
}

