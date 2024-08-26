#include "../include/Application.h"
#include "quickfix/config.h"
#include "quickfix/Session.h"

#include "quickfix/fix42/ExecutionReport.h" // Not sure I need this include

void Application::onLogon( const FIX::SessionID& sessionID ) {}

void Application::onLogout( const FIX::SessionID& sessionID ) {}

void Application::fromApp( const FIX::Message& message,const FIX::SessionID& sessionID )
                    EXCEPT( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue,
                     FIX::UnsupportedMessageType )
{
  crack( message, sessionID );
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

void Application::onMessage( const FIX43::MarketDataRequest& message, const FIX::SessionID& )
{
    std::cout << message.toXML() << std::endl;
}
