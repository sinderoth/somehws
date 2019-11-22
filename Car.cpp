#include "Car.h"
#include "Utilizer.h"
#include <iostream>


Car::Car(std::string driver_name){
  this->driver_name = driver_name;
  this->performance = Utilizer::generatePerformance();
  next=NULL;
  head=NULL;
}



Car::Car(const Car& rhs){
  this->driver_name = rhs.driver_name;
  this->performance = rhs.performance;

  Laptime* clsptr;
  Laptime* ptr = rhs.head;
  if (ptr == NULL){

    this->head = NULL;
    next = NULL;
    }
  else{

  Laptime *newlap = new Laptime(ptr->getLaptime());
  this->head = newlap;
  clsptr = newlap;

  if (ptr->getNext() != NULL) ptr = ptr->getNext();
  else clsptr->setNext(NULL);

  while (ptr != NULL){
    Laptime *newlap = new Laptime(ptr->getLaptime());
    clsptr->setNext(newlap);
    ptr = ptr->getNext();
  }
}

}


Car::~Car(){

n
  Laptime* ptr = this->head;
  if (ptr != NULL){
  while (ptr->getNext() != NULL){
    Laptime* temp = ptr->getNext();
    ptr->setNext(temp->getNext());
    delete temp;
  }

  delete ptr;
  head = NULL;
  next= NULL;
  }
  else ;

}

std::string Car::getDriverName() const{
  return driver_name;
}

double Car::getPerformance() const{
  return performance;
}

void Car::addCar(Car *next){
  if (next==NULL){
    this->next =NULL;
  }
  else this->next = next;
}

bool Car::operator<(const Car& rhs) const{
  Laptime *ptr = this->head;
  int totalf=0;
  int totals=0;
  while (ptr){
    totalf += ptr->getLaptime();
    ptr = ptr->getNext();
  }
  ptr = rhs.head;
  while (ptr){
    totals += ptr->getLaptime();
    ptr = ptr->getNext();
  }
  if (totalf < totals) return true;
  return false;
}

bool Car::operator>(const Car& rhs) const{
  Laptime *ptr = this->head;
  int totalf=0;
  int totals=0;
  while (ptr){
    totalf += ptr->getLaptime();
    ptr = ptr->getNext();
  }
  ptr = rhs.head;
  while (ptr){
    totals += ptr->getLaptime();
    ptr = ptr->getNext();
  }
  if (totalf > totals) return true;
  return false;
}


Laptime Car::operator[](const int lap) const{
  Laptime *ptr = this->head;
  int index = 0;

  if (ptr == NULL){
    Laptime *la = new Laptime(0);
    return *la;
  }
  while (index<lap){
    ptr=ptr->getNext();
    if (ptr == NULL){
      Laptime *la = new Laptime(0);
      return *la;
    }
    index++;
  }
  Laptime *newlaptime = new Laptime(ptr->getLaptime());
  return *newlaptime;
}


void Car::Lap(const Laptime& average_laptime){
  int time = Utilizer::generateLaptimeVariance(performance)+(average_laptime).getLaptime();
  Laptime *newlap = new Laptime(time);
  Laptime *ptr = head;
  if (ptr == NULL){
    head=newlap;
    newlap->addLaptime(NULL);
  }
  else{
  while (ptr->getNext() != NULL){
    ptr = ptr->getNext();
  }
  ptr->addLaptime(newlap);
  newlap->addLaptime(NULL);
  }
}

Car* Car::getNext(){
  return this->next;
}

Laptime* Car::getHead(){
  return head;
}

int Car::getTotalTime() const{
  Laptime *ptr = head;
  int total=0;
  while(ptr!=NULL){
    total += ptr->getLaptime();
    ptr = ptr->getNext();
  }
  return total;
}

void Car::setPerformance(double perf){
  this->performance =perf;
}
double Car::getPerformance(){
  return this->performance;
}

/**
 * Stream overload.
 *
 * What to stream:
 * First Three letters of the drivers surname(Capitalized)--Latest Laptime--Fastest Laptime--Total Laptime
 * Example:
 * For Lewis Hamilton
 * HAM--1:19.235--1:18.832--90:03.312
 */
std::ostream& operator<<(std::ostream& os, const Car& car){
  std::string name=car.driver_name;
  std::size_t pos = name.find(" ");
  std::string threeletter = name.substr(pos+1,3);
  for (int i=0 ; i <3 ; i++){
        if (threeletter[i]<=90 && threeletter[i]>=65){
            ;
        }
        else threeletter[i]-=32;
  }
  Laptime *latest = car.head;
  while (latest->getNext()!=NULL){
    latest = latest->getNext();
  }
  Laptime *iter = car.head;
  Laptime *fastest=car.head;
  while(iter!=NULL){
    if (*fastest>*iter){
      fastest = iter;
    }
    iter=iter->getNext();
  }
  int totalT=0;
  iter = car.head;
  while(iter!=NULL){
    totalT+=iter->getLaptime();
    iter= iter->getNext();
  }
  Laptime *help = new Laptime(totalT);

  os << threeletter <<"--"<< *latest <<"--"<<*fastest<<"--"<<*help;
  delete help;
  return os;
}
