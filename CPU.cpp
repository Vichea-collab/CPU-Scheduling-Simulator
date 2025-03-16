#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int id, arrivalTime, burstTime, waitingTime, turnaroundTime, completionTime, remainingTime, startTime;
};

void displayResults(const vector<Process>& processes) {
    double totalWaitingTime = 0, totalTurnaroundTime = 0;
    cout << "\nProcess\tAT\tBT\tWT\tTAT\n";
    for (const auto& p : processes) {
        cout << "P" << p.id << "\t" << p.arrivalTime << "\t" << p.burstTime << "\t"
             << p.waitingTime << "\t" << p.turnaroundTime << "\n";
        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
    }
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << totalWaitingTime / processes.size() << "\n";
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << totalTurnaroundTime / processes.size() << "\n";
}

void FCFS(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });
    int currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.arrivalTime)
            currentTime = p.arrivalTime;
        p.startTime = currentTime;
        p.waitingTime = currentTime - p.arrivalTime;
        p.turnaroundTime = p.waitingTime + p.burstTime;
        currentTime += p.burstTime;
        p.completionTime = currentTime;
    }
    displayResults(processes);
}

void SJF(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.burstTime < b.burstTime;
    });
    FCFS(processes);
}

void SRT(vector<Process>& processes) {
    int time = 0, completed = 0, n = processes.size();
    vector<bool> isCompleted(n, false);
    while (completed < n) {
        int minIdx = -1, minTime = 1e9;
        for (int i = 0; i < n; i++) {
            if (!isCompleted[i] && processes[i].arrivalTime <= time && processes[i].remainingTime < minTime) {
                minIdx = i;
                minTime = processes[i].remainingTime;
            }
        }
        if (minIdx == -1) {
            time++;
            continue;
        }
        processes[minIdx].remainingTime--;
        if (processes[minIdx].remainingTime == 0) {
            isCompleted[minIdx] = true;
            completed++;
            processes[minIdx].completionTime = time + 1;
            processes[minIdx].turnaroundTime = processes[minIdx].completionTime - processes[minIdx].arrivalTime;
            processes[minIdx].waitingTime = processes[minIdx].turnaroundTime - processes[minIdx].burstTime;
        }
        time++;
    }
    displayResults(processes);
}

void RoundRobin(vector<Process>& processes, int quantum) {
    queue<int> q;
    int time = 0, n = processes.size();
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime == 0)
            q.push(i);
    }
    while (!q.empty()) {
        int i = q.front(); q.pop();
        if (processes[i].remainingTime > quantum) {
            processes[i].remainingTime -= quantum;
            time += quantum;
            q.push(i);
        } else {
            time += processes[i].remainingTime;
            processes[i].remainingTime = 0;
            processes[i].completionTime = time;
            processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
            processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
        }
    }
    displayResults(processes);
}

int main() {
    int n, choice, quantum;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> processes(n);
    cout << "Enter Arrival Time and Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;
    }
    cout << "Choose Scheduling Algorithm:\n1. FCFS\n2. SJF\n3. SRT\n4. Round Robin\n5. Exit\n";
    cin >> choice;
    switch (choice) {
        case 1: FCFS(processes); break;
        case 2: SJF(processes); break;
        case 3: SRT(processes); break;
        case 4: cout << "Enter Time Quantum: "; cin >> quantum; RoundRobin(processes, quantum); break;
        case 5: return 0;
        default: cout << "Invalid Choice!"; return 1;
    }
    return 0;
}
