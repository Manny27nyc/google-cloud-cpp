// Copyright 2020 Google LLC
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

#include "google/cloud/storage/testing/remove_stale_buckets.h"
#include <regex>

namespace google {
namespace cloud {
namespace storage {
namespace testing {

Status RemoveBucketAndContents(
    google::cloud::storage::Client client,
    google::cloud::storage::BucketMetadata const& bucket) {
  storage::UserProject project;
  if (bucket.has_billing() && bucket.billing().requester_pays) {
    project = storage::UserProject(std::to_string(bucket.project_number()));
  }
  // List all the objects and versions, and then delete each.
  for (auto o : client.ListObjects(bucket.name(), Versions(true), project)) {
    if (!o) return std::move(o).status();
    auto status = client.DeleteObject(bucket.name(), o->name(),
                                      Generation(o->generation()), project);
    if (!status.ok()) return status;
  }
  return client.DeleteBucket(bucket.name(), project);
}

Status RemoveBucketAndContents(google::cloud::storage::Client client,
                               std::string const& bucket_name) {
  auto bucket = client.GetBucketMetadata(bucket_name);
  if (!bucket) return std::move(bucket).status();
  return RemoveBucketAndContents(std::move(client), *bucket);
}

Status RemoveStaleBuckets(
    google::cloud::storage::Client client, std::string const& prefix,
    std::chrono::system_clock::time_point created_time_limit) {
  std::regex re("^" + prefix + R"re(-\d{4}-\d{2}-\d{2}_.*$)re");
  for (auto& bucket : client.ListBuckets()) {
    if (!bucket) return std::move(bucket).status();
    if (!std::regex_match(bucket->name(), re)) continue;
    if (bucket->time_created() > created_time_limit) continue;
    (void)RemoveBucketAndContents(client, *bucket);
  }
  return {};
}

}  // namespace testing
}  // namespace storage
}  // namespace cloud
}  // namespace google
