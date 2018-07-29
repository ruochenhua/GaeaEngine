#include "stdafx.h"

#include "RenderWindow.h"
#include "Script.h"


int main(int argc, char* argv[])
{

	CScriptSystem* script_system = GetScriptSystem();
	script_system->Init();

	return 0;
}
