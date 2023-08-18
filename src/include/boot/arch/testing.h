
/*
    Debugging tool. The argument passed will be written to ecx and ebx will be set to 0xcafebabe.
    Interrupts are disabled and the OS is halted

    @param test_num : The value which will be returned to ecx
*/
void __attribute__((cdecl)) test_work(uint32_t test_num);