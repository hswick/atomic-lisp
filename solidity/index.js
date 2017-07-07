fs = require('fs');
Web3 = require('web3');

web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:8545"));

greeterABI = fs.readFileSync("greeter.sol:greeter.abi").toString();
greeterBin = fs.readFileSync("greeter.sol:greeter.bin");

var Greeter = web3.eth.contract(JSON.parse(greeterABI));

function executeContract() {
	var account = web3.eth.accounts[0];
	var deployedContract = Greeter.new("Hello there!",
	{data: greeterBin, from: account, gas: 3000000},
	function(e, contract) {
		if (e) {
			console.log(e);
		}else{
			if(!contract.address) {//contract has not been mined
				console.log("Contract transaction send: TransactionHash: " + contract.transactionHash + " waiting to be mined...");
			}else{
				console.log("Contract mined");
				console.log(contract.address);
				greeterInstance = Greeter.at(account);
				console.log(greeterInstance.greet());
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