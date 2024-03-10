/****************************************************************************************************************************
  AsyncDNSServer_ESP32_ENC_Impl.h

  For ESP32_ENC (ESP32 + ENC28J60)

  AsyncDNSServer_ESP32_ENC is a Async DNS Server library for the ESP32_ENC

  Based on and modified from ESPAsyncDNSServer Library (https://github.com/devyte/ESPAsyncDNSServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC
  Licensed under MIT license

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      28/11/2022 Initial coding for ESP32 + LwIP ENC28J60. Sync with AsyncDNSServer_STM32
  *****************************************************************************************************************************/

#pragma once

#ifndef AsyncDNSServer_ESP32_ENC_Impl_h
#define AsyncDNSServer_ESP32_ENC_Impl_h

#include <Arduino.h>

extern "C"
{
#include <lwip/def.h>
};

////////////////////////////////////////////////

namespace
{
struct DNSHeader
{
  uint16_t ID;               // identification number
  unsigned char RD : 1;      // recursion desired
  unsigned char TC : 1;      // truncated message
  unsigned char AA : 1;      // authoritive answer
  unsigned char OPCode : 4;  // message_type
  unsigned char QR : 1;      // query/response flag
  unsigned char RCode : 4;   // response code
  unsigned char Z : 3;       // its z! reserved
  unsigned char RA : 1;      // recursion available
  uint16_t QDCount;          // number of question entries
  uint16_t ANCount;          // number of answer entries
  uint16_t NSCount;          // number of authority entries
  uint16_t ARCount;          // number of resource entries
};

////////////////////////////////////////////////

bool requestIncludesOnlyOneQuestion(DNSHeader * _dnsHeader)
{
  return ntohs(_dnsHeader->QDCount) == 1 &&
         _dnsHeader->ANCount == 0 &&
         _dnsHeader->NSCount == 0 &&
         _dnsHeader->ARCount == 0;
}

////////////////////////////////////////////////

void downcaseAndRemoveWwwPrefix(String &domainName)
{
  domainName.toLowerCase();
  domainName.replace("www.", "");
}

////////////////////////////////////////////////

String getDomainNameWithoutWwwPrefix(unsigned char *start)
{
  String parsedDomainName = "";

  if (start == nullptr || *start == 0)
    return parsedDomainName;

  int pos = 0;

  while (true)
  {
    unsigned char labelLength = *(start + pos);

    for (int i = 0; i < labelLength; i++)
    {
      pos++;
      parsedDomainName += (char) * (start + pos);
    }

    pos++;

    if (*(start + pos) == 0)
    {
      downcaseAndRemoveWwwPrefix(parsedDomainName);
      return parsedDomainName;
    }
    else
    {
      parsedDomainName += ".";
    }
  }
}
}

/////////////////////////////////////////////////////

AsyncDNSServer::AsyncDNSServer()
{
  _ttl = htonl(60);
  _errorReplyCode = AsyncDNSReplyCode::NonExistentDomain;
}

////////////////////////////////////////////////

bool AsyncDNSServer::start(const uint16_t port, const String &domainName,
                           const IPAddress &resolvedIP)
{
  _port = port;
  _domainName = domainName;
  _resolvedIP[0] = resolvedIP[0];
  _resolvedIP[1] = resolvedIP[1];
  _resolvedIP[2] = resolvedIP[2];
  _resolvedIP[3] = resolvedIP[3];
  downcaseAndRemoveWwwPrefix(_domainName);

  if (_udp.listen(_port))
  {
    _udp.onPacket(
      [&](AsyncUDPPacket & packet)
    {
      this->processRequest(packet);
    }
    );

    return true;
  }

  return false;
}

////////////////////////////////////////////////

void AsyncDNSServer::setErrorReplyCode(const AsyncDNSReplyCode &replyCode)
{
  _errorReplyCode = replyCode;
}

////////////////////////////////////////////////

void AsyncDNSServer::setTTL(const uint32_t ttl)
{
  _ttl = htonl(ttl);
}

////////////////////////////////////////////////

void AsyncDNSServer::stop()
{
  DNS_LOGDEBUG("Stop");

  _udp.close();
}

////////////////////////////////////////////////

void AsyncDNSServer::processRequest(AsyncUDPPacket &packet)
{
  if (packet.length() >= sizeof(DNSHeader))
  {
    unsigned char * _buffer = packet.data();
    DNSHeader * _dnsHeader = (DNSHeader*) _buffer;

    String domainNameWithoutWwwPrefix = (_buffer == nullptr ? "" : getDomainNameWithoutWwwPrefix(_buffer + sizeof(
                                                                                                   DNSHeader)));

    DNS_LOGDEBUG1("processRequest: domainNameWithoutWwwPrefix =", domainNameWithoutWwwPrefix);

    if (_dnsHeader->QR == DNS_QR_QUERY && _dnsHeader->OPCode == DNS_OPCODE_QUERY &&
        requestIncludesOnlyOneQuestion(_dnsHeader) &&
        (_domainName == "*" || domainNameWithoutWwwPrefix == _domainName)
       )
    {
      replyWithIP(packet);
    }
    else if (_dnsHeader->QR == DNS_QR_QUERY)
    {
      replyWithCustomCode(packet);
    }
  }
}

////////////////////////////////////////////////

void AsyncDNSServer::replyWithIP(AsyncUDPPacket &packet)
{
  //6 bytes below + szeof(ttl) + 2 bytes. Precalculate to avoid using default of 1460, which is way too much
  AsyncUDPMessage msg(packet.length() + 12 + sizeof(_resolvedIP));

  msg.write(packet.data(), packet.length());
  DNSHeader * _dnsHeader = (DNSHeader *)msg.data();

  _dnsHeader->QR = DNS_QR_RESPONSE;
  _dnsHeader->ANCount = _dnsHeader->QDCount;
  _dnsHeader->QDCount = _dnsHeader->QDCount;
  //_dnsHeader->RA = 1;

  msg.write((uint8_t)192); //  answer name is a pointer
  msg.write((uint8_t)12);  // pointer to offset at 0x00c

  msg.write((uint8_t)0);   // 0x0001  answer is type A query (host address)
  msg.write((uint8_t)1);

  msg.write((uint8_t)0);   //0x0001 answer is class IN (internet address)
  msg.write((uint8_t)1);

  msg.write((uint8_t *)&_ttl, sizeof(_ttl));

  // Length of RData is 4 bytes (because, in this case, RData is IPv4)
  msg.write((uint8_t)0);
  msg.write((uint8_t)4);
  msg.write(_resolvedIP, sizeof(_resolvedIP));

  packet.send(msg);

  DNS_LOGDEBUG1(F("replyWithIP: DNS responds: "), (IPAddress) _resolvedIP);
}

////////////////////////////////////////////////

void AsyncDNSServer::replyWithCustomCode(AsyncUDPPacket &packet)
{
  AsyncUDPMessage msg(packet.length());

  msg.write(packet.data(), packet.length());
  DNSHeader * _dnsHeader = (DNSHeader *)msg.data();

  _dnsHeader->QR = DNS_QR_RESPONSE;
  _dnsHeader->RCode = (unsigned char)_errorReplyCode; //default is AsyncDNSReplyCode::NonExistentDomain
  _dnsHeader->QDCount = 0;

  packet.send(msg);
}

////////////////////////////////////////////////


#endif    // AsyncDNSServer_ESP32_ENC_Impl_h
