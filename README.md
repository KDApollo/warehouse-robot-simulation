# ระบบจำลองหุ่นยนต์คลังสินค้า (Warehouse Robot Simulation)

## รายละเอียดโครงการ

Warehouse Robot Simulation เป็นโปรแกรมที่พัฒนาด้วยภาษา C++ เพื่อจำลองการทำงานของหุ่นยนต์หลายตัวภายในคลังสินค้า

ระบงงบประกอบด้วยหุ่นยนต์ 2 ประเภท ได้แก่ Picker และ Packer โดยหุ่นยนต์จะเคลื่อนที่ภายในคลังสินค้าเพื่อทำงานที่ได้รับมอบหมาย พร้อมจัดการพลัานแบตเตอรี่ และเดินทางกลับไปยังสถานีชาร์จเมื่อแบตเตอรี่เหลือน้อยหรือมีพลังงานไม่เพียงพอสำหรับการทำงาน

## ความสามารถของระบบ (Features)

- **หุ่นยนต์หลายตัว:** รองรับหุ่นยนต์ประเภท Picker และ Packer
- **การจัดการงาน:** รองรับงานประเภท Picking และ Packing
- **การเคลื่อนที่บน Grid:** หุ่นยนต์เคลื่อนที่ครั้งละ 1 ช่องต่อหนึ่งรอบเวลา
- **การมอบหมายงาน:** มอบหมายงานให้หุ่นยนต์ที่สามารถทำงานประเภทนั้นได้
- **การจัดการแบตเตอรี่:** แบตเตอรี่ลดลงเมื่อหุ่นยนต์เคลื่อนที่และทำงาน
- **ระบบชาร์จอัตโนมัติ:** หุ่นยนต์กลับไปยังสถานีชาร์จเมื่อแบตเตอรี่เหลือน้อยหรือไม่เพียงพอสำหรับงานถัดไป
- **การแสดงผลการจำลอง:** แสดงแผนผังคลังสินค้า สถานะหุ่นยนต์ และความคืบหน้าของงานในแต่ละรอบเวลา
- **โหมดการใช้งาน:** รองรับทั้ง Default Mode สำหรับการตั้งค่าเริ่มต้น และ Custom Mode สำหรับกำหนดค่าเอง

## ประเภทของหุ่นยนต์ (Robot Types)

| ประเภทหุ่นยนต์ | หน้าที่             |
| -------------- | ------------------- |
| Picker         | ทำงานประเภท Picking |
| Packer         | ทำงานประเภท Packing |

## ประเภทของงาน (Task Types)

| ประเภทงาน | ระยะเวลาการทำงาน |
| --------- | ---------------: |
| Picking   |        2 รอบเวลา |
| Packing   |        3 รอบเวลา |

## เทคโนโลยีที่ใช้ (Technologies)

- ภาษา C++
- การเขียนโปรแกรมเชิงวัตถุ (Object-Oriented Programming: OOP)
- Standard Template Library (STL)

## โครงสร้างโปรเจกต์ (Project Structure)

```text
warehouse-robot-simulation/
├── include/
│   ├── Robot.h
│   ├── Picker.h
│   ├── Packer.h
│   ├── Task.h
│   └── Warehouse.h
├── src/
│   ├── Robot.cpp
│   ├── Picker.cpp
│   ├── Packer.cpp
│   ├── Task.cpp
│   ├── Warehouse.cpp
│   └── main.cpp
├── tests/
│   ├── robot_test.cpp
│   └── task_test.cpp
└── README.md
```

## วิธีติดตั้งและเรียกใช้งาน (How to Compile and Run)

ติดตั้ง Compiler สำหรับภาษา C++ เช่น g++ ก่อนเริ่มใช้งาน

### 1. Compile โปรแกรมหลัก

เปิด Terminal ในโฟลเดอร์โปรเจกต์ แล้วใช้คำสั่ง:

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o warehouse.exe
```

### 2. เริ่มต้นโปรแกรมจำลอง

สำหรับ Windows ใช้คำสั่ง:

```powershell
.\warehouse.exe
```

## การทดสอบโปรแกรม (Unit Testing)

โปรเจกต์มีการทดสอบการทำงานแยกตามคลาส ได้แก่ Robot และ Task

### 1. Robot Unit Test

Compile:

```bash
g++ -std=c++17 -Iinclude tests/robot_test.cpp src/Robot.cpp src/Picker.cpp -o robot_test.exe
```

Run:

```powershell
.\robot_test.exe
```

### 2. Task Unit Test

Compile:

```bash
g++ -std=c++17 -Iinclude tests/task_test.cpp src/Task.cpp -o task_test.exe
```

Run:

```powershell
.\task_test.exe
```

## ผู้พัฒนา (Authors)

โครงการ Warehouse Robot Simulation

พัฒนาขึ้นเป็นส่วนหนึ่งของรายวิชา FRA143
