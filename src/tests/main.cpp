#include <vm.h>

#include <iostream>

int Vlk_Print(CVolkano_VM *pVM)
{
	std::cout << Vlk_GetString(pVM, -1);

	return 0;
}

int main()
{
	CVolkano_VM *pVM = Vlk_NewVM();
	int Status = Vlk_LoadFromFile(pVM, "../examples/print.vlk");

	Vlk_SetGlobal(pVM, "print", Vlk_Print);

	Vlk_GetGlobal(pVM, "main");
	Vlk_Execute(pVM);

	Vlk_CloseVM(pVM);

	return Status;
}
