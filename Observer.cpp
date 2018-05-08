#include <iostream>
#include <cstring>

// C_M_1
// C_M_2

branch pull request
using namespace std;

class Observer {
	
	public:
	virtual void update(double st1, double st2, int st3) = 0;
};

class Subject {

	public:
	virtual void attach(Observer *o) = 0;
	virtual void detach(Observer *o) = 0;
	virtual void notify() = 0;
};

class ConcreteSubject : public Subject {
	private: 
	double st1;
	double st2;
	int st3;
	Observer *objs[100];
	int index;

	public:
	ConcreteSubject() {
		index = 0;
	}
	virtual void attach(Observer *o) {
		objs[index++] = o;
	}

	virtual void detach(Observer *o) {
		for(int i=0;i<index;i++) {
			if(objs[i]==o) {
				memmove(objs+i, objs+i+1, sizeof(Observer)*(100-i));
			}
		}
	}
	
	virtual void notify() {
		for(int i=0;i<index;i++) {
			objs[i]->update(st1,st2,st3);
		}
	}

	void setStatus(double st1, double st2, int st3) {
		this->st1 = st1;
		this->st2 = st2;
		this->st3 = st3;
	}
};

class ConcreteObserverA : public Observer {
	private:
		double st1;
		double st2;
		int st3;
		Subject *info;
	public:
		ConcreteObserverA(Subject *info) {
			this->info = info;
			info->attach(this);
		}

	virtual void update(double st1, double st2, int st3) {
		this->st1 = st1;
		this->st2 = st2;
		this->st3 = st3;
		cout << st1 << " & " << st2 << endl;
	}

};

class ConcreteObserverB : public Observer {
	private:
		double st1;
		double st2;
		int st3;
		Subject *info;
	public:
		ConcreteObserverB(Subject *info) {
			this->info = info;
			info->attach(this);
		}

	virtual void update(double st1, double st2, int st3) {
		this->st1 = st1;
		this->st2 = st2;
		this->st3 = st3;
		cout << st2 << " & " << st3 << endl;
	}

};

int main() {
	ConcreteSubject * CS = new ConcreteSubject();
	Observer *o1 = new ConcreteObserverA(CS);
	Observer *o2 = new ConcreteObserverB(CS);
	CS->setStatus(400.5,39.3,9);
	CS->notify();
}


