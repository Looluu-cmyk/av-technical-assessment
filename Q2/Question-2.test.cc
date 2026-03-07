#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>
#include <sstream>

// We include the implementation file to get access to the classes.
// In a real project, you would have separate header files.
#include "Question-2.cc"

// Sample test case for SimpleTask processing
// TEST(SampleTest, SimpleTaskProcessing) {
//     float initial_value = 12.5f;
//     SimpleTask task(initial_value);

//     task.process();

//     float expected_value = 25.0f;
//     EXPECT_FLOAT_EQ(task.getProcessedValue(), expected_value);
//     EXPECT_EQ(task.getTaskType(), 0);
// }

TEST(SimpleTaskTests, SimpleTaskProcessing) {
    float initial_value = 12.5f;
    SimpleTask task(initial_value);

    task.process();

    float expected_value = 25.0f;
    EXPECT_FLOAT_EQ(task.getProcessedValue(), expected_value);
    EXPECT_EQ(task.getTaskType(), 0);
}

TEST(ComplexTaskTests, ComplexTaskProcessing) {
    std::vector<int> vec = {1, 2, 3, 4};
    ComplexTask task(vec);

    task.process();

    int expected_value = 10;
    EXPECT_FLOAT_EQ(task.getProcessedValue(), expected_value);
    EXPECT_EQ(task.getTaskType(), (0b01) << 6);
}

TEST(BitPackingTests, SimpleTask) {
    ThreadSafeQueue<std::unique_ptr<ITask>> processed_queue;
    processed_queue.push(std::make_unique<SimpleTask>(20.0f));

    std::atomic<bool> shutdown_flag{false};

    std::filesystem::path temp_dir;
    temp_dir = std::filesystem::temp_directory_path();
    std::filesystem::create_directory(temp_dir);
    std::filesystem::path temp_file_path = temp_dir / "output.txt";
    std::ofstream output_file(temp_file_path);
    ASSERT_TRUE(output_file.is_open());

    PacketTransmitter transmitter(processed_queue, shutdown_flag, output_file);
    std::thread transmitter_thread(&PacketTransmitter::run, &transmitter);

    auto now = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();

    std::string output = "Packet: 0x00 0x14 0x00 0x00 0x00 ";
    std::stringstream ss;
    for (int i = 0; i < 3; i++) {
        uint8_t bytes = (milliseconds >> (8 * (2 - i)));
        ss << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int) bytes << " ";
    }
    output += ss.str();

    shutdown_flag = true;

    transmitter_thread.join();
    output_file.close();

    std::ifstream input_file(temp_file_path);
    std::string content;
    std::getline(input_file, content);
    EXPECT_EQ(content, output);

    input_file.close();
}

// could potentially create a testing class which sets up temp_dir test environment
TEST(BitPackingTests, ComplexTask) {
    std::vector<int> v = {1, 2, 3, 4};
    ThreadSafeQueue<std::unique_ptr<ITask>> task_queue;
    task_queue.push(std::make_unique<ComplexTask>(std::move(v)));

    ThreadSafeQueue<std::unique_ptr<ITask>> processed_queue;

    std::atomic<bool> shutdown_flag{false};

    std::filesystem::path temp_dir;
    temp_dir = std::filesystem::temp_directory_path();
    std::filesystem::create_directory(temp_dir);
    std::filesystem::path temp_file_path = temp_dir / "output.txt";
    std::ofstream output_file(temp_file_path);
    ASSERT_TRUE(output_file.is_open());

    TaskProcessor processor(task_queue, processed_queue, shutdown_flag);
    PacketTransmitter transmitter(processed_queue, shutdown_flag, output_file);
    std::thread processor_thread(&TaskProcessor::run, &processor);
    std::thread transmitter_thread(&PacketTransmitter::run, &transmitter);

    auto now = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();

    std::string expected_output = "Packet: 0x40 0x0a 0x00 0x00 0x00 ";
    std::stringstream ss;
    for (int i = 0; i < 3; i++) {
        uint8_t bytes = (milliseconds >> (8 * (2 - i)));
        ss << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int) bytes << " ";
    }
    expected_output += ss.str();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    shutdown_flag = true;

    processor_thread.join();
    transmitter_thread.join();
    output_file.close();

    std::ifstream input_file(temp_file_path);
    std::string content;
    std::getline(input_file, content);
    EXPECT_EQ(content, expected_output);

    input_file.close();
}

TEST(BitPackingTests, ComplexTaskLong) {
    std::vector<int> v = {250000000, 250000000, 250000000, 250000000};
    ThreadSafeQueue<std::unique_ptr<ITask>> task_queue;
    task_queue.push(std::make_unique<ComplexTask>(std::move(v)));

    ThreadSafeQueue<std::unique_ptr<ITask>> processed_queue;

    std::atomic<bool> shutdown_flag{false};

    std::filesystem::path temp_dir;
    temp_dir = std::filesystem::temp_directory_path();
    std::filesystem::create_directory(temp_dir);
    std::filesystem::path temp_file_path = temp_dir / "output.txt";
    std::ofstream output_file(temp_file_path);
    ASSERT_TRUE(output_file.is_open());

    TaskProcessor processor(task_queue, processed_queue, shutdown_flag);
    PacketTransmitter transmitter(processed_queue, shutdown_flag, output_file);
    std::thread processor_thread(&TaskProcessor::run, &processor);
    std::thread transmitter_thread(&PacketTransmitter::run, &transmitter);

    auto now = std::chrono::system_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();

    std::string expected_output = "Packet: 0x40 0x00 0xca 0x9a 0x3b ";
    std::stringstream ss;
    for (int i = 0; i < 3; i++) {
        uint8_t bytes = (milliseconds >> (8 * (2 - i)));
        ss << "0x" << std::setw(2) << std::setfill('0') << std::hex << (int) bytes << " ";
    }
    expected_output += ss.str();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    shutdown_flag = true;

    processor_thread.join();
    transmitter_thread.join();
    output_file.close();

    std::ifstream input_file(temp_file_path);
    std::string content;
    std::getline(input_file, content);
    EXPECT_EQ(content, expected_output);

    input_file.close();
}
