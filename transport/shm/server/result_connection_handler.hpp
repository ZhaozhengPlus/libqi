/*
 * result_connection_handfler.hpp
 *
 *  Created on: Nov 9, 2009 at 5:33:35 PM
 *      Author: Jean-Charles DELAY
 * 			Mail  : delay.jc@gmail.com
 */

#ifndef LIBIPPC_RESULTCONNECTIONHANDLER_HPP_
#define LIBIPPC_RESULTCONNECTIONHANDLER_HPP_

#include <alippc/serialization/result_definition.hpp>
#include <alippc/transport/common/runnable.hpp>

#include <alippc/transport/shm/client/result_handler.hpp>

namespace AL {
  namespace Messaging {

class ResultConnectionHandler : public Runnable {
public:
  ResultConnectionHandler (const std::string & rdv_name, ResultHandler & resultHandler);
  virtual ~ResultConnectionHandler ();

  virtual void run ();

private:
  std::string rdv_name;
  ResultHandler & m_resultHandler;
};

}
}

#endif /* !LIBIPPC_RESULTCONNECTIONHANDLER_HPP_ */
