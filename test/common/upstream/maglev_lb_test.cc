#include "common/upstream/maglev_lb.h"

#include "test/mocks/upstream/mocks.h"

namespace Envoy {
namespace Upstream {

// Note: ThreadAwareLoadBalancer base is heavily tested by RingHashLoadBalancerTest. Only basic
//       functionality is covered here.
class MaglevLoadBalancerTest : public ::testing::Test {
public:
  MaglevLoadBalancerTest() : stats_(ClusterInfoImpl::generateStats(stats_store_)) {}

  void init() {
    lb_.reset(new MaglevLoadBalancer(priority_set_, stats_, runtime_, random_, common_config_, 10));
    lb_->initialize();
  }

  NiceMock<MockPrioritySet> priority_set_;
  MockHostSet& host_set_ = *priority_set_.getMockHostSet(0);
  std::shared_ptr<MockClusterInfo> info_{new NiceMock<MockClusterInfo>()};
  Stats::IsolatedStoreImpl stats_store_;
  ClusterStats stats_;
  envoy::api::v2::Cluster::CommonLbConfig common_config_;
  NiceMock<Runtime::MockLoader> runtime_;
  NiceMock<Runtime::MockRandomGenerator> random_;
  std::unique_ptr<MaglevLoadBalancer> lb_;
};

// Works correctly without any hosts.
TEST_F(MaglevLoadBalancerTest, NoHost) {
  init();
  EXPECT_EQ(nullptr, lb_->factory()->create()->chooseHost(nullptr));
};

} // namespace Upstream
} // namespace Envoy
