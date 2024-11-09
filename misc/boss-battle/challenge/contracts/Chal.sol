// SPDX-License-Identifier: UNLICENSED

// Challenge author: vakzz, Zellic Inc.
// Challenge prepared for CSAW CTF 2024
// forge build --evm-version cancun --via-ir Chal.sol

pragma solidity ^0.8.28;

contract Chal {
    struct Character {
        uint16 health;
        uint16 attack;
        uint16 defense;
        Item item;
    }

    struct Item {
        uint16 attackBonus;
        uint16 defenseBonus;
    }

    // Bosses for each level
    Character[] public bosses;

    // Player character and current level
    Character public playerCharacter;
    uint256 public playerLevel;

    // Items arrays
    Item[0x10000] public items;

    constructor() {
        // Initialize player level
        playerLevel = 1;

        // Initialize bosses for levels 1 to 3
        bosses.push(Character(50, 20, 10, Item(0, 0))); // Level 1 Boss
        bosses.push(Character(200, 50, 20, Item(0, 0))); // Level 2 Boss
        bosses.push(Character(10000, 250, 250, Item(0, 0))); // Level 3 Boss
    }

    // Function to generate an item based on item ID and category
    function generateItem(uint256 itemId, uint16 category) internal pure returns (Item memory) {
        uint16 baseAttack;
        uint16 baseDefense;

        if (category == 0) {
            // Player item
            baseAttack = 1;
            baseDefense = 1;
        } else if (category <= 4) {
            // Boss item
            uint16 multiplier = category * category * category;
            baseAttack = multiplier * 10;
            baseDefense = multiplier * 5;
        } else {
            // Invalid category
            return Item(0, 0);
        }

        uint16 attackBonus =
            baseAttack + toUint16(uint256(keccak256(abi.encodePacked(itemId, category, "attack"))) % 20) + 1;
        uint16 defenseBonus =
            baseDefense + toUint16(uint256(keccak256(abi.encodePacked(itemId, category, "defense"))) % 20) + 1;

        return Item(attackBonus, defenseBonus);
    }

    function getItem(uint256 index) public returns (Item memory) {
        if (items[index].attackBonus == 0) {
            items[index] = generateItem(index % 0x100, toUint16(index / 0x100));
        }
        return items[index];
    }

    function getPlayerItem(uint256 i) internal returns (Item memory) {
        uint8 index = toUint8(i);
        if (items[index].attackBonus == 0) {
            items[index] = generateItem(index % 0x100, toUint16(index / 0x100));
        }
        return items[index];
    }

    function equipItem(uint256 itemId) public {
        playerCharacter.item = getPlayerItem(itemId);
    }

    function createCharacter() public {
        playerCharacter = Character(randomStat(100, 1), randomStat(50, 2), randomStat(50, 3), Item(0, 0));
        playerLevel = 1; // Start at level 1

        // Reset boss health
        for (uint256 i = 0; i < bosses.length; i++) {
            bosses[i].health = (i == 0) ? 50 : (i == 1) ? 200 : 10000;
        }
    }

    function fight() public returns (string memory) {
        require(playerCharacter.health > 0, "Create a character first");
        uint256 currentLevel = playerLevel;
        require(currentLevel <= bosses.length, "You have completed all levels!");

        uint256 bossIndex = currentLevel - 1;

        Character storage boss = bosses[bossIndex];

        // Get player's equipped item bonuses
        Item storage playerItem = playerCharacter.item;

        // Get the boss item
        uint256 bossItemId = bossIndex * 256 + randomStat(256, 8);
        boss.item = getItem(bossItemId);

        // Calculate total stats with item bonuses
        uint16 playerAttack = playerCharacter.attack + playerItem.attackBonus;
        uint16 playerDefense = playerCharacter.defense + playerItem.defenseBonus;

        uint16 bossAttack = boss.attack + boss.item.attackBonus;
        uint16 bossDefense = boss.defense + boss.item.defenseBonus;

        uint16 playerDamage = playerAttack > bossDefense ? playerAttack - bossDefense : 0;
        uint16 bossDamage = bossAttack > playerDefense ? bossAttack - playerDefense : 0;

        if (playerDamage >= boss.health && bossDamage >= playerCharacter.health) {
            // Both die
            playerCharacter.health = 0;
            boss.health = 0;
            return "It's a draw!";
        } else if (playerDamage >= boss.health) {
            // Player wins: advance to next level
            playerLevel++;
            boss.health = 0;
            return "You defeated the boss and advanced to the next level!";
        } else if (bossDamage >= playerCharacter.health) {
            // Boss wins: player needs to reset or try again
            playerCharacter.health = 0;
            return "You were defeated by the boss!";
        } else {
            // Both survive: update health and continue
            boss.health = boss.health - playerDamage;
            playerCharacter.health = playerCharacter.health - bossDamage;

            return "The battle continues!";
        }
    }

    function randomStat(uint16 max, uint256 seed) private view returns (uint16) {
        return toUint16(
            uint256(keccak256(abi.encodePacked(block.timestamp, blockhash(block.number - 1), msg.sender, seed)))
        ) % max + 1;
    }

    function toUint8(uint256 n) internal pure returns (uint8 result) {
        assembly {
            result := n
        }
    }

    function toUint16(uint256 n) internal pure returns (uint16 result) {
        assembly {
            result := n
        }
    }

    // Get player's current level
    function getPlayerLevel() public view returns (uint256) {
        return playerLevel;
    }

    function hasWon() public view returns (bool) {
        return playerLevel > bosses.length;
    }
}
