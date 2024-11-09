package virtualization

import kotlin.system.exitProcess

class Executor(program: GetProgram) {
    private val mappings: Map<Int, Int> // offset in memory of each data
    private val code: List<ULong>
    private var memory: IntArray
    private var registers: MutableMap<Int, List<Int>>
    private var zf: Int
    private var exit: Boolean

    private data class BitFields(
        val rhsMode: ULong,
        val rhsVal: ULong,
        val signBit: ULong,
        val lhsMode: ULong,
        val lhsVal: ULong
    )

    private fun decodeNumber(number: ULong): BitFields {
        val rhsMode = number and 0b11uL                      // Extracts the first 2 bits (LSB)
        val rhsVal = (number shr 2) and 0xFFuL               // Next 8 bits after rhs_mode
        val signBit = (number shr 10) and 0b1uL              // Next 1 bit after rhs_val
        val lhsMode = (number shr 11) and 0b11uL             // Next 2 bits after sign_bit
        val lhsVal = (number shr 13) and 0xFFuL              // Next 8 bits after lhs_mode

        return BitFields(rhsMode, rhsVal, signBit, lhsMode, lhsVal)
    }

    private fun writeToReg(regCode: ULong, value: ULong) {
        // Create a mutable list with 8 bytes
        val bytes = MutableList(8) { 0 }

        if(regCode < 18uL) {
            // Fill the list with each byte of the longValue in little-endian order
            for (i in 0 until 8) {
                bytes[i] = (value shr (i * 8) and 255u).toInt()
            }
        }
        else if (regCode == 18uL) {
            // Fill the list with each byte of the longValue in little-endian order
            for (i in 0 until 8) {
                bytes[i] = registers[regCode.toInt()]!![i]
            }
            // Modify LSB
            bytes[0] = (value and 255u).toInt()
        }
        else {
            throw Exception("Unsupported")
        }

        registers[regCode.toInt()] = bytes
    }

    private fun readFromReg(regCode: ULong): ULong {
        var ret = 0uL

        if(regCode < 18uL) {
            // Fill the list with each byte of the longValue in little-endian order
            for (i in 0 until 8) {
                ret = ret or ((registers[regCode.toInt()]!![i].toULong() shl (i * 8)))
            }
        }
        else if (regCode == 18uL) {
            ret = (registers[regCode.toInt()]!![0].toULong())
        }
        else {
            throw Exception("Unsupported")
        }

        return ret
    }

    private fun writeToMem(addr: ULong, value: ULong, size: Int) {
        for (i in 0 until size) {
            memory[addr.toInt() + i] = (value shr (i * 8) and 255u).toInt()
        }
    }

    private fun readByteFromMem(addr: ULong): Int {
        return memory[addr.toInt()].toInt()
    }

    private fun readULongFromMem(addr: ULong): ULong {
        var ret = 0uL

        for (i in 0 until 8) {
            ret = ret or ((memory[addr.toInt() + i].toULong() shl (i * 8)))
        }

        return ret
    }

    private fun mov(opm: ULong, lhs: ULong, rhs: ULong) {
        when (opm) {
            1uL -> { // REG_REG
                writeToReg(lhs, readFromReg(rhs))
            }
            2uL -> { // REG_IMM
                writeToReg(lhs, rhs)
            }
            3uL -> { // REG_LAB buffer
                writeToReg(lhs, mappings[rhs.toInt()]!!.toULong())
            }
            4uL -> { // REG_PTR
                val (rhsMode, rhsVal, signBit, lhsMode, lhsVal) = decodeNumber(rhs)
                val lhsSum: ULong = ( if ( lhsMode == 1uL ) readFromReg(lhsVal) else if (lhsMode == 2uL) mappings[lhsVal.toInt()]!!.toULong() else lhsVal )
                val rhsSum: ULong = ( if ( rhsMode == 1uL ) readFromReg(rhsVal) else if (rhsMode == 2uL) mappings[rhsVal.toInt()]!!.toULong() else rhsVal )
                writeToReg(lhs, readULongFromMem( if (signBit == 0uL) lhsSum + rhsSum else lhsSum - rhsSum))
            }
            5uL -> { // PTR_IMM, 1 bytes wide only
                val (rhsMode, rhsVal, signBit, lhsMode, lhsVal) = decodeNumber(lhs)
                val lhsSum: ULong = ( if ( lhsMode == 1uL ) readFromReg(lhsVal) else if (lhsMode == 2uL) mappings[lhsVal.toInt()]!!.toULong() else lhsVal )
                val rhsSum: ULong = ( if ( rhsMode == 1uL ) readFromReg(rhsVal) else if (rhsMode == 2uL) mappings[rhsVal.toInt()]!!.toULong() else rhsVal )
                writeToMem(if (signBit == 0uL) lhsSum + rhsSum else lhsSum - rhsSum, rhs, 1)
            }
            6uL -> { // PTR_REG
                val (rhsMode, rhsVal, signBit, lhsMode, lhsVal) = decodeNumber(lhs)
                val lhsSum: ULong = ( if ( lhsMode == 1uL ) readFromReg(lhsVal) else if (lhsMode == 2uL) mappings[lhsVal.toInt()]!!.toULong() else lhsVal )
                val rhsSum: ULong = ( if ( rhsMode == 1uL ) readFromReg(rhsVal) else if (rhsMode == 2uL) mappings[rhsVal.toInt()]!!.toULong() else rhsVal )
                writeToMem(if (signBit == 0uL) lhsSum + rhsSum else lhsSum - rhsSum, readFromReg(rhs), if (rhsVal == 18uL) 1 else 8)
            }
            else -> throw Exception("Unimplemented")
        }
    }
    private fun xor(opm: ULong, lhs: ULong, rhs: ULong) {
        when (opm) {
            1uL -> { // PTR_REG, 8 bytes only
                val (rhsMode, rhsVal, signBit, lhsMode, lhsVal) = decodeNumber(lhs)
                val lhsSum: ULong = ( if ( lhsMode == 1uL ) readFromReg(lhsVal) else if (lhsMode == 2uL) mappings[lhsVal.toInt()]!!.toULong() else lhsVal )
                val rhsSum: ULong = ( if ( rhsMode == 1uL ) readFromReg(rhsVal) else if (rhsMode == 2uL) mappings[rhsVal.toInt()]!!.toULong() else rhsVal )
                val lhsFinal: ULong = if (signBit == 0uL) lhsSum + rhsSum else lhsSum - rhsSum
                writeToMem(lhsFinal, readFromReg(rhs).xor(readULongFromMem(lhsFinal)), if (rhsVal == 18uL) 1 else 8)
            }
            2uL -> { // REG_REG
                writeToReg(lhs, readFromReg(rhs).xor(readFromReg(lhs)))
            }
            3uL -> { // REG_PTR
                val (rhsMode, rhsVal, signBit, lhsMode, lhsVal) = decodeNumber(rhs)
                val lhsSum: ULong = ( if ( lhsMode == 1uL ) readFromReg(lhsVal) else if (lhsMode == 2uL) mappings[lhsVal.toInt()]!!.toULong() else lhsVal )
                val rhsSum: ULong = ( if ( rhsMode == 1uL ) readFromReg(rhsVal) else if (rhsMode == 2uL) mappings[rhsVal.toInt()]!!.toULong() else rhsVal )
                val rhsFinal: ULong = if (signBit == 0uL) lhsSum + rhsSum else lhsSum - rhsSum
                writeToReg(lhs, rhsFinal)
            }
            else -> throw Exception("Unimplemented")
        }
    }


    private fun ror(opm: ULong, lhs: ULong, rhs: ULong) {
        val rorImpl = { inp: ULong, bits: ULong ->
            val nbits = 64uL
            var value = inp
            var count = nbits - bits

            count %= nbits
            var high = value shr ((nbits - count).toInt())
            value = value shl count.toInt()
            value = value or high

            value
        }
        when (opm) {
            1uL -> { // REG_IMM
                writeToReg(lhs, rorImpl(readFromReg(lhs), rhs))
            }
            2uL -> { // PTR_IMM, 8 bytes wide only
                val (rhsMode, rhsVal, signBit, lhsMode, lhsVal) = decodeNumber(lhs)
                val lhsSum: ULong = ( if ( lhsMode == 1uL ) readFromReg(lhsVal) else if (lhsMode == 2uL) mappings[lhsVal.toInt()]!!.toULong() else lhsVal )
                val rhsSum: ULong = ( if ( rhsMode == 1uL ) readFromReg(rhsVal) else if (rhsMode == 2uL) mappings[rhsVal.toInt()]!!.toULong() else rhsVal )
                val lhsFinal: ULong = if (signBit == 0uL) lhsSum + rhsSum else lhsSum - rhsSum
                writeToMem(lhsFinal, rorImpl(readULongFromMem(lhsFinal), rhs), 8)
            }
            else -> throw Exception("Unimplemented")
        }
    }
    private fun cmp(opm: ULong, lhs: ULong, rhs: ULong) {
        // REG_IMM only
        if (opm != 1uL) throw Exception("Unimplemented")
        val determineVal = readFromReg(lhs).toLong() - rhs.toLong()
        zf = if (determineVal == 0L) 1 else 0
    }
    private fun jne(lhs: ULong) {
        if (zf == 0) {
            writeToReg(17uL, lhs * 4uL)
        }
    }
    private fun jmp(lhs: ULong) {
        writeToReg(17uL, lhs * 4uL)
    }
    private fun call(lhs: ULong) {
        writeToReg(7uL, readFromReg(7uL) - 8uL)
        writeToMem(readFromReg(7uL), readFromReg(17uL), 8)
        writeToReg(17uL, lhs * 4uL)
    }
    private fun loop( lhs: ULong) {
        writeToReg(3uL, readFromReg(3uL) - 1uL)
        if (readFromReg(3uL) and 0xFFFFFFFFuL != 0uL) writeToReg(17uL, lhs * 4uL)
    }
    private fun push( lhs: ULong) {
        writeToReg(7uL, readFromReg(7uL) - 8uL)
        writeToMem(readFromReg(7uL), readFromReg(lhs), 8)
    }
    private fun pop( lhs: ULong ) {
        writeToReg(lhs, readULongFromMem(readFromReg(7uL)))
        writeToReg(7uL, readFromReg(7uL) + 8uL)
    }
    private fun repe() {
        //repe cmpsb
        while(readFromReg(3uL) != 0uL){
            if(readByteFromMem(readFromReg(5uL)) != readByteFromMem(readFromReg(6uL))) {
                zf = 0
                break
            }
            zf = 1
            writeToReg(3uL, readFromReg(3uL) - 1uL)
        }
    }
    private fun dec( lhs: ULong ) {
        writeToReg(lhs, readFromReg(lhs) - 1uL)
    }
    private fun inc( lhs: ULong ) {
        writeToReg(lhs, readFromReg(lhs) + 1uL)
    }
    private fun ret() {
        pop(17uL)
    }

    fun IntArray.toAsciiString(startIndex: Int, count: Int): String {
        val result = StringBuilder()
        for (i in startIndex until startIndex + count) {
            if (i in this.indices) {
                result.append(this[i].toChar())
            } else {
                break
            }
        }
        return result.toString()
    }

    fun writeAsciiBytesToMemory(start: Int, bytes: ByteArray, length: Int) {
        for (i in 0 until length) {
            if (i + start in memory.indices && i in bytes.indices) {
                memory[i + start] = bytes[i].toInt()
            }
        }
    }

    private fun syscall() {
        when (readFromReg(1uL)) {
            0uL -> {
                // sys_read
                if (readFromReg(6uL) != 0uL) throw Exception("UNSUPPORTED")
                val length = readFromReg(4uL)
                val start = readFromReg(5uL)
                val input = readlnOrNull() ?: ""
                val bytes = input.split(" ").mapNotNull { it.toIntOrNull()?.toByte() }.toByteArray()

                writeAsciiBytesToMemory(start.toInt(), bytes, length.toInt())
            }
            1uL -> {
                // sys_write
                if (readFromReg(6uL) != 1uL) throw Exception("UNSUPPORTED")
                val length = readFromReg(4uL)
                val start = readFromReg(5uL)
                val output = memory.toAsciiString(start.toInt(), length.toInt())
                print(output)
            }
            60uL -> {
                // sys_exit
                println("Goodbye")
                exitProcess(0)
            }
            else -> {
                throw Exception("UNSUPPORTED")
            }
        }
    }

    private fun processOpcode(ops: ULong, opm: ULong, lhs: ULong, rhs: ULong) {
        when (ops) {
            1uL -> mov(opm, lhs, rhs)
            2uL -> xor(opm, lhs, rhs)
            3uL -> ror(opm, lhs, rhs)
            4uL -> cmp(opm, lhs, rhs)
            5uL -> throw Exception("Unimplemented")
            6uL -> jne(lhs)
            7uL -> jmp(lhs)
            8uL -> call( lhs)
            9uL -> loop( lhs)
            10uL -> push(lhs)
            11uL -> pop( lhs)
            12uL -> repe()
            13uL -> dec( lhs)
            14uL -> inc( lhs)
            15uL -> throw Exception("Unimplemented")
            16uL -> ret()
            17uL -> syscall()
            else -> throw Exception("Unimplemented")
        }
    }

    fun run() {
        while (!exit) {
            val (ops, opm, lhs, rhs) = code.slice(readFromReg(17uL).toInt() until readFromReg(17uL).toInt() + 4)
            // RIP += 4
            writeToReg(17uL, readFromReg(17uL) + 4uL)
            processOpcode(ops, opm, lhs, rhs)
        }
    }

    init {
        this.code = program.code
        this.memory = IntArray(100000) // arbitrary
        this.registers = (1..18).associateWith { listOf(0,0,0,0,0,0,0,0) }.toMutableMap()
        this.zf = 0
        this.exit = false


        //populate the memory
        var ctr = 0
        this.mappings = program.buf.map { (lhs, rhs) ->
            val ptr = ctr
            rhs.forEach{b ->
                this.memory[ctr] = b
                ctr++
            }
            lhs.toInt() to ptr
        }.toMap()

        //set up RSP
        writeToReg(7uL, 20000uL)
    }
}