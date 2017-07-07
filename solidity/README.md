You will need to install node.js

You will also need to install the solc compiler

http://solidity.readthedocs.io/en/develop/installing-solidity.html

With node we can install ethereumjs-testrpc, web3, and solc

Make sure you initialize project first
`npm init`

`npm install solc testrpc web3`


Compile your solidity contracts:
`./node_modules/.bin/solcjs "AtomicLisp.sol" --abi --bin`

This will create abi and binary files for your contracts. You can load them up in your script for later use.

Start testrpc in another window
`./node_modules/.bin/testrpc`

Then you can run your script `node index.js`