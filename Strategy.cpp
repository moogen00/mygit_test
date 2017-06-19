#include <iostream>
//
// local branch
// 20170619 - 0001
using namespace std;

class Strategy {
	public:
	virtual void execute() = 0;	
};


//
class ConcreteStrategyA : public Strategy {
	public:
		virtual void execute() {
			cout << "ConcreteStrategyA's behavior" << endl;
		}
};


class ConcreteStrategyB : public Strategy {
	public:
		virtual void execute() {
			cout << "ConcreteStrategyB's behavior" << endl;
		}
};


class Context { 
	private:
		Strategy *stg;
	public:
		void setStrategy(Strategy *stg) {
			this->stg = stg;
		}	

		void perForm() {
			stg->execute();
		}
};

int main() {
	Context *S = new Context();
	Strategy *A = new ConcreteStrategyA();
	S->setStrategy(A);
	S->perForm();

	Strategy *B = new ConcreteStrategyB();
	S->setStrategy(B);
	S->perForm();
}



