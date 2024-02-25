#include <iostream>
#include <future>
#include <algorithm>
#include <vector>
#include <iterator>

template<typename It, typename T, size_t size= 25>
T async_for_each(It begin, It end, T init) {
	//���������� ������� ������ �������
	auto curr_side = std::distance(begin, end);
	//���� ������� ������ ������ ��� ����� ������� �������� �������
	//��������� �� ���� ���������� ����������� ���_��� � ����� ����������
	if (curr_side <= size) {
		return std::for_each(begin, end, init);
	}
	//�����, ��������� ������� ������ ������� ������� � ���������� ��������� �� ��� ����������
	auto mid = begin;
	std::advance(mid, curr_side / 2);

	auto left = std::async(async_for_each<It,T>, begin, mid, init);
	auto right = std::async(async_for_each<It,T>, mid, end, init);

	//���������� ����������
	left.get();
	right.get();
}


std::mutex m;

int main() {
	std::vector<double> vec(200, 11);

	auto print_x_2 = [](const int& x) {
		m.lock();
		std::cout << x * 2 << " ";
		m.unlock();
		};
	async_for_each(vec.begin(), vec.end(), print_x_2);
	

	return 0;
}