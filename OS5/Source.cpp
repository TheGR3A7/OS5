#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <queue>


using namespace std;

struct Process {
	int arrivalTime; // время появления
	int executionTime; // t - время выполнения
	int remainingTime; // оставшееся время для завершения процесса
	bool isForeground; // является ли процесс foreground или background
};

Process generateProcess(int max_arrivalTime, int max_executionTime) 
{
	Process proc;
	proc.arrivalTime = rand() % max_arrivalTime;
	proc.executionTime = rand() % max_executionTime + 1;
	proc.remainingTime = proc.executionTime;
	proc.isForeground = rand() % 2 == 0;
	return proc;
}

Process RR(queue<Process>& processes, int& currentTime, int quantTime)
{
	Process selectedProcess;
	if (!processes.empty()) 
	{
		selectedProcess = processes.front();
		processes.pop();
		if (selectedProcess.arrivalTime <= currentTime && selectedProcess.remainingTime > 0) 
		{
			if (selectedProcess.remainingTime > quantTime) 
			{
				currentTime += quantTime;
				selectedProcess.remainingTime -= quantTime;
				processes.push(selectedProcess); // процесс не завершен, поэтому возвращаем его обратно в очередь
			}
			else 
			{
				currentTime += selectedProcess.remainingTime;
				selectedProcess.remainingTime = 0;
			}
		}
	}
	return selectedProcess;
}

void runRR(queue<Process>& processes_RR, int num_processes, int max_executionTime) 
{
	int currentTime = 100;

	float total_T_RR = 0.0; // время пребывания
	float total_M_RR = 0.0; // потерянное время
	float total_R_RR = 0.0; // отношение реактивности
	float total_P_RR = 0.0; // штраф. время

	for (int i = 0; i < num_processes; i++) 
	{
		Process selectedProcess_RR = RR(processes_RR, currentTime, max_executionTime);
		float T_RR = currentTime - selectedProcess_RR.arrivalTime;
		float M_RR = T_RR - selectedProcess_RR.executionTime;
		float R_RR = static_cast<float>(selectedProcess_RR.executionTime) / T_RR;
		float P_RR = T_RR / selectedProcess_RR.executionTime;

		total_T_RR += T_RR;
		total_M_RR += M_RR;
		total_R_RR += R_RR;
		total_P_RR += P_RR;
	}

	float avg_T_RR = total_T_RR / num_processes;
	float avg_M_RR = total_M_RR / num_processes;
	float avg_R_RR = total_R_RR / num_processes;
	float avg_P_RR = total_P_RR / num_processes;

	cout << "RR - Среднее T: " << avg_T_RR << endl;
	cout << "RR - Среднее M: " << avg_M_RR << endl;
	cout << "RR - Среднее R: " << avg_R_RR << endl;
	cout << "RR - Среднее P: " << avg_P_RR << endl;
}

void runFB(vector<Process>& processes_FB, int num_processes, int max_executionTime) 
{
	int currentTime = 100;

	float total_T_FB = 0.0; // время пребывания
	float total_M_FB = 0.0; // потерянное время
 	float total_R_FB = 0.0; // отношение реактивности
	float total_P_FB = 0.0; // штраф. время

	queue<Process> foregroundQueue;
	queue<Process> backgroundQueue;

	for (const Process& proc : processes_FB) 
	{
		if (proc.isForeground) {
			foregroundQueue.push(proc);
		}
		else {
			backgroundQueue.push(proc);
		}
	}

	while (!foregroundQueue.empty() || !backgroundQueue.empty())
	{
		Process selectedProcess_FB;
		if (!foregroundQueue.empty()) {
			selectedProcess_FB = RR(foregroundQueue, currentTime, max_executionTime);
		}
		else if (!backgroundQueue.empty()) {
			selectedProcess_FB = RR(backgroundQueue, currentTime, max_executionTime);
		}

		float T_FB = currentTime - selectedProcess_FB.arrivalTime;
		float M_FB = T_FB - selectedProcess_FB.executionTime;
		float R_FB = static_cast<float>(selectedProcess_FB.executionTime) / T_FB;
		float P_FB = T_FB / selectedProcess_FB.executionTime;

		total_T_FB += T_FB;
		total_M_FB += M_FB;
		total_R_FB += R_FB;
		total_P_FB += P_FB;
	}

	float avg_T_FB = total_T_FB / num_processes;
	float avg_M_FB = total_M_FB / num_processes;
	float avg_R_FB = total_R_FB / num_processes;
	float avg_P_FB = total_P_FB / num_processes;

	cout << "FB - Среднее T: " << avg_T_FB << endl;
	cout << "FB - Среднее M: " << avg_M_FB << endl;
	cout << "FB - Среднее R: " << avg_R_FB << endl;
	cout << "FB - Среднее P: " << avg_P_FB << endl;
}

int main() 
{
	system("chcp 1251");

	srand(static_cast<unsigned>(time(nullptr)));

	int num_processes, max_executionTime;

	cout << "Введите количество процессов: ";
	cin >> num_processes;
	cout << "Введите ограничение на максимальное время выполнения каждого процесса: ";
	cin >> max_executionTime;

	vector<Process> processes_FB;
	queue<Process> processes_RR;

	for (int i = 0; i < num_processes; ++i) 
{
		Process p = generateProcess(100, max_executionTime);
		processes_FB.push_back(p);
		processes_RR.push(p);
	}

	cout << "Round Robin (RR) результаты: " << endl;
	runRR(processes_RR, num_processes, max_executionTime);

	cout << "\nForeground-Background (FB) результаты: " << endl;
	runFB(processes_FB, num_processes, max_executionTime);

	return 0;
}
