/*
Бирюков В.В. М8О-107Б-19
Создать шаблон динамической коллекцию, согласно варианту задания:
  1. Коллекция должна быть реализована с помощью умных указателей (std::shared_ptr, std::weak_ptr). Опционально использование std::unique_ptr;
  2. В качестве параметра шаблона коллекция должна принимать тип данных;
  3. Коллекция должна содержать метод доступа:
     - Стек – pop, push, top;
     - Очередь – pop, push, top;
     - Список, Динамический массив – доступ к элементу по оператору [];
  4. Реализовать аллокатор, который выделяет фиксированный размер памяти (количество блоков памяти – является параметром шаблона аллокатора). Внутри аллокатор должен хранить указатель на используемый блок памяти и динамическую коллекцию указателей на свободные блоки. Динамическая коллекция должна соответствовать варианту задания (Динамический массив, Список, Стек, Очередь);
  5. Коллекция должна использовать аллокатор для выделения и освобождения памяти для своих элементов.
  6. Аллокатор должен быть совместим с контейнерами std::map и std::list (опционально – vector).
  7. Реализовать программу, которая:
     - Позволяет вводить с клавиатуры фигуры (с типом int в качестве параметра шаблона фигуры) и добавлять в коллекцию использующую аллокатор;
     - Позволяет удалять элемент из коллекции по номеру элемента;
     - Выводит на экран введенные фигуры c помощью std::for_each;
Вариант 17: треугольник, очередь.
*/

#include <iostream>
#include <algorithm>

#include "queue.hpp"
#include "triangle.hpp"
#include "allocator.hpp"

int main() {
	Queue<Triangle<int>, Allocator<Triangle<int>,256>> queue;

	std::cout << "a TRIANGLE - Push" << std::endl
			<< "d - Pop" << std::endl
			<< "t - Top" << std::endl
			<< "i INDEX TRIANGLE - Insert" << std::endl
			<< "e INDEX - Erase" << std::endl
			<< "p - Print" << std::endl
			<< "q - Exit" << std::endl;

	char command;
	std::cout << "> ";
	while (std::cin >> command && command != 'q') {
		try {
			if (command == 'a') {
				Triangle<int> tr;
				std::cin >> tr;
				queue.push(tr);

			} else if (command == 'd') {
				queue.pop();

			} else if (command == 't') {
				std::cout << queue.top() << std::endl;
			
			} else if (command == 'i') {
				int index;
				std::cin >> index;
				Triangle<int> tr;
				std::cin >> tr;
				if (index < 0) {
					throw std::runtime_error("Error: Out of bounds");
				}
				auto iter = queue.begin();
				while (index--) {
					++iter;
				}
				queue.insert(iter, tr);

			} else if (command == 'e') {
				int index;
				std::cin >> index;
				if (index < 0) {
					throw std::runtime_error("Error: Out of bounds");
				}
				auto iter = queue.begin();
				while (index--) {
					++iter;
				}
				queue.erase(iter);

			} else if (command == 'p') {
				std::for_each(queue.begin(), queue.end(), [](const Triangle<int>& tr) { std::cout << tr << std::endl; });
			}
		} catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		std::cout << "> ";
	}
}