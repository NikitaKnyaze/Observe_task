#include <iostream>
#include <fstream>
#include <vector>

class Observer
{
public:
   virtual ~Observer() = default;
   virtual void onWarning(const std::string& message) = 0;
   virtual void onError(const std::string& message) = 0;
   virtual void onFatalError(const std::string& message) = 0;
};

class Observed
{ 
private:
   std::vector<Observer*> observers;

public:
   void addObs(Observer* obs)
   {
      observers.push_back(obs);
   }

   void removeObs(Observer* obs)
   {
      auto it = std::remove(observers.begin(), observers.end(), obs);
      observers.erase(it, observers.end());
   }

   void warning(const std::string& message) const
   {
      for (Observer* obs : observers)
      {
         if (obs)
         {
            obs->onWarning(message);
         }
      }
   }

   void error(const std::string& message) const
   {
      for (Observer* obs : observers)
      {
         if (obs)
         {
            obs->onError(message);
         }
      }
   }

   void fatalError(const std::string& message) const
   {
      for (Observer* obs : observers)
      {
         if (obs)
         {
            obs->onFatalError(message);
         }
      }
   }
};


class WarningError : public Observer
{
public:
   WarningError() {};
   void onWarning(const std::string& message) override
   {
      std::cout <<  message << std::endl;
   }
   
      void onError(const std::string& message) override {}

      void onFatalError(const std::string& message) override {}
};


class onErrorFile : public Observer
{
private:
   std::ofstream outFile;

public:

   onErrorFile()
   {
      outFile.open("outFile.txt", std::ios::app); 
      if (!outFile.is_open())
      {
         std::cerr << "Error: File error";
      }
   }

   void onWarning(const std::string& message) override {}

   void onFatalError(const std::string& message) override {}

   void onError(const std::string& message) 
   {
         outFile << message << std::endl;
   }

   ~onErrorFile()
   { 
      if(outFile.is_open())
      outFile.close();
   }
};


class ConsoleFileOutput : public Observer
{
private:
   std::ofstream outFile_1;
public:

   ConsoleFileOutput()
   {
      outFile_1.open("outFile_1.txt", std::ios::app);
      if (!outFile_1.is_open())
      {
         std::cerr << "Error: File error";
      }
   }

   void onWarning(const std::string& message) override {}

   void onError(const std::string& message) override {}

   void onFatalError(const std::string& message) override
   {
         outFile_1 << message;
         std::cout << message << std::endl;
   }

   ~ConsoleFileOutput() 
   { 
      if(outFile_1.is_open())
      outFile_1.close();
   }
};

int main()
{
   Observed obs_1;

   WarningError W_obs;
   onErrorFile oEF;
   ConsoleFileOutput CFO;

   Observed obs;

   obs.addObs(&W_obs);
   obs.addObs(&oEF);
   obs.addObs(&CFO);

   obs.error("Error message");
   obs.warning("Warning message");
   obs.fatalError("fatalError message");

   return 0;
}