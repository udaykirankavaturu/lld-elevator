#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

// Forward declarations
class Elevator;
class ElevatorManager;
class OuterPanel;

// Enums
enum class Direction { UP, DOWN };
enum class StateType { IDLE, MOVING_UP, MOVING_DOWN }; 

string getStateName(StateType state) {
    switch(state) {
        case StateType::IDLE: return "IDLE";
        case StateType::MOVING_UP: return "MOVING_UP";
        case StateType::MOVING_DOWN: return "MOVING_DOWN";
        default: return "UNKNOWN";
    }
}

// Observer Interface
class ElevatorObserver {
public:
    virtual void update(int floor, StateType state) = 0;
    virtual ~ElevatorObserver() = default;
};

// State Pattern
class State {
protected:
    Elevator* elevator;

public:
    explicit State(Elevator* e) : elevator(e) {}
    virtual void moveUp() = 0;
    virtual void moveDown() = 0;
    virtual void stop() = 0;
    virtual StateType getType() = 0;
    virtual ~State() = default;
};

// Forward declare concrete states
class IdleState;
class MovingUpState;
class MovingDownState;

// Core Elevator Class
class Elevator {
private:
    int id;
    int currentFloor;
    State* state;
    queue<int> floorQueue;
    ElevatorManager* manager;

public:
    Elevator(int id, ElevatorManager* mgr);
    ~Elevator() { delete state; }

    void setState(State* newState);
    void addToQueue(int floor);
    void processQueue();
    void simulateMovement(int targetFloor);
    
    int getCurrentFloor() const { return currentFloor; }
    int getId() const { return id; }
    State* getState() const { return state; }
};

// Strategy Pattern
class ElevatorSelectionStrategy {
public:
    virtual Elevator* selectElevator(int floor, Direction direction, const vector<Elevator*>& elevators) = 0;
    virtual ~ElevatorSelectionStrategy() = default;
};

class NearestElevatorStrategy : public ElevatorSelectionStrategy {
public:
    Elevator* selectElevator(int floor, Direction direction, const vector<Elevator*>& elevators) override {
        if (elevators.empty()) return nullptr;

        Elevator* nearestElevator = elevators[0];
        int shortestDistance = abs(floor - elevators[0]->getCurrentFloor());

        for (Elevator* elevator : elevators) {
            int distance = abs(floor - elevator->getCurrentFloor());
            StateType currentState = elevator->getState()->getType();

            // Prioritize idle elevators
            if (currentState == StateType::IDLE && distance < shortestDistance) {
                shortestDistance = distance;
                nearestElevator = elevator;
                continue;
            }

            // For moving elevators, consider their direction
            if (direction == Direction::UP && currentState == StateType::MOVING_UP && 
                elevator->getCurrentFloor() < floor && distance < shortestDistance) {
                shortestDistance = distance;
                nearestElevator = elevator;
            }
            else if (direction == Direction::DOWN && currentState == StateType::MOVING_DOWN && 
                     elevator->getCurrentFloor() > floor && distance < shortestDistance) {
                shortestDistance = distance;
                nearestElevator = elevator;
            }
        }

        return nearestElevator;
    }
};

// Concrete States
class IdleState : public State {
public:
    explicit IdleState(Elevator* e) : State(e) {}
    void moveUp() override;
    void moveDown() override;
    void stop() override {} // Already idle
    StateType getType() override { return StateType::IDLE; }
};

class MovingUpState : public State {
public:
    explicit MovingUpState(Elevator* e) : State(e) {}
    void moveUp() override {} // Continue moving up
    void moveDown() override;
    void stop() override;
    StateType getType() override { return StateType::MOVING_UP; }
};

class MovingDownState : public State {
public:
    explicit MovingDownState(Elevator* e) : State(e) {}
    void moveUp() override;
    void moveDown() override {} // Continue moving down
    void stop() override;
    StateType getType() override { return StateType::MOVING_DOWN; }
};

// Manager Class
class ElevatorManager {
private:
    vector<Elevator*> elevators;
    vector<OuterPanel*> panels;
    vector<ElevatorObserver*> observers;
    ElevatorSelectionStrategy* selectionStrategy;

public:
    ElevatorManager() : selectionStrategy(new NearestElevatorStrategy()) {
        cout << "Elevator Manager created\n";
    }

    ~ElevatorManager() {
        delete selectionStrategy;
    }

    void setSelectionStrategy(ElevatorSelectionStrategy* strategy) {
        delete selectionStrategy;
        selectionStrategy = strategy;
    }

    void addToQueue(int floor, Direction direction) {
        cout << "Request received for floor " << floor << "\n";
        Elevator* selectedElevator = selectionStrategy->selectElevator(floor, direction, elevators);
        if (selectedElevator) {
            selectedElevator->addToQueue(floor);
        }
    }

    void notifyObservers(int floor, StateType state) {
        for (auto observer : observers) {
            observer->update(floor, state);
        }
    }

    void addPanel(OuterPanel* panel);

    void addElevator(Elevator* elevator) {
        elevators.push_back(elevator);
    }
};

// Outer Panel Class
class OuterPanel : public ElevatorObserver {
private:
    int floor;
    ElevatorManager* manager;
    int currentDisplayFloor;
    Direction currentDirection;

public:
    OuterPanel(int floorNum, ElevatorManager* mgr) 
        : floor(floorNum), manager(mgr), currentDisplayFloor(1) {
        cout << "Panel created at floor " << floorNum << "\n";
    }

    void requestElevator(Direction direction) {
        cout << "Panel at floor " << floor << " requesting elevator\n";
        manager->addToQueue(floor, direction);
    }

    void update(int floor, StateType state) override {
        currentDisplayFloor = floor;
        cout << "Panel at floor " << this->floor << " updated: Elevator at floor " 
                 << floor << " (" << getStateName(state) << ")\n";
    }
};

// Elevator Implementation
Elevator::Elevator(int id, ElevatorManager* mgr) 
    : id(id), currentFloor(1), manager(mgr) {
    state = new IdleState(this);
    cout << "Elevator " << id << " created at floor " << currentFloor << "\n";
}

void Elevator::setState(State* newState) {
    delete state;
    state = newState;
    cout << "Elevator " << id << " changed state to " << getStateName(newState->getType()) << "\n";
}

void Elevator::addToQueue(int floor) {
    floorQueue.push(floor);
    cout << "Elevator " << id << " received request for floor " << floor << "\n";
    processQueue();
}

void Elevator::processQueue() {
    if (floorQueue.empty()) return;
    
    int targetFloor = floorQueue.front();
    cout << "Elevator " << id << " processing request for floor " << targetFloor << "\n";
    
    if (targetFloor > currentFloor) {
        state->moveUp();
        simulateMovement(targetFloor);
    } else if (targetFloor < currentFloor) {
        state->moveDown();
        simulateMovement(targetFloor);
    }
}

void Elevator::simulateMovement(int targetFloor) {
    while (currentFloor != targetFloor) {
        if (targetFloor > currentFloor) {
            currentFloor++;
        } else {
            currentFloor--;
        }
        cout << "Elevator " << id << " is now at floor " << currentFloor << "\n";
    }
    floorQueue.pop();
    state->stop();
}

// State Implementations
void IdleState::moveUp() {
    elevator->setState(new MovingUpState(elevator));
}

void IdleState::moveDown() {
    elevator->setState(new MovingDownState(elevator));
}

void MovingUpState::moveDown() {
    elevator->setState(new MovingDownState(elevator));
}

void MovingUpState::stop() {
    elevator->setState(new IdleState(elevator));
}

void MovingDownState::moveUp() {
    elevator->setState(new MovingUpState(elevator));
}

void MovingDownState::stop() {
    elevator->setState(new IdleState(elevator));
}

// Implement addPanel after OuterPanel is fully defined
void ElevatorManager::addPanel(OuterPanel* panel) {
    panels.push_back(panel);
    observers.push_back(panel);
}

int main() {
    cout << "Starting Elevator System Simulation\n";
    cout << "===================================\n\n";

    // Create manager
    ElevatorManager* manager = new ElevatorManager();

    // Create multiple elevators
    Elevator* elevator1 = new Elevator(1, manager);
    Elevator* elevator2 = new Elevator(2, manager);
    manager->addElevator(elevator1);
    manager->addElevator(elevator2);

    // Create panels for floors
    OuterPanel* panel1 = new OuterPanel(1, manager);
    OuterPanel* panel2 = new OuterPanel(2, manager);
    OuterPanel* panel3 = new OuterPanel(3, manager);
    manager->addPanel(panel1);
    manager->addPanel(panel2);
    manager->addPanel(panel3);

    cout << "\nSimulating elevator requests\n";
    cout << "===================================\n";

    // Simulate requests
    panel3->requestElevator(Direction::DOWN);  // Should select nearest elevator
    panel1->requestElevator(Direction::UP);    // Should select different elevator
    panel2->requestElevator(Direction::UP);    // Should select optimal elevator based on direction

    // Cleanup
    delete manager;
    delete elevator1;
    delete elevator2;
    delete panel1;
    delete panel2;
    delete panel3;

    return 0;
}