#include <cpu_6502.h>
#include <iostream>

namespace nes
{
    uint8_t op_sz_matrix[0xFF] = { 0 };

    void setup_opcode_size_matrix();

    uint16_t cpu_6502::get_PC()
    {
        return PC;
    }

    std::string cpu_6502::get_state()
    {
        char message[1024];
        sprintf(message, " A:%.2X X:%.2X Y:%.2X P:%.2X SP:%.2X Cycles: %d", 
                A, X, Y, status, SP, cycles);
        return std::string(message);
    }

    void cpu_6502::setZ(uint16_t check)
    {
        if(check == 0) status |= 0x2;
        else status &= ~0x2; 
    }

    void cpu_6502::not_implemented(cpu_6502 *cpu, opcode_t *op)
    {
        char message[1024];
        sprintf(message, "0x%.4X: NOT IMPLEMENTED %s\n", cpu->get_PC(), op->to_string().c_str());
        throw std::string(message);
    }

    void cpu_6502::jmp_abs(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x4C
        cpu->PC = op->imm;
        cpu->cycles += 3;
    }

    void cpu_6502::stx_zp(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x86
        uint16_t addr = (0 << 8) | op->imm;
        cpu->mmu->write_byte(addr, cpu->X);
        cpu->cycles += 3;
        cpu->PC += op_sz_matrix[STX_zp];
    }

    void cpu_6502::ldx_imm(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xA2
        cpu->X = op->imm;
        cpu->cycles += 2;
        cpu->PC += op_sz_matrix[LDX_imm];
        cpu->setZ(cpu->X);
    }

    cpu_6502::cpu_6502()
    {
        mmu = new MMU();

        PC = 0xC000;

        uint16_t counter = 0;
        while(counter < 256)
        {
            opcode[counter] = not_implemented;
            counter++;
        }

        opcode[JMP_abs] = jmp_abs;
        opcode[STX_zp] = stx_zp;
        opcode[LDX_imm] = ldx_imm;
    }

    void cpu_6502::load_rom(std::string filename)
    {
        try
        {
            mmu->load_rom(filename); 
        }
        catch(std::string e)
        {
            throw e;
        }
    }

    std::string cpu_6502::step()
    {
        opcode_t *op = new opcode_t(mmu, PC);
        char message[1024];
        sprintf(message, "%.4X %.2X %s", PC, op->opcode,op->to_string().c_str());

        try
        {
            opcode[op->opcode](this, op);
        }
        catch(std::string e)
        {
            throw e;
        }

        std::string state = message + get_state();
        return state;
    }

    opcode_t::opcode_t(nes::MMU *mmu, uint16_t addr)
    {
        if(op_sz_matrix[JMP_abs] == 0) setup_opcode_size_matrix();

        opcode = mmu->read_byte(addr);
        sz = op_sz_matrix[opcode];
        instruction = opcode;

        switch(opcode)
        {
            case NOP_zp_04:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case ORA_zp:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_abs_0C:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case PHP:
                break;
            case NOP_zp_14:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case CLC:
                break;
            case NOP_1A:
                break;
            case NOP_abs_1C:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case BPL:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case JSR:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case BIT_zp:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case PLP:
                break;
            case AND_imm:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case BIT_abs:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case NOP_zp_34:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case SEC:
                break;
            case NOP_3A:
                break;
            case NOP_zp_44:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case PHA:
                break;
            case LSR_acc:
                break;
            case JMP_abs:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case NOP_zp_54:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_5A:
                break;
            case NOP_abs_5C:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case RTS:
                break;
            case NOP_zp_64:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case PLA:
                break;
            case BVS:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_zp_74:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case SEI:
                break;
            case NOP_7A:
                break;
            case NOP_abs_7C:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case NOP_imm_80:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case STY_zp_x:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case STA_zp:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case STX_zp:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case DEY:
                break;
            case STY_abs:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case STA_abs:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case STX_abs:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case TXS:
                break;
            case BCC:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDY_imm:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDA_zp:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDX_zp:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDX_imm:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDA_imm:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDA_abs:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case BCS:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDA_abs_x:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case DEC_zp:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case INY:
                break;
            case CMP_imm:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case DEX:
                break;
            case BNE:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_zp_D4:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case CLD:
                break;
            case NOP_DA:
                break;
            case NOP_abs_DC:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case CPX_imm:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case INC_zp:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case INX:
                break;
            case NOP_EA:
                break;
            case BEQ:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_zp_F4:
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_FA:
                break;
            case NOP_abs_FC:
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            default:
                char message[1024];
                sprintf(message, "Unknown opcode 0x%.2X", opcode);
                throw std::string(message);
                break;
        }
    }

    std::string opcode_t::to_string()
    {
        char temp[1024];

        switch(opcode)
        {
            case NOP_zp_04:
                sprintf(temp, "NOP(0x04) 0x%.2X, 0", imm);
                break;
            case ORA_zp:
                sprintf(temp, "ORA 0x%.2X, 0", imm);
                break;
            case NOP_abs_0C:
                sprintf(temp, "NOP(0x0C) 0x%.4X", imm);
                break;
            case PHP:
                sprintf(temp, "PHP");
                break;
            case NOP_zp_14:
                sprintf(temp, "NOP(0x14) 0x%.2X, 0", imm);
                break;
            case CLC:
                sprintf(temp, "CLC");
                break;
            case NOP_1A:
                sprintf(temp, "NOP(0x1A)");
                break;
            case NOP_abs_1C:
                sprintf(temp, "NOP(0x1C) 0x%.2X", imm);
                break;
            case BPL:
                sprintf(temp, "BPL #0x%.2X", imm);
                break;
            case JSR:
                sprintf(temp, "JSR 0x%.4X", imm);
                break;
            case BIT_zp:
                sprintf(temp, "BIT 0x%.2X", imm);
                break;
            case PLP:
                sprintf(temp, "PLP");
                break;
            case AND_imm:
                sprintf(temp, "AND #0x%.2X", imm);
                break;
            case BIT_abs:
                sprintf(temp, "BIT 0x%.4X", imm);
                break;
            case NOP_zp_34:
                sprintf(temp, "NOP(0x34) 0x%.2X, 0", imm);
                break;
            case SEC:
                sprintf(temp, "SEC");
                break;
            case NOP_3A:
                sprintf(temp, "NOP(0x3A)");
                break;
            case NOP_zp_44:
                sprintf(temp, "NOP(0x44) 0x%.2X, 0", imm);
                break;
            case PHA:
                sprintf(temp, "PHA");
                break;
            case LSR_acc:
                sprintf(temp, "LSR accumulator");
                break;
            case JMP_abs:
                sprintf(temp, "JMP 0x%.4X", imm);
                break;
            case NOP_zp_54:
                sprintf(temp, "NOP(0x54) 0x%.2X, 0", imm);
                break;
            case NOP_5A:
                sprintf(temp, "NOP(0x5A)");
                break;
            case NOP_abs_5C:
                sprintf(temp, "NOP(0x5C) 0x%.4X, X", imm);
                break;
            case RTS:
                sprintf(temp, "RTS");
                break;
            case NOP_zp_64:
                sprintf(temp, "NOP(0x64) 0x%.2X, 0", imm);
                break;
            case PLA:
                sprintf(temp, "PLA");
                break;
            case BVS:
                sprintf(temp, "BVS 0x%.2X, 0", imm);
                break;
            case NOP_zp_74:
                sprintf(temp, "NOP(0x74) 0x%.2X, 0", imm);
                break;
            case NOP_abs_7C:
                sprintf(temp, "NOP(0x7C) 0x%.4X, X", imm);
                break;
            case SEI:
                sprintf(temp, "SEI");
                break;
            case NOP_7A:
                sprintf(temp, "NOP(0x7A)");
                break;
            case NOP_imm_80:
                sprintf(temp, "NOP(0x80) 0x%.2X, 0", imm);
                break;
            case STY_zp_x:
                sprintf(temp, "STY 0x%.2X, X", imm);
                break;
            case STA_zp:
                sprintf(temp, "STA 0x%.2X, 0", imm);
                break;
            case STX_zp:
                sprintf(temp, "STX 0x%.2X, 0", imm);
                break;
            case DEY:
                sprintf(temp, "DEY");
                break;
            case STY_abs:
                sprintf(temp, "STY 0x%.4X", imm);
                break;
            case STA_abs:
                sprintf(temp, "STA 0x%.4X", imm);
                break;
            case STX_abs:
                sprintf(temp, "STX 0x%.4X", imm);
                break;
            case TXS:
                sprintf(temp, "TXS");
                break;
            case BCC:
                sprintf(temp, "BCC 0x%.2X", imm);
                break;
            case LDY_imm:
                sprintf(temp, "LDY #0x%.2X", imm);
                break;
            case LDX_imm:
                sprintf(temp, "LDX #0x%.2X", imm);
                break;
            case LDA_zp:
                sprintf(temp, "LDA 0x%.2X, 0", imm);
                break;
            case LDX_zp:
                sprintf(temp, "LDX 0x%.2X, 0", imm);
                break;
            case LDA_imm:
                sprintf(temp, "LDA #0x%.2X", imm);
                break;
            case LDA_abs:
                sprintf(temp, "LDA 0x%.4X", imm);
                break;
            case BCS:
                sprintf(temp, "BCS 0x%.2X", imm);
                break;
            case LDA_abs_x:
                sprintf(temp, "LDA 0x%.4X, X", imm);
                break;
            case DEC_zp:
                sprintf(temp, "DEC 0x%.2X, 0", imm);
                break;
            case INY:
                sprintf(temp, "INY");
                break;
            case CMP_imm:
                sprintf(temp, "CMP #0x%.2X, 0", imm);
                break;
            case DEX:
                sprintf(temp, "DEX");
                break;
            case BNE:
                sprintf(temp, "BNE #0x%.2X", imm);
                break;
            case NOP_zp_D4:
                sprintf(temp, "NOP(0xD4) 0x%.2X, 0", imm);
                break;
            case CLD:
                sprintf(temp, "CLD");
                break;
            case NOP_DA:
                sprintf(temp, "NOP(0xDA)");
                break;
            case NOP_abs_DC:
                sprintf(temp, "NOP(0xDC) 0x%.4X, X", imm);
                break;
            case CPX_imm:
                sprintf(temp, "CPX #0x%.2X", imm);
                break;
            case INC_zp:
                sprintf(temp, "INC 0x%.2X, 0", imm);
                break;
            case INX:
                sprintf(temp, "INX");
                break;
            case NOP_EA:
                sprintf(temp, "NOP(0xEA)");
                break;
            case BEQ:
                sprintf(temp, "BEQ 0x%.2X, 0", imm);
                break;
            case NOP_zp_F4:
                sprintf(temp, "NOP(0xF4) 0x%.2X, 0", imm);
                break;
            case NOP_FA:
                sprintf(temp, "NOP(0xFA)");
                break;
            case NOP_abs_FC:
                sprintf(temp, "NOP(0xFC) 0x%.4X, X", imm);
                break;
            default:
                sprintf(temp, "Unknown opcode 0x%.2X", opcode);
                break;
        }

        char message[1024];
        sprintf(message, "    %s", temp);
        return std::string(message);
    }

    void setup_opcode_size_matrix()
    {
        op_sz_matrix[NOP_zp_04] = 2;
        op_sz_matrix[ORA_zp] = 2;
        op_sz_matrix[NOP_abs_0C] = 3;
        op_sz_matrix[PHP] = 1;
        op_sz_matrix[BPL] = 2;
        op_sz_matrix[NOP_zp_14] = 2;
        op_sz_matrix[CLC] = 1;
        op_sz_matrix[NOP_1A] = 1;
        op_sz_matrix[NOP_abs_1C] = 2;
        op_sz_matrix[JSR] = 3;
        op_sz_matrix[BIT_zp] = 2;
        op_sz_matrix[PLP] = 1;
        op_sz_matrix[AND_imm] = 2;
        op_sz_matrix[BIT_abs] = 3;
        op_sz_matrix[NOP_zp_34] = 2;
        op_sz_matrix[SEC] = 1;
        op_sz_matrix[NOP_3A] = 1;
        op_sz_matrix[NOP_zp_44] = 2;
        op_sz_matrix[PHA] = 1;
        op_sz_matrix[LSR_acc] = 1;
        op_sz_matrix[JMP_abs] = 3;
        op_sz_matrix[NOP_zp_54] = 2;
        op_sz_matrix[NOP_5A] = 1;
        op_sz_matrix[NOP_abs_5C] = 3;
        op_sz_matrix[RTS] = 1; 
        op_sz_matrix[NOP_zp_64] = 2;
        op_sz_matrix[PLA] = 1;
        op_sz_matrix[BVS] = 2;
        op_sz_matrix[NOP_zp_74] = 2;
        op_sz_matrix[SEI] = 1;
        op_sz_matrix[NOP_7A] = 1;
        op_sz_matrix[NOP_abs_7C] = 3;
        op_sz_matrix[NOP_imm_80] = 2;
        op_sz_matrix[STY_zp_x] = 2;
        op_sz_matrix[STA_zp] = 2;
        op_sz_matrix[STX_zp] = 2;
        op_sz_matrix[DEY] = 1;
        op_sz_matrix[STY_abs] = 3;
        op_sz_matrix[STA_abs] = 3;
        op_sz_matrix[STX_abs] = 3;
        op_sz_matrix[TXS] = 1;
        op_sz_matrix[BCC] = 2;
        op_sz_matrix[LDY_imm] = 2;
        op_sz_matrix[LDX_imm] = 2;
        op_sz_matrix[LDA_zp] = 2;
        op_sz_matrix[LDX_zp] = 2;
        op_sz_matrix[LDA_imm] = 2;
        op_sz_matrix[LDA_abs] = 3;
        op_sz_matrix[BCS] = 2;
        op_sz_matrix[LDA_abs_x] = 3;
        op_sz_matrix[DEC_zp] = 2;
        op_sz_matrix[INY] = 1;
        op_sz_matrix[CMP_imm] = 2;
        op_sz_matrix[DEX] = 1;
        op_sz_matrix[BNE] = 2;
        op_sz_matrix[NOP_zp_D4] = 2;
        op_sz_matrix[CLD] = 1;
        op_sz_matrix[NOP_DA] = 1;
        op_sz_matrix[NOP_abs_DC] = 3;
        op_sz_matrix[CPX_imm] = 2;
        op_sz_matrix[INC_zp] = 2;
        op_sz_matrix[INX] = 1;
        op_sz_matrix[NOP_EA] = 1;
        op_sz_matrix[BEQ] = 2;
        op_sz_matrix[NOP_zp_F4] = 2;
        op_sz_matrix[NOP_FA] = 1;
        op_sz_matrix[NOP_abs_FC] = 3;
    }
}
