#include <iostream>
#include <memory>
#include <chrono>

#include "structures/memPool.h"
#include "structures/lockFreeQueue.cpp"

class Foo {
	public:
		Foo(size_t x, size_t y) : x_(x), y_(y) {
			//std::cout<<"I have been created\n";
		}
		
		~Foo() {
			
			//std::cout<<"I have been destroyed\n";
		}
		
		void getData() {
			std::cout<<"x: "<<x_<<" y: "<<y_<<std::endl;
		}

	private:
		size_t x_;
		size_t y_;
};

void mockEngine(MemoryPool& pool, std::size_t size) {
	for (std::size_t i = 0; i < size; ++i) {
		std::byte* block = pool.allocate();
		if (!block) break;

		Foo* f = reinterpret_cast<Foo*>(block);
		std::construct_at(f, i, i * 2);
		std::destroy_at(f);
		pool.deallocate(block);
	}
}

void mockEngineNew(std::size_t size) {
	for (std::size_t i = 0; i < size; ++i) {
		Foo* f = new Foo(i,i*2);
		delete f;
	}
}

struct Node {
	Node(int data_) : data(data_) {};

	Node* next = nullptr;
	Node* prev = nullptr;
	
	int data;
};

#include <unordered_map> 

int main() {

	Node* a = new Node(1);
	Node* b = new Node(2);
	Node* c = new Node(3);

	a->next = b;
	b->prev = a;
	b->next = c;
	c->prev = b;

	// a <-> b <-> c
	using orderID = int;
	using orderAddress = Node*;

	std::unordered_map<orderID, orderAddress> orderMapper;

	orderMapper[a->data] = a;
	orderMapper[b->data] = b;
	orderMapper[c->data] = c;

	Node* ac = orderMapper[2];

	Node * t = ac->prev;
	ac->next->prev = ac->prev;
	t->next = ac->next;

	std::cout<<orderMapper.size()<<std::endl;
	orderMapper.erase(2);
	std::cout<<orderMapper.size()<<std::endl;
	delete ac;

	std::cout<<a->next->data<<std::endl;
	std::cout<<c->prev->data;

	delete a;
	delete c;

	// LockFreeQueue<int> q(100);
	// int x = 5;
	// int* p = &x;
	// int y = 10;
	// int* py = &y;
	// int j = 11;
	// int* pj = &j;

	// q.push_back(p);
	// q.push_back(py);
	// q.push_back(pj);

	// int* z = q.front();
	// int* k = q.back();
	// std::cout<<"z: "<<*z<<" k: "<<*k<<std::endl; 

	// MemoryPool pool(sizeof(Foo), 10000, alignof(Foo));

	// const auto start{std::chrono::steady_clock::now()};
	// mockEngine(pool, 10000);
	// const auto finish{std::chrono::steady_clock::now()};
	// const std::chrono::duration<double> elapsed_seconds{finish-start};
	// std::cout << "Memory Pool: " << elapsed_seconds << '\n';

	// const auto start2{std::chrono::steady_clock::now()};
	// mockEngineNew(10000);
	// const auto finish2{std::chrono::steady_clock::now()};
	// const std::chrono::duration<double> elapsed_seconds2{finish2-start2};
	// std::cout << "Run Time Allocation" << elapsed_seconds2 << '\n';


	// std::byte* block = pool.allocate();
	// Foo* f = reinterpret_cast<Foo*>(block);
	// std::byte* block2 = pool.allocate();
	// Foo* k = reinterpret_cast<Foo*>(block2);
	// std::_Construct(f, 5, 10);
	// std::_Construct(k, 3, 2);
	// f->getData();
	// k->getData();
	// std::_Destroy(f);
	// std::_Destroy(k);
	// pool.deallocate(block);
	// pool.deallocate(block2);


	return 0;
}
