// Copyright 2018 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "google/cloud/bigtable/instance_admin_client.h"
#include "google/cloud/bigtable/internal/common_client.h"
#include "google/cloud/bigtable/internal/logging_instance_admin_client.h"
#include <google/longrunning/operations.grpc.pb.h>

namespace google {
namespace cloud {
namespace bigtable {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN
namespace {

namespace btadmin = ::google::bigtable::admin::v2;

/**
 * An InstanceAdminClient for single-threaded programs that refreshes
 * credentials on all gRPC errors.
 *
 * This class should not be used by multiple threads, it makes no attempt to
 * protect its critical sections.  While it is rare that the admin interface
 * will be used by multiple threads, we should use the same approach here and in
 * the regular client to support multi-threaded programs.
 *
 * The class also aggressively reconnects on any gRPC errors. A future version
 * should only reconnect on those errors that indicate the credentials or
 * connections need refreshing.
 */
class DefaultInstanceAdminClient : public InstanceAdminClient {
 public:
  DefaultInstanceAdminClient(std::string project, Options options)
      : project_(std::move(project)), impl_(std::move(options)) {}

  std::string const& project() const override { return project_; }
  std::shared_ptr<grpc::Channel> Channel() override { return impl_.Channel(); }
  void reset() override { return impl_.reset(); }

  grpc::Status ListInstances(
      grpc::ClientContext* context,
      btadmin::ListInstancesRequest const& request,
      btadmin::ListInstancesResponse* response) override {
    return impl_.Stub()->ListInstances(context, request, response);
  }

  grpc::Status CreateInstance(
      grpc::ClientContext* context,
      btadmin::CreateInstanceRequest const& request,
      google::longrunning::Operation* response) override {
    return impl_.Stub()->CreateInstance(context, request, response);
  }

  grpc::Status UpdateInstance(
      grpc::ClientContext* context,
      btadmin::PartialUpdateInstanceRequest const& request,
      google::longrunning::Operation* response) override {
    return impl_.Stub()->PartialUpdateInstance(context, request, response);
  }

  grpc::Status GetOperation(
      grpc::ClientContext* context,
      google::longrunning::GetOperationRequest const& request,
      google::longrunning::Operation* response) override {
    auto stub = google::longrunning::Operations::NewStub(Channel());
    return stub->GetOperation(context, request, response);
  }

  grpc::Status GetInstance(grpc::ClientContext* context,
                           btadmin::GetInstanceRequest const& request,
                           btadmin::Instance* response) override {
    return impl_.Stub()->GetInstance(context, request, response);
  }

  grpc::Status DeleteInstance(grpc::ClientContext* context,
                              btadmin::DeleteInstanceRequest const& request,
                              google::protobuf::Empty* response) override {
    return impl_.Stub()->DeleteInstance(context, request, response);
  }

  grpc::Status ListClusters(grpc::ClientContext* context,
                            btadmin::ListClustersRequest const& request,
                            btadmin::ListClustersResponse* response) override {
    return impl_.Stub()->ListClusters(context, request, response);
  }

  grpc::Status GetCluster(grpc::ClientContext* context,
                          btadmin::GetClusterRequest const& request,
                          btadmin::Cluster* response) override {
    return impl_.Stub()->GetCluster(context, request, response);
  }

  grpc::Status DeleteCluster(grpc::ClientContext* context,
                             btadmin::DeleteClusterRequest const& request,
                             google::protobuf::Empty* response) override {
    return impl_.Stub()->DeleteCluster(context, request, response);
  }

  grpc::Status CreateCluster(
      grpc::ClientContext* context,
      btadmin::CreateClusterRequest const& request,
      google::longrunning::Operation* response) override {
    return impl_.Stub()->CreateCluster(context, request, response);
  }

  grpc::Status UpdateCluster(
      grpc::ClientContext* context, btadmin::Cluster const& request,
      google::longrunning::Operation* response) override {
    return impl_.Stub()->UpdateCluster(context, request, response);
  }

  grpc::Status CreateAppProfile(grpc::ClientContext* context,
                                btadmin::CreateAppProfileRequest const& request,
                                btadmin::AppProfile* response) override {
    return impl_.Stub()->CreateAppProfile(context, request, response);
  }

  grpc::Status GetAppProfile(grpc::ClientContext* context,
                             btadmin::GetAppProfileRequest const& request,
                             btadmin::AppProfile* response) override {
    return impl_.Stub()->GetAppProfile(context, request, response);
  }

  grpc::Status ListAppProfiles(
      grpc::ClientContext* context,
      btadmin::ListAppProfilesRequest const& request,
      btadmin::ListAppProfilesResponse* response) override {
    return impl_.Stub()->ListAppProfiles(context, request, response);
  }

  grpc::Status UpdateAppProfile(
      grpc::ClientContext* context,
      btadmin::UpdateAppProfileRequest const& request,
      google::longrunning::Operation* response) override {
    return impl_.Stub()->UpdateAppProfile(context, request, response);
  }

  grpc::Status DeleteAppProfile(grpc::ClientContext* context,
                                btadmin::DeleteAppProfileRequest const& request,
                                google::protobuf::Empty* response) override {
    return impl_.Stub()->DeleteAppProfile(context, request, response);
  }

  grpc::Status GetIamPolicy(grpc::ClientContext* context,
                            google::iam::v1::GetIamPolicyRequest const& request,
                            google::iam::v1::Policy* response) override {
    return impl_.Stub()->GetIamPolicy(context, request, response);
  }

  grpc::Status SetIamPolicy(grpc::ClientContext* context,
                            google::iam::v1::SetIamPolicyRequest const& request,
                            google::iam::v1::Policy* response) override {
    return impl_.Stub()->SetIamPolicy(context, request, response);
  }

  grpc::Status TestIamPermissions(
      grpc::ClientContext* context,
      google::iam::v1::TestIamPermissionsRequest const& request,
      google::iam::v1::TestIamPermissionsResponse* response) override {
    return impl_.Stub()->TestIamPermissions(context, request, response);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<btadmin::ListInstancesResponse>>
  AsyncListInstances(grpc::ClientContext* context,
                     btadmin::ListInstancesRequest const& request,
                     grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncListInstances(context, request, cq);
  }

  std::unique_ptr<grpc::ClientAsyncResponseReaderInterface<btadmin::Instance>>
  AsyncGetInstance(grpc::ClientContext* context,
                   btadmin::GetInstanceRequest const& request,
                   grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncGetInstance(context, request, cq);
  }

  std::unique_ptr<grpc::ClientAsyncResponseReaderInterface<btadmin::Cluster>>
  AsyncGetCluster(grpc::ClientContext* context,
                  btadmin::GetClusterRequest const& request,
                  grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncGetCluster(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::protobuf::Empty>>
  AsyncDeleteCluster(grpc::ClientContext* context,
                     btadmin::DeleteClusterRequest const& request,
                     grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncDeleteCluster(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::longrunning::Operation>>
  AsyncCreateCluster(grpc::ClientContext* context,
                     btadmin::CreateClusterRequest const& request,
                     grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncCreateCluster(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::longrunning::Operation>>
  AsyncCreateInstance(grpc::ClientContext* context,
                      btadmin::CreateInstanceRequest const& request,
                      grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncCreateInstance(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::longrunning::Operation>>
  AsyncUpdateInstance(grpc::ClientContext* context,
                      btadmin::PartialUpdateInstanceRequest const& request,
                      grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncPartialUpdateInstance(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::longrunning::Operation>>
  AsyncUpdateCluster(grpc::ClientContext* context,
                     btadmin::Cluster const& request,
                     grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncUpdateCluster(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::protobuf::Empty>>
  AsyncDeleteInstance(grpc::ClientContext* context,
                      btadmin::DeleteInstanceRequest const& request,
                      grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncDeleteInstance(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<btadmin::ListClustersResponse>>
  AsyncListClusters(grpc::ClientContext* context,
                    btadmin::ListClustersRequest const& request,
                    grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncListClusters(context, request, cq);
  }

  std::unique_ptr<grpc::ClientAsyncResponseReaderInterface<btadmin::AppProfile>>
  AsyncGetAppProfile(grpc::ClientContext* context,
                     btadmin::GetAppProfileRequest const& request,
                     grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncGetAppProfile(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::protobuf::Empty>>
  AsyncDeleteAppProfile(grpc::ClientContext* context,
                        btadmin::DeleteAppProfileRequest const& request,
                        grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncDeleteAppProfile(context, request, cq);
  }

  std::unique_ptr<grpc::ClientAsyncResponseReaderInterface<btadmin::AppProfile>>
  AsyncCreateAppProfile(grpc::ClientContext* context,
                        btadmin::CreateAppProfileRequest const& request,
                        grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncCreateAppProfile(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::longrunning::Operation>>
  AsyncUpdateAppProfile(grpc::ClientContext* context,
                        btadmin::UpdateAppProfileRequest const& request,
                        grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncUpdateAppProfile(context, request, cq);
  }

  std::unique_ptr<grpc::ClientAsyncResponseReaderInterface<
      btadmin::ListAppProfilesResponse>>
  AsyncListAppProfiles(grpc::ClientContext* context,
                       btadmin::ListAppProfilesRequest const& request,
                       grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncListAppProfiles(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::iam::v1::Policy>>
  AsyncGetIamPolicy(grpc::ClientContext* context,
                    google::iam::v1::GetIamPolicyRequest const& request,
                    grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncGetIamPolicy(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::iam::v1::Policy>>
  AsyncSetIamPolicy(grpc::ClientContext* context,
                    google::iam::v1::SetIamPolicyRequest const& request,
                    grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncSetIamPolicy(context, request, cq);
  }

  std::unique_ptr<grpc::ClientAsyncResponseReaderInterface<
      google::iam::v1::TestIamPermissionsResponse>>
  AsyncTestIamPermissions(
      grpc::ClientContext* context,
      google::iam::v1::TestIamPermissionsRequest const& request,
      grpc::CompletionQueue* cq) override {
    return impl_.Stub()->AsyncTestIamPermissions(context, request, cq);
  }

  std::unique_ptr<
      grpc::ClientAsyncResponseReaderInterface<google::longrunning::Operation>>
  AsyncGetOperation(grpc::ClientContext* context,
                    google::longrunning::GetOperationRequest const& request,
                    grpc::CompletionQueue* cq) override {
    auto stub = google::longrunning::Operations::NewStub(Channel());
    return std::unique_ptr<grpc::ClientAsyncResponseReaderInterface<
        google::longrunning::Operation>>(
        stub->AsyncGetOperation(context, request, cq).release());
  }

 private:
  google::cloud::BackgroundThreadsFactory BackgroundThreadsFactory() override {
    return impl_.BackgroundThreadsFactory();
  }

  struct Traits {
    static std::string const& Endpoint(Options const& options) {
      return options.get<InstanceAdminEndpointOption>();
    }
  };

  std::string project_;
  internal::CommonClient<Traits, btadmin::BigtableInstanceAdmin> impl_;
};

}  // anonymous namespace

std::shared_ptr<InstanceAdminClient> MakeInstanceAdminClient(
    std::string project, Options options) {
  options = internal::DefaultOptions(std::move(options));
  bool tracing_enabled = google::cloud::internal::Contains(
      options.get<TracingComponentsOption>(), "rpc");
  auto tracing_options = options.get<GrpcTracingOptionsOption>();

  std::shared_ptr<InstanceAdminClient> client =
      std::make_shared<DefaultInstanceAdminClient>(std::move(project),
                                                   std::move(options));
  if (tracing_enabled) {
    GCP_LOG(INFO) << "Enabled logging for gRPC calls";
    client = std::make_shared<internal::LoggingInstanceAdminClient>(
        std::move(client), std::move(tracing_options));
  }
  return client;
}

std::shared_ptr<InstanceAdminClient> CreateDefaultInstanceAdminClient(
    std::string project, ClientOptions options) {
  return MakeInstanceAdminClient(std::move(project),
                                 internal::MakeOptions(std::move(options)));
}

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace bigtable
}  // namespace cloud
}  // namespace google
