#pragma once
/*
*  Author(s):
*  - Cedric Gestes <gestes@aldebaran-robotics.com>
*  - Chris  Kilner <ckilner@aldebaran-robotics.com>
*
*  Copyright (C) 2010 Aldebaran Robotics
*/


#ifndef _QI_MESSAGING_MASTER_HPP_
#define _QI_MESSAGING_MASTER_HPP_

#include <string>
#include <boost/scoped_ptr.hpp>

namespace qi {
  namespace detail {
    class MasterImpl;
  }

  /// <summary> Master. </summary>
  class Master {
  public:

    /// <summary> Constructor. </summary>
    /// <param name="masterAddress">
    /// The master address.
    /// e.g. 127.0.0.1:5555
    /// </param>
    /// <seealso cref="qi::Server"/>
    /// <seealso cref="qi::Client"/>
    /// <seealso cref="qi::Publisher"/>
    /// <seealso cref="qi::Subscriber"/>
    explicit Master(const std::string& masterAddress = "127.0.0.1:5555");

    /// <summary>Destructor. </summary>
    virtual ~Master();

    bool isInitialized() const;

  private:
    /// <summary> The private implementation </summary>
    boost::scoped_ptr<detail::MasterImpl> _impl;
  };

}

#endif  // _QI_MESSAGING_MASTER_HPP_

