/* 
 * Copyright © 2015 Sergei Khairulin <sergei.khairulin@gmail.com>. 
 * All rights reserved.
 *
 * This file is part of Bloom++.
 *
 * Bloom++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bloom++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bloom++.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cstring>
#include <iostream>

#ifdef LINUX
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <bloom++/log.h>
//#include "../../param.dod"
#define CSTPCSTR
#elif defined MS_WINDOWS
#include <winsock2.h>
#include <iphlpapi.h>
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ip_mreq
{
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
} IP_MREQ, *PIP_MREQ;
#define CSTPCSTR (const char *)
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#endif

#include <bloom++/net/socket_base.h>


#ifdef NET_DEBUG
#define __BLOOM_WITH_DEBUG
#endif
#include <bloom++/_bits/debug.h>

namespace bloom
{

namespace net
{

//=======================================
// _socket
//=======================================

socket_base::socket_base():
sd_(0)
{
}

#ifdef LINUX
socket_base::socket_base(int sd, const addr_ipv4 &addr) :
#elif MS_WINDOWS
socket_base::socket_base(SOCKET sd) :
#endif
sd_(sd),
addr_(addr)
{
}

socket_base::~socket_base()
{
    close();
}

void socket_base::close()
{
    if(!sd_)return;
#ifdef LINUX
    int error = ::close(sd_);
#elif MS_WINDOWS
    int error = closesocket(sd_);
#endif
    sd_ = 0;
    addr_ = addr_ipv4();
    if (error == -1)
        DEBUG_WARN("socket - could not close socket properly\n");
}

bool socket_base::is_closed() const
{
    if(!sd_)return true;
    return false;
}

bloom::string socket_base::get_if_name() const
{
    bloom::string if_str;

#ifdef LINUX
    struct ifaddrs *addrs, *addr;
    struct sockaddr_in *saddr;
    shared_ptr<addr_ipv4> socketAddr(new addr_ipv4());

    getifaddrs(&addrs);
    for (addr = addrs; addr != 0; addr = addr->ifa_next)
    {
        if (addr->ifa_addr && (addr->ifa_flags & IFF_UP) && addr->ifa_addr->sa_family == AF_INET)
        {
            saddr = (struct sockaddr_in *) (addr->ifa_addr);
            if (saddr->sin_addr.s_addr == htonl(socketAddr->ip_int()))
            {
                if_str = addr->ifa_name;
                break;
            }
        }
    }
    freeifaddrs(addrs);

#else
    ULONG outAddrsLen = 16384;
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;

    pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outAddrsLen);
    ret = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL,
                               pAddresses, &outAddrsLen);
    if (ret == ERROR_BUFFER_OVERFLOW)
    {
        FREE(pAddresses);
        pAddresses = NULL;
    }
    else
    {
        pCurrAddresses = pAddresses;
        while (pCurrAddresses)
        {
            printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
            printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);
            if_str = pCurrAddresses->AdapterName;
            break;
        }
        FREE(pAddresses);
    }
#endif
    return if_str;
}

int socket_base::get_MAC(const bloom::string &ifname, char *mac) const
{
#ifdef LINUX
    struct ifreq buffer;
    memset(&buffer, 0x00, sizeof (buffer));
    strcpy(buffer.ifr_name, ifname.c_str());
    ioctl(sd_, SIOCGIFHWADDR, &buffer);
    for (int i = 0; i < 6; i++)
    {
        mac[i] = (unsigned char) buffer.ifr_hwaddr.sa_data[i];
    }
#else
    int ret, i;
    ULONG outAddrsLen = 16384;
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;

    pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outAddrsLen);
    ret = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL,
                               pAddresses, &outAddrsLen);
    if (ret == ERROR_BUFFER_OVERFLOW)
    {
        FREE(pAddresses);
        pAddresses = NULL;
    }
    else
    {
        pCurrAddresses = pAddresses;
        while (pCurrAddresses)
        {
            if (strcmp(pCurrAddresses->AdapterName, ifname))
            {
                for (i = 0; i < 6; i++)
                    mac[i] = pCurrAddresses->PhysicalAddress[i];
                break;
            }
            pCurrAddresses = pCurrAddresses->Next;
        }
        FREE(pAddresses);
    }
#endif
    return 0;
}

int socket_base::bind__(const addr_ipv4 &addr)
{
    int error = ::bind(sd_, (sockaddr*) (&addr.sockaddr_in_), sizeof (struct sockaddr_in));
    if (error == -1)
    {
        //throw CSockException ("socket::bind - could not bind socket to a provided name", Sock_BIND_ERROR);
        DEBUG_WARN(log::pf("socket::bind - could not bind socket to <%s:%d>\n", addr.ip().c_str(), addr.port()));
        return sock_BIND_ERROR;
    }
    addr_ = addr;
    return sock_OK;
}

size_t socket_base::send(const char * data, size_t len)
{
    int sendRet = ::send(sd_, data, len, 0);
    if (sendRet == -1){
        DEBUG_WARN("send error...\n");
        return sock_ERROR;
    }
    return sendRet;
}

size_t socket_base::sendto(const addr_ipv4 &dest, const char * data, size_t len)
{
    int sendRet = ::sendto(sd_, data, len, 0, (sockaddr *) (&dest.sockaddr_in_), sizeof (struct sockaddr_in));
    if (sendRet == -1){
        DEBUG_WARN(log::pf("send to <%s:%d> error...\n", dest.ip().c_str(), dest.port()));
        return sock_ERROR;
    }
    return sendRet;
}

size_t socket_base::recv(char * data, size_t len)
{
    int recvRet = 0;
    char *mem = data;

    recvRet = ::recv(sd_, mem, len, 0);
    if (recvRet < 0)
    {
        DEBUG_WARN("socket::recv error = "<<recvRet<<"\n");
        return sock_ERROR;
    }
    //No wait full len
    return recvRet;
}

size_t socket_base::recvfrom(char * data, size_t len, addr_ipv4 &from)
{
#ifdef LINUX
    unsigned int dest_len =
#else
    int dest_len =
#endif
            sizeof (struct sockaddr_in);

    int recvRet = ::recvfrom(sd_, data, len, 0, (sockaddr *) (&from.sockaddr_in_), &dest_len);
    if (recvRet < 0)
    {
        DEBUG_WARN("socket::recvfrom - recvRet = "<<recvRet<<"\n");
        return sock_ERROR;
    }
    //No wait full len
    return recvRet;
}

int socket_base::select(int tv_sec, int tv_usec, select_type type)
{
    struct timeval timeVal = {tv_sec, tv_usec};
    return select(timeVal, type);
}

int socket_base::select(timeval &timeVal, select_type type)
{
    fd_set m_listener;
    int ret;
    fd_set *fdRead = 0, *fdWrite = 0;
    
    FD_ZERO(&m_listener);
    FD_SET(sd_, &m_listener);
    
    if(type == select_Read){
        fdRead = (fd_set *)&m_listener;
    }
    else if(type == select_Write){
        fdWrite = (fd_set *)&m_listener;
    }

    int selectRet = ::select(sd_ + 1, fdRead, fdWrite, (fd_set *) 0, &timeVal);
    
    switch (selectRet)
    {
    case -1:
        ret = sock_SELECT_ERROR;
        break;
        
    case 0:
        ret = sock_SELECT_NOTHING;
        break;
        
    default:
        if (!FD_ISSET(sd_, &m_listener))
            ret = sock_FD_ERROR;
        else
            ret = sock_SELECT_READY;
    }

    return ret;
}
    
int socket_base::read_socket_addr(addr_ipv4 &addr) const
{
#ifdef LINUX
    unsigned int addr_len =
#elif defined MS_WINDOWS
    int addr_len =
#endif
            sizeof (struct sockaddr_in);

    int ret = getsockname(sd_, (struct sockaddr *) &addr.sockaddr_in_, &addr_len);
    if (ret == -1)
    {
        DEBUG_WARN("getsockname failed!\n");
        return sock_ERROR;
    }
    return sock_OK;
}

const addr_ipv4& socket_base::socket_addr() const
{
    return addr_;
}

unsigned int socket_base::hash() const
{
    return sd_;
}

} //namespace net

} //namespace bloom
