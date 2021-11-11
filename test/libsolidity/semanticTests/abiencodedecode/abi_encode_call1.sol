pragma abicoder v2;
contract C {
	function f(uint256 p, string memory t) public {}

	function f0() public pure returns (bytes memory) {
		return abi.encodeWithSignature("f(uint256,string)", 1, "123");
	}
	function f0Call() public view returns (bytes memory) {
		return abi.encodeCall(this.f, (1, "123"));
	}
	function f1() public pure returns (bytes memory) {
		string memory x = "f(uint256,string)";
		return abi.encodeWithSignature(x, 1, "123");
	}
	function f1Call() public view returns (bytes memory) {
		return abi.encodeCall(this.f, (1,"123"));
	}
	string xstor;
	function f1s() public returns (bytes memory) {
		xstor = "f(uint256,string)";
		return abi.encodeWithSignature(xstor, 1);
	}
}
// ====
// compileViaYul: also
// ----
// f0() -> 0x20, 0x84, 12509815770525584054630284573553241396200922565476366893976854430870086352896, 26959946667150639794667015087019630673637144422540572481103610249216, 1725436586697640946858688965569256363112777243042596638790631055949824, 86060793054017993816230018372407419485142305772921726565498526629888, 0
// f0Call() -> 0x20, 0x84, 12509815770525584054630284573553241396200922565476366893976854430870086352896, 26959946667150639794667015087019630673637144422540572481103610249216, 1725436586697640946858688965569256363112777243042596638790631055949824, 86060793054017993816230018372407419485142305772921726565498526629888, 0
// f1() -> 0x20, 0x84, 12509815770525584054630284573553241396200922565476366893976854430870086352896, 26959946667150639794667015087019630673637144422540572481103610249216, 1725436586697640946858688965569256363112777243042596638790631055949824, 86060793054017993816230018372407419485142305772921726565498526629888, 0
// f1Call() -> 0x20, 0x84, 12509815770525584054630284573553241396200922565476366893976854430870086352896, 26959946667150639794667015087019630673637144422540572481103610249216, 1725436586697640946858688965569256363112777243042596638790631055949824, 86060793054017993816230018372407419485142305772921726565498526629888, 0
// f1s() -> 0x20, 0x24, 12509815770525584054630284573553241396200922565476366893976854430870086352896, 26959946667150639794667015087019630673637144422540572481103610249216
