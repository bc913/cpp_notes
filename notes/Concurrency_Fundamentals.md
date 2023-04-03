# Concurrency Fundamentals
Concurrency can be defined as `occurence of multiple tasks or operations or activities at the same time`.

## Terminology

### **Instruction**
`Instruction` is a single unit of operation to be executed by processor and defined by the processor instruction set.

### **Process**
It is an instance of a computer program in execution. Some specific components (i.e. stack, heap, data, text, etc) are reserved or dedicated for the process. 

### **Memory**
Memory is the hardware component that remembers(stores) the information. `Primary Memory` is the fast and the volatile one. `RAM` is considered as primary memory. `Secondary memory` is slower and non-volatile. `SSD` and `HDD` are considered as secondary memory and they are permanent storages. [2]

### **CPU**
`Central Processing Unit (CPU)` is the brain of the computer systems. It consists of microscopic transistors which are tiny switches that control the flow of electricity through the integrated circuits[1]. 

#### Components of CPU:
- Registers: Storage in the CPU
- Instruction Register:
- Instruction Counter:
- Arithmetic Logic Unit(ALU):
- Control Unit:

#### How does it work?
In general, CPU's main task is to process instructions provided by the processes. This achieved basically as follows[2]:
- Instructions for the current process are stored in the primary memory.
- `Instruction counter` (a register component of the CPU) keeps track of the address in `Primary Memory` for the current instruction to execute.
-  Then, CPU completes the following cycle to carry out a program:
	1. **Fetch**:
	2. **Decode**:
	3. **Execute**:
> This cycle is carried out millions of time per second.
- This cycle is performed with other components of CPU in sync.

### **Clock Speed**
It is the value measured for number of cycles your CPU executes per second, measured in GHz. During each cycle, billions of transistors within the processor open and close. This is how the CPU executes the calculations contained in the instructions it receives.[3]

Sometimes, multiple instructions are completed in a single clock cycle; in other cases, one instruction might be handled over multiple clock cycles. Since different CPU designs handle instructions differently, it’s best to compare clock speeds within the same CPU brand and generation.[3]

### **CPU Core**
Actually, the processing of the instructions is performed by a seperate physical unit inside the CPU chip, named `core`. Early CPUs had single core inside.

> NOTE: The clock speed is measured on `CPU` not `Core`

As the techonology advanced, the CPU and processor designs became more compact, heat, power consumption and other physical constraints started to limit maximum `clock speed` of processors and so chip manufacturers searched for other ways to increase the performance and efficieny of the chips. [4] Manufacturers has selected to provide multiple cores within CPU chip to increase efficiency and performance through parallelization instead of improving the clock speeds. That's why clock speeds for the older CPUs are better than the new ones.

`Core` is another physical unit within CPU with its own components like CPU. It acts as a CPU inside a CPU. It has the following benefits: [2] 
- Multiple cores allow for simultaneous execution of different programs. One can assign one core to one process and another core to another process.
- Multiple cores can be assigned to a single process but that depends on the process being written to take advantage of multiple cores.

## Why do we need concurrency?
Concurrency provide the opportunity to run multiple processes (and so different instructions) simulataneously at the higher level. This can be achieved by utilizing the multiple cores (manually or automatically with system).

## Types of concurrency


1. Multi - process
2. Multi-threading
3. Asynchrony (???)
4. Parallelism
5. Distributed data

## Resources
[[1] What is CPU? Meaning, Definition, and What CPU Stands For](https://www.freecodecamp.org/news/what-is-cpu-meaning-definition-and-what-cpu-stands-for/)

[[2] Stanford University - CS106E - Lecture #4-5: Computer hardware(Overview and CPUs)](https://web.stanford.edu/class/cs106e/lectureNotes/L04-5NHardwareCPU.pdf)

[[3] Intel: What is clock speed?](https://www.intel.com/content/www/us/en/gaming/resources/cpu-clock-speed.html#:~:text=The%20clock%20speed%20measures%20the,the%20processor%20open%20and%20close%20.)

[[4] Concurrency and Application Design](https://developer.apple.com/library/archive/documentation/General/Conceptual/ConcurrencyProgrammingGuide/ConcurrencyandApplicationDesign/ConcurrencyandApplicationDesign.html#//apple_ref/doc/uid/TP40008091-CH100-SW1)

[[5] Standofrd University: Principles of Computer systems](https://web.stanford.edu/class/cs110/summer-2021/)

[[6] Computer Organizations and Systems](https://web.stanford.edu/class/archive/cs/cs107/cs107.1216/lectures/)