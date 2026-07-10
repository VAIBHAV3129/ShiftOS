Devlog 24
May 17, 2026

Took help from gemini to debug, and update a part of my code, and to fix some more pain messages, now even when all of these are fixed I am still left with a blank screen during the boot process, it is too frustrating at this point that I really feel like giving up on this stupid project....

image

Devlog 23
May 17, 2026

0.6h
Read countess documents, watched so many tutorials, fixed so many panic messages like

[config file not found]

Lower half PHDRs are not allowed

relocation truncated to fit

.limine_reqs entry outside kernel image

and yet they come again and again
<img width="900" height="603" alt="Screen Shot 2026-05-17 at 17 21 02 PM" src="https://github.com/user-attachments/assets/c56256ae-a5c8-4d76-b6f0-8b8bdd87c077" />
<img width="273" height="122" alt="Screen Shot 2026-05-17 at 16 45 32 PM" src="https://github.com/user-attachments/assets/d514a348-3904-4175-8b9a-40e4f621406b" />


Devlog 22
May 17, 2026

0.5h
Made sure that limine boots up inside QEMU ,but now ran into another issue, took help from GitHub copilot and began my work to fix it



Devlog 22
May 17, 2026

Running into limine issues, again and again and again.................

<img width="844" height="535" alt="Screen Shot 2026-05-17 at 13 43 34 PM" src="https://github.com/user-attachments/assets/53fd461b-5a1e-4c74-92ce-d81009e50646" />
<img width="1574" height="904" alt="Screen Shot 2026-05-17 at 13 54 40 PM png" src="https://github.com/user-attachments/assets/42d44672-4c5d-42af-9ff2-5931ddc1138a" />
<img width="1730" height="846" alt="Screen Shot 2026-05-17 at 13 54 46 PM png" src="https://github.com/user-attachments/assets/6b3ab09e-fe19-41f2-9d42-2264725b4dca" />
<img width="1754" height="914" alt="Screen Shot 2026-05-17 at 13 54 51 PM png" src="https://github.com/user-attachments/assets/c722e7da-0bf1-41f6-b939-e7be8cad1dfe" />


What a truly horrible time!!!

Devlog 21
May 17, 2026

The first step in my debugging is that I added a small on‑screen tick counter to confirm IRQ0/PIT is firing in QEMU.

Devlog 20
May 16, 2026

I was truly stuck, so I used Github Copilot for debugging and to know how to proceed further

I fixed a macOS inline-assembly constraint issue for the PIC IO wait, corrected the Limine kernel path to match the ISO boot directory, and added a full ISO build target so the bootloader can find the kernel consistently on macOS.

Now just keeping my fingers crossed hoping that everything might work as expected

Devlog 19
May 16, 2026

1h
Tried to run it from my terminal using QEMU, but boot failed , installed all the frameworks and dependencies


Looking into the issue now

Devlog 18

And ow I am super duper happy that he kernel now boots with a stable UI, has a working interrupt path for timing, and displays errors visually. The graphics stack supports text, glow, and window primitives, and the boot config is emulator‑friendly.

Devlog 17
May 16, 2026

0.5h
Fixed Limine bootloader request UUIDs so the base revision and framebuffer requests are properly recognized. added an IRQ stub in NASM, set up the IDT in C, and wired a PIT handler to increment a tick counter. I also Expanded the font system to cover ASCII 32–126, with a fallback glyph for out‑of‑range characters.

Updated Limine config to allow any resolution to improve emulator compatibility. Problems I ran into

Timer never advanced because g_ticks was never incremented (no hardware interrupt path existed). This caused animation timing loops to effectively lock. Bootloader requests weren’t parsed because the Limine UUIDs were zeroed out.

Added a proper IRQ0 handler stub and IDT initialization to route PIT interrupts. Wired PIT to fire at a stable frequency, advancing a global tick counter. Replaced the minimal font table with a standard ASCII 8×8 map. Made bootinfo tolerant of different framebuffer configs. Added a proper visual panic screen and forced CPU halt afterward.

Devlog 16
May 16, 2026

So yesterday, my first Maths exam was finally over, and today I am back to business, I continued with my code and today I added a basic window-drawing helper (frame, title bar, title text) and rendered two sample windows under the boot UI.

Devlog 15
May 15, 2026

Today , I added a minimal time module and text measurement helpers so the boot UI can be aligned correctly.

Devlog 14
May 14, 2026

I added a minimal bitmap font system and text renderer, then built a cyan pulsing ShiftOS title with a glowing halo and a loading indicator bar.

The boot screen is now animated and visually branded while staying fully freestanding.

Devlog 13
May 14, 2026

I added circle drawing and filled-circle stuff to the framebuffer layer and used them to render rounded UI accents in the demo scene.

Devlog 12
May 14, 2026

I added screen dimension helpers to the graphics layer so my drawings can scale to any framebuffer size. I also fixed a Makefile typo

Devlog 11
May 14, 2026

I added gradient and checkerboard helpers so the framebuffer can show richer test patterns and visual depth. This makes it so easier to validate resolution, color handling, and some future UI layering.

Devlog 10
May 13, 2026

I added a color utility layer with RGBA packing and alpha blending, then wired it into the graphics module.

Now I can draw translucent UI panels, which is a big step toward real window effects.

Devlog 9 - Updates
May 13, 2026

I added line drawing and outlined-rectangle to the framebuffer layer and used them to sketch a reallyy simple UI frames. This shall give me early wireframing tools that will later map directly to window borders and layout guides.

Devlog 8
May 12, 2026

I expanded the graphics layer with a filled-rectangle primitive and used it to draw test UI blocks on the framebuffer. This is the first step that I have taken toward real window rendering and layout work. And finally here comes my fav part.

Devlog 8
May 12, 2026

0.7h
Time spent on reading the Intel 64 and IA-32 Architectures Software Developer’s Manual for better clarity and understanding image

Devlog 7 Debugging
May 12, 2026

0.3h
It’s my huge attempt to build a modern, 64-bit operating system from the ground up, starting with a clean on the Limine protocol. image Today Debuging and rectifications of the errors and for improvement of the code workflow through C,

Devlog 6
May 11, 2026

0.3h
I added a boot info parser that reads Limine’s framebuffer response( after reading a lot and watching a few tutorials) and stores it in a clean struct for later graphics work. This gives me a stable foundation for the GUI pipeline without touching any host libraries.

Really excited to continue with the rest of my code, but unfortunately I have my board exams coming up, and I have to concentrate on it, leaving me no time to read, nevertheless, I at least hope to keep my streak active

Here is the code snippet of bootinfo.c image

Devlog 5 - Debugging and Research
May 10, 2026

0.7h
I now realise what a tedious job it is to create an OS from scratch, I have been. running into a lot of issues and my head spins when I think about all the work ahead It is so tempting to give up,, but I am determined to finish the task that I started. Researched and watched some video tutorials, but only then I realised that most of them were for the 32 bit and not the 64 bit, the unnecessary venture that I myself dumped upon my head.



Devlog 4
May 10, 2026

I set up the Limine request structures and wired them into the linker so the bootloader can discover our kernel’s boot requests. So now the Makefile now compiles the Limine request module and preserves .

limine_reqs
during linking to keep boot protocol data intact.

This is the cose snippet from limine.h image

Devlog 3 - Research and Design
May 10, 2026

2h
Spent significant time studying the Limine Boot Protocol and the x86_64 ABI. Specifically focused on how to maintain stack alignment for 64-bit function calls. Finallyy I Configured the x86_64-elf-gcc toolchain on my Mac mini M4. It was a huuuge headache ensuring the linker correctly utilized the kernel section boundaries for future memory safety.

now currently the kernel boots, enters a safe halt loop, and the build system is automated. And I am ready for the next phase which is GDT and CPU Exception Handling.


The Image is the code snippet of the makefile

Devlog 2
May 10, 2026

I wired in the initial Limine boot configuration and added an ISO staging target without breaking the kernel build. The project now produces a clean ELF and prepares the boot directory layout needed for Limine. Ran into some issues with the boot configuration , so I took help from Github Copilot for debugging, but other wise all the code was written by me only. Makefile remains freestanding and now works alongside linker-defined kernel layout symbols, keeping the build stable and deterministic.


Devlog 1
May 10, 2026

I first initialized the ShiftOS repo structure and wired a working cross-compile build for a freestanding x86_64 kernel. I also added a tiny NASM entry stub and a C kernel entry point so the build produces a clean ELF target
