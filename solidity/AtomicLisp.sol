pragma solidity ^0.4.11;

contract atomiclisp {

	address owner;

	string program;

	function atomiclisp(string _program) public {
		program = _program;
		owner = msg.sender;
	}

	function interpret() constant returns (string) {
		return program;
	}

	function kill() {
		if(msg.sender == owner) selfdestruct(owner);
	}

}