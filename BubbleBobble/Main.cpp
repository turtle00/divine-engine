#include "MiniginPCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "BubbleBobbleGame.h"
#include "BaseGame.h"

int main(int, char*[]) {
	
	
	BubbleBobbleGame pGame;
	pGame.Run();
    return 0;
}