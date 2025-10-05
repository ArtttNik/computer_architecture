#include <iostream>
using namespace std;

void shift_stack(long long int a, long long int b, long long int& mul)
{
	//стек
	//...
	//ip
	//bp = sp
	//8 байт
	//a 8байт                   слово1 и слово 2  слово11 слово22                1*11 1*22 2*11 2*22
	//b 8байт
	//mul 8байт                   eax * ebx = eax:edx
	__asm
	{
		mov eax, [ebp + 8]
		mov ebx, [ebp + 16]
		mul ebx
		mov esi, [ebp + 24]
		mov[esi], eax
		mov[esi + 4], edx

		mov eax, [ebp + 8]
		mov ebx, [ebp + 20]
		mul ebx
		adc[esi], eax
		adc[esi + 4], edx

		mov eax, [ebp + 12]
		mov ebx, [ebp + 16]
		mul ebx
		adc[esi], eax
		adc[esi + 4], edx

		mov eax, [ebp + 12]
		mov ebx, [ebp + 20]
		mul ebx
		adc[esi], eax
		adc[esi + 4], edx
	}
}

void shift_reg()
{
	__asm
	{

		push edx
		push ebx
		//ebx и edx - заняты

		mov eax, [ebx] //низ а
		mov ecx, [edx] // низ b
		mul ecx
		mov[esi], eax
		mov[esi + 4], edx

		pop edx
		pop ebx
		push edx
		push ebx

		mov eax, [ebx]
		mov ebx, [edx + 4]
		mul ebx
		adc[esi], eax
		adc[esi + 4], edx

		pop edx
		pop ebx
		push edx
		push ebx

		mov eax, [ebx + 4]
		mov ebx, [edx]
		mul ebx
		adc[esi], eax
		adc[esi + 4], edx

		pop edx
		pop ebx
		push edx
		push ebx

		mov eax, [ebx + 4]
		mov ebx, [edx + 4]
		mul ebx
		adc[esi], eax
		adc[esi + 4], edx

		pop edx
		pop ebx

	}
}


long long int globA = 0;
long long int globB = 0;
long long int* globMul = 0;

void shift_global()
{
	__asm
	{
		lea ebx, globA
		lea edx, globB
		mov esi, globMul
		push edx
		push ebx
		mov eax, [ebx] //низ а
		mov ecx, [edx] // низ b
		mul ecx
		mov[esi], eax
		mov[esi + 4], edx

		pop edx
		pop ebx
		push edx
		push ebx

		mov eax, [ebx]
		mov ebx, [edx + 4]
		mul ebx
		adc[esi], eax
		adc[esi + 4], edx

		pop edx
		pop ebx
		push edx
		push ebx

		mov eax, [ebx + 4]
		mov ebx, [edx]
		mul ebx
		adc[esi], eax
		adc[esi + 4], edx

		pop edx
		pop ebx
		push edx
		push ebx

		mov eax, [ebx + 4]
		mov ebx, [edx + 4]
		mul ebx
		adc[esi], eax
		adc[esi + 4], edx

		pop edx
		pop ebx

	}
}


int main()
{
	setlocale(LC_ALL, "rus");
	long long int a = 0x0F00000000F00007; //двойное слово - 32 бита
	long long int b = 0x00000000000ACF02;
	long long int mul = 0;
	//передача через стек
	shift_stack(a, b, mul);
	cout << "Умножение через стек: " << mul << endl;
	mul = 0;

	//передача через регистры
	__asm
	{
		lea ebx, a
		lea edx, b
		lea esi, mul
	}
	shift_reg();
	cout << "Умножение через регистры: " << mul << endl;
	mul = 0;

	//передача через глобальные переменные
	globA = a;
	globB = b;
	globMul = &mul;
	shift_global();
	cout << "Умножение через глобальные переменные: " << mul << endl;
	


	return 0;
}