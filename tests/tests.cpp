#include "../metric.hpp"
#include <gtest/gtest.h>

TEST(MetricCollectorTest, MetricCollect) {
    MetricCollector collector;
    collector.addMetric<double>("Metric1");
    collector.addMetric<int>("Metric2");
    collector.addMetricValue("Metric1", 3.14);
    collector.addMetricValue("Metric2", 42);
    auto metrics = collector.getMetrics();
    ASSERT_EQ(metrics.size(), 2);
    EXPECT_EQ(metrics["Metric1"], "3.14");
    EXPECT_EQ(metrics["Metric2"], "42");
}

TEST(MetricCollectorTest, ResetMetrics) {
    MetricCollector collector;
    collector.addMetric<double>("Metric1");
    collector.addMetricValue("Metric1", 3.14);
    collector.resetMetrics();
    auto metrics = collector.getMetrics();
    EXPECT_EQ(metrics["Metric1"], "0");
}

TEST(MetricWriterTest, MetricWrite1) {
    MetricCollector collector;
    collector.addMetric<double>("Metric1");
    collector.addMetricValue("Metric1", 3.14);

    std::string filename = "test_metrics.log";
    {
        MetricWriter writer(collector, filename);
        std::this_thread::sleep_for(std::chrono::seconds(2)); 
    }

    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    std::string line;
    std::getline(file, line);
    EXPECT_FALSE(line.empty());
    file.close();
    std::remove(filename.c_str());
}

TEST(MetricWriterTest, MetricWrite2) {
    MetricCollector collector;
    collector.addMetric<double>("Metric1");
    collector.addMetricValue("Metric1", 3.14);

    std::string filename = "test_metrics.log";
    {
        MetricWriter writer(collector, filename);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    std::string line;
    std::getline(file, line);
    file.close();
    std::remove(filename.c_str());
    EXPECT_TRUE(line.find("\"Metric1\" 3.14") != std::string::npos);
}