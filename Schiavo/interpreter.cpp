#include "functions.h"

//spezzo anche contesti per codeblock?
//prendi riga per riga finchè non finisce il testo   (riga =fino al ;)
//spezza le righe in atom
//controlla le necessità(parametri) dell'atom
//per le assegnazioni devo capire la grandezza del valore assegnato e salvarmi pointer e moniker della variabile in una hashmap
// o magari un oggetto, altrimenti come salvo la grandezza di una variabile? con uno struct?
// tratterei tutte le variabili come VOID* e userei un reinterpret_cast a qualsiasi tipo di variabile



struct {
	int len;
	void* mem;
} typedef variable;

struct {
	size_t return_size;
	std::vector<size_t> arg_sizes;
} typedef signature;

struct {
	void* (*pointer)();
	signature signature;
} typedef function;

std::unordered_map<std::wstring, function> funcMap;
std::unordered_map<std::wstring, variable> varMap;

namespace interpreter {

	std::wstring InterpretTask(std::wstring task) {

		MessageBoxW(NULL, task.c_str(), L"", MB_OK);

		funcMap[L"testfile"].pointer = (*createfilewrap);

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
				funcMap[atoms[0]].pointer();

			//check variables
			if (varMap.contains(atoms[0])) {

				//parsare tipo del valore da assegnare cacatore
				
				/*if (!_wcsicmp(atoms[1].c_str(), L"="))
					memcpy_s(varMap[atoms[0]].mem*/
			}

		}


		return L"All is well, executed.";
	}
	
	void* createfilewrap() {
		std::wofstream cfg_file(L"C:\\Users\\bratv\\Desktop\\Schiavo\\x64\\test.txt");
		cfg_file << L"agent_id";
		cfg_file.close();
		return nullptr;
	}
	



}


