// custom-app.hpp

#ifndef CUSTOM_APP_H_
#define CUSTOM_APP_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "configApp.cc"
#include "ns3/ndnSIM/ndn-cxx/security/identity-certificate.hpp"

namespace ns3 {

class CustomApp1 : public ndn::App {
public:
  // register NS-3 type "CustomApp"
  static TypeId
  GetTypeId();

  // (overridden from ndn::App) Pssdfrocessing upon start of the application
  virtual void
  StartApplication();

  // (overridden from ndn::App) Processing when application is stopped
  virtual void
  StopApplication();

  // (overridden from ndn::App) Callback that will be called when Interest arrives
  virtual void
  OnInterest(std::shared_ptr<const ndn::Interest> interest);

  // (overridden from ndn::App) Callback that will be called when Data arrives
  virtual void
  OnData(std::shared_ptr<const ndn::Data> contentObject);

private:
  void
  SendInterest();
  ndn::KeyChain kc;
  ndn::Name govtKeyName;
  ndn::Name govtName;
  std::shared_ptr<::ndn::IdentityCertificate> govtCert;
  std::unique_ptr<::ndn::examples::Consumer> m_instance;
};

} // namespace ns3

#endif // CUSTOM_APP_H_
