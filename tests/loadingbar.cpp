#include<iostream>
#include<cmath>
#include<chrono>
#include<thread>
#include<cstdio>
#include<vector>

//function for the loading bar needs to be housed in a while or for loop to operate with a couple 
//variable outside of the scope to handle progress
void LoadingBar(int currentProgress, int lastProgress, int currentUnit, int MaxUnit, int BarId, int numStatusBars){
  
   if(BarId < 4 &&  !(BarId < 0)){

    float  progressBarInc = 60 / MaxUnit;
    
    currentProgress += progressBarInc;

    if(currentUnit < MaxUnit){
        
        if(std::round(currentProgress) > lastProgress){
        
            lastProgress = std::round(currentProgress);

            std::cout << "\x1b[0G" << "\x1b[" <<  BarId << "A" << MaxUnit - currentUnit << "/" << MaxUnit << ":";
                std::fflush(stdout);
   
               int lastProgressTemp = lastProgress;
                while(lastProgressTemp--){
                     std::cout << "#";
   
                 }
                 std::cout << "\x1b[1B\x1b[0G";
        
        }
    
    }
   }

}




int main(int argc, char* argv[]){

    int generations = 20;
    int genMax = generations;

    int digitOffset = (generations < 10 ? 1 :
    (generations < 100 ? 2 :
    (generations < 1000 ? 3 :
    (generations < 10000 ? 4 :
    (generations < 100000 ? 5 :
    (generations < 1000000 ? 6 :
    (generations < 10000000 ? 7 :
    (generations < 100000000 ? 8 :
    (generations < 1000000000 ? 9 :
     10)))))))));

    float  progressBarInc = 60 / generations;

    float currProgress = 0;
    int lastProgress = 0;
    
    std::cout << "\n";


    while(generations--){
        //sleep to check that this is really working
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        currProgress += progressBarInc;
       

        if(std::round(currProgress) > lastProgress){
           
            lastProgress = std::round(currProgress);

             std::cout << "\x1b[0G" << "\x1b[1A" << genMax - generations << "/" << genMax << ":";
             std::fflush(stdout);
            
             int lastProgressTemp = lastProgress;
             while(lastProgressTemp--){
                 std::cout << "#";
            
             }
             std::cout << "\x1b[1B\x1b[0G";


        }   

    }


    return 0;
}

