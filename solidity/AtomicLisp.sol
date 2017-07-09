pragma solidity ^0.4.11;

import "strings.sol";

contract mortal {//Keeping this to be a good citizen
    /* Define variable owner of the type address*/
    address owner;

    /* this function is executed at initialization and sets the owner of the contract */
    function mortal() { owner = msg.sender; }

    /* Function to recover the funds on the contract */
    function kill() { if (msg.sender == owner) selfdestruct(owner); }
}

contract atom {

	using strings for *;

	//using the label type gives an error
	string t;//type
	uint intData;
	string symbolData;

	function atom() public {
		t = "null";
	}

	function initialize() {
		t = "null";
	}

	function initialize(string s) {
		bytes memory b = bytes(s);
		if(b[0] >= 48 && b[0] <= 57) {//b[0] is an int
			uint result = 0;
		    for (uint i = 0; i < b.length; i++) { // c = b[i] was not needed
		        if (b[i] >= 48 && b[i] <= 57) {
		            result = result * 10 + (uint(b[i]) - 48); // bytes and int are not compatible with the operator -.
		        }
		    }
		    t = "int";
		    intData = result;
		}else{
			t = "symbol";
			symbolData = s;
		}
	}

	function getInt() returns (uint) {
		return intData;
	}

	function uintToString(uint v) constant returns (string) {
	    uint maxlength = 100;
	    bytes memory reversed = new bytes(maxlength);
	    uint i = 0;
	    while (v != 0) {
	        uint remainder = v % 10;
	        v = v / 10;
	        reversed[i++] = byte(48 + remainder);
	    }
	    bytes memory s = new bytes(i); // i + 1 is inefficient
	    for (uint j = 0; j < i; j++) {
	        s[j] = reversed[i - j - 1]; // to avoid the off-by-one error
	    }
	    string memory str = string(s);  // memory isn't implicitly convertible to storage
	    return str; // this was missing
	}

	function toString() returns (string) {
		var kind = t;
		if(kind.toSlice().compare("int".toSlice()) > 0) {
			return uintToString(intData);
		}else if(kind.toSlice().compare("symbol".toSlice()) > 0) {
			return symbolData;
		}else{
			string memory s = "uh oh";
			return s;
		}
	}

	function getSymbol() returns (bytes32 result) {
		assembly {
        	result := mload(add(symbolData, 32))
    	}
	}

	function stringToAtom(string s) constant returns (atom) {
		atom a;
		return a;
	}
	//https://ethereum.stackexchange.com/questions/10932/how-to-convert-string-to-int
	function stringToUint(string s) constant returns (uint) {
	    bytes memory b = bytes(s);
	    uint result = 0;

	    return result; // this was missing
	}
}

contract atomiclisp is mortal {
	using strings for *;

	string code;

	function atomiclisp(string _code) public {
		code = _code;
	}

	function run() constant returns (bytes32) {
		atom a;
		a.initialize("foo");
		return a.getSymbol();
	}

}