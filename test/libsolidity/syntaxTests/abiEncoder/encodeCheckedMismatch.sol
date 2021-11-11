
contract C {
    function f(int a) public {}
    function f2(int a, string memory b) public {}

	function fail() public returns(bytes memory) {
		return abi.encodeCall(this.f, ("test"));
	}
	function fail2() public returns(bytes memory) {
		return abi.encodeCall(this.f, (1, 2));
	}
	function fail3() public returns(bytes memory) {
		return abi.encodeCall(this.f, ());
	}
	function fail4() public returns(bytes memory) {
		return abi.encodeCall(this.f);
	}
	function fail5() public returns(bytes memory) {
		return abi.encodeCall(1, this.f);
	}
	function fail6() public returns(bytes memory) {
		return abi.encodeCall(abi.encodeCall, (1, 2, 3, "test"));
	}
	function success() public returns(bytes memory) {
		return abi.encodeCall(this.f, (1));
	}
	function success2() public returns(bytes memory) {
		return abi.encodeCall(this.f, 1);
	}
	function success3() public returns(bytes memory) {
		return abi.encodeCall(this.f2, (1, "test"));
	}
}
// ----
// TypeError 5407: (178-184): Cannot implicitly convert component at position 0 from "literal_string "test"" to "int256".
// TypeError 7788: (249-279): Expected 1 instead of 2 components for the tuple parameter.
// TypeError 7788: (342-368): Expected 1 instead of 0 components for the tuple parameter.
// TypeError 6219: (431-453): Expected two arguments: a function pointer followed by a tuple.
// TypeError 5511: (531-532): Expected first argument to be a function pointer, not "int_const 1".
// TypeError 3509: (619-633): Function must be "public" or "external". Did you forget to prefix "this."?
