#include "metric.hpp"


std::string MetricWriter::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&currentTime);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void MetricWriter::writer() {
    std::ofstream file(filename, std::ios::app);
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto metrics = collector.getMetrics();
        if (!metrics.empty()) {
            std::string line = getCurrentTime() + " ";
            for (const auto& pair : metrics)
                line += "\"" + pair.first + "\" " + pair.second + " ";
            file << line << std::endl;
            collector.resetMetrics();
        }
    }
}
