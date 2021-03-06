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

#ifndef __DLDB_SRC_CLIENT_RPCCLIENT_H__
#define __DLDB_SRC_CLIENT_RPCCLIENT_H__

#include <string>
#include <memory>
#include <map>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>

#include <grpc++/grpc++.h>

#include "dldb.grpc.pb.h"
#include "common/ThreadPool.h"
#include "common/CountDownLatch.h"

#define DEFAULT_MAX_POOL_SIZE 20
#define DEFAULT_MAX_WORKER_NUM 10

namespace dldb
{
	template<typename ResponseType>
	struct AsyncClientCall
	{
		AsyncClientCall()
		{
		}

		ResponseType response;
		grpc::ClientContext context;
		grpc::Status status;
		std::unique_ptr<grpc::ClientAsyncResponseReader<ResponseType> > response_reader;
	};

	template<typename RpcType>
	class RpcClient
	{
		public:
			static RpcClient& getInstance()
			{
				static RpcClient instance;
				return instance;
			}

			inline void setMaxAsyncWorkerNum(int worker_num)
			{
				max_worker_num = worker_num;
			}

			inline int getMaxAsyncWorkerNum() const
			{
				return max_worker_num;
			}

			inline void setMaxAsyncPoolSize(int pool_size)
			{
				max_pool_size = pool_size;
			}

			inline int getMaxAsyncPoolSize() const
			{
				return max_pool_size;
			}

			void start()
			{
				thread_pool.setMaxPoolSize(max_pool_size);
				thread_pool.start(max_worker_num);
			}

			void stop()
			{
				common::CountDownLatch latch(1);
				thread_pool.addTask(std::bind(&common::CountDownLatch::countDown, &latch));
				latch.wait();
				thread_pool.stop();
			}

			~RpcClient()
			{
				stop();
			}
			
			std::shared_ptr<typename RpcType::Stub> getStub(const std::string& address)
			{
				std::unique_lock<std::mutex> lock(map_mutex);
				auto iter = stub_map.find(address);
				if (iter != stub_map.end())
				{
					return iter->second;
				}
				else
				{
					std::shared_ptr<grpc::Channel> channel = 
						grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
					std::shared_ptr<typename RpcType::Stub> stub = rpc::NewStub(channel);
					stub_map[address] = stub;
					return stub;
				}
			}

			template<typename StubType, typename RequestType, typename ResponseType>
			grpc::Status requestSync(
					std::shared_ptr<StubType> stub,
					grpc::Status (StubType::*func)(grpc::ClientContext*, const RequestType&, ResponseType*),
					const RequestType& request, ResponseType* response,
					int timeout = -1, int retry = 0
					)
			{
				grpc::ClientContext context;
				if (timeout > 0)
				{
					std::chrono::system_clock::time_point deadline = 
						std::chrono::system_clock::now() + std::chrono::seconds(timeout);
					context.set_deadline(deadline);
				}
				
				grpc::Status status;
				while (retry >= 0)
				{
					status = (stub.get()->*func)(&context, request, response);
					if (status.ok())
						return status;
					else
					{
						retry--;
					}
				}

				return status;
			}

			template<typename StubType, typename RequestType, typename ResponseType, typename CallbackType>
			void requestAsync(
					std::shared_ptr<StubType> stub,
					std::unique_ptr<grpc::ClientAsyncResponseReader<ResponseType> >(StubType::*func)(
							grpc::ClientContext*, const RequestType&, grpc::CompletionQueue*),
					const RequestType& request, ResponseType* response,
					CallbackType callback
					)
			{
				auto call = new AsyncClientCall<ResponseType>();
				auto cq = new grpc::CompletionQueue();

				call->response_reader = (stub.get()->*func)(&call->context, request, cq);
				call->response_reader->Finish(&call->response, &call->status, (void*)call);
				
				auto wait_for_callback = 
					[&](grpc::CompletionQueue* q, CallbackType callback)
					{
						void* got_tag;
						bool ok = false;

						// Block until the next result is available in the completion queue "cq".
						if (q->Next(&got_tag, &ok))
						{
							// The tag in this example is the memory location of the call object
							auto call = static_cast<AsyncClientCall<ResponseType>* >(got_tag);

							// Verify that the request was completed successfully. Note that "ok"
							// corresponds solely to the request for updates introduced by Finish().
							callback(&call->response, call->status.ok());

							// Once we're complete, deallocate the call object.
							delete call;
						}
						delete q;
					};

				thread_pool.addTask(std::bind(wait_for_callback, cq, callback));
			}
							
		private:
			RpcClient()
				: max_pool_size(DEFAULT_MAX_POOL_SIZE),
				  max_worker_num(DEFAULT_MAX_WORKER_NUM)
			{
				start();
			}

			RpcClient(const RpcClient& );
			RpcClient& operator = (const RpcClient& );

		private:
			int max_pool_size;
			int max_worker_num;
			std::map<std::string, std::shared_ptr<typename RpcType::Stub> > stub_map;
			mutable std::mutex map_mutex;
			common::ThreadPool thread_pool;
	};
}  // namespace dldb

#endif  // __DLDB_SRC_CLIENT_RPCCLIENT_H__

