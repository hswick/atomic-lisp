pragma solidity ^0.4.11;

contract mortal {//Keeping this to be a good citizen
    /* Define variable owner of the type address*/
    address owner;

    /* this function is executed at initialization and sets the owner of the contract */
    function mortal() { owner = msg.sender; }

    /* Function to recover the funds on the contract */
    function kill() { if (msg.sender == owner) selfdestruct(owner); }
}

contract atomiclisp is mortal {

	string code;

	function atomiclisp(string _code) public {
		code = _code;
	}

	function run() constant returns (string) {
		return code;
	}

}