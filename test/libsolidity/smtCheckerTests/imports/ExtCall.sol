==== Source: ExtCall.sol ====
// SPDX-License-Identifier: GPL-3.0-or-later
pragma solidity ^0.8.6;

interface Unknown {
	function callme() external;
}

contract ExtCall {
	uint x;

	bool lock;
	modifier mutex {
		require(!lock);
		lock = true;
		_;
		lock = false;
	}

	function setX(uint y) mutex public {
		x = y;
	}

	function xMut(Unknown u) public {
		uint x_prev = x;
		u.callme();
		assert(x_prev == x);
	}
}
==== Source: ExtCall.t.sol ====
// SPDX-License-Identifier: GPL-3.0-or-later
pragma solidity ^0.8.6;

import "ExtCall.sol";

contract ExtCallTest {
    ExtCall call;

    function setUp() public {
        call = new ExtCall();
    }
}
// ====
// SMTEngine: all
// SMTIgnoreCex: yes
// ----
// Warning 6328: (ExtCall.sol:360-379): CHC: Assertion violation happens here.
// Warning 4588: (ExtCall.t.sol:180-193): Assertion checker does not yet implement this type of function call.
