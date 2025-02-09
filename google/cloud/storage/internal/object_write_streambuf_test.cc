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

#include "google/cloud/storage/internal/object_write_streambuf.h"
#include "google/cloud/storage/object_metadata.h"
#include "google/cloud/storage/testing/mock_client.h"
#include "google/cloud/testing_util/status_matchers.h"
#include "absl/memory/memory.h"
#include <gmock/gmock.h>

namespace google {
namespace cloud {
namespace storage {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN
namespace internal {
namespace {

using ::google::cloud::testing_util::StatusIs;
using ::testing::_;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::InvokeWithoutArgs;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

/// @test Verify that uploading an empty stream creates a single chunk.
TEST(ObjectWriteStreambufTest, EmptyStream) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;

  int count = 0;
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t s,
                    HashValues const&) {
        ++count;
        EXPECT_EQ(1, count);
        EXPECT_EQ(0, TotalBytes(p));
        EXPECT_EQ(0, s);
        return make_status_or(ResumableUploadResponse{
            "{}", 0, {}, ResumableUploadResponse::kInProgress, {}});
      });
  EXPECT_CALL(*mock, next_expected_byte()).WillOnce(Return(0));

  ObjectWriteStream stream(absl::make_unique<ObjectWriteStreambuf>(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled));
  stream.Close();
  EXPECT_STATUS_OK(stream.last_status());
}

/// @test Verify that streams auto-finalize if enabled.
TEST(ObjectWriteStreambufTest, AutoFinalizeEnabled) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;

  int count = 0;
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t s,
                    HashValues const&) {
        ++count;
        EXPECT_EQ(1, count);
        EXPECT_EQ(0, TotalBytes(p));
        EXPECT_EQ(0, s);
        return make_status_or(ResumableUploadResponse{
            {}, 0, {}, ResumableUploadResponse::kDone, {}});
      });
  EXPECT_CALL(*mock, next_expected_byte()).WillOnce(Return(0));

  {
    ObjectWriteStream stream(absl::make_unique<ObjectWriteStreambuf>(
        std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
        CreateNullHashValidator(), AutoFinalizeConfig::kEnabled));
  }
}

/// @test Verify that streams do not auto-finalize if so configured.
TEST(ObjectWriteStreambufTest, AutoFinalizeDisabled) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;

  EXPECT_CALL(*mock, UploadFinalChunk).Times(0);
  EXPECT_CALL(*mock, next_expected_byte()).Times(0);

  {
    ObjectWriteStream stream(absl::make_unique<ObjectWriteStreambuf>(
        std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
        CreateNullHashValidator(), AutoFinalizeConfig::kDisabled));
  }
}

/// @test Verify that uploading a small stream creates a single chunk.
TEST(ObjectWriteStreambufTest, SmallStream) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload = "small test payload";

  int count = 0;
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t s,
                    HashValues const&) {
        ++count;
        EXPECT_EQ(1, count);
        EXPECT_THAT(p, ElementsAre(ConstBuffer{payload}));
        EXPECT_EQ(payload.size(), s);
        auto last_committed_byte = payload.size() - 1;
        return make_status_or(
            ResumableUploadResponse{"{}",
                                    last_committed_byte,
                                    {},
                                    ResumableUploadResponse::kInProgress,
                                    {}});
      });
  EXPECT_CALL(*mock, next_expected_byte()).WillOnce(Return(0));

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  streambuf.sputn(payload.data(), payload.size());
  auto response = streambuf.Close();
  EXPECT_STATUS_OK(response);
}

/// @test Verify that uploading a stream which ends on a upload chunk quantum
/// works as expected.
TEST(ObjectWriteStreambufTest, EmptyTrailer) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload(quantum, '*');

  int count = 0;
  size_t next_byte = 0;
  EXPECT_CALL(*mock, UploadChunk).WillOnce([&](ConstBufferSequence const& p) {
    ++count;
    EXPECT_EQ(1, count);
    EXPECT_THAT(p, ElementsAre(ConstBuffer{payload}));
    auto last_committed_byte = payload.size() - 1;
    next_byte = last_committed_byte + 1;
    return make_status_or(ResumableUploadResponse{
        "", last_committed_byte, {}, ResumableUploadResponse::kInProgress, {}});
  });
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t s,
                    HashValues const&) {
        ++count;
        EXPECT_EQ(2, count);
        EXPECT_EQ(0, TotalBytes(p));
        EXPECT_EQ(quantum, s);
        auto last_committed_byte = quantum - 1;
        return make_status_or(
            ResumableUploadResponse{"{}",
                                    last_committed_byte,
                                    {},
                                    ResumableUploadResponse::kInProgress,
                                    {}});
      });
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return next_byte;
  });

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  streambuf.sputn(payload.data(), payload.size());
  auto response = streambuf.Close();
  EXPECT_STATUS_OK(response);
}

/// @test Verify that a stream sends a single message for large payloads.
TEST(ObjectWriteStreambufTest, FlushAfterLargePayload) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload_1(3 * quantum, '*');
  std::string const payload_2("trailer");
  size_t next_byte = 0;
  {
    InSequence seq;
    EXPECT_CALL(*mock, UploadChunk).WillOnce(InvokeWithoutArgs([&]() {
      next_byte = payload_1.size();
      return make_status_or(
          ResumableUploadResponse{"",
                                  payload_1.size() - 1,
                                  {},
                                  ResumableUploadResponse::kInProgress,
                                  {}});
    }));
    EXPECT_CALL(*mock, UploadFinalChunk({{payload_2}},
                                        payload_1.size() + payload_2.size(), _))
        .WillOnce(Return(make_status_or(
            ResumableUploadResponse{"{}",
                                    payload_1.size() + payload_2.size() - 1,
                                    {},
                                    ResumableUploadResponse::kInProgress,
                                    {}})));
  }
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return next_byte;
  });

  ObjectWriteStreambuf streambuf(
      std::move(mock), 3 * quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  streambuf.sputn(payload_1.data(), payload_1.size());
  streambuf.sputn(payload_2.data(), payload_2.size());
  auto response = streambuf.Close();
  EXPECT_STATUS_OK(response);
}

/// @test Verify that a stream flushes when a full quantum is available.
TEST(ObjectWriteStreambufTest, FlushAfterFullQuantum) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload_1("header");
  std::string const payload_2(quantum, '*');

  int count = 0;
  size_t next_byte = 0;
  EXPECT_CALL(*mock, UploadChunk).WillOnce([&](ConstBufferSequence const& p) {
    ++count;
    EXPECT_EQ(1, count);
    auto trailer = payload_2.substr(0, quantum - payload_1.size());
    EXPECT_THAT(p, ElementsAre(ConstBuffer{payload_1}, ConstBuffer{trailer}));
    next_byte += TotalBytes(p);
    return make_status_or(ResumableUploadResponse{
        "", quantum - 1, {}, ResumableUploadResponse::kInProgress, {}});
  });
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t s,
                    HashValues const&) {
        ++count;
        EXPECT_EQ(2, count);
        auto expected = payload_2.substr(payload_2.size() - payload_1.size());
        EXPECT_THAT(p, ElementsAre(ConstBuffer{expected}));
        EXPECT_EQ(payload_1.size() + payload_2.size(), s);
        auto last_committed_byte = payload_1.size() + payload_2.size() - 1;
        return make_status_or(
            ResumableUploadResponse{"{}",
                                    last_committed_byte,
                                    {},
                                    ResumableUploadResponse::kInProgress,
                                    {}});
      });
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return next_byte;
  });

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  streambuf.sputn(payload_1.data(), payload_1.size());
  streambuf.sputn(payload_2.data(), payload_2.size());
  auto response = streambuf.Close();
  EXPECT_STATUS_OK(response);
}

/// @test Verify that a stream flushes when adding one character at a time.
TEST(ObjectWriteStreambufTest, OverflowFlushAtFullQuantum) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload(quantum, '*');

  int count = 0;
  std::size_t next_byte = 0;
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return next_byte;
  });
  bool mock_done = false;
  EXPECT_CALL(*mock, done).WillRepeatedly([&] { return mock_done; });
  EXPECT_CALL(*mock, UploadChunk).WillOnce([&](ConstBufferSequence const& p) {
    ++count;
    EXPECT_EQ(1, count);
    EXPECT_THAT(p, ElementsAre(ConstBuffer{payload}));
    next_byte += TotalBytes(p);
    return make_status_or(ResumableUploadResponse{
        "", next_byte - 1, {}, ResumableUploadResponse::kInProgress, {}});
  });
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t s,
                    HashValues const&) {
        ++count;
        EXPECT_EQ(2, count);
        std::string expected = " ";
        EXPECT_THAT(p, ElementsAre(ConstBuffer{expected}));
        next_byte += 1;
        EXPECT_EQ(next_byte, s);
        mock_done = true;
        return make_status_or(ResumableUploadResponse{
            "{}", next_byte - 1, {}, ResumableUploadResponse::kDone, {}});
      });

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  for (auto const& c : payload) {
    EXPECT_EQ(c, streambuf.sputc(c));
  }
  EXPECT_EQ(' ', streambuf.sputc(' '));
  auto response = streambuf.Close();
  EXPECT_STATUS_OK(response);
  EXPECT_EQ(ObjectWriteStreambuf::traits_type::eof(), streambuf.sputc(' '));
}

/// @test verify that bytes not accepted by GCS will be re-uploaded next Flush.
TEST(ObjectWriteStreambufTest, SomeBytesNotAccepted) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload = std::string(quantum - 2, '*') + "abcde";

  size_t next_byte = 0;
  EXPECT_CALL(*mock, UploadChunk).WillOnce([&](ConstBufferSequence const& p) {
    auto expected = payload.substr(0, quantum);
    EXPECT_THAT(p, ElementsAre(ConstBuffer{expected}));
    next_byte += quantum;
    return make_status_or(ResumableUploadResponse{
        "", next_byte - 1, {}, ResumableUploadResponse::kInProgress, {}});
  });
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t s,
                    HashValues const&) {
        auto const content = payload.substr(quantum);
        EXPECT_THAT(p, ElementsAre(ConstBuffer{content}));
        EXPECT_EQ(payload.size(), s);
        next_byte += content.size();
        return make_status_or(ResumableUploadResponse{
            "{}", next_byte - 1, {}, ResumableUploadResponse::kInProgress, {}});
      });
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return next_byte;
  });
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  std::ostream output(&streambuf);
  output << payload;
  auto response = streambuf.Close();
  EXPECT_STATUS_OK(response);
}

/// @test verify that the upload steam transitions to a bad state if the next
/// expected byte jumps.
TEST(ObjectWriteStreambufTest, NextExpectedByteJumpsAhead) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload = std::string(quantum * 2, '*');

  size_t next_byte = 0;
  EXPECT_CALL(*mock, UploadChunk).WillOnce([&](ConstBufferSequence const& p) {
    auto expected = payload.substr(0, 2 * quantum);
    EXPECT_THAT(p, ElementsAre(ConstBuffer{expected}));
    // Simulate a condition where the server reports more bytes committed
    // than expected
    next_byte += quantum * 3;
    return make_status_or(ResumableUploadResponse{
        "", next_byte - 1, {}, ResumableUploadResponse::kInProgress, {}});
  });
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return next_byte;
  });
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));
  std::string id = "id";
  EXPECT_CALL(*mock, session_id).WillOnce(ReturnRef(id));

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);
  std::ostream output(&streambuf);
  output << payload;
  EXPECT_FALSE(output.good());
  EXPECT_THAT(streambuf.last_status(), StatusIs(StatusCode::kAborted));
}

/// @test verify that the upload steam transitions to a bad state if the next
/// expected byte decreases.
TEST(ObjectWriteStreambufTest, NextExpectedByteDecreases) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload = std::string(quantum * 2, '*');

  auto next_byte = quantum;
  EXPECT_CALL(*mock, UploadChunk).WillOnce(InvokeWithoutArgs([&]() {
    next_byte--;
    return make_status_or(ResumableUploadResponse{
        "", next_byte - 1, {}, ResumableUploadResponse::kInProgress, {}});
  }));
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return next_byte;
  });
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));
  std::string id = "id";
  EXPECT_CALL(*mock, session_id).WillOnce(ReturnRef(id));

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);
  std::ostream output(&streambuf);
  output << payload;
  EXPECT_FALSE(output.good());
  EXPECT_THAT(streambuf.last_status(), StatusIs(StatusCode::kAborted));
}

/// @test Verify that a stream flushes when mixing operations that add one
/// character at a time and operations that add buffers.
TEST(ObjectWriteStreambufTest, MixPutcPutn) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload_1("header");
  std::string const payload_2(quantum, '*');

  int count = 0;
  size_t next_byte = 0;
  EXPECT_CALL(*mock, UploadChunk).WillOnce([&](ConstBufferSequence const& p) {
    ++count;
    EXPECT_EQ(1, count);
    auto expected = payload_2.substr(0, quantum - payload_1.size());
    EXPECT_THAT(p, ElementsAre(ConstBuffer{payload_1}, ConstBuffer{expected}));
    next_byte += TotalBytes(p);
    return make_status_or(ResumableUploadResponse{
        "", quantum - 1, {}, ResumableUploadResponse::kInProgress, {}});
  });
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t s,
                    HashValues const&) {
        ++count;
        EXPECT_EQ(2, count);
        auto expected = payload_2.substr(payload_2.size() - payload_1.size());
        EXPECT_THAT(p, ElementsAre(ConstBuffer{expected}));
        EXPECT_EQ(payload_1.size() + payload_2.size(), s);
        auto last_committed_byte = payload_1.size() + payload_2.size() - 1;
        return make_status_or(
            ResumableUploadResponse{"{}",
                                    last_committed_byte,
                                    {},
                                    ResumableUploadResponse::kInProgress,
                                    {}});
      });
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return next_byte;
  });

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  for (auto const& c : payload_1) {
    streambuf.sputc(c);
  }
  streambuf.sputn(payload_2.data(), payload_2.size());
  auto response = streambuf.Close();
  EXPECT_STATUS_OK(response);
}

/// @test Verify that a stream created for a finished upload starts out as
/// closed.
TEST(ObjectWriteStreambufTest, CreatedForFinalizedUpload) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(true));
  auto last_upload_response = make_status_or(ResumableUploadResponse{
      "url-for-test", 0, {}, ResumableUploadResponse::kDone, {}});
  EXPECT_CALL(*mock, last_response).WillOnce(ReturnRef(last_upload_response));

  ObjectWriteStreambuf streambuf(
      std::move(mock), UploadChunkRequest::kChunkSizeQuantum,
      CreateNullHashFunction(), HashValues{}, CreateNullHashValidator(),
      AutoFinalizeConfig::kEnabled);
  EXPECT_EQ(streambuf.IsOpen(), false);
  StatusOr<ResumableUploadResponse> close_result = streambuf.Close();
  ASSERT_STATUS_OK(close_result);
  EXPECT_EQ("url-for-test", close_result.value().upload_session_url);
}

/// @test Verify that last error status is accessible for small payload.
TEST(ObjectWriteStreambufTest, ErroneousStream) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload = "small test payload";

  int count = 0;
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t n,
                    HashValues const&) {
        ++count;
        EXPECT_EQ(1, count);
        EXPECT_THAT(p, ElementsAre(ConstBuffer{payload}));
        EXPECT_EQ(payload.size(), n);
        return Status(StatusCode::kInvalidArgument, "Invalid Argument");
      });
  EXPECT_CALL(*mock, next_expected_byte()).WillOnce(Return(0));

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  streambuf.sputn(payload.data(), payload.size());
  auto response = streambuf.Close();
  EXPECT_THAT(response, StatusIs(StatusCode::kInvalidArgument));
  EXPECT_THAT(streambuf.last_status(), StatusIs(StatusCode::kInvalidArgument));
}

/// @test Verify that last error status is accessible for large payloads.
TEST(ObjectWriteStreambufTest, ErrorInLargePayload) {
  auto mock =
      absl::make_unique<NiceMock<testing::MockResumableUploadSession>>();
  EXPECT_CALL(*mock, done).WillRepeatedly(Return(false));

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload_1(3 * quantum, '*');
  std::string const payload_2("trailer");
  std::string const session_id = "upload_id";

  ON_CALL(*mock, next_expected_byte()).WillByDefault(Return(0));
  EXPECT_CALL(*mock, UploadChunk).WillOnce([&](ConstBufferSequence const& p) {
    EXPECT_EQ(3 * quantum, TotalBytes(p));
    return Status(StatusCode::kInvalidArgument, "Invalid Argument");
  });
  EXPECT_CALL(*mock, session_id).WillOnce(ReturnRef(session_id));
  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  streambuf.sputn(payload_1.data(), payload_1.size());
  EXPECT_THAT(streambuf.last_status(), StatusIs(StatusCode::kInvalidArgument));
  EXPECT_EQ(streambuf.resumable_session_id(), session_id);

  streambuf.sputn(payload_2.data(), payload_2.size());
  EXPECT_THAT(streambuf.last_status(), StatusIs(StatusCode::kInvalidArgument));

  auto response = streambuf.Close();
  EXPECT_THAT(response, StatusIs(StatusCode::kInvalidArgument));
}

/// @test Verify that uploads of known size work.
TEST(ObjectWriteStreambufTest, KnownSizeUpload) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload(2 * quantum, '*');

  std::size_t mock_next_byte = 0;
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return mock_next_byte;
  });
  bool mock_is_done = false;
  EXPECT_CALL(*mock, done()).WillRepeatedly([&]() { return mock_is_done; });
  std::string const mock_session_id = "session-id";
  EXPECT_CALL(*mock, session_id()).WillRepeatedly(ReturnRef(mock_session_id));
  EXPECT_CALL(*mock, UploadChunk)
      .WillOnce([&](ConstBufferSequence const& p) {
        EXPECT_THAT(p, ElementsAre(ConstBuffer{payload}));
        mock_next_byte += TotalBytes(p);
        return make_status_or(ResumableUploadResponse{
            "", 2 * quantum - 1, {}, ResumableUploadResponse::kInProgress, {}});
      })
      .WillOnce([&](ConstBufferSequence const& p) {
        EXPECT_THAT(p, ElementsAre(ConstBuffer{payload}));
        mock_next_byte += TotalBytes(p);
        return make_status_or(ResumableUploadResponse{
            "", 4 * quantum - 1, {}, ResumableUploadResponse::kInProgress, {}});
      })
      .WillOnce([&](ConstBufferSequence const& p) {
        EXPECT_THAT(p, ElementsAre(ConstBuffer{payload.data(), quantum}));
        mock_next_byte += TotalBytes(p);
        // When using X-Upload-Content-Length GCS finalizes the upload when
        // enough data is sent, regardless of whether we use UploadChunk() or
        // UploadFinalChunk(). Furthermore the response has a last committed
        // byte of "0".
        mock_is_done = true;
        return make_status_or(ResumableUploadResponse{
            "", 0, {}, ResumableUploadResponse::kDone, {}});
      });

  ObjectWriteStreambuf streambuf(
      std::move(mock), quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  streambuf.sputn(payload.data(), 2 * quantum);
  streambuf.sputn(payload.data(), 2 * quantum);
  streambuf.sputn(payload.data(), quantum);
  EXPECT_EQ(5 * quantum, streambuf.next_expected_byte());
  EXPECT_FALSE(streambuf.IsOpen());
  EXPECT_STATUS_OK(streambuf.last_status());
  auto response = streambuf.Close();
  EXPECT_STATUS_OK(response);
}

/// @test Verify flushing partially full buffers works.
TEST(ObjectWriteStreambufTest, Pubsync) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  std::string const payload(quantum, '*');

  std::size_t mock_next_byte = 0;
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return mock_next_byte;
  });
  bool mock_is_done = false;
  EXPECT_CALL(*mock, done()).WillRepeatedly([&]() { return mock_is_done; });
  std::string const mock_session_id = "session-id";
  EXPECT_CALL(*mock, session_id()).WillRepeatedly(ReturnRef(mock_session_id));
  EXPECT_CALL(*mock, UploadChunk).WillOnce([&](ConstBufferSequence const& p) {
    EXPECT_THAT(p, ElementsAre(ConstBuffer{payload}));
    mock_next_byte += TotalBytes(p);
    return make_status_or(ResumableUploadResponse{
        "", mock_next_byte - 1, {}, ResumableUploadResponse::kInProgress, {}});
  });
  EXPECT_CALL(*mock, UploadFinalChunk)
      .WillOnce([&](ConstBufferSequence const& p, std::uint64_t,
                    HashValues const&) {
        EXPECT_THAT(p, ElementsAre(ConstBuffer{payload}));
        mock_next_byte += TotalBytes(p);
        mock_is_done = true;
        return make_status_or(ResumableUploadResponse{
            "", mock_next_byte - 1, {}, ResumableUploadResponse::kDone, {}});
      });

  ObjectWriteStreambuf streambuf(
      std::move(mock), 2 * quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  EXPECT_EQ(quantum, streambuf.sputn(payload.data(), quantum));
  EXPECT_EQ(0, streambuf.pubsync());
  EXPECT_EQ(quantum, streambuf.sputn(payload.data(), quantum));
  auto response = streambuf.Close();
  EXPECT_STATUS_OK(response);
}

/// @test Verify flushing too small a buffer does nothing.
TEST(ObjectWriteStreambufTest, PubsyncTooSmall) {
  auto mock = absl::make_unique<testing::MockResumableUploadSession>();

  auto const quantum = UploadChunkRequest::kChunkSizeQuantum;
  auto const half = quantum / 2;
  std::string const p0(half, '0');
  std::string const p1(half, '1');
  std::string const p2(half, '2');

  std::size_t mock_next_byte = 0;
  EXPECT_CALL(*mock, next_expected_byte()).WillRepeatedly([&]() {
    return mock_next_byte;
  });
  bool mock_is_done = false;
  EXPECT_CALL(*mock, done()).WillRepeatedly([&]() { return mock_is_done; });
  std::string const mock_session_id = "session-id";
  EXPECT_CALL(*mock, session_id()).WillRepeatedly(ReturnRef(mock_session_id));

  // Write some data and flush it, because there are no EXPECT_CALLS for
  // UploadChunk yet this fails if we flush too early.
  ObjectWriteStreambuf streambuf(
      std::move(mock), 2 * quantum, CreateNullHashFunction(), HashValues{},
      CreateNullHashValidator(), AutoFinalizeConfig::kEnabled);

  EXPECT_EQ(half, streambuf.sputn(p0.data(), half));
  EXPECT_EQ(0, streambuf.pubsync());
}

}  // namespace
}  // namespace internal
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace storage
}  // namespace cloud
}  // namespace google
