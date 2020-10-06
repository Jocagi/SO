#include <thread>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include <sys/prctl.h>
//LINUX
class kProcess {
public:
    void sleep_for(const std::string &tname, int num)
    {
        prctl(PR_SET_NAME,tname.c_str(),0,0,0);        
        sleep(num);
    }


    void start_thread(const std::string &tname)
    {
        std::thread * thrd = new std::thread(&kProcess::sleep_for, this, tname, 3600);
        tm_[tname] = thrd->native_handle();
        thrd->detach();
        std::cout << "Thread " << tname << " created:" << std::endl;
    }


    void stop_thread(const std::string &tname)
    {
        ThreadMap::const_iterator it = tm_.find(tname);
        if (it != tm_.end()) {
            pthread_cancel(it->second);
            tm_.erase(tname);
            std::cout << "Thread " << tname << " killed:" << std::endl;
        }
    }


private:
    typedef std::unordered_map<std::string, pthread_t> ThreadMap;
    ThreadMap tm_;
};


void show_thread(const std::string &keyword)
{
    std::string cmd("ps -T | grep ");
    cmd += keyword;
    system(cmd.c_str());
}
 
int main()
{
    kProcess kprocess;
    std::string keyword("test_thread");
    std::string tname1 = keyword + "1";
    std::string tname2 = keyword + "2";


    // Crea y hace kill al thread 1 
    kprocess.start_thread(tname1);
    show_thread(keyword);
    kprocess.stop_thread(tname1);
    show_thread(keyword);


    // Crea y hace kill al thread 2
    kprocess.start_thread(tname2);
    show_thread(keyword);
    kprocess.stop_thread(tname2);
    show_thread(keyword);


    return 0;
}