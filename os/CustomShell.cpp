
// this is question 1 of assign 3 of OS 
//L226551
//Ali Zain
//BCS 4D

#include<iostream>
#include<string>
#include<vector>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<errno.h>
#include<sstream>
#include<fcntl.h>

using namespace std;


//Keeping history as a global variable
vector<string> History;


int strlength(char arr[])
{
    int count = 0;
    for (int i = 0; arr[i] != '\0'; i++)
    {
        count++;
    }
    return count;

}
void strcopy(char destination[], char source[])
{
    //Deep copy.
    int i = 0;
    for (; source[i] != '\0'; i++)
    {
        destination[i] = source[i];
    }
    destination[i] = '\0';
}
void Display(char** str, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << str[i];
        cout << endl;
    }
}


int WordCount(char* str)
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ' ' || str[i + 1] == '\0')
        {
            count++;
        }
    }
    return count;
}
char** StringTokens(char* str)
{
    int l = strlength(str);
    int w = WordCount(str);
    char** Toks = new char* [w];
    char temp[20];
    int k = 0;
    int i = 0;
    for (; i < w; i++)
    {
        for (int j = 0; str[k] != '\0'; j++)
        {
            if (str[k] != ' ' && str[k] != '<' && str[k] != '>' && str[k] != '\0')
            {
                temp[j] = str[k];
                k++;
            }

            if (str[k] == '&')
            {
                temp[j + 1] = '\0';
                break;
            }

            else if (str[k] == '<')
            {
                temp[j + 1] = '\0';
                k++;

                while (str[k] != ' ' && str[k] != '>' && str[k] != '\0')
                {
                    temp[j++] = str[k];
                    k++;
                }
                temp[j] = '\0';
                int fd = open(temp, O_RDONLY, 0666);
                dup2(fd, 0);// stdin (0) is replaced by fd 

            }

            else if (str[k] == '>')
            {

                temp[j + 1] = '\0';
                k++;

                while (str[k] != ' ' && str[k] != '<' && str[k] != '\0')
                {
                    temp[j++] = str[k];
                    k++;
                }
                temp[j] = '\0';
                int fd = open(temp, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                dup2(fd, 1);// stdin (0) is replaced by fd 

            }
            else if (str[k] == ' ' || str[k] == '\0')
            {

                temp[j + 1] = '\0';
                k++;
                break;
            }


        }
        int s = strlength(temp) + 1;
        Toks[i] = new char[s];
        strcopy(Toks[i], temp);
    }
    Toks[i] = NULL;
    return Toks;
}


//this function converts the string to tokens and takes an arduments vector by refrence to store those tokens
void make_tokens(string& input, vector<char*>& Arguments) {
    string Token; // for storing the tokens 
    istringstream i(input);//for extracting tokkens from the input string
    //extract the tokken from the i and store in tokkens
    while (i >> Token) {
        // making the tokkens of the string to char*
        char* Args = strdup(Token.c_str());
        //pushing the copy to the vector 
        Arguments.push_back(Args);
    }
}

bool check_IN_FromFile(vector<char*>& Arg, string& filename)
{
    for (int i = 0; i < Arg.size(); i++)
    {
        if (strcmp(Arg[i], "<") == 0)
        {
            //getting the file name after the symbol is found
            filename = Arg[i + 1];
            // Removing both the symbols and the filename from list of arguments
            Arg.erase(Arg.begin() + i, Arg.begin() + i + 2);
            return true;
        }
    }
    return false;
}
bool check_OUT_ToFile(vector<char*>& Arg, string& filename)
{
    for (int i = 0; i < Arg.size(); i++)
    {
        if (strcmp(Arg[i], ">") == 0)
        {
            //getting the file name after the symbol is found
            filename = Arg[i + 1];
            // Removing both the symbols and the filename from list of arguments
            Arg.erase(Arg.begin() + i, Arg.begin() + i + 2);
            return true;
        }
    }
    return false;
}

bool checkFlag(vector<char*>& Arg)
{
    int s = Arg.size();
    int n = strlen(Arg[s - 1]);
    //if the last argument is & or not
    if (strcmp(Arg.back(), "&") == 0)
    {
        Arg.pop_back();
        cout << "Done[" << getpid() << "]\n";
        return true;
    }
    //making the last element to null
    else if (Arg[s - 1][n - 1] == '&')
    {
        Arg[s - 1][n - 1] = '\0';
    }
    return false;
}

void printHistory()
{
    if (History.size() >= 10)
    {
        int i = History.size() - 1;
        for (int j = 0; j < 10; j++, i--)
        {
            cout << "   " << i + 1 << ": " << History[i] << "\n";
        }
    }
    else
    {
        for (int i = History.size() - 1; i >= 0; i--)
        {
            cout << "   " << i + 1 << ": " << History[i] << endl;

        }
    }
}


bool check_pipes(vector<char*> Arg, vector<vector<char*>>& comm)
{
    vector<char*> com;
    //this loop will traverse the command and where it finds the pipe it stroes it in the com and pushes com to comm
    for (auto itr = Arg.begin(); itr != Arg.end(); itr++)
    {
        if (strcmp(*itr, "|") == 0)
        {
            comm.push_back(com);
            com.clear();
        }
        else
        {
            com.push_back(*itr);
        }
    }
    comm.push_back(com);

    // this check returns true if there is a pipe in the commands

    if (com.size() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }


}

int main()
{
    string input;

    while (1)
    {
        cout << "\nEnter the command $ ";
        getline(cin, input);
        //as we get the command as input we push it in the history vector for maintain  the history
        History.push_back(input);

        //we split the input string into vector of char* i.e arguments
        vector< char*> Arguments;
        make_tokens(input, Arguments);

        //checkif the last char entered was &
        bool check_ampercent = checkFlag(Arguments);

        //we will get the file name from the function if there is any identifire present
        string In_file = "";
        string out_file = "";

        // Checking if the command has a file for input.
        bool in_fromfile = check_IN_FromFile(Arguments, In_file);

        // Checking if the command has a file for output.
        bool out_toFile = check_OUT_ToFile(Arguments, out_file);


        // checkig is there any pipes in the arguments
        vector<vector<char*>> com;
        bool pipes_check = check_pipes(Arguments, com);

        // to check the if the user enter the cd command 
        if (strcmp(com[0][0], "cd") == 0)
        {
            chdir(com[0][1]);
            continue;
        }
        //check for exit
        else if (strcmp(com[0][0], "exit") == 0)
        {
            return 0;
        }
        //check to print history
        else if (strcmp(com[0][0], "history") == 0 || strcmp(com[0][0], "History") == 0)
        {
            printHistory();
            continue;
        }


        Arguments.push_back(NULL);
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        //child
        else if (pid == 0)
        {
            if (in_fromfile)
            {
                int in_fd = open(In_file.c_str(), 0);
                if (in_fd < 0)
                {
                    perror("open input");
                    exit(0);
                }
                dup2(in_fd, STDIN_FILENO);
            }
            if (out_toFile)
            {
                int out_fd = open(out_file.c_str(), O_WRONLY | O_CREAT, 0666); //opens the file if it is present else it will create the file 
                if (out_fd < 0)
                {
                    perror("open output file");
                    exit(0);
                }
                dup2(out_fd, STDOUT_FILENO);
            }
            // Create pipes for  communication b/w processes
            vector<int*> pipes(com.size() - 1);
            for (size_t i = 0; i < pipes.size(); ++i)
            {
                //this creates the pipes for each of the process  
                pipes[i] = new int[2];
                pipe(pipes[i]);
            }
            pid_t pid;
            for (size_t i = 0; i < com.size(); ++i)
            {
                pid = fork();
                if (pid < 0) {
                    perror("fork");
                    exit(1);
                }
                // Child process
                else if (pid == 0) {

                    //redirecting the input to the given file
                    if (i != 0) {
                        dup2(pipes[i - 1][0], STDIN_FILENO);
                        close(pipes[i - 1][0]);
                    }
                    //redirecting the output to the given file
                    if (i != com.size() - 1) {
                        dup2(pipes[i][1], STDOUT_FILENO);
                        close(pipes[i][1]);
                    }
                    //closing the pipes
                    for (size_t j = 0; j < pipes.size(); ++j) {
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }

                    int retval_exevp = execvp(com[i][0], com[i].data());
                    if (retval_exevp < 0)
                    {
                        perror("execvp");
                        exit(1);
                    }
                    exit(0);
                }
            }


            for (size_t i = 0; i < pipes.size(); ++i) {  // Closing pipes 
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            // Wait for all child processes to finish by checking the &
            if (!check_ampercent)
            {
                int status;
                while (wait(&status) > 0);
            }

        }
        else
        {
            if (!check_ampercent)
                wait(NULL);
        }


    }
    return 0;
}
