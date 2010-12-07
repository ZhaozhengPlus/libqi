#pragma once
/*
*  Author(s):
*  - Chris  Kilner <ckilner@aldebaran-robotics.com>
*  - Cedric Gestes <gestes@aldebaran-robotics.com>
*
*  Copyright (C) 2010 Aldebaran Robotics
*/


#ifndef _QI_TRANSPORT_TRANSPORT_CLIENT_HPP_
#define _QI_TRANSPORT_TRANSPORT_CLIENT_HPP_

#include <string>
#include <qi/transport/buffer.hpp>

namespace qi {
  namespace transport {

    namespace detail {
      class ClientBackend;
    }

    class TransportClient {
    public:
      TransportClient();

      bool connect(const std::string &endpoint);

      void send(const qi::transport::Buffer &request, qi::transport::Buffer &reply);

    protected:
     bool                                  _isInitialized;
     qi::transport::detail::ClientBackend *_client;
    };

  }
}

#endif  // _QI_TRANSPORT_TRANSPORT_CLIENT_HPP_
