# Elevator System Simulation

## Overview

This project demonstrates a **Low-Level Design (LLD)** of an elevator system using core **Object-Oriented Programming (OOP)** principles and **Design Patterns**. The system models real-world interactions between elevators, panels, and users while showcasing best practices in software design and state management.

---

## Features

### Functional Features

- Simulates multiple elevators.
- Handles user requests for elevators from floor panels.
- Optimally selects elevators based on the nearest available one and current direction.
- Updates floor panels with real-time elevator status.

### Design Features

- **State Management**: Dynamically transitions between states like Idle, Moving Up, and Moving Down.
- **Flexible Elevator Selection**: Encapsulates the selection logic in the `NearestElevatorStrategy`.
- **Observer Notifications**: Floor panels automatically update their displays when elevators move.

---

## Code Structure

### Classes and Responsibilities

#### Core Classes

- **Elevator**: Represents an individual elevator and manages its state, movement, and floor queue.
- **ElevatorManager**: Central controller that manages multiple elevators and panels.
- **OuterPanel**: Represents floor panels where users can request elevators.

#### Patterns in Action

- **State Pattern**

  - **`IdleState`**: Represents an elevator that is not moving.
  - **`MovingUpState`**: Represents an elevator moving upwards.
  - **`MovingDownState`**: Represents an elevator moving downwards.

- **Strategy Pattern**

  - **`ElevatorSelectionStrategy`**: Abstract class for elevator selection logic.
  - **`NearestElevatorStrategy`**: Concrete implementation that selects the nearest suitable elevator.

- **Observer Pattern**
  - **`ElevatorObserver`**: Interface for classes that need to observe elevator state changes.

---

## How to Use

1. **Run the Simulation**

   - Compile the code using a C++ compiler:
     ```bash
     g++ -std=c++14 main.cpp -o main
     ```
   - Execute the program:
     ```bash
     ./main
     ```

2. **Observe the Output**
   - The simulation will demonstrate the interactions between elevators, panels, and the manager.
   - Example output:
     - Elevator movements between floors.
     - Floor panel updates based on elevator status.

---

## Key Learnings

### Design Patterns in Practice

1. **State Pattern**

   - Simplifies state transitions for elevators.
   - Promotes the **Single Responsibility Principle** (SRP) by separating state-specific behaviors.

2. **Strategy Pattern**

   - Encapsulates the logic for selecting the best elevator.
   - Adheres to the **Open/Closed Principle** (OCP) by allowing new strategies without modifying existing code.

3. **Observer Pattern**
   - Decouples panels from elevators, ensuring **low coupling** and **high cohesion**.

---

## Possible Extensions

- Add support for multiple users requesting elevators simultaneously.
- Extend the strategy pattern to support advanced selection criteria (e.g., load balancing).
- Introduce priority-based requests (e.g., VIP floors).
- Add maintenance modes, emergency stops.

---

## Conclusion

This project provides a comprehensive example of designing a simple elevator system. It serves as a foundation for understanding **design patterns**, **state management**, and **OOP principles** in the context of LLD. By studying and extending this implementation, developers can enhance their design skills and gain confidence in applying patterns to solve real-world problems.
