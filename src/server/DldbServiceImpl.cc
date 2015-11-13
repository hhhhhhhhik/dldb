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

#include <assert.h>

#include "DldbServiceImpl.h"

namespace dldb
{
	DldbServiceImpl::DldbServiceImpl(const std::string& _dataDir)
 			: dataDir(_dataDir),
 			  initFlag(false),
 			  ldb(NULL)
 	{
 	}

 	DldbServiceImpl::~DldbServiceImpl()
 	{
 		delete ldb;
 		ldb = NULL;
 	}

 	void DldbServiceImpl::init()
 	{
 		if (initFlag)
 			return;
 		else
 		{
 			ldb = new LeveldbUtil(dataDir);
 			if (ldb->connect())
 				initFlag = true;
 		}
 	}

 	grpc::Status DldbServiceImpl::Insert(grpc::ServerContext* context, 
			const InsertRequest* request, InsertReply* reply)
 	{
 		assert(request != NULL);
 		assert(reply != NULL);
 		
 		init();

 		if (!initFlag)
 		{
 			return grpc::Status(grpc::StatusCode::UNAVAILABLE, 
 				"Cannot open the ldb instance");
 		}
 		else
 		{
 			int code;
 			std::string message;

 			ldb->insert(request->key(), request->values(),
 						&code, &message);

 			dldb::ReturnMsg msg;
 			msg.set_code(code);
 			msg.set_ret_msg(message);

 			reply->mutable_msg()->CopyFrom(msg);

 			return grpc::Status::OK;
 		}
 	}

 	grpc::Status DldbServiceImpl::Delete(grpc::ServerContext* context,
			const DeleteRequest* request, DeleteReply* reply)
 	{
 		assert(request != NULL);
 		assert(reply != NULL);
 		
 		init();

 		if (!initFlag)
 		{
 			return grpc::Status(grpc::StatusCode::UNAVAILABLE, 
 				"Cannot open the ldb instance");
 		}
 		else
 		{
 			int code;
 			std::string message;

 			ldb->del(request->key(), &code, &message);

 			dldb::ReturnMsg msg;
 			msg.set_code(code);
 			msg.set_ret_msg(message);

 			reply->mutable_msg()->CopyFrom(msg);

 			return grpc::Status::OK;
 		}
 	}

 	grpc::Status DldbServiceImpl::Get(grpc::ServerContext* context,
			const GetRequest* request, GetReply* reply)
 	{
 		assert(request != NULL);
 		assert(reply != NULL);
 		
 		init();

 		if (!initFlag)
 		{
 			return grpc::Status(grpc::StatusCode::UNAVAILABLE, 
 				"Cannot open the ldb instance");
 		}
 		else
 		{
 			int code;
 			std::string message;

 			std::string value = ldb->get(request->key(), &code, &message);

 			dldb::ReturnMsg msg;
 			msg.set_code(code);
 			msg.set_ret_msg(message);

 			reply->mutable_msg()->CopyFrom(msg);

 			reply->set_value(value);

 			return grpc::Status::OK;
 		}
 	}
}
