# rtk-ir-tools

A suite of lightweight tools designed to initialize the Infrared (IR) emitter on Realtek/Dell cameras. This allows the IR hardware to be activated for use with face authentication software like Howdy.

These tools implement the specific 5-step UVC extension unit sequence required to toggle the hardware switch into IR mode.

## Components

The project provides three different ways to trigger the hardware:

* **rtk-ir-ctl**: A standalone command-line tool for manual initialization or scripting.
* **rtk-ir-hook.so**: An LD_PRELOAD hook that transparently initializes the IR hardware when an application attempts to access the camera.
* **pam_rtk_ir.so**: A PAM module designed to trigger the IR emitter during the authentication process.

## Installation

### Prerequisites

* A C compiler (gcc)
* make
* libpam headers (usually pam or libpam0g-dev)

### Build and Install

```bash
git clone https://github.com/mrwinux/rtk-ir-tools
cd rtk-ir-tools
make
sudo make install

```

By default, this installs to /usr/bin, /usr/lib, and /usr/lib/security.

## Integration and usage with Howdy

To ensure the IR emitter is active before Howdy begins face detection, you can use either the PAM module or the Library Hook.

### 1. Using the PAM Module

In all files returned by `grep -rl howdy /etc/pam.d`, add the following line before the one mentioning "howdy":

```text
auth optional pam_rtk_ir.so device=/dev/video0

```

This ensures that whenever Howdy is triggered, the IR emitter is toggled on first.

### 2. Using the Library Hook

This is useful for testing or for setup:

```bash
sudo LD_PRELOAD=/usr/lib/rtk-ir-hook.so howdy test

```

### 3. Standalone Tool

Use this to manually test if your hardware responds to the sequence:

```bash
sudo rtk-ir-ctl /dev/video0

```

## Acknowledgements

This project is based on the research conducted by SeeleVolleri: [https://github.com/SeeleVolleri/research_0jcxg0](https://github.com/SeeleVolleri/research_0jcxg0)
