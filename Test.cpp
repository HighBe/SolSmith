#include "Test.h"
#include "Variable.h"


string generateScript() {
    string res;
    res = "const {expect} = require(\"""chai\""");\n" \
        "const {loadFixture} = require(\"""@nomicfoundation/hardhat-network-helpers\""");\n" \
        "const web3 = require(\"""web3\""");\n\n" \
        "describe(\"ContractName\",function(){\n";
    res += "\tasync function deployOneYearLockFixture(){\n" \
        "\t\tconst _Contract = await ethers.getContractFactory(\"ContractName\");\n" \
        "\t\tconst [account0,account1,account2] = await ethers.getSigners();\n" \
        "\t\tconst _contract = await _Contract.deploy();\n" \
        "\t\treturn {_contract,account0,account1,account2};" \
        "\n\t}\n\n";
    res += "\tdescribe(\"func_a\", function(){\n"
        "\t\tit(\"testing func_a\", async function() {\n"
        "\t\t\tconst {_contract, account0, account1, account2} = await loadFixture(deployOneYearLockFixture);\n"
        "\t\t\tawait _contract.connect(account0).func_a();\n"
        "\t\t\tconst filter = {\n\t\t\t\tfromBlock:0,\n\t\t\t\ttoBlock : 50\n\t\t\t}\n"
        "\t\t\tconst events = await _contract.runner.provider.getLogs(filter);\n"
        "\t\t\tconst parseEvents = events.map((event) => _contract.interface.parseLog(event));\n"
        "\t\t\tfor (var i = 0; i < parseEvents.length; i++) {\n"
        "\t\t\t\tif (parseEvents[i].name == \"testtest\") {\n"
        "\t\t\t\t\tfor (var j = 0; j < parseEvents[i].args.length; j++) {\n"
        "\t\t\t\t\t\tconsole.log('', parseEvents[i].args[j]);\n"
        "\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t});\n\t});\n";
    res += "\tdescribe(\"func_b\", function(){\n"
        "\t\tit(\"testing func_b\", async function() {\n"
        "\t\t\tconst {_contract, account0, account1, account2} = await loadFixture(deployOneYearLockFixture);\n"
        "\t\t\tawait _contract.connect(account0).func_b();\n"
        "\t\t\tconst filter = {\n\t\t\t\tfromBlock:0,\n\t\t\t\ttoBlock : 50\n\t\t\t}\n"
        "\t\t\tconst events = await _contract.runner.provider.getLogs(filter);\n"
        "\t\t\tconst parseEvents = events.map((event) => _contract.interface.parseLog(event));\n"
        "\t\t\tfor (var i = 0; i < parseEvents.length; i++) {\n"
        "\t\t\t\tif (parseEvents[i].name == \"testtest\") {\n"
        "\t\t\t\t\tfor (var j = 0; j < parseEvents[i].args.length; j++) {\n"
        "\t\t\t\t\t\tconsole.log('', parseEvents[i].args[j]);\n"
        "\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t});\n\t});\n";
    res += "\tdescribe(\"func_c\", function(){\n"
        "\t\tit(\"testing func_c\", async function() {\n"
        "\t\t\tconst \{_contract, account0, account1, account2\} = await loadFixture(deployOneYearLockFixture);\n"
        "\t\t\tawait _contract.connect(account0).func_c();\n"
        "\t\t\tconst filter = {\n\t\t\t\tfromBlock:0,\n\t\t\t\ttoBlock : 50\n\t\t\t}\n"
        "\t\t\tconst events = await _contract.runner.provider.getLogs(filter);\n"
        "\t\t\tconst parseEvents = events.map((event) => _contract.interface.parseLog(event));\n"
        "\t\t\tfor (var i = 0; i < parseEvents.length; i++) {\n"
        "\t\t\t\tif (parseEvents[i].name == \"testtest\") {\n"
        "\t\t\t\t\tfor (var j = 0; j < parseEvents[i].args.length; j++) {\n"
        "\t\t\t\t\t\tconsole.log('', parseEvents[i].args[j]);\n"
        "\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t});\n\t});\n";
    res += "});";
    return res;
}