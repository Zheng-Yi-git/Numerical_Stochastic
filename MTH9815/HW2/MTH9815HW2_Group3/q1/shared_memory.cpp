#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

int main()
{
    using namespace boost::interprocess;

    try
    {
        // Remove shared memory if it exists
        std::cout << "Writer: Removing existing shared memory (if any)." << std::endl;
        shared_memory_object::remove("SharedMemory");

        // Create a shared memory object
        std::cout << "Writer: Creating shared memory object." << std::endl;
        shared_memory_object shm(create_only, "SharedMemory", read_write);

        // Set size of shared memory
        std::cout << "Writer: Setting shared memory size to hold an integer." << std::endl;
        shm.truncate(sizeof(int));

        // Map the shared memory to this process
        std::cout << "Writer: Mapping shared memory to this process." << std::endl;
        mapped_region region(shm, read_write);

        // Get the address of the mapped region
        void *addr = region.get_address();
        std::cout << "Writer: Shared memory mapped at address " << addr << "." << std::endl;

        // Write an integer to the shared memory
        int *shared_data = static_cast<int *>(addr);
        *shared_data = 42; // Example integer to share
        std::cout << "Writer: Wrote value " << *shared_data << " to shared memory." << std::endl;

        // Wait to keep the shared memory alive
        std::cout << "Writer: Shared memory is ready for reading. Press Enter to exit..." << std::endl;
        std::cin.get();

        // Reading back the value to ensure consistency
        std::cout << "Writer: Reading back value from shared memory: " << *shared_data << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Writer Error: " << e.what() << std::endl;
        return 1;
    }

    // Remove shared memory
    std::cout << "Writer: Removing shared memory object." << std::endl;
    shared_memory_object::remove("SharedMemory");

    return 0;
}