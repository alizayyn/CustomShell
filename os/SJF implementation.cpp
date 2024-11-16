

// this is question 2 of assign 3 of OS 
//L226551
//Ali Zain
//BCS 4D


#include <iostream>
#include <queue>
#include <vector>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<errno.h>
#include<sstream>
#include<fcntl.h>
using namespace std;

//global variables

int id_counter = 0;
class Job
{
public:
    int duration;
    int ID;
    Job()
    {
        duration = 0;
        ID = ++id_counter;

    }
};

//this function compares the jobs on the basis of there duration and if the duration is same then it checkes the first come .
struct JobsCompare {
    bool operator()(Job first, Job second) const
    {

        if (first.duration != second.duration)
        {
            return first.duration > second.duration;
        }
        // If times are equal, compare IDs on which these jobs arrived
        return first.ID > second.ID;
    }
};


void Insert(priority_queue<Job, vector<Job>, JobsCompare>& que)
{
    cout << "Jobs:\n";
    int n;
    cout << "How many jobs do you have: ";
    cin >> n;
    int i = 0;
    for (; i < n; i++)
    {
        Job j;
        cout << "Job:" << i + 1 << " Duration = ";
        cin >> j.duration;
        que.push(j);
    }

}
void Remove(priority_queue<Job, vector<Job>, JobsCompare>& que, vector<int>& o)
{

    for (; !que.empty();)
    {
        cout << "Processing Job " << que.top().ID << " (Duration = " << que.top().duration << " units)\n";
        o.push_back(que.top().ID);
        que.pop();
    }
}

int main() {
    // Initialize an empty min-heap priority queue of Jobs.
    priority_queue<Job, vector<Job>, JobsCompare> que;



    cout << endl << endl << "--- Simulation of Shortest Job First (SJF) Scheduling Algorithm ---" << endl << endl;

    // Insert elements into the min-heap priority queue
    Insert(que);


    cout << endl << endl << "--- Starting SJF Scheduling Simulation ---" << endl << endl;

    vector<int> o; // this vector will store the order of the job in SJF manner
    //Removing the jobs from the priority queue and maintaing the order in the o vecotr
    Remove(que, o);


    cout << endl << endl << "--- SJF Scheduling Simulation Complete ---" << endl << endl;


    cout << "Job Processing Order: ";

    int i = 0;

    for (; i < o.size() - 1; i++)
    {
        cout << "Job " << o[i] << " -> ";
    }



    cout << "Job " << o[i] << endl;


    return 0;
}
