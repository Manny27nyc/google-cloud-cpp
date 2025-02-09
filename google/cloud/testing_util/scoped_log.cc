// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "google/cloud/testing_util/scoped_log.h"
#include "absl/strings/str_split.h"
#include <iterator>

namespace google {
namespace cloud {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN
namespace testing_util {

std::vector<std::string> ScopedLog::Backend::ExtractLines() {
  std::vector<std::string> result;
  {
    std::lock_guard<std::mutex> lk(mu_);
    result.swap(log_lines_);
  }
  return result;
}

void ScopedLog::Backend::Process(LogRecord const& lr) {
  // Break the record into lines. It is easier to analyze them as such.
  std::vector<std::string> result = absl::StrSplit(lr.message, '\n');
  std::lock_guard<std::mutex> lk(mu_);
  log_lines_.insert(log_lines_.end(), std::make_move_iterator(result.begin()),
                    std::make_move_iterator(result.end()));
}

void ScopedLog::Backend::ProcessWithOwnership(LogRecord lr) { Process(lr); }

}  // namespace testing_util
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace cloud
}  // namespace google
