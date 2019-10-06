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
DIR *dir2;
dirent *ent;
string dir_sample = "./SAMPLES/";
vector<string> sample_list;
vector<string> sd_list;
string sample;
string ls;
int no_samples;
int no_sd;

bool mycomp(string a, string b){
	return a<b;
}

vector<string> alphabaticallySort(vector<string> a){
	//int n=a.size();
	sort(a.begin(),a.end(),mycomp);
	return a;
}

string GetStdoutFromCommand(string cmd)
{
  string data;
  FILE* stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  cmd.append(" 2>&1");
  
  stream = popen(cmd.c_str(), "r");
  if(stream)
  {
    while(!feof(stream))
      if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
  }
  return data;
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
      closedir(dir);
      
      sample_list = alphabaticallySort(sample_list);
      for(int i=0;i<no_samples;i++) cout<<sample_list[i]<<endl;      
      cout << "No. Samples:" << no_samples <<endl;
    
  }
  else  cout << "Error Opening DIR" << endl;

  //string ls = GetStdoutFromCommand("ls -l /dev/disk/by-uuid/");
  ls = GetStdoutFromCommand("ls -l /dev/sd*[a-z]");
  cout << "LS: " << ls << endl;
  
  if(ls.length()>50)
  {
    istringstream iss(ls);
    string s;
    while (getline(iss, s, ' ' ) ) {
      sd_list.push_back(s);
      no_sd++;
    }
    
    for(int i=0;i<no_sd;i++) cout<<sd_list[i]<<endl;
    
    if((dir2 = opendir ("/media/rpi_usb/")) != NULL)
    {
        closedir(dir2);
        cout<<"DIR exists"<<endl;
    }
    else
    {
        ls = GetStdoutFromCommand("sudo mkdir /media/rpi_usb");
        cout << "mkdir: " << ls << endl;

        ls = GetStdoutFromCommand("sudo chown -R pi:pi /media/rpi_usb");
        cout << "chown: " << ls << endl;
    }
   
  //ls = GetStdoutFromCommand("sudo mount /dev/sda5 /media/samples -o uid=pi,gid=pi");
  //cout << "mount: " << ls << endl;*/
  }
  
  
  

  while (keepRunning) {
    sleep(100);
   
  }


  return 0;
}
