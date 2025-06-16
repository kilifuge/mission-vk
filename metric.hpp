#ifndef METRIC_H
#define METRIC_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <mutex>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>

class MetricBase {
public:
    virtual ~MetricBase() = default;
    virtual std::string getName() const = 0;
    virtual std::string getValue() const = 0;
    virtual void reset() = 0;
};

template <typename T>
class SomeMetric : public MetricBase {
    std::string name{};
    T value{};

public:
    SomeMetric(std::string name = "Metric Name") : name{name}, value(T())
        { }

    std::string getName() const override
        {return name;}

    std::string getValue() const override { //значение возвращается в виде строки
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    void reset() override
        {value = T();}

    void setValue(const T& val) 
        {value = val;}
};

// для сбора метрик
class MetricCollector {
private:
    std::map<std::string, std::unique_ptr<MetricBase>> metrics;
    std::mutex mtx;

public:
    template<typename T>
    void addMetric(const std::string& name) {
        std::unique_lock<std::mutex> lock(mtx);
        metrics[name] = std::make_unique<SomeMetric<T>>(name);
    }

    template<typename T>
    void addMetricValue(const std::string& name, const T& val) {
        std::unique_lock<std::mutex> lock(mtx);
        if (metrics.find(name) != metrics.end()) {
            if (auto* metric = dynamic_cast<SomeMetric<T>*>(metrics[name].get()))
                metric->setValue(val);
        }       
    }

    //возвращает мапу с "название метрики" - "нынешнее значение"
    std::map<std::string, std::string> getMetrics();
    void resetMetrics();
};

//для записи метрик в файл
class MetricWriter {
    MetricCollector& collector;
    std::string filename;
    std::thread th;
    bool running;

    std::string getCurrentTime() const;
    void writer();

public:
    //запускает поток, в котором каждую секунды выписывает метрики в файл, пока не выйдет за пределы видимости
    MetricWriter(MetricCollector& collector, const std::string& filename)
        : collector(collector), filename(filename), running(true) 
        {th = std::thread(&MetricWriter::writer, this);}

    ~MetricWriter() {
        running = false;
        th.join();
    }
};

#endif