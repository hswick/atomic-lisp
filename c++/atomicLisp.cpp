#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <math.h>

//should change name of Atom to Value
class Atom {
public:
	std::string type;
	int intData;
	float floatData;
	std::vector<Atom> listData;
	std::string symbolData;

	Atom() {
		type = "null";
	}

	Atom(std::string s) {
		//Do type conversion for value
		try{
			float num = std::stof(s);

			if(num == floor(num)) {
				intData = (int)num;
				type = "int";
			}else{
				floatData = num;
				type = "float";
			}
		}catch ( const std::invalid_argument& ia) {
				type = "symbol";
				symbolData = s;
		}
	}

	Atom(int i) {
		type = "int";
		intData = i;
	}

	Atom(float f) {
		type = "float";
		floatData = f;
	}

	//Used by the interpreter but not the user
	Atom(std::vector<Atom> v) {
		type = "list";
		listData = v;
	}

	static std::vector<Atom> atoms() {
		std::vector<Atom> as;
		return as;
	}

	//Shouldve called list s-expressions
	//and seq should be list
	std::string toString() {
		if(type == "int") {
			return std::to_string(intData);
		}else if(type == "float") {
			return std::to_string(floatData);
		}else if(type == "symbol") {
			return symbolData;
		}else if(type == "list"){
			std::string sum = "[";
			for(int i = 0; i < listData.size(); i++) {
				sum+=" "+listData[i].toString();
			}
			return sum+" ]";
		}else if(type =="function") {
			std::string sum = "[";
			for(int i = 0; i < listData.size(); i++) {
				sum+=" "+listData[i].toString();
			}
			return sum+" ]";			
		}else{
			std::cout << "COULD NOT CONVERT ATOM TO STRING" << std::endl;
			return " ";
		}
	}

	float getFloat() {
	 	if(type == "float") {
	 		return floatData;
	 	}else{
	 		return intData;
	 	}
	}	

	int getInt() {
	 	return intData;
	}
};

//Copying code from here
//http://norvig.com/lispy.html

//Also doing some filtering here
std::vector<std::string> split(std::string input) {
	std::vector<std::string> tokens;
	std::string s = "";
	for(int i = 0; i < input.size(); i++) {
		char c = input[i];
		if(c != ' ' && c != '\n' && c != '\t') {
			s+=c;
		}else{
			if(s != "") {
				tokens.push_back(s.c_str());//do a deep copy
			}
			s = "";
		}
	}
	return tokens;
}

//Returns a new string with the string replacing all instances of a char
std::string replaceChar(std::string oldString, char oldChar, std::string rep) {
	std::string newString = "";
	for(int i = 0; i < oldString.size(); i++) {
		char c = oldString[i];
		if(c == oldChar) {
			newString+=rep;
		}else{
			newString+=c;
		}
	}
	return newString;
}

//Splits a string into tokens, does not include whitespace
std::vector<std::string> tokenize(std::string program) {
	std::string step1 = replaceChar(program, '(', " ( ");
	std::string step2 = replaceChar(step1, ')', " ) ");
	return split(step2);
}

Atom atomize(std::vector<std::string> tokens) {
	Atom atoms(Atom::atoms());
	for(int i = 0; i < tokens.size(); i++) {
		atoms.listData.push_back(Atom(tokens[i]));
	}
	return atoms;
}

//pops the first element off of a vector
Atom popAtom(Atom& atoms) {
	Atom a = atoms.listData[0];
	atoms.listData.erase(atoms.listData.begin());//0th element
	return a;
}

//Turn a list of atoms into an ast
Atom readFromAtoms(Atom& atoms) {
	if (atoms.listData.size() == 0) {
		throw std::invalid_argument("Tokens length is zero");
	}
	Atom a = popAtom(atoms);
	if("(" == a.toString()) {
		//An atom can also be a list of atoms;
		Atom newAtoms(Atom::atoms());//make an atom of type list
		while(atoms.listData[0].toString() != ")") {
			newAtoms.listData.push_back(readFromAtoms(atoms));
		}
		popAtom(atoms);
		return newAtoms;
	}else if(")" == a.toString()) {
		throw std::invalid_argument("Unexpected )");
	}else{//This works because it is only called within the while loop
		return a;
	}
}

//Create standard eval environment
std::map<std::string, Atom> standardEnv() {
	std::map<std::string, Atom> env;
	return env;
}

std::map<std::string, Atom> globalEnv = standardEnv();

Atom eval(Atom ast, std::map<std::string, Atom>& env);

Atom evalList(Atom fnName, Atom args, std::map<std::string, Atom>& env) {
	const char* fn = fnName.toString().c_str();

	//This is where base functionality goes
	if(std::strncmp(fn, "+", 1) == 0) {
		if(args.listData[0].type == "float" || args.listData[1].type == "float") {
			return Atom(args.listData[0].getFloat() + args.listData[1].getFloat());
		}else{
			return Atom(args.listData[0].getInt() + args.listData[1].getInt());
		}
	}else if(std::strncmp(fn, "-", 1) == 0) {
		if(args.listData[0].type == "float" || args.listData[1].type == "float") {
			return Atom(args.listData[0].getFloat() - args.listData[1].getFloat());
		}else{
			return Atom(args.listData[0].getInt() - args.listData[1].getInt());
		}
	}else if(std::strncmp(fn, "*", 1) == 0) {
		if(args.listData[0].type == "float" || args.listData[1].type == "float") {
			return Atom(args.listData[0].getFloat() * args.listData[1].getFloat());
		}else{
			return Atom(args.listData[0].getInt() * args.listData[1].getInt());
		}
	}else if(std::strncmp(fn, "/", 1) == 0) {
		if(args.listData[0].type == "float" || args.listData[1].type == "float") {
			return Atom(args.listData[0].getFloat() / args.listData[1].getFloat());
		}else{
			return Atom(args.listData[0].getInt() / args.listData[1].getInt());
		}
	}else if(std::strncmp(fn, "do", 2) == 0) {
		if(args.listData.size() > 1) {
			int i = 0;
			for(;i < args.listData.size()-1; i++) {
				eval(args.listData[i], env);
			}
			return eval(args.listData[i], env);//Always return last result
		}else{
			return eval(args.listData[0], env);
		}
	}else if(std::strncmp(fn, "def", 3) == 0){
		std::string varName = args.listData[0].toString();
		env.insert(std::pair<std::string, Atom>(varName, eval(args.listData[1], env)));
		return Atom();
	}else if(std::strncmp(fn, "set!", 4) == 0){
		std::string varName = args.listData[0].toString();
		env[varName] = eval(args.listData[1], env);
		return Atom();
	}else{//Call procedure
		std::cout << fnName.toString() << " PROCEDURE CALL" << std::endl;
		Atom proc;
		//Apply arguments to function
		if(fnName.type == "symbol") {
			proc = eval(fnName, env);
		}else if(fnName.type == "function"){
			proc = fnName;
		}else{
			std::cout << fnName.toString() << " NOT A PROPER PROCEDURE CALL" << std::endl;
			throw 20;
		}
		std::map<std::string, Atom> localEnv = standardEnv();
		Atom params = proc.listData[0];
		Atom fnBody = proc.listData[1];

		if(params.listData.size() != args.listData.size()) {
			std::cout << fnName.toString() << " params and args not equal length" << std::endl;
			throw 20;
		}

		if(params.listData.size() > 0) {
			for(int i = 0; i < params.listData.size(); i++) {
				localEnv.insert(std::pair<std::string, Atom>(params.listData[i].toString(), args.listData[i]));
			}
		}
		return eval(fnBody, localEnv);
	}
}

Atom eval(Atom ast, std::map<std::string, Atom>& env) {
	std::cout<< ast.type <<std::endl;

	if(ast.type == "symbol") {
		if(env.count(ast.toString())) {
			return env[ast.toString()];
		}else if(globalEnv.count(ast.toString())) {
			return globalEnv[ast.toString()];
		}else {
			return ast;
		}
	}else if(ast.type != "list") {
		return ast;
	}else{
		if(ast.listData.size() > 0) {
			Atom fnName = ast.listData[0];
			if(std::strncmp(fnName.toString().c_str(), "quote", 5) == 0) {
				Atom arg = ast.listData[1];
				return arg.toString();
			}else if(std::strncmp(fnName.toString().c_str(), "fn", 2) == 0) {
				std::map<std::string, Atom> localEnv = standardEnv();
				Atom fnArgs = ast.listData[1];
				Atom fnBody = ast.listData[2];
				//Produce (args body env)
				std::vector<Atom> els;
				els.push_back(fnArgs);
				els.push_back(fnBody);
				//els.push_back(Atom(std::vector<Atom>()));
				Atom lambda = Atom(els);
				lambda.type = "function";
				return lambda;
			}else{
				std::vector<Atom> args;//Evaluates all arguments
				for(int i = 1; i < ast.listData.size(); i++) {
					args.push_back(eval(ast.listData[i], env));
				}
				return evalList(ast.listData[0], args, env);
			}
		}else{
			return Atom();//Empty list is equivalent to null
		}
	}
}

Atom eval(Atom ast) {
	return eval(ast, globalEnv);
}

std::string fileToString(std::string filename) {
	std::ifstream ifs(filename);
  	std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
  	return content;
}

//Segmentation faults are from delimiter errors. Need to be better about checking
int main()
{
	std::string program = fileToString("test5.al");
	//std::cout << program << std::endl;
	std::vector<std::string> tokens = tokenize(program);
	Atom atoms = atomize(tokens);
	//std::cout << atoms.listData.size() << std::endl;
	Atom ast = readFromAtoms(atoms);
	//std::cout << atoms.listData.size() << std::endl;
	std::cout << ast.toString() << std::endl;
	Atom result = eval(ast);
	//std::cout << result.type << std::endl;
	std::cout << result.toString() << std::endl;
	return 0;
}