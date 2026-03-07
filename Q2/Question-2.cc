#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <iomanip>
#include <atomic>
#include <numeric>
#include <mutex>
#include <queue>
#include <chrono>
#include <bitset>

template<typename T>
class ThreadSafeQueue {
private:
    // Implement this
    std::mutex mtx;
    std::queue<T> queue;
public:
    void push(T value) {
        // Implement this
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(value));
    }
    T pop() {
        // Implement this
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.size() != 0) {
            T ret = std::move(queue.front());
            queue.pop();
            return ret;
        } else {
            return nullptr;
        }
    }
    size_t size() {
        // Implement this
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }
    // A non-blocking pop for graceful shutdown
    T pop_for_shutdown() {
        // Implement this
        std::lock_guard<std::mutex> lock(mtx);
        while (!queue.empty()) {
            queue.pop();
        }
        return nullptr;
    }
};

// Class Representation of a Task
class ITask {
public:
    virtual ~ITask() = default;
    virtual void process() = 0;
    virtual float getProcessedValue() const = 0;
    virtual uint8_t getTaskType() const = 0;
};

class SimpleTask : public ITask {
private:
    // You can define the members as per your requirement
    float val_;
public:
    explicit SimpleTask(float val): val_(val) {}
    // Implement the necessary functions
    void process() override {
        val_ *= 2.0;
    }

    float getProcessedValue() const override {
        return val_;
    }

    uint8_t getTaskType() const override {
        return 0;
    }
};

class ComplexTask : public ITask {
private:
    // You can define the members as per your requirement
    std::vector<int> nums_;
    int val_;
public:
    explicit ComplexTask(std::vector<int> nums): nums_(nums) {}
    // Implement the necessary functions
    void process() override {
        val_ = std::accumulate(nums_.begin(), nums_.end(), 0);
    }

    float getProcessedValue() const override {
        return val_;
    }

    uint8_t getTaskType() const override {
        return 0b01 << 6;
    }
};


class TaskGenerator {
private:
    ThreadSafeQueue<std::unique_ptr<ITask>>& task_queue_;
    std::atomic<bool>& shutdown_;
public:
    TaskGenerator(ThreadSafeQueue<std::unique_ptr<ITask>>& queue, std::atomic<bool>& shutdown)
        : task_queue_(queue), shutdown_(shutdown) {}
    void run() {
        // Implement the task generation loop with a shutdown check
        task_queue_.push(std::make_unique<SimpleTask>(10.f));

        while (true) {
            if (shutdown_) {
                task_queue_.pop_for_shutdown();
                break;
            }
        }
    }
};

class TaskProcessor {
private:
    ThreadSafeQueue<std::unique_ptr<ITask>>& task_queue_;
    ThreadSafeQueue<std::unique_ptr<ITask>>& processed_queue_;
    std::atomic<bool>& shutdown_;
public:
    TaskProcessor(ThreadSafeQueue<std::unique_ptr<ITask>>& t_queue, ThreadSafeQueue<std::unique_ptr<ITask>>& p_queue, std::atomic<bool>& shutdown)
        : task_queue_(t_queue), processed_queue_(p_queue), shutdown_(shutdown) {}
    void run() {
        // Implement the data processing loop with a shutdown check
        while (true) {
            if (auto task = task_queue_.pop()) {
                task.get()->process();
                processed_queue_.push(std::move(task));
            }

            if (shutdown_) {
                task_queue_.pop_for_shutdown();
                processed_queue_.pop_for_shutdown();
                break;
            }
        }
    }
};

class PacketTransmitter {
private:
    ThreadSafeQueue<std::unique_ptr<ITask>>& processed_queue_;
    std::atomic<bool>& shutdown_;
    std::ostream& stream_;
public:
    PacketTransmitter(ThreadSafeQueue<std::unique_ptr<ITask>>& queue, std::atomic<bool>& shutdown, std::ostream& stream)
        : processed_queue_(queue), shutdown_(shutdown), stream_(stream) {}
    void run() {
        // Implement the data transmission (bitpacking) loop with a shutdown check
        while (true) {
            if (auto task = processed_queue_.pop()) {
                transmit(task, stream_);
            }

            if (shutdown_) {
                processed_queue_.pop_for_shutdown();
                break;
            }
        }
    }
    void transmit(const std::unique_ptr<ITask>& data, std::ostream& os) {
        uint8_t buffer[8] = {0};
        
        // Bitpacking logic
        // Implement the bitpacking logic here
        auto now = std::chrono::system_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count();

        buffer[0] = data.get()->getTaskType();

        float processed_value = data.get()->getProcessedValue();
        for (int i = 0; i < 4; i++) {
            buffer[i + 1] = (int) processed_value >> (8 * i);
        }

        for (int i = 0; i < 3; i++) {
            buffer[i + 5] = milliseconds >> (8 * (2 - i));
        }

        // Print the buffer in hex format for verification
        os << "Packet: ";
        for (int i = 0; i < 8; ++i) {
            os << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)buffer[i] << " ";
        }
        os << std::dec << std::endl;
    }
};

#ifndef Q2_BUILDING_TESTS
int main() {
    std::cout << "Starting the data generation pipeline" << std::endl;

    std::atomic<bool> shutdown_flag{false};

    ThreadSafeQueue<std::unique_ptr<ITask>> task_queue;
    ThreadSafeQueue<std::unique_ptr<ITask>> processed_queue;

    TaskGenerator generator(task_queue, shutdown_flag);
    TaskProcessor processor(task_queue, processed_queue, shutdown_flag);
    PacketTransmitter transmitter(processed_queue, shutdown_flag, std::cout);

    std::thread generator_thread(&TaskGenerator::run, &generator);
    std::thread processor_thread(&TaskProcessor::run, &processor);
    std::thread transmitter_thread(&PacketTransmitter::run, &transmitter);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    shutdown_flag = true;

    generator_thread.join();
    processor_thread.join();
    transmitter_thread.join();

    std::cout << "Data Gen pipeline Finished." << std::endl;

    return 0;
}
#endif
