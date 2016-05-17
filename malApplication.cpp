// custom-app.cpp

#include "malApplication.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"
#include "ns3/ndnSIM/name.h"


NS_LOG_COMPONENT_DEFINE("CustomApp``1");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(CustomApp1);

// register NS-3 type
TypeId
CustomApp1::GetTypeId()
{
  static TypeId tid = TypeId("CustomApp1").SetParent<ndn::App>().AddConstructor<CustomApp1>();
  return tid;
}

// Processing upon start of the application
void
CustomApp1::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  // Add entry to FIB for `/prefix/sub`
  ndn::FibHelper::AddRoute(GetNode(), "/prefix", m_face, 0);
  std::cout<<"Started application"<<std:endl;
  // Schedule send of first interest
  Simulator::Schedule(Seconds(1.0), &CustomApp1::SendInterest, this);
}

// Processing when application is stopped
void
CustomApp1::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
CustomApp1::SendInterest()
{
  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////

  // Create and configure ndn::Interest
  auto interest = std::make_shared<ndn::Interest>("/prefix");
  
  
  NS_LOG_DEBUG("Sending Interest packet for " << *interest);

  // Call trace (for logging purposes)
  m_transmittedInterests(interest, this, m_face);

  m_face->onReceiveInterest(*interest);

  Simulator::Schedule(Seconds(1.0), &CustomApp1::SendInterest, this);

}

// Callback that will be called when Interest arrives
void
CustomApp1::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
  ndn::App::OnInterest(interest);

  NS_LOG_DEBUG("Received Interest packet for " << interest->getName());

  // Note that Interests send out by the app will not be sent back to the app !
  Name sender("/malicious");
  auto data = std::make_shared<ndn::Data>(interest->getName());
  data->setFreshnessPeriod(ndn::time::milliseconds(1000));
  data->setContent(std::make_shared< ::ndn::Buffer>(1024));
  ndn::StackHelper::getKeyChain().sign(*data);

  NS_LOG_DEBUG("Sending Data packet for " << data->getName());

  // Call trace (for logging purposes)
  m_transmittedDatas(data, this, m_face);

  m_face->onReceiveData(*data);
}

// Callback that will be called when Data arrives
void
CustomApp1::OnData(std::shared_ptr<const ndn::Data> data)
{
  NS_LOG_DEBUG("Receiving Data packet for " << data->getName());
  std::cout<<"data dropped "<<std::endl;
  std::cout << "DATA received for name " << data->getName() << std::endl;
}

} // namespace ns3
