GCYD35 Gemini Reference Template (ESP32-3248S035C)

🚀 Mission: The End of the Expertise Gap
This project is a high-quality, fully configured base template for the popular ESP32-3248S035C (CYD 3.5-inch) board.
We believe human creativity is boundless, but the fear of technical complexity (the expertise gap) often prevents people from starting. This project's Mission is to prove that AI assistance eliminates this barrier. By leveraging AI to generate the daunting, low-level hardware configuration, we free creative minds to focus immediately on application logic and innovative UI design.

| Collaborative Creator | AI Generator | Board Model | Display/Touch |

| Marvin A Cabico (Developer & Systems Integrator) | Gemini | ESP32-3248S035C | ST7796 / GT911 |

✨ Key Features
•	100% Self-Contained: The GCYD35_Template.ino file includes all necessary configurations. No external .h files are required or used, making setup simple and reliable.
•	Dynamic Pinout: All hardware pins are defined using simple #define macros at the top, allowing users to easily adapt the template for boards with different pinouts without touching the complex driver code.
•	Touch Testing UI: Includes a fully functional touch demonstration with 5 responsive buttons (Left, Right, Top, Bottom, Clear) and an interactive draw-on-screen feature to test touch accuracy.
•	Clean Architecture: Built on the robust LovyanGFX library for high-speed graphics and reliable touch handling.

🛠️ Quick Start Guide
Follow these steps to get the template running on your ESP32-3248S035C.
1. Arduino IDE Setup
1.1.	Install ESP32 Board Core:
o	Open Boards Manager in the Arduino IDE.
o	Search for and install the esp32 board package by Espressif Systems.
1.2.	Install LovyanGFX Library:
o	Open Library Manager in the Arduino IDE.
o	Search for and install the LovyanGFX library.
2. Configure & Upload
2.1.	Copy the contents of GCYD35_Template.ino into a new Arduino sketch.
2.2.	Verify Pinout: Check the 1. PIN DEFINITIONS & CONFIGURATION section in the sketch header. These pins are correct for the standard ESP32-3248S035C board. If your board version differs, only these #define values need to be changed.
2.3.	Select the appropriate board (e.g., "ESP32 Dev Module" or similar) and COM port.
2.4.	Upload the sketch!

💻 Code Structure Philosophy
The template is divided into clear sections to guide beginners on where to make changes:
| Section | Purpose | User Modification |
| 1. PIN DEFINITIONS | Contains all simple #define pin numbers (e.g., TFT_CS, TOUCH_SDA). | Primary Edit Area (for custom pinouts) |
| 2. HARDWARE CONFIGURATION | Contains the complex class LGFX logic (the drivers and bus setup). | Do Not Modify (This is the pre-configured core) |
| BUTTON STRUCTURES | Defines the touch buttons' position and size. | Optional Edit Area (to customize UI layout) |
| setup() / loop() | Contains the initialization and touch/button logic. | Primary Development Area (to build your application) |
📄 License
This project is released under the MIT License.
This is the most permissive open-source license, allowing its free use, modification, and distribution. A separate LICENSE file containing the full text has been included in this repository.



