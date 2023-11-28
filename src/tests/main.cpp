#include <vm.h>

#include <iostream>

int Vlk_Print(CVolkano_VM *pVM)
{
	int Top = Vlk_GetTop(pVM) + 1;
	for(int i = 0; i < Top; i++)
	{
		if(Vlk_IsInt(pVM, i))
			std::cout << Vlk_GetInt(pVM, i);
		else if(Vlk_IsString(pVM, i))
			std::cout << Vlk_GetString(pVM, i);
	}

	return 0;
}

int main()
{
	CVolkano_VM *pVM = Vlk_NewVM();
	int Status = Vlk_LoadFromFile(pVM, "../examples/functions.vlk");

	Vlk_SetGlobal(pVM, "print", Vlk_Print);

	Vlk_GetGlobal(pVM, "main");
	Vlk_Execute(pVM);

	Vlk_CloseVM(pVM);

	return Status;
}
