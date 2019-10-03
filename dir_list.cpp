#include <iostream>
#include <signal.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <vector>
#include <bits/stdc++.h>

bool volatile keepRunning=true;

using namespace std;

DIR *dir;
dirent *ent;
string dir_sample = "./SAMPLES/";
vector<string> sample_list;
string sample;
int no_samples;

bool mycomp(string a, string b){
	return a<b;
}

vector<string> alphabaticallySort(vector<string> a){
	//int n=a.size();
	sort(a.begin(),a.end(),mycomp);
	return a;
}

int main(void) {
  
  cout << "Hello World" << endl;
  
  if ((dir = opendir (dir_sample.c_str())) != NULL)
  {
    cout << "Open DIR "<< dir_sample.c_str() << endl;
    
     while((ent = readdir(dir)) != NULL)
     {
      sample = ent->d_name;
        if((sample.compare("..") > 0))
        {
        
         sample_list.push_back(ent->d_name);
         no_samples++;
         //cout<<sample.c_str()<<endl;
        }
      }
      
      sample_list = alphabaticallySort(sample_list);
      for(int i=0;i<no_samples;i++) cout<<sample_list[i]<<endl;      
      cout << "No. Samples:" << no_samples <<endl;
    
  }
  else  cout << "Error Opening DIR" << endl;


  while (keepRunning) {
    sleep(100);
   
  }


  return 0;
}
