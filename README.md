# Multi-Format Image Sharpening on Blackfin ADSP-BF533

An optimized, low-level digital image processing pipeline implementing **Image Sharpening** algorithms on the **Analog Devices ADSP-BF533** Blackfin Digital Signal Processor (DSP). Developed and simulated using **VisualDSP++**, this project showcases embedded C engineering tailored for real-time performance constraints.

## 🚀 Key Features

* **Dual Color-Space Support:** Separate optimized pipelines for both **Grayscale (8-bit)** and **RGB (24-bit)** image formats.
* **Variable Kernel Sizes:** Supports spatial convolution using **3x3, 5x5, and 7x7** sharpening matrices (e.g., Laplacian / High-pass approximations).
* **High-Resolution Processing:** Scalable architecture capable of dynamic image allocation and handling resolutions **up to Full HD (1920x1080)**.
* **Hardware Optimization:** Fixed-point boundary clamping (saturation logic) to prevent integer overflow during intensive pixel accumulation on the Blackfin core.

---

## 🛠️ Hardware & Software Stack

* **Processor:** [Analog Devices ADSP-BF533](https://analog.com) (Blackfin Embedded Processor)
* **IDE / Toolchain:** [VisualDSP++](https://analog.com) (Compiler, Linker, and Cycle-Accurate Simulator)
* **Language:** Embedded C / Assembly-optimized functions for matrix operations

---

## 📂 Project Architecture & Filters

The system applies 2D spatial convolution across image buffers. For RGB images, the filtering is performed independently across the Red, Green, and Blue channels before recombining into the final output buffer.

### Supported Matrices:
* **3x3 Kernel:** Low computational overhead, ideal for micro-optimizations.
* **5x5 Kernel:** Balanced sharpening with mid-range high-frequency boost.
* **7x7 Kernel:** Aggressive high-pass filtering for complex texture extraction, utilizing loop unrolling and efficient memory strides.

---

## 📊 Performance & Testing Benchmarks

The project was validated using cycle-accurate DSP simulation across various setups:

| Format | Kernel Size | Max Supported Resolution | Memory Layout |
| :--- | :--- | :--- | :--- |
| Grayscale | 3x3, 5x5, 7x7 | **1920 x 1080 (Full HD)** | Internal L1 / External SDRAM |
| RGB (24-bit) | 3x3, 5x5, 7x7 | **1920 x 1080 (Full HD)** | Stratified Channels in External Memory |

---

## 🔧 How to Run

1. Open **VisualDSP++** and import the project configuration file (`.dpj`).
2. Set up the target session to the **ADSP-BF533 Simulator** or matching hardware board.
3. Configure your input pixel arrays (Grayscale/RGB bytes) up to Full HD dimensions.
4. Compile and execute (`F5`). Use VisualDSP++ memory mapping or plotting tools to verify the sharp output images.
