# Challenge

<details> 
  <summary>Click to reveal flag</summary>
   csawctf{br0k3n_c0mp1l3r_for_th3_w1n}
</details>

## Description

This challenge is a simple battle simulator where a player generates a random character who can equip items, and has to defeat three bosses that get progressively harder with the final boss being impossible through normal means.

## Solution
 <details> 
  <summary>Click to reveal solution</summary>
The goal is to exploit a [bug in the solidity compiler](https://github.com/ethereum/solidity/issues/15142) that incorrectly truncates numbers when compiling with `viaIR`, allowing you to access array elements that should be out of bounds and to equipt items that are much stronger that normal.
</details>