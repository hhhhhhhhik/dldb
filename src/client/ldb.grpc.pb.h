// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: ldb.proto
#ifndef GRPC_ldb_2eproto__INCLUDED
#define GRPC_ldb_2eproto__INCLUDED

#include "ldb.pb.h"

#include <grpc++/support/async_stream.h>
#include <grpc++/impl/rpc_method.h>
#include <grpc++/impl/proto_utils.h>
#include <grpc++/impl/service_type.h>
#include <grpc++/support/async_unary_call.h>
#include <grpc++/support/status.h>
#include <grpc++/support/stub_options.h>
#include <grpc++/support/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace ldb {

class rpc GRPC_FINAL {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status Insert(::grpc::ClientContext* context, const ::ldb::InsertRequest& request, ::ldb::InsertReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ldb::InsertReply>> AsyncInsert(::grpc::ClientContext* context, const ::ldb::InsertRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ldb::InsertReply>>(AsyncInsertRaw(context, request, cq));
    }
    virtual ::grpc::Status Delete(::grpc::ClientContext* context, const ::ldb::DeleteRequest& request, ::ldb::DeleteReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ldb::DeleteReply>> AsyncDelete(::grpc::ClientContext* context, const ::ldb::DeleteRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ldb::DeleteReply>>(AsyncDeleteRaw(context, request, cq));
    }
    virtual ::grpc::Status Get(::grpc::ClientContext* context, const ::ldb::GetRequest& request, ::ldb::GetReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ldb::GetReply>> AsyncGet(::grpc::ClientContext* context, const ::ldb::GetRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ldb::GetReply>>(AsyncGetRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::ldb::InsertReply>* AsyncInsertRaw(::grpc::ClientContext* context, const ::ldb::InsertRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::ldb::DeleteReply>* AsyncDeleteRaw(::grpc::ClientContext* context, const ::ldb::DeleteRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::ldb::GetReply>* AsyncGetRaw(::grpc::ClientContext* context, const ::ldb::GetRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub GRPC_FINAL : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::Channel>& channel);
    ::grpc::Status Insert(::grpc::ClientContext* context, const ::ldb::InsertRequest& request, ::ldb::InsertReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ldb::InsertReply>> AsyncInsert(::grpc::ClientContext* context, const ::ldb::InsertRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ldb::InsertReply>>(AsyncInsertRaw(context, request, cq));
    }
    ::grpc::Status Delete(::grpc::ClientContext* context, const ::ldb::DeleteRequest& request, ::ldb::DeleteReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ldb::DeleteReply>> AsyncDelete(::grpc::ClientContext* context, const ::ldb::DeleteRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ldb::DeleteReply>>(AsyncDeleteRaw(context, request, cq));
    }
    ::grpc::Status Get(::grpc::ClientContext* context, const ::ldb::GetRequest& request, ::ldb::GetReply* response) GRPC_OVERRIDE;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ldb::GetReply>> AsyncGet(::grpc::ClientContext* context, const ::ldb::GetRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ldb::GetReply>>(AsyncGetRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::Channel> channel_;
    ::grpc::ClientAsyncResponseReader< ::ldb::InsertReply>* AsyncInsertRaw(::grpc::ClientContext* context, const ::ldb::InsertRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::ldb::DeleteReply>* AsyncDeleteRaw(::grpc::ClientContext* context, const ::ldb::DeleteRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    ::grpc::ClientAsyncResponseReader< ::ldb::GetReply>* AsyncGetRaw(::grpc::ClientContext* context, const ::ldb::GetRequest& request, ::grpc::CompletionQueue* cq) GRPC_OVERRIDE;
    const ::grpc::RpcMethod rpcmethod_Insert_;
    const ::grpc::RpcMethod rpcmethod_Delete_;
    const ::grpc::RpcMethod rpcmethod_Get_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::Channel>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::SynchronousService {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Insert(::grpc::ServerContext* context, const ::ldb::InsertRequest* request, ::ldb::InsertReply* response);
    virtual ::grpc::Status Delete(::grpc::ServerContext* context, const ::ldb::DeleteRequest* request, ::ldb::DeleteReply* response);
    virtual ::grpc::Status Get(::grpc::ServerContext* context, const ::ldb::GetRequest* request, ::ldb::GetReply* response);
    ::grpc::RpcService* service() GRPC_OVERRIDE GRPC_FINAL;
   private:
    std::unique_ptr< ::grpc::RpcService> service_;
  };
  class AsyncService GRPC_FINAL : public ::grpc::AsynchronousService {
   public:
    explicit AsyncService();
    ~AsyncService() {};
    void RequestInsert(::grpc::ServerContext* context, ::ldb::InsertRequest* request, ::grpc::ServerAsyncResponseWriter< ::ldb::InsertReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestDelete(::grpc::ServerContext* context, ::ldb::DeleteRequest* request, ::grpc::ServerAsyncResponseWriter< ::ldb::DeleteReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
    void RequestGet(::grpc::ServerContext* context, ::ldb::GetRequest* request, ::grpc::ServerAsyncResponseWriter< ::ldb::GetReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag);
  };
};

}  // namespace ldb


#endif  // GRPC_ldb_2eproto__INCLUDED