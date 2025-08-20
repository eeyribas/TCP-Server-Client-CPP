#include "communication.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h>

void Communication::Start(int core_index)
{
    th = std::thread(&Communication::Process,
                     this, core_index);
}

void Communication::Process(int core_index)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(core_index, &cpu);

    if (sched_setaffinity(syscall(SYS_gettid), sizeof(cpu), &cpu) == -1)
        std::cout << "Core Error!" << std::endl;

    for (;;) {
        std::lock_guard<std::mutex> grd(mtx);

        if (!tcp_init.GetThreadState()) {
            tcp_init.Init(2);
            std::cout << "Init" << std::endl;
        }

        if (tcp_init.GetConnStatus()) {
            tcp_init.Stop();
            tcp_init.SetThreadState(true);
            std::this_thread::sleep_for(std::chrono::microseconds(100));

            tcp_receiver.Receiver(2);
            tcp_sender.Sender(2);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
