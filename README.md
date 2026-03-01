# AV Technical

### **Submission Date**: 01/01/2026 11:59PM
### **Submission Instructions**: Look at the Submission sub-heading below

An important engineering skill is to be able to fix and resolve issues that pop up through research (i.e googling and experimenting). The AV task will test your ability to read, debug and resolve C++ compile time errors as well as runtime errors.

You do not need prior or extensive knowledge of C++ to complete this task. This task will test how well you can figure out issues even if you've never seen them or never worked with them before.

# Problems

There are 3 questions attached. Each one is more increasingly difficult than the other. Question 3 is the extended task for those particularly keen on pushing and demonstrating their ability. Remember to read each question carefully and understand what is required.

Please use CMake to compile (do not use g++ or any other compilers). Attached are some useful resources which should help you start (but you can figure a lot of this out by just googling).

Each question has a description of the task in its C++ or in a README.md. Please reach out to the AV team with any questions.

# **Submission**
For submission please zip the entire folder (but exclude build files) and upload it using the link below. The form will close at 11:59PM on the 7th of March 2026. Late submissions will not be accepted.

## **Submission Link:** TODO

# How to Compile

To compile with CMake create a build folder using

`mkdir build`

and then run CMake

`cmake CMakeLists.txt -B build/`

This will generate a bunch of files in the build folder. Just change directory into the build folder using

`cd build`

and then compile using make

`make`

This should generate a bunch of executables `Question-1-out`, `Question-2-out`, `Question-3-out`, upon success.


# How to Submit
Zip the file, and upload using the google form above. Please avoid including build files

# Some useful resources

https://www.learncpp.com/ (Use this as a resource for this unfamiliar with C++)

https://en.cppreference.com/w/cpp/language

Intro to git: https://opensource.com/article/18/1/step-step-guide-git

Introduction to CMake: https://cliutils.gitlab.io/modern-cmake/ (there is a section on Googletests as well)

Basic Cmake tutorial (This is all you should really need for cmake): https://matgomes.com/cmake-add-executable/

Synchronization in C++: https://medium.com/@AlexanderObregon/concurrency-and-multithreading-in-c-5ede6aa06241

