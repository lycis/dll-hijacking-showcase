# DLL Hijacking Showcase 🔐

[![DLL Hijacking Showcase - Secure Development Practices](https://img.youtube.com/vi/PYG7ymUlPXA/0.jpg)](https://youtu.be/PYG7ymUlPXA)  
Click above to watch the full demo of DLL Hijacking and Secure Development Practices on YouTube!

This repository demonstrates a **DLL Hijacking attack** using a simple C++ application, along with progressive techniques to mitigate this vulnerability. By following the steps in this repository, you’ll understand how DLL Hijacking works, how an attacker can exploit this vulnerability, and how to implement secure development practices to reduce the risk of such attacks.

## 🔍 What is DLL Hijacking?

DLL Hijacking is a technique where an attacker tricks an application into loading a **malicious DLL** (Dynamic Link Library) in place of the legitimate one. This can happen when an application doesn’t securely specify where it expects its required DLLs to be loaded from, allowing an attacker to introduce malicious code.

### How it Works:

1. The application requests a DLL to be loaded.
2. Due to an insecure search path, the application looks in various directories, including insecure ones.
3. An attacker places a malicious DLL with the same name in a higher-priority directory.
4. The application loads the **malicious DLL** instead of the legitimate one, allowing the attacker to execute harmful code within the application’s context.

## 🗂️ Repository Overview

This repository contains the following:

- **00-vulnerable.cpp**: A vulnerable application that does not securely specify the DLL path, demonstrating how DLL hijacking can occur.
- **mylog.cpp**: The legitimate logging library (`mylog.dll`) that the application is supposed to use.
- **mylog-evil.cpp**: A malicious version of the logging library (`mylog.dll`) that demonstrates the DLL hijacking attack.
- **01-vulnerable_fullpath.cpp**: A more secure version of the application that loads the DLL from a specified full path, preventing DLL hijacking.
- **02-vulnerable_checksum.cpp**: The most secure implementation, which includes checksum validation to ensure the DLL hasn't been tampered with.
- **md5.cpp**: A utility to generate checksums for validating the integrity of the DLL.

## 🛠️ Requirements

This repository requires a Windows system and the following tools:

- **Clang** (`clang++`) with lld for linking
- **Make** (for using the provided Makefile)

These tools can be easily installed via **MSYS2**, a Unix-like environment for Windows. You can set it up by following these steps:

1. Download and install MSYS2 from [https://www.msys2.org](https://www.msys2.org).
2. Open the MSYS2 terminal and run:

    ```bash
    pacman -S mingw-w64-x86_64-clang mingw-w64-x86_64-make
    ```

Once installed, you’ll have both `clang++` and `make` available for compiling the examples.

> Note: This repository is designed to work on Windows only, as it demonstrates a Windows-specific vulnerability related to DLL handling.

## 🚀 How to Use This Repository

Follow the steps below to reproduce the DLL Hijacking attack and explore how to secure your application against it.

### Step 1: Compile the Vulnerable Application

First, compile the vulnerable application (`00-vulnerable.cpp`) that loads the legitimate DLL insecurely.

```bash
make vulnerable00
```

### Step 2: Compile the Legitimate Library

Next, compile the legitimate DLL (`mylog.cpp`), which the application is supposed to load.

```bash
make goodlib
```

### Step 3: Verify the Application

Run the compiled application to confirm that it works correctly with the legitimate `mylog.dll` library.

```bash
./vuln00.exe
```

The application should function as expected, logging data using the legitimate DLL.

### Step 4: Compile the Malicious Library

Now, compile the malicious version of the DLL (`mylog-evil.cpp`), which mimics the legitimate one but performs unauthorized actions.

```bash
make evillib
```

### Step 5: Replace the Legitimate DLL

Replace the legitimate `mylog.dll` with the malicious one, and run the vulnerable application again:

```bash
./vuln00.exe
```

Now you’ll see that the malicious DLL is loaded, and data is being stolen into `stolen_logs.txt`, demonstrating the DLL Hijacking attack.

### Step 6: Explore the Secure Implementations

The repository contains two progressively more secure versions of the application to prevent DLL hijacking:

1. **Full Path Loading**: In `01-vulnerable_fullpath.cpp`, the application loads the DLL from a secure, fully specified path, preventing the loading of arbitrary DLLs.

    To compile and run this version:

    ```bash
    make vulnerable01
    ./vuln01.exe
    ```

2. **Checksum Verification**: In `02-vulnerable_checksum.cpp`, the application verifies the DLL’s checksum before loading it, ensuring that the DLL hasn’t been tampered with.

    To compile and run this version:

    ```bash
    make vulnerable02
    ./vuln02.exe
    ```

By verifying the checksum, even if a malicious DLL is placed in the correct location, it will not be loaded unless its integrity matches the expected value.

## 📄 Makefile Targets

To simplify the build process, the provided **Makefile** offers the following targets:

- **`goodlib`**: Compiles the legitimate DLL (`mylog.dll`).
- **`evillib`**: Compiles the malicious DLL (`mylog.dll`).
- **`vulnerable00`**: Compiles the first vulnerable application (`vuln00.exe`).
- **`vulnerable01`**: Compiles the second, more secure application (`vuln01.exe`) using full path loading.
- **`vulnerable02`**: Compiles the most secure application (`vuln02.exe`) using checksum verification.
- **`md5sum`**: Compiles the MD5 checksum utility.
- **`clean`**: Cleans up compiled binaries, DLLs, and temporary files.

### Cleaning Up

To remove the compiled binaries and clean up the project, simply run:

```bash
make clean
```

## 🎓 Conclusion

This repository provides a practical demonstration of how DLL Hijacking can be exploited, along with secure coding practices to defend against it. By following the steps and using techniques like full path specification and checksum validation, you can greatly reduce the risk of such attacks in your own applications.

For a full walkthrough and explanation, be sure to check out the [YouTube video demonstration](https://youtu.be/PYG7ymUlPXA).

