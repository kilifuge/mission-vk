#include "metric.hpp"

std::map<std::string, std::string> MetricCollector::getMetrics() {
    std::unique_lock<std::mutex> lock(mtx);
    std::map<std::string, std::string> metric_copy;
    for (auto& pair : metrics)
        metric_copy[pair.first] = pair.second->getValue();
    return metric_copy;
}

void MetricCollector::resetMetrics() {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& pair : metrics)
        pair.second->reset();
}