#include <iostream>
using namespace std;

long long int shift_stack(long long int a, long long int b, long long int& result)
{

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
	return result;
}

void shift_reg()
{
	__asm
	{
		push edx
		push ebx

		mov eax, [ebx]
		mov ecx, [edx]
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
long long int* globResult = 0;

void shift_global()
{
	__asm
	{
		lea ebx, globA
		lea edx, globB
		mov esi, globResult
		push edx
		push ebx
		mov eax, [ebx]
		mov ecx, [edx]
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
	const long long int a = 987654321;
	const long long int b = 123456789;
	
	long long int result = 0;


	long long int result1 = shift_stack(a, b, result);

	cout << "Умножение через стек: " << result1 << endl;
	result = 0;

	__asm
	{
		lea ebx, a
		lea edx, b
		lea esi, result
	}
	shift_reg();
	cout << "Умножение через регистры: " << result << endl;
	result = 0;


	globA = a;
	globB = b;
	globResult = &result;
	shift_global();
	cout << "Умножение через глобальные переменные: " << result << endl;
	


	return 0;
}