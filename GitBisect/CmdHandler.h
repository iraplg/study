#pragma once
#include <fstream>



struct CmdHandler
{
	std::vector<std::string> getCommits(const std::string& firstCommit, const std::string& lastCommit)
	{
		auto commandLine = "git rev-list --ancestry-path " + firstCommit + ".." + lastCommit;

		HANDLE handleFile = CreateFile("out.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		SetHandleInformation(handleFile, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

		STARTUPINFO si = { sizeof(si) };
		si.dwFlags |= STARTF_USESTDHANDLES;
		si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
		si.hStdError = handleFile;
		si.hStdOutput = handleFile;

		PROCESS_INFORMATION pi = {};
		DWORD exit_code;

		if (!CreateProcess(NULL,
			(LPSTR)commandLine.c_str(),
			NULL,
			NULL,
			TRUE,
			NULL,
			NULL,
			NULL,
			&si,
			&pi)
			)
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			return {};
		}

		WaitForSingleObject(pi.hProcess, INFINITE);
		GetExitCodeProcess(pi.hProcess, &exit_code);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		auto commits = readToLines("out.txt");

		return commits;
	}



	bool runCmd(const std::string& currCommit, const std::string& cmd)
	{
		std::string commandLine = "git checkout " + currCommit;

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		DWORD exit_code;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		if (!CreateProcess(NULL,
			(LPSTR)commandLine.c_str(),
			NULL,
			NULL,
			FALSE,
			NULL,
			NULL,
			NULL,
			&si,
			&pi)
			)
		{
			printf("CreateProcess failed 2 (%d).\n", GetLastError());
			return {};
		}

		WaitForSingleObject(pi.hProcess, INFINITE);
		GetExitCodeProcess(pi.hProcess, &exit_code);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		FILE* file = fopen(cmd.c_str(), "at");
		
		if (file)
			return true;

		return false;
	}



	std::string bisect(const std::string& firstCommit, const std::string& lastCommit, const std::string& cmd)
	{
		auto commits = getCommits(firstCommit, lastCommit);

		if (commits.empty())
			return "commits not found";

		int left = -1;
		int right = commits.size() - 1;

		while (left + 1 != right)
		{
			int mid = (right + left) / 2;

			if (runCmd(commits[mid], cmd))
				right = mid;
			else
				left = mid;
		}

		return commits[right];
	}



	static std::vector<std::string> readToLines(const std::string& fileName)
	{
		std::ifstream file(fileName);

		std::vector<std::string> ret;
		std::string s;

		while (std::getline(file, s))
		{
			if (s.length())
			{
				ret.push_back(s);
			}
		}

		return ret;
	}
};
