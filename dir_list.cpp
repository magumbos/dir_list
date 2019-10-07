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
//string dir_sample = "./SAMPLES/";
vector<string> sample_list;
vector<string> loop_list;
vector<string> transfer_list;
vector<string> sd_list;
string sample;
string ls;
int no_samples;
int counter;
int no_sd;
bool usb;
int sense;

bool mycomp(string a, string b){
	return a<b;
}

vector<string> alphabaticallySort(vector<string> a){
	//int n=a.size();
	sort(a.begin(),a.end(),mycomp);
	return a;
}

vector<string> load_wav(string location)
{
vector<string> a;
  
  if ((dir = opendir (location.c_str())) != NULL)
  {
    counter = 0;
    cout << "Open DIR "<< location.c_str() << endl;
    
     while((ent = readdir(dir)) != NULL)
     {
      sample = ent->d_name;
        if((sample.compare("..") > 0))
        {
         a.push_back(ent->d_name);
         no_samples++;
         counter++;
         //cout<<sample.c_str()<<endl;
        }
      }
      closedir(dir);
    }
  else  cout << "Error Opening DIR" << endl;
      
  return a;
}

string GetStdoutFromCommand(string cmd, bool command)
{
  string data;
  FILE* stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  
  
  if(command)
  {
    cmd.append(" 2>&1");
    stream = popen(cmd.c_str(), "r");
  }
  else
  stream = fopen(cmd.c_str(), "r");
  
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
  
  //string ls = GetStdoutFromCommand("ls -l /dev/disk/by-uuid/");
  ls = GetStdoutFromCommand("ls -l /dev/sd*[a-z]", true);
  cout << "LS: " << ls << endl;
  
  if(ls.length()>55)//then there must be more than one disk
  {
    usb = true;
    istringstream iss(ls);
    string s;
    while (getline(iss, s, ' ' ) ) {
      sd_list.push_back(s);
      no_sd++;
    }
    
    for(int i=0;i<no_sd;i+=11) cout<<sd_list[i]<<endl;
    
    if((dir = opendir ("/media/rpi_usb/")) != NULL)
    {
        closedir(dir);
        cout<<"DIR exists"<<endl;
    }
    else
    {
        ls = GetStdoutFromCommand("sudo mkdir /media/rpi_usb", true);
        cout << "mkdir: " << ls << endl;

        ls = GetStdoutFromCommand("sudo chown -R pi:pi /media/rpi_usb", true);
        cout << "chown: " << ls << endl;
    }
   
  ls = "sudo mount" + sd_list[11] + "/media/rpi_usb -o uid=pi,gid=pi";
  ls = GetStdoutFromCommand(ls, true); //assume that the first disk is the sd card and second is usb. Ignore everything else.
  cout << "mount: " << ls << endl;
  }
  
  if(usb) //load usbs first
  {
  sample_list = load_wav("/media/rpi_usb/SAMPLES");
  sample_list = alphabaticallySort(sample_list);
  loop_list = load_wav("/media/rpi_usb/LOOPS");
  sample_list = alphabaticallySort(loop_list);
  }
  
  transfer_list = load_wav("./SAMPLES");
  transfer_list = alphabaticallySort(transfer_list);
  for(int i = 0; i<counter; i++)
  {
    sample_list.push_back(transfer_list[i]);
  }
  
  
  loop_list = load_wav("./LOOPS");
  transfer_list = alphabaticallySort(transfer_list);
  for(int i = 0; i<counter; i++)
  {
    loop_list.push_back(transfer_list[i]);
  }
  
  cout << "Total Files Found:" << no_samples <<endl;
  
  //find sensitivity
  if(usb)ls = GetStdoutFromCommand("/media/rpi_usb/sense.txt", false);
  else ls = GetStdoutFromCommand("sense.txt", false);
  
  sense = stoi(ls);
  cout << "Sensitivity: " << sense << endl;
  
  while (keepRunning) {
    sleep(100);
   
  }


  return 0;
}
