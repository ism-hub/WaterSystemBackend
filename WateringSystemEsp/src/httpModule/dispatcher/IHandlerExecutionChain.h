/*
 * IHandlerExecutionChain.h
 *
 *  Created on: 28 αιεμι 2018
 *      Author: IsM
 */

#ifndef HTTPMODULE_DISPATCHER_IHANDLEREXECUTIONCHAIN_H_
#define HTTPMODULE_DISPATCHER_IHANDLEREXECUTIONCHAIN_H_

namespace Http {

class IHandlerExecutionChain {
public:
	IHandlerExecutionChain() {}
	virtual ~IHandlerExecutionChain() {}

	virtual bool canHandle(HttpServletRequest& req) = 0;

	virtual std::shared_ptr<HttpServletResponse> execute(HttpServletRequest& req)  = 0;
};

} /* namespace Http */

#endif /* HTTPMODULE_DISPATCHER_IHANDLEREXECUTIONCHAIN_H_ */
