#pragma once
/*
** Author(s):
**  - Chris Kilner <ckilner@aldebaran-robotics.com>
**
** Copyright (C) 2010 Aldebaran Robotics
*/

#ifndef SERIALIZATION_CALL_REQUEST_SERIALIZATION_H_
#define SERIALIZATION_CALL_REQUEST_SERIALIZATION_H_

#include <alcommon-ng/serialization/call_request.h>
#include <boost/serialization/nvp.hpp>

namespace boost {
  namespace serialization {

    /// <summary> Serializes a CallRequest </summary>
    /// <param name="ar">      [in,out] the archive. </param>
    /// <param name="item">    [in,out] the CallRequest item. </param>
    /// <param name="version"> The version. </param>
    template<class Archive>
    void serialize(Archive & ar, const AL::Messaging::CallRequest& item,
      const unsigned int version) {
          ar & make_nvp("module", item.module);
          ar & make_nvp("method", item.method);
          ar & make_nvp("args", item.args);
    }
  }
}

#endif  // SERIALIZATION_CALL_REQUEST_SERIALIZATION_H_
