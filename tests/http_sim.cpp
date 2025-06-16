#include "../metric.hpp"
#include <random>
#include <limits>

int main() {
    MetricCollector collector;
    const std::string metric_name = "HTTP requests RPS";
    collector.addMetric<int>(metric_name);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> requests(0, std::numeric_limits<int>::max());
    
    std::string filename = "http_metrics.log";
    MetricWriter writer(collector, filename);
    
    std::cout << "Симуляция HTTP запросов, Ctrl+C для завершения" << std::endl;
    std::cout << "Метрики в файле: " << filename << std::endl;
    
    try {
        while (true) {
            int requests_count = requests(gen);
            collector.addMetricValue(metric_name, requests_count);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 