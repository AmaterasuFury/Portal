# 🌀 Portal-Inspired UE5 Prototype (Work in Progress)

A **personal Unreal Engine 5 project** replicating and expanding on the core mechanics of the game *Portal*. Built in C++ to emphasize performance, modularity, and gameplay logic, this prototype demonstrates a deep understanding of both **Unreal Engine internals** and **physics-based interaction systems**.

> **Focus**: Game systems architecture, C++ programming, real-time interaction mechanics

---

## 🎯 Core Features

### 🔫 Portal Gun System (C++)
- Fully functional **portal spawning** system using raycast surface detection.
- Supports **projectile-based targeting**, with correct portal placement logic.
- Planned: **teleportation logic** with **real-time camera warping** and seamless actor traversal.

### 🧱 Interactable Object Mechanics
- **Item pickup/carry system** implemented via a flexible **base class**.
- Reusable for interactables like **companion cubes**, **turrets**, and future puzzle elements.

### 🧩 Interaction Framework
- Created a **universal interaction interface** using Unreal's `UInterface`.
- Scales easily across gameplay systems, enabling interaction with doors, switches, and objects without duplicating logic.

---

## 🧠 Environment Systems & Gameplay Logic

### 🛡️ Turret AI
- Autonomous turrets with:
  - **Field-of-view detection** using **Dot Product vector math**
  - **Line-of-sight checks** and **reactive firing**
  - Modular blueprint/C++ architecture for expandability

### 🚪 Triggered Doors
- Built with **collision-based activators** (e.g., pressure plates).
- Event-driven door system with condition/state control.

### 🌉 Energy Bridge Generator
- Modular blueprint/C++ system to create **navigable energy bridges**.
- Dynamically forms bridges based on user-defined endpoints and conditions.

---

## 🚀 Technologies & Tools

- **Unreal Engine 5.1+**
- **C++** (Primary)
- **Blueprints** (for quick visual prototyping)
- **Vector Math / Dot Product** (for AI + targeting logic)
- **Actor Interfaces & Inheritance**
- **Line Trace / Physics Handles / Components**

---

## 🎮 Project Goals

- Reinforce Unreal C++ fundamentals through hands-on system replication.
- Build a **fully playable prototype** inspired by Portal's mechanics.
- Focus on **clean architecture**, **performance**, and **expandability**.
- Later phases (Planned):
  - Real-time portal rendering (camera warping)
  - Object physics across portals
  - Full level with puzzle elements

---

Status: *In progress.*

---

## 📝 Notes

> This is a **solo project** focused on replicating complex gameplay mechanics from the ground up using Unreal Engine and C++. It is a passion project intended to challenge and grow my knowledge of gameplay architecture, vector math, and game physics.

---

## ✅ Recruiter Summary

- Engineered gameplay mechanics from scratch with clean, reusable C++ architecture.
- Demonstrated understanding of Unreal Engine’s physics, input, tracing, component systems, and AI logic.
- Developed scalable gameplay systems (e.g. interaction interface, bridge generator).
- Strong grasp of math in gameplay (angle-based detection, collision, projection).
