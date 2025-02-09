// Copyright 2019 Google LLC
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

#include "google/cloud/storage/internal/hash_validator_impl.h"
#include "google/cloud/storage/object_metadata.h"

namespace google {
namespace cloud {
namespace storage {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN
namespace internal {

HashValidator::Result NullHashValidator::Finish(HashValues computed) && {
  return Result{{}, std::move(computed), false};
}

void CompositeValidator::ProcessMetadata(ObjectMetadata const& meta) {
  a_->ProcessMetadata(meta);
  b_->ProcessMetadata(meta);
}

void CompositeValidator::ProcessHashValues(HashValues const& values) {
  a_->ProcessHashValues(values);
  b_->ProcessHashValues(values);
}

HashValidator::Result CompositeValidator::Finish(HashValues computed) && {
  auto a = std::move(*a_).Finish(computed);
  auto b = std::move(*b_).Finish(std::move(computed));

  a.received = Merge(std::move(a.received), std::move(b.received));
  a.computed = Merge(std::move(a.computed), std::move(b.computed));
  a.is_mismatch = a.is_mismatch || b.is_mismatch;
  return a;
}

void MD5HashValidator::ProcessMetadata(ObjectMetadata const& meta) {
  if (meta.md5_hash().empty()) {
    // When using the XML API the metadata is empty, but the headers are not. In
    // that case we do not want to replace the received hash with an empty
    // value.
    return;
  }
  received_hash_ = meta.md5_hash();
}

void MD5HashValidator::ProcessHashValues(HashValues const& values) {
  if (!values.md5.empty()) received_hash_ = values.md5;
}

HashValidator::Result MD5HashValidator::Finish(HashValues computed) && {
  if (received_hash_.empty()) return Result{{}, std::move(computed), false};
  auto is_mismatch = (received_hash_ != computed.md5);
  HashValues received{/*.crc32c=*/{}, /*.md5=*/std::move(received_hash_)};
  return Result{std::move(received), std::move(computed), is_mismatch};
}

void Crc32cHashValidator::ProcessMetadata(ObjectMetadata const& meta) {
  if (meta.crc32c().empty()) {
    // When using the XML API the metadata is empty, but the headers are not. In
    // that case we do not want to replace the received hash with an empty
    // value.
    return;
  }
  received_hash_ = meta.crc32c();
}

void Crc32cHashValidator::ProcessHashValues(HashValues const& values) {
  if (!values.crc32c.empty()) received_hash_ = values.crc32c;
}

HashValidator::Result Crc32cHashValidator::Finish(HashValues computed) && {
  if (received_hash_.empty()) return Result{{}, std::move(computed), false};
  auto is_mismatch = (received_hash_ != computed.crc32c);
  HashValues received{/*.crc32c=*/std::move(received_hash_), /*.md5=*/{}};
  return Result{std::move(received), std::move(computed), is_mismatch};
}

}  // namespace internal
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace storage
}  // namespace cloud
}  // namespace google
