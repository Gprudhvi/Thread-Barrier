#include <iostream>
#include <thread>
#include<ctime>
#include<atomic>
#include<bits/stdc++.h>
#include<semaphore.h>
#include<chrono>
 

 using namespace std;
pthread_barrier_t mybarrier;
sem_t s,m;
int n;
int k;
int p=0;
int l=0;
double *avg;
double prex=1, postx=1;
const std::string currentDateTime() { //function to print the system time
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);

    return buf;
}
string timeconv(time_t tc)
{
	string te = currentDateTime();
	return te;
}

FILE *f;

 void test(int id)					// test CS function into which all threads are sent
  {
		  		time_t invtime,enttime,extime,cstime,remtime;
          int count;
          int BeforeBarrSleep;
          int AfterBarrSleep;
	  srand(prex);
          for(int i=0;i<k;i++)
          {
		
          	 BeforeBarrSleep=rand();
          	 time(&enttime);
          	 fprintf(f,"going to sleep before %d th barrier invocation by %d thread at time %s\n",i,id,timeconv(enttime).c_str());
          	 this_thread::sleep_for(chrono::milliseconds(BeforeBarrSleep%100));	
          	 time(&invtime);
          	 fprintf(f,"before %d barrier invocation by %d thread at time %s \n",i,id,timeconv(invtime).c_str());
          	 sem_wait(&m);		//exit section
          	 l++;
          	 count =l;
    		cout<<p++<<endl;
          	 sem_post(&m);
          	 if(count<n&& count>=0)
			{
          			sem_wait(&s);
          		}
          		 else if(count==n)
			{
          			sem_wait(&m);
          			 l=0;
          			 count =0;
          			 sem_post(&m);
          			 for (int i=0;i<n-1;i++)	sem_post(&s);
          		 }
          		time(&extime);	
			avg[id] =avg[id]+double(extime-enttime);

          	fprintf(f,"after %d barrier invocation by %d thread at time %s \n",i,id,timeconv(extime).c_str());

          	srand(postx);

          	 AfterBarrSleep=rand();
          	 time(&cstime);
          	 fprintf(f,"going to sleep after %d th barrier invocation by %d thread at time %s \n",i,id,timeconv(cstime).c_str());
          	 this_thread::sleep_for(chrono::milliseconds(AfterBarrSleep%100));
          	

             
          }
       }

       int main()
       {
	ifstream fp;
	fp.open("inp-params.txt");//taking input from file
  
       	fp>> n>>k>>prex>>postx;
       	thread t1[n];
       	sem_init(&s,1,1);
       	sem_init(&m,1,1);
	double sum;
	f = fopen("new-barr-log.txt", "w");
	avg = new double[n];
       	for (int i = 0; i < n; ++i)
          	{
             	t1[i] = thread(test,i);

          	}
	for (int i = 0; i < n; ++i)
        	 {
           	 	 t1[i].join();
         	 }
        fstream file;
	file.open("Average_time.txt",fstream::in|fstream::out|fstream::app);
	for(int i=0;i<n;i++)
	{
 		file<<"average time for" <<i<<"th  thread is "<<(avg[i]/k)<< endl;
	}
	for(int i=0;i<n;i++)
	{

		sum += avg[i];
	}
	file<< "waiting time for each new-barrier is " <<(double)(sum/(k*n))<<endl;
	file<< "***the end**"<<endl;
        }
