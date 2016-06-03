/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __DLDB_SRC_CLIENT_LDBCLIENT_H__
#define __DLDB_SRC_CLIENT_LDBCLIENT_H__

#include <string>
#include <memory>

#include <grpc++/grpc++.h>

#include "RpcClient.h"
#include "Status.h"
#include "ldb.grpc.pb.h"

namespace ldb
{
	class LdbClient
	{
		public:
			LdbClient(std::shared_ptr<grpc::Channel> _channel)
				: client(RpcClient<ldb::rpc>::getInstance()),
				  proxy(ldb::rpc::NewStub(_channel))

			{
			}

			/*
			LdbClient(const std::string& address)
				: proxy(ldb::rpc::NewStub
						(grpc::CreateChannel
						 (address, grpc::InsecureChannelCredentials())))
			{
			}
			*/
			LdbClient(const std::string& address)
				: client(RpcClient<ldb::rpc>::getInstance()),
				  proxy(client.getStub(address))
				  
			{
			}

			~LdbClient()
			{
			}

			Status insert(const std::string& key, const std::string& value)
			{
				InsertRequest request;
				request.set_key(key);
				request.set_values(value);

				InsertReply reply;

				// grpc::Status status = proxy->Insert(&context, request, &reply);

				grpc::Status status = client.requestSync(proxy, &ldb::rpc::Stub::Insert,
						request, &reply);

				return Status(status.error_code(), status.error_message());
			}

			Status deleteByKey(const std::string& key)
			{
				DeleteRequest request;
				request.set_key(key);

				DeleteReply reply;
				grpc::ClientContext context;

				// grpc::Status status = proxy->Delete(&context, request, &reply);
				grpc::Status status = client.requestSync(proxy, &ldb::rpc::Stub::Delete,
						request, &reply);

				return Status(status.error_code(), status.error_message());
			}

			Status getByKey(const std::string& key, std::string* value)
			{
				GetRequest request;
				request.set_key(key);

				GetReply reply;
				grpc::ClientContext context;
				// context.set_timeout(1);

				// grpc::Status status = proxy->Get(&context, request, &reply);

				grpc::Status status = client.requestSync(proxy, &ldb::rpc::Stub::Get,
						request, &reply);

				if (status.ok())
				{
					if (reply.msg().code() == 0)
						*value = reply.value();
					
					return Status(reply.msg().code(), reply.msg().ret_msg());
				}

				return Status(status.error_code(), status.error_message());
			}

			void getByKeyAsync(const std::string& key)
			{
				GetRequest request;
				request.set_key(key);

				GetReply reply;
				grpc::ClientContext context;
				// context.set_timeout(1);

				// grpc::Status status = proxy->Get(&context, request, &reply);

				client.requestAsync(proxy, &ldb::rpc::Stub::AsyncGet,
						request, &reply, getCallback);
			}

		private:
			static void getCallback(GetReply* reply)
			{
				if (reply->msg().code() == 0)
					std::cout << "Get Async:" << reply->value() << std::endl;
				else 
					std::cout << reply->msg().code() << ":" << reply->msg().ret_msg() << std::endl;
			}

		private:
			// FOR NON COPYABLE
			LdbClient(const LdbClient& );
			LdbClient& operator = (const LdbClient& );

		private:
			RpcClient<ldb::rpc>& client;
			std::shared_ptr<ldb::rpc::Stub> proxy;

	};
}

#endif  // __DLDB_SRC_CLIENT_DLDBCLIENT_H__
