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

#ifndef __DLDB_SRC_SERVER_DLDBSERVICEIMPL_H__
#define __DLDB_SRC_SERVER_DLDBSERVICEIMPL_H__

#include <grpc++/grpc++.h>

#include "LeveldbUtil.h"

#include "dldb.grpc.pb.h"

namespace dldb
{
	class DldbServiceImpl : public dldb::rpc::Service 
	{
		public:
			DldbServiceImpl(const std::string& _dataDir);
			
			virtual ~DldbServiceImpl();

			grpc::Status insert(grpc::ServerContext* context, 
				const InsertRequest* request, InsertReply* reply) override;

			grpc::Status delete(grpc::ServerContext* context,
				const DeleteRequest* request, DeleteReply* reply) override;

			grpc::Status get(grpc::ServerContext* context, 
				const GetRequest* request, GetReply* reply) override;
		private:
			// FOR NONCOPYABLE
			DldbServiceImpl(const DldbServiceImpl& );
			DldbServiceImpl& operator = (const DldbServiceImpl& );

			void init();
		private:
			std::string dataDir;
			LeveldbUtil* ldb;
			bool initFlag;
	};
}

#endif  // __DLDB_SRC_SERVER_DLDBSERVICEIMPL_H__





