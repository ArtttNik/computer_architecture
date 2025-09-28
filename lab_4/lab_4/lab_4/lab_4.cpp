#include <iostream>

void print(int* arr)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 3; ++j) {
			std::cout << arr[i * 3 + j] << "\t";
		}
		std::cout << std::endl;
	}
}

int main()
{
	std::cout << "TASK1" << std::endl << std::endl;
	
	int A[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	__asm	lea ebx, A[0]

	__asm	mov edi, 8

	__asm	mov esi, [ebx + 4]

	__asm	mov esi, edi

	__asm	mov[ebx + 4], 15

	__asm	mov esi, 8

	__asm	mov[ebx + esi], 14

	__asm	mov esi, 8

	__asm	mov[ebx + esi + 4], 13

	__asm {
	loop1:
	cmp ecx, 8
	ja loop1
	}

	std::cout << A[1] << " ";
	std::cout << A[2] << " ";
	std::cout << A[3] << " " << std::endl;
	



	//N2
	__int64 t = __rdtsc();

	int B[10] = { 1, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	__asm lea ebx, B[0]

		__asm {
		mov ch, 2
		mov cl, 1
	}
	//1
	t = __rdtsc();
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	__asm add ch, cl
	std::cout << __rdtsc() - t << std::endl;

	//2
	__asm mov ecx, 1
	__asm mov esi, 4
	t = __rdtsc();
	__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		__asm add ecx, [ebx + esi + 8]
		std::cout << __rdtsc() - t << std::endl;

	//3
	__asm mov ecx, 1
	t = __rdtsc();
	__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		__asm add ecx, [ebx]
		std::cout << __rdtsc() - t << std::endl;

	//4
	__asm mov ecx, 10
	t = __rdtsc();
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	__asm add[ebx], ecx
	std::cout << __rdtsc() - t << std::endl;


	
	int C[4][3] =
	{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 },
		{ 10, 11, 12 }
	};

	int D[4] = { 0 };

	print(*C);


	__asm {
		cld
		mov ebx, 2
		lea edi, [C + 12]

		loop11:
		lea esi, D
		mov ecx, 3
		rep movsd
		add edi, 12
		dec ebx
		cmp ebx, 0
		ja loop11
	}

	std::cout << std::endl;
	print(*C);

	__asm {
		cld

		lea edi, C[0]
		mov eax, 0
		mov bl, 2

		loop2_2:

		add eax, 12

		mov bh, 3
		//loop2_1
		loop2_1 :

		mov [edi + eax], 13

		add eax, 4

		dec bh
		cmp bh, 0
		ja loop2_1

		dec bl
		cmp bl, 0
		ja loop2_2
	}

	std::cout << std::endl;
	print(*C);
	

	std::cout << std::endl;
	system("pause");
	//return 0;

}

/*#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

std::chrono::milliseconds;

void outArray(int* a, int nRow, int nColumn)
{
	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nColumn; ++j)
		{
			cout << *(a + nColumn*i + j) << " ";
		}
		cout << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "rus");

	const long long number = 10E+9;

	chrono::steady_clock timer1; // пустой цикл
	auto start = timer1.now();
	for (long long i = 0; i < number; ++i);
	auto end = timer1.now();

	cout << "Время выполнения пустого цикла в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

	chrono::steady_clock timer2; // непосредственная адресация
	start = timer2.now();
	for (long long i = 0; i < number; ++i) {
		__asm {	 
			 add eax, ebx
			 add edx, ecx
			 add eax, ebx
			 add edx, ecx
			 add eax, ebx
			 add edx, ecx
			 add eax, ebx
			 add edx, ecx
			 add eax, ebx
			 add edx, ecx
			 add eax, ebx
			 add edx, ecx
			 add eax, ebx
			 add edx, ecx
			 add eax, ebx
			 add edx, ecx
		}
	}
	end = timer2.now();
	cout << "Время выполнения c использованием непосрественной адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;


	chrono::steady_clock timer3; // регистровая адресация
	start = timer3.now();
	for (long long i = 0; i < number; ++i) {
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
	for (long long i = 0; i < number; ++i) {
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
			add eax, [ebx]
			add eax, [ebx]
			add eax, [ebx]
			add eax, [ebx]
			add eax, [ebx]
		}
	}
	end = timer4.now();

	cout << "Время выполнения c использованием косвенно-регистровой адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

	int arr[4][5] = { 1, 1, 1, 1, 1,
					  2, 2, 2, 2, 2,
					  3, 3, 3, 3, 3,
					  4, 4, 4, 4, 4};
	int k = 0;
	cout << "Исходный массив: " << endl;
    outArray(arr[0],4,5);
	
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
	outArray(arr[0], 4, 5);
	return 0;
}*/