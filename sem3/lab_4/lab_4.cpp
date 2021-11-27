#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

std::chrono::milliseconds;

void outArray(int* a, int nRow, int nColumn)
{
	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nColumn; ++j)
			cout << *(a + nColumn*i + j) << " ";
		cout << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "rus");

	const long long NUM = 10E+7;

	chrono::steady_clock timer1; // замер времени пустого цикла
	auto start = timer1.now();
	for (long long i = 0; i < NUM; ++i) {} ;
	auto end = timer1.now();
	cout << "Время выполнения пустого цикла в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

	chrono::steady_clock timer2; // непосредственная адресация
	start = timer2.now();
	for (long long i = 0; i < NUM; ++i) {
		__asm {	 
			 add eax, 10
			 add eax, 10
			 add eax, 10
			 add eax, 10
			 add eax, 10
			 add eax, 10
			 add eax, 10
			 add eax, 10
			 add eax, 10
			 add eax, 10
		}
	}
	end = timer2.now();
	cout << "Время выполнения c использованием непосрественной адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;


	chrono::steady_clock timer3; // регистровая адресация
	start = timer3.now();
	for (long long i = 0; i < NUM; ++i) {
		__asm {
			 add eax, ebx
			 add eax, ebx
			 add eax, ebx
			 add eax, ebx
			 add eax, ebx
			 add eax, ebx
			 add eax, ebx
			 add eax, ebx
			 add eax, ebx
			 add eax, ebx
		}
	}
    end = timer3.now();
	cout << "Время выполнения c использованием регистровой адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

	chrono::steady_clock timer4; // косвенно-регистровая адресация
	start = timer4.now();
	for (long long i = 0; i < NUM; ++i) {
		__asm {
			add eax, [ebx]
			add eax, [ebx]
			add eax, [ebx]
		    add eax, [ebx]
			add eax, [ebx]
			add eax, [ebx]
			add eax, [ebx]
			add eax, [ebx]
			add eax, [ebx]
			add eax, [ebx]
	
		}
	}
	end = timer4.now();

	cout << "Время выполнения c использованием косвенно-регистровой адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

	__asm mov eax, esi

	chrono::steady_clock timer44; // косвенно-регистровая адресация
	start = timer44.now();
	for (long long i = 0; i < NUM; ++i) {
		__asm {
			add [ebx], eax
			add [ebx], ebx
			add [ebx], eax
			add [ebx], ebx
			add [ebx], eax
			add [ebx], ebx
			add [ebx], ebx
			add [ebx], ebx
			add [ebx], ebx
			add [ebx], ebx
		}
	}

	end = timer44.now();
	cout << "Время выполнения c использованием косвенно-регистровой адресации 2 в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

	chrono::steady_clock timer5; // базовая
	start = timer5.now();
	for (long long i = 0; i < NUM; ++i) {
		__asm {
			add eax, [ebx + 4]
			add eax, [ebx + 4]
			add eax, [ebx + 4]
			add eax, [ebx + 4]
			add eax, [ebx + 4]
			add eax, [ebx + 4]
			add eax, [ebx + 4]
			add eax, [ebx + 4]
			add eax, [ebx + 4]
			add eax, [ebx + 4]
		}
	}
	end = timer5.now();

	cout << "Время выполнения c использованием базовой со смещением адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

	__asm mov esi, 1

	chrono::steady_clock timer6; // базово-индексная со смещением
	start = timer6.now();
	for (long long i = 0; i < NUM; ++i) {
		__asm {
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
			add eax, [ebx + esi + 10]
		}
	}
	end = timer6.now();

	cout << "Время выполнения c использованием базово-индексной со смещением адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

	const int M = 5, N = 4;
	int arr[N][M] = { 1, 1, 1, 1, 1,
					  2, 2, 2, 2, 2,
					  3, 3, 3, 3, 3,
					  4, 4, 4, 4, 4};
	int k = 0;
	cout << "Исходный массив: " << endl;
    outArray(arr[0], N, M);
	
	__asm {
		cld
		mov ebx, 2
		lea edi, [arr + 20]

		loop1:
		mov eax, k
	    mov ecx, 5
	    rep stosd
		add edi, 20
		dec ebx
		cmp ebx, 0
		jne loop1
	}
	cout << "Массив после изменения четных строк : " << endl; 
	outArray(arr[0], N, M);

	return 0;
}