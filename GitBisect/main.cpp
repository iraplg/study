#include <stdio.h>
#include <Windows.h>
#include <string>
#include <vector>
#include "CmdHandler.h"



void main(int argc, char** argv)
{
	if (argc != 5)
	{
		printf("usages: git-bisect <last-commit-in-old-state> <first-commit-in-new-state> <command...>\n");
		printf("Argc count error, %d\n", argc);
		return;
	}

	std::string firstCommit = argv[2];
	std::string lastCommit = argv[3];
	std::string cmd = argv[4];

	CmdHandler handler;
	auto res = handler.bisect(firstCommit, lastCommit, cmd);

	printf("RESULT - %s\n", res.c_str());
}
