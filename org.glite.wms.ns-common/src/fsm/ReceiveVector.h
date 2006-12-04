#ifndef _GLITE_WMS_MANAGER_NS_FSM_RECEIVEVECTOR_H_
#define _GLITE_WMS_MANAGER_NS_FSM_RECEIVEVECTOR_H_

/*
 * ReceiveVector.h
 *
 * Copyright (c) 2002 EU DataGrid.
 * For license conditions see http://www.eu-datagrid.org/license.html
 */

#include "CommandState.h"
#include <string>
#include <vector>

namespace glite {
namespace wms {
namespace manager {
namespace ns {

namespace commands {
class Command;
}

namespace fsm {

/**
 * This State receives a command vector attribute over a connection. 
 *
 * @version 1.0
 * @date September 16 2002
 * @author Salvatore Monforte, Marco Pappalardo
 */
class ReceiveVector : public CommandState 
{
 public:
  /**
   * Constructor
   * @param s the name of the attribute whose value is to be sent.
   */
  ReceiveVector(const std::string& s);
  
  /**
   * Describes the action this state must perform.
   * @param cmd the command the state belongs to.
   * @return whether an error code indicating whether error occurred or not.
   * @throws JDLParsingException if parsing error occurs.
   */
  bool execute (commands::Command* cmd);

 private:
  std::string vectorName;
};

} // namespace fsm
} // namespace ns
} // namespace manager
} // namespace wms
} // namespace glite

#endif 







