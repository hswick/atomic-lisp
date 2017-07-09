fs = require('fs');
Web3 = require('web3');

web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:8545"));

atomicLispABI = fs.readFileSync("AtomicLisp.sol:atomiclisp.abi").toString();
atomicLispBin = fs.readFileSync("AtomicLisp.sol:atomiclisp.bin");

var AtomicLisp = web3.eth.contract(JSON.parse(atomicLispABI));

function executeContract() {
	var account = web3.eth.accounts[0];
	var deployedContract = AtomicLisp.new("(+ 2 2)",
	{data: atomicLispBin, from: account, gas: 3000000},
	function(e, contract) {
		if (e) {
			console.log(e);
		}else{
			if(!contract.address) {//contract has not been mined
				console.log("Contract transaction send: TransactionHash: " + contract.transactionHash + " waiting to be mined...");
			}else{
				console.log("Contract mined");
				console.log(contract.address);
				program = AtomicLisp.at(contract.address);
				console.log(web3.toAscii(program.run()));
				//console.log(program.run());
			}
		}
	});
}

if(web3.isConnected()) {
	console.log("Web3 connected to blockchain network");
	executeContract();
}else{
	console.log("Web3 could not connect to blockchain network");
}