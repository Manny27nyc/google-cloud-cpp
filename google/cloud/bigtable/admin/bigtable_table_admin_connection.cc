// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Generated by the Codegen C++ plugin.
// If you make any local changes, they will be lost.
// source: google/bigtable/admin/v2/bigtable_table_admin.proto

#include "google/cloud/bigtable/admin/bigtable_table_admin_connection.h"
#include "google/cloud/bigtable/admin/bigtable_table_admin_options.h"
#include "google/cloud/bigtable/admin/internal/bigtable_table_admin_option_defaults.h"
#include "google/cloud/bigtable/admin/internal/bigtable_table_admin_stub_factory.h"
#include "google/cloud/background_threads.h"
#include "google/cloud/common_options.h"
#include "google/cloud/grpc_options.h"
#include "google/cloud/internal/async_long_running_operation.h"
#include "google/cloud/internal/pagination_range.h"
#include "google/cloud/internal/retry_loop.h"
#include <memory>

namespace google {
namespace cloud {
namespace bigtable_admin {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

BigtableTableAdminConnection::~BigtableTableAdminConnection() = default;

StatusOr<google::bigtable::admin::v2::Table>
BigtableTableAdminConnection::CreateTable(
    google::bigtable::admin::v2::CreateTableRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

StreamRange<google::bigtable::admin::v2::Table>
BigtableTableAdminConnection::ListTables(
    google::bigtable::admin::v2::ListTablesRequest request) {
  return google::cloud::internal::MakePaginationRange<
      StreamRange<google::bigtable::admin::v2::Table>>(
      std::move(request),
      [](google::bigtable::admin::v2::ListTablesRequest const&) {
        return StatusOr<google::bigtable::admin::v2::ListTablesResponse>{};
      },
      [](google::bigtable::admin::v2::ListTablesResponse const&) {
        return std::vector<google::bigtable::admin::v2::Table>();
      });
}

StatusOr<google::bigtable::admin::v2::Table>
BigtableTableAdminConnection::GetTable(
    google::bigtable::admin::v2::GetTableRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

Status BigtableTableAdminConnection::DeleteTable(
    google::bigtable::admin::v2::DeleteTableRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

StatusOr<google::bigtable::admin::v2::Table>
BigtableTableAdminConnection::ModifyColumnFamilies(
    google::bigtable::admin::v2::ModifyColumnFamiliesRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

Status BigtableTableAdminConnection::DropRowRange(
    google::bigtable::admin::v2::DropRowRangeRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

StatusOr<google::bigtable::admin::v2::GenerateConsistencyTokenResponse>
BigtableTableAdminConnection::GenerateConsistencyToken(
    google::bigtable::admin::v2::GenerateConsistencyTokenRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

StatusOr<google::bigtable::admin::v2::CheckConsistencyResponse>
BigtableTableAdminConnection::CheckConsistency(
    google::bigtable::admin::v2::CheckConsistencyRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

future<StatusOr<google::bigtable::admin::v2::Backup>>
BigtableTableAdminConnection::CreateBackup(
    google::bigtable::admin::v2::CreateBackupRequest const&) {
  return google::cloud::make_ready_future<
      StatusOr<google::bigtable::admin::v2::Backup>>(
      Status(StatusCode::kUnimplemented, "not implemented"));
}

StatusOr<google::bigtable::admin::v2::Backup>
BigtableTableAdminConnection::GetBackup(
    google::bigtable::admin::v2::GetBackupRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

StatusOr<google::bigtable::admin::v2::Backup>
BigtableTableAdminConnection::UpdateBackup(
    google::bigtable::admin::v2::UpdateBackupRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

Status BigtableTableAdminConnection::DeleteBackup(
    google::bigtable::admin::v2::DeleteBackupRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

StreamRange<google::bigtable::admin::v2::Backup>
BigtableTableAdminConnection::ListBackups(
    google::bigtable::admin::v2::ListBackupsRequest request) {
  return google::cloud::internal::MakePaginationRange<
      StreamRange<google::bigtable::admin::v2::Backup>>(
      std::move(request),
      [](google::bigtable::admin::v2::ListBackupsRequest const&) {
        return StatusOr<google::bigtable::admin::v2::ListBackupsResponse>{};
      },
      [](google::bigtable::admin::v2::ListBackupsResponse const&) {
        return std::vector<google::bigtable::admin::v2::Backup>();
      });
}

future<StatusOr<google::bigtable::admin::v2::Table>>
BigtableTableAdminConnection::RestoreTable(
    google::bigtable::admin::v2::RestoreTableRequest const&) {
  return google::cloud::make_ready_future<
      StatusOr<google::bigtable::admin::v2::Table>>(
      Status(StatusCode::kUnimplemented, "not implemented"));
}

StatusOr<google::iam::v1::Policy> BigtableTableAdminConnection::GetIamPolicy(
    google::iam::v1::GetIamPolicyRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

StatusOr<google::iam::v1::Policy> BigtableTableAdminConnection::SetIamPolicy(
    google::iam::v1::SetIamPolicyRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

StatusOr<google::iam::v1::TestIamPermissionsResponse>
BigtableTableAdminConnection::TestIamPermissions(
    google::iam::v1::TestIamPermissionsRequest const&) {
  return Status(StatusCode::kUnimplemented, "not implemented");
}

future<StatusOr<google::bigtable::admin::v2::CheckConsistencyResponse>>
BigtableTableAdminConnection::AsyncCheckConsistency(
    google::bigtable::admin::v2::CheckConsistencyRequest const&) {
  return google::cloud::make_ready_future<
      StatusOr<google::bigtable::admin::v2::CheckConsistencyResponse>>(
      Status(StatusCode::kUnimplemented, "not implemented"));
}

namespace {
class BigtableTableAdminConnectionImpl : public BigtableTableAdminConnection {
 public:
  BigtableTableAdminConnectionImpl(
      std::unique_ptr<google::cloud::BackgroundThreads> background,
      std::shared_ptr<bigtable_admin_internal::BigtableTableAdminStub> stub,
      Options const& options)
      : background_(std::move(background)),
        stub_(std::move(stub)),
        retry_policy_prototype_(
            options.get<BigtableTableAdminRetryPolicyOption>()->clone()),
        backoff_policy_prototype_(
            options.get<BigtableTableAdminBackoffPolicyOption>()->clone()),
        polling_policy_prototype_(
            options.get<BigtableTableAdminPollingPolicyOption>()->clone()),
        idempotency_policy_(
            options.get<BigtableTableAdminConnectionIdempotencyPolicyOption>()
                ->clone()) {}

  ~BigtableTableAdminConnectionImpl() override = default;

  StatusOr<google::bigtable::admin::v2::Table> CreateTable(
      google::bigtable::admin::v2::CreateTableRequest const& request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->CreateTable(request),
        [this](grpc::ClientContext& context,
               google::bigtable::admin::v2::CreateTableRequest const& request) {
          return stub_->CreateTable(context, request);
        },
        request, __func__);
  }

  StreamRange<google::bigtable::admin::v2::Table> ListTables(
      google::bigtable::admin::v2::ListTablesRequest request) override {
    request.clear_page_token();
    auto stub = stub_;
    auto retry = std::shared_ptr<BigtableTableAdminRetryPolicy const>(
        retry_policy_prototype_->clone());
    auto backoff = std::shared_ptr<BackoffPolicy const>(
        backoff_policy_prototype_->clone());
    auto idempotency = idempotency_policy_->ListTables(request);
    char const* function_name = __func__;
    return google::cloud::internal::MakePaginationRange<
        StreamRange<google::bigtable::admin::v2::Table>>(
        std::move(request),
        [stub, retry, backoff, idempotency, function_name](
            google::bigtable::admin::v2::ListTablesRequest const& r) {
          return google::cloud::internal::RetryLoop(
              retry->clone(), backoff->clone(), idempotency,
              [stub](grpc::ClientContext& context,
                     google::bigtable::admin::v2::ListTablesRequest const&
                         request) {
                return stub->ListTables(context, request);
              },
              r, function_name);
        },
        [](google::bigtable::admin::v2::ListTablesResponse r) {
          std::vector<google::bigtable::admin::v2::Table> result(
              r.tables().size());
          auto& messages = *r.mutable_tables();
          std::move(messages.begin(), messages.end(), result.begin());
          return result;
        });
  }

  StatusOr<google::bigtable::admin::v2::Table> GetTable(
      google::bigtable::admin::v2::GetTableRequest const& request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->GetTable(request),
        [this](grpc::ClientContext& context,
               google::bigtable::admin::v2::GetTableRequest const& request) {
          return stub_->GetTable(context, request);
        },
        request, __func__);
  }

  Status DeleteTable(
      google::bigtable::admin::v2::DeleteTableRequest const& request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->DeleteTable(request),
        [this](grpc::ClientContext& context,
               google::bigtable::admin::v2::DeleteTableRequest const& request) {
          return stub_->DeleteTable(context, request);
        },
        request, __func__);
  }

  StatusOr<google::bigtable::admin::v2::Table> ModifyColumnFamilies(
      google::bigtable::admin::v2::ModifyColumnFamiliesRequest const& request)
      override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->ModifyColumnFamilies(request),
        [this](grpc::ClientContext& context,
               google::bigtable::admin::v2::ModifyColumnFamiliesRequest const&
                   request) {
          return stub_->ModifyColumnFamilies(context, request);
        },
        request, __func__);
  }

  Status DropRowRange(google::bigtable::admin::v2::DropRowRangeRequest const&
                          request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->DropRowRange(request),
        [this](
            grpc::ClientContext& context,
            google::bigtable::admin::v2::DropRowRangeRequest const& request) {
          return stub_->DropRowRange(context, request);
        },
        request, __func__);
  }

  StatusOr<google::bigtable::admin::v2::GenerateConsistencyTokenResponse>
  GenerateConsistencyToken(
      google::bigtable::admin::v2::GenerateConsistencyTokenRequest const&
          request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->GenerateConsistencyToken(request),
        [this](
            grpc::ClientContext& context,
            google::bigtable::admin::v2::GenerateConsistencyTokenRequest const&
                request) {
          return stub_->GenerateConsistencyToken(context, request);
        },
        request, __func__);
  }

  StatusOr<google::bigtable::admin::v2::CheckConsistencyResponse>
  CheckConsistency(google::bigtable::admin::v2::CheckConsistencyRequest const&
                       request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->CheckConsistency(request),
        [this](grpc::ClientContext& context,
               google::bigtable::admin::v2::CheckConsistencyRequest const&
                   request) {
          return stub_->CheckConsistency(context, request);
        },
        request, __func__);
  }

  future<StatusOr<google::bigtable::admin::v2::Backup>> CreateBackup(
      google::bigtable::admin::v2::CreateBackupRequest const& request)
      override {
    auto stub = stub_;
    return google::cloud::internal::AsyncLongRunningOperation<
        google::bigtable::admin::v2::Backup>(
        background_->cq(), request,
        [stub](
            google::cloud::CompletionQueue& cq,
            std::unique_ptr<grpc::ClientContext> context,
            google::bigtable::admin::v2::CreateBackupRequest const& request) {
          return stub->AsyncCreateBackup(cq, std::move(context), request);
        },
        [stub](google::cloud::CompletionQueue& cq,
               std::unique_ptr<grpc::ClientContext> context,
               google::longrunning::GetOperationRequest const& request) {
          return stub->AsyncGetOperation(cq, std::move(context), request);
        },
        [stub](google::cloud::CompletionQueue& cq,
               std::unique_ptr<grpc::ClientContext> context,
               google::longrunning::CancelOperationRequest const& request) {
          return stub->AsyncCancelOperation(cq, std::move(context), request);
        },
        &google::cloud::internal::ExtractLongRunningResultResponse<
            google::bigtable::admin::v2::Backup>,
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->CreateBackup(request),
        polling_policy_prototype_->clone(), __func__);
  }

  StatusOr<google::bigtable::admin::v2::Backup> GetBackup(
      google::bigtable::admin::v2::GetBackupRequest const& request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->GetBackup(request),
        [this](grpc::ClientContext& context,
               google::bigtable::admin::v2::GetBackupRequest const& request) {
          return stub_->GetBackup(context, request);
        },
        request, __func__);
  }

  StatusOr<google::bigtable::admin::v2::Backup> UpdateBackup(
      google::bigtable::admin::v2::UpdateBackupRequest const& request)
      override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->UpdateBackup(request),
        [this](
            grpc::ClientContext& context,
            google::bigtable::admin::v2::UpdateBackupRequest const& request) {
          return stub_->UpdateBackup(context, request);
        },
        request, __func__);
  }

  Status DeleteBackup(google::bigtable::admin::v2::DeleteBackupRequest const&
                          request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->DeleteBackup(request),
        [this](
            grpc::ClientContext& context,
            google::bigtable::admin::v2::DeleteBackupRequest const& request) {
          return stub_->DeleteBackup(context, request);
        },
        request, __func__);
  }

  StreamRange<google::bigtable::admin::v2::Backup> ListBackups(
      google::bigtable::admin::v2::ListBackupsRequest request) override {
    request.clear_page_token();
    auto stub = stub_;
    auto retry = std::shared_ptr<BigtableTableAdminRetryPolicy const>(
        retry_policy_prototype_->clone());
    auto backoff = std::shared_ptr<BackoffPolicy const>(
        backoff_policy_prototype_->clone());
    auto idempotency = idempotency_policy_->ListBackups(request);
    char const* function_name = __func__;
    return google::cloud::internal::MakePaginationRange<
        StreamRange<google::bigtable::admin::v2::Backup>>(
        std::move(request),
        [stub, retry, backoff, idempotency, function_name](
            google::bigtable::admin::v2::ListBackupsRequest const& r) {
          return google::cloud::internal::RetryLoop(
              retry->clone(), backoff->clone(), idempotency,
              [stub](grpc::ClientContext& context,
                     google::bigtable::admin::v2::ListBackupsRequest const&
                         request) {
                return stub->ListBackups(context, request);
              },
              r, function_name);
        },
        [](google::bigtable::admin::v2::ListBackupsResponse r) {
          std::vector<google::bigtable::admin::v2::Backup> result(
              r.backups().size());
          auto& messages = *r.mutable_backups();
          std::move(messages.begin(), messages.end(), result.begin());
          return result;
        });
  }

  future<StatusOr<google::bigtable::admin::v2::Table>> RestoreTable(
      google::bigtable::admin::v2::RestoreTableRequest const& request)
      override {
    auto stub = stub_;
    return google::cloud::internal::AsyncLongRunningOperation<
        google::bigtable::admin::v2::Table>(
        background_->cq(), request,
        [stub](
            google::cloud::CompletionQueue& cq,
            std::unique_ptr<grpc::ClientContext> context,
            google::bigtable::admin::v2::RestoreTableRequest const& request) {
          return stub->AsyncRestoreTable(cq, std::move(context), request);
        },
        [stub](google::cloud::CompletionQueue& cq,
               std::unique_ptr<grpc::ClientContext> context,
               google::longrunning::GetOperationRequest const& request) {
          return stub->AsyncGetOperation(cq, std::move(context), request);
        },
        [stub](google::cloud::CompletionQueue& cq,
               std::unique_ptr<grpc::ClientContext> context,
               google::longrunning::CancelOperationRequest const& request) {
          return stub->AsyncCancelOperation(cq, std::move(context), request);
        },
        &google::cloud::internal::ExtractLongRunningResultResponse<
            google::bigtable::admin::v2::Table>,
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->RestoreTable(request),
        polling_policy_prototype_->clone(), __func__);
  }

  StatusOr<google::iam::v1::Policy> GetIamPolicy(
      google::iam::v1::GetIamPolicyRequest const& request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->GetIamPolicy(request),
        [this](grpc::ClientContext& context,
               google::iam::v1::GetIamPolicyRequest const& request) {
          return stub_->GetIamPolicy(context, request);
        },
        request, __func__);
  }

  StatusOr<google::iam::v1::Policy> SetIamPolicy(
      google::iam::v1::SetIamPolicyRequest const& request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->SetIamPolicy(request),
        [this](grpc::ClientContext& context,
               google::iam::v1::SetIamPolicyRequest const& request) {
          return stub_->SetIamPolicy(context, request);
        },
        request, __func__);
  }

  StatusOr<google::iam::v1::TestIamPermissionsResponse> TestIamPermissions(
      google::iam::v1::TestIamPermissionsRequest const& request) override {
    return google::cloud::internal::RetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->TestIamPermissions(request),
        [this](grpc::ClientContext& context,
               google::iam::v1::TestIamPermissionsRequest const& request) {
          return stub_->TestIamPermissions(context, request);
        },
        request, __func__);
  }

  future<StatusOr<google::bigtable::admin::v2::CheckConsistencyResponse>>
  AsyncCheckConsistency(
      google::bigtable::admin::v2::CheckConsistencyRequest const& request)
      override {
    auto& stub = stub_;
    return google::cloud::internal::AsyncRetryLoop(
        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),
        idempotency_policy_->CheckConsistency(request), background_->cq(),
        [stub](CompletionQueue& cq,
               std::unique_ptr<grpc::ClientContext> context,
               google::bigtable::admin::v2::CheckConsistencyRequest const&
                   request) {
          return stub->AsyncCheckConsistency(cq, std::move(context), request);
        },
        request, __func__);
  }

 private:
  std::unique_ptr<google::cloud::BackgroundThreads> background_;
  std::shared_ptr<bigtable_admin_internal::BigtableTableAdminStub> stub_;
  std::unique_ptr<BigtableTableAdminRetryPolicy const> retry_policy_prototype_;
  std::unique_ptr<BackoffPolicy const> backoff_policy_prototype_;
  std::unique_ptr<PollingPolicy const> polling_policy_prototype_;
  std::unique_ptr<BigtableTableAdminConnectionIdempotencyPolicy>
      idempotency_policy_;
};
}  // namespace

std::shared_ptr<BigtableTableAdminConnection> MakeBigtableTableAdminConnection(
    Options options) {
  internal::CheckExpectedOptions<CommonOptionList, GrpcOptionList,
                                 BigtableTableAdminPolicyOptionList>(options,
                                                                     __func__);
  options = bigtable_admin_internal::BigtableTableAdminDefaultOptions(
      std::move(options));
  auto background = internal::MakeBackgroundThreadsFactory(options)();
  auto stub = bigtable_admin_internal::CreateDefaultBigtableTableAdminStub(
      background->cq(), options);
  return std::make_shared<BigtableTableAdminConnectionImpl>(
      std::move(background), std::move(stub), options);
}

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace bigtable_admin
}  // namespace cloud
}  // namespace google

namespace google {
namespace cloud {
namespace bigtable_admin_internal {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN

std::shared_ptr<bigtable_admin::BigtableTableAdminConnection>
MakeBigtableTableAdminConnection(std::shared_ptr<BigtableTableAdminStub> stub,
                                 Options options) {
  options = BigtableTableAdminDefaultOptions(std::move(options));
  return std::make_shared<bigtable_admin::BigtableTableAdminConnectionImpl>(
      internal::MakeBackgroundThreadsFactory(options)(), std::move(stub),
      std::move(options));
}

GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace bigtable_admin_internal
}  // namespace cloud
}  // namespace google
