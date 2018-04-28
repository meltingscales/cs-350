#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc3.h"

static void usage(char * argv[])
{
    printf("Usage: %s <.hex file>\n", argv[0]);
    exit(0);
}

static int
read_hex_number(FILE * datafile, unsigned int * value_read)
{

#define HEX_BUFFER_LEN 80
    char hex_buffer[HEX_BUFFER_LEN];
    int words_read;
    char * read_success;
    int found_nbr = 0;
    read_success = fgets(hex_buffer, HEX_BUFFER_LEN, datafile);

    while (read_success != NULL)
    {
        words_read = sscanf(hex_buffer, "%x", value_read);

        if (hex_buffer[0] == ';')
        {
            words_read = 0;
        }

        if (words_read > 0)
        {
            return 1;
        }
        read_success = fgets(hex_buffer, HEX_BUFFER_LEN, datafile);
    }

    return 0;
} 

static const char
cc2char(cpu_t * cpu)
{
    cc_t cc = cpu->cc;

    if (cc == CC_NEG)
    {
        return 'N';
    }
    else if (cc == CC_ZER)
    {
        return 'Z';
    }
    else if (cc == CC_POS)
    {
        return 'P';
    }
    else
    {
        return '?';
    }
}

static void set_cc(cpu_t * cpu, word_t value, instr_t * instr)
{
    cc_t cc = 0;

    if (value > 0)
    {
        cc = CC_POS;
    }
    else if (value == 0)
    {
        cc = CC_ZER;
    }
    else
    {
        cc = CC_NEG;
    }

    cpu->cc = cc;
    printf("CC = %c", cc2char(cpu));

    if (instr->opcode != 0xF)
    {
        printf("\n");
    }
} 

static word_t
field(word_t value, int high, int low)
{

    word_t mask = (1 << (high - low + 1)) - 1;

    return (value >> low) & mask;
}

static word_t
sign_ext(word_t value, int high, int low)
{

    word_t val = value << (15 - high);
    word_t val2 = val >> (15 - high + low);

    return val2;

}

static void print_op(char * op)
{
    printf("%-5s %13s", op, "");
}

static void print_val(char * op, word_t val)
{
    printf("%-5s %-6d %6s", op, val, "");
}

static void print_hexval(char * op, word_t val)
{
    printf("%-5s x%-2hX %9s", op, val, "");
}

static void print_reg_val(char * op, reg_t reg, word_t val)
{
    printf("%-5s R%d, %-5d %3s", op, reg, val, "");
}

static void print_2reg_val(char * op, reg_t r1, reg_t r2, word_t val)
{
    printf("%-5s R%d, R%d, %-4d ", op, r1, r2, val);
}

static void print_reg(char * op, reg_t reg)
{
    printf("%-5s R%d %10s", op, reg, "");
}

static void print_2reg(char * op, reg_t r1, reg_t r2)
{
    printf("%-5s R%d, R%d %6s", op, r1, r2, "");
}

static void print_3reg(char * op, reg_t r1, reg_t r2, reg_t r3)
{
    printf("%-5s R%d, R%d, R%d %2s", op, r1, r2, r3, "");
}

void print_instr(instr_t * instr)
{
    char * op_mnemonic[16] = { "BR",  "ADD",  "LD",   "ST",  "JSR",  "AND",  "LDR", "STR",
                               "RTI", "NOT",  "LDI",  "STI", "JMP",  "ERR",  "LEA", "TRAP" };
    int op_field = instr->opcode;
    char * op = op_mnemonic[op_field];
    char * br_mnemonic[8] = { "NOP", "BRP", "BRZ", "BRZP", "BRN", "BRNP", "BRNZ", "BR" };

    switch (op_field)
    {
        case 0:
        {
            br_t * b = (br_t *)instr;
            int cc_field = (b->val >> 9) & 0x7;
            print_val(br_mnemonic[cc_field], sign_ext(instr->val, 8, 0));
            break;
        }
        case 1:
        case 5:
        {
            add_and_reg_t * a = (add_and_reg_t *)instr;

            if (a->flag == 0)
            {
                print_3reg(op, a->dr, a->sr1, a->sr2);
            }
            else
            {
                add_and_imm_t * addi = (add_and_imm_t *)instr;
                print_2reg_val(op, addi->dr, addi->sr1, sign_ext(addi->val, 4, 0));
            }
            break;
        }
        case 2:
        case 3:
        case 10:
        case 11:
        case 14:
        {
            ld_ldi_lea_t * l = (ld_ldi_lea_t *)instr;
            print_reg_val(op, l->dr, sign_ext(l->val, 8, 0));
            break;
        }
        case 4:
        {
            jsr_t * jsr = (jsr_t *)instr;

            if (jsr->flag == 0)
            {
                jsrr_t * jsrr = (jsrr_t *)instr;
                print_reg("JSRR", jsrr->baser);
            }
            else
            {
                print_val("JSR", sign_ext(jsr->val, 10, 0));
            }
            break;
        }
        case 6:
        case 7:
        {
            ldr_t * l = (ldr_t *)instr;
            print_2reg_val(op, l->dr, l->baser, sign_ext(l->val, 5, 0));
            break;
        }
        case 8:
        case 13:
            print_op(op);
            break;
        case 9:
        {
            not_t * n = (not_t *)instr;
            print_2reg(op, n->dr, n->sr);
            break;
        }
        case 12:
        {
            jmp_ret_t * j = (jmp_ret_t *)instr;
            print_reg(op, j->baser);
            break;
        }
        case 15:
        {
            trap_t * t = (trap_t *)instr;
            print_hexval(op, t->trapvect8);
            break;
        }
        default:
            break;
    } 
} 

static void print_addr_val_hex_instr(address_t addr, instr_t * instr)
{
    printf("x%04hX: x%04hX  ", addr, instr->val);
    print_instr(instr);
}

static void print_addr_val_hex_dec_instr(address_t addr, instr_t * instr)
{
    if (instr->val != 0)
    {
        printf("x%04hX: x%04hX  % 6d  ", addr, instr->val, instr->val);
        print_instr(instr);
        printf("\n");
    }
}

static void help_msg(void)
{
    printf("Simulator commands:\n");
    printf("h, H, or ? for help (prints this message)\n");
    printf("q or Q to quit\n");
    printf("d or D to dump the control unit and memory\n");
    printf("An integer > 0 to execute that many instruction cycles\n");
    printf("Or just return, which executes one instruction cycle\n");
}

static void decode_instr(word_t ir, instr_t * instr)
{

    instr->val = ir;
}

static void instr_br(cpu_t * cpu, instr_t * instr)
{
    br_t * branch_instr = (br_t *)instr;

    uint8_t cc = 4 * (branch_instr->n) + 2 * (branch_instr->z) + branch_instr->p;

    printf("; CC = %c, ", cc2char(cpu));

    if ((cc & cpu->cc) == 0)
    {
        printf("no branch\n");
    }
    else
    {
        address_t target = (cpu->pc) + sign_ext(branch_instr->pcoffset9, 8, 0);
        printf("PC <- x%hX%+d = x%hX\n", cpu->pc, branch_instr->pcoffset9, target);
        cpu->pc = target;
    }
}

static void instr_add(cpu_t * cpu, instr_t * instr)
{

    add_and_reg_t * add_instr = (add_and_reg_t *)instr;

    int r1 = cpu->reg[add_instr->sr1];

    if (add_instr->flag == 1)
    {
        add_and_imm_t * a = (add_and_imm_t *)instr;
        word_t sr1 =  cpu->reg[add_instr->sr1];
        cpu->reg[add_instr->dr] = sr1 + sign_ext(a->imm5, 4, 0);

        printf("; R%d <- x%hX %+d = x%hX; ", add_instr->dr, sr1, sign_ext(a->imm5, 4, 0), cpu->reg[add_instr->dr]);
        set_cc(cpu, cpu->reg[add_instr->dr], instr);
    }
    else
    {
        word_t sr1 =  cpu->reg[add_instr->sr1];
        word_t r2 = cpu->reg[add_instr->sr2];
        cpu->reg[add_instr->dr] = sr1 + r2;
        printf("; R%d <- x%hX + x%hX = x%hX; ", add_instr->dr, sr1, r2, cpu->reg[add_instr->dr]);
        set_cc(cpu, cpu->reg[add_instr->dr], instr);
    }

} 

static void instr_ld(cpu_t * cpu, instr_t * instr)
{

    ld_ldi_lea_t * l = (ld_ldi_lea_t *)instr;

    cpu->reg[l->dr] = cpu->mem[(cpu->pc) + sign_ext(l->pcoffset9, 8, 0)];

    printf("; R%d <- M[x%04hX] = x%hX; ", l->dr, cpu->pc + sign_ext(l->pcoffset9, 8, 0), cpu->reg[l->dr]);
    set_cc(cpu, cpu->reg[l->dr], instr);

}

static void instr_st(cpu_t * cpu, instr_t * instr)
{
    st_sti_t * s = (st_sti_t *)instr;

    cpu->mem[(cpu->pc) + sign_ext(s->pcoffset9, 8, 0)] = cpu->reg[s->sr];

    printf("; M[x%04hX] <- x%hX\n", ((cpu->pc) + sign_ext(s->pcoffset9, 8, 0)), cpu->reg[s->sr]);
}

static void instr_jsr(cpu_t * cpu, instr_t * instr)
{
    jsr_t * j = (jsr_t *)instr;

    if (j->flag == 1)
    {
        word_t temp = cpu->pc;
        cpu->pc = (cpu->pc) + sign_ext(j->pcoffset11, 10, 0);
        cpu->reg[7] = temp;
        printf("; PC <- x%hX %+d = x%hX, R7 <- x%hX\n", temp, sign_ext(j->pcoffset11, 10, 0), cpu->pc, temp);
    }
    else
    {
        jsrr_t * jsrr = (jsrr_t *)instr;
        word_t temp = cpu->pc;
        cpu->pc = cpu->reg[jsrr->baser];
        cpu->reg[7] = temp;
        printf("; PC <- x%hX, R7 <- x%hX\n",  cpu->reg[jsrr->baser], temp);
    }

}

static void instr_and(cpu_t * cpu, instr_t * instr)
{
    add_and_reg_t * a = (add_and_reg_t *)instr;

    int r1 = cpu->reg[a->sr1];

    if (a->flag == 1)
    {
        add_and_imm_t * cool = (add_and_imm_t *)instr;
        word_t sr1 =  cpu->reg[a->sr1];

        cpu->reg[a->dr] = sr1 & sign_ext(cool->imm5, 4, 0);

        printf("; R%d <- x%04hX & x%04hX = x%hX; ", a->dr, sr1, sign_ext(cool->imm5, 4, 0), cpu->reg[a->dr]);
        set_cc(cpu, cpu->reg[a->dr], instr);
    }
    else
    {
        word_t sr1 = cpu->reg[a->sr1];
        word_t sr2 = cpu->reg[a->sr2];
        cpu->reg[a->dr] = sr1 & sr2;
        printf("; x%04hX & x%04hX = x%hX; ", sr1, sr2, cpu->reg[a->dr]);
        set_cc(cpu, cpu->reg[a->dr], instr);
    }

} 

static void instr_ldr(cpu_t * cpu, instr_t * instr)
{
    ldr_t * l = (ldr_t *)instr;

    cpu->reg[l->dr] = cpu->mem[(cpu->reg[l->baser]) + sign_ext(l->offset6, 5, 0)];

    printf("; R%d <- M[x%04hX %+d] = M[x%04hX] = x%hX; ", l->dr, cpu->reg[l->baser], sign_ext(l->offset6, 5, 0), cpu->reg[l->baser] + sign_ext(l->offset6, 5, 0), cpu->reg[l->dr]);
    set_cc(cpu, cpu->reg[l->dr], instr);

}

static void instr_str(cpu_t * cpu, instr_t * instr)
{
    str_t * s = (str_t *)instr;

    cpu->mem[cpu->reg[s->baser] + sign_ext(s->offset6, 5, 0)] = cpu->reg[s->sr];

    printf("; M[x%04hX %+d] = M[x%04hX] <- x%hX\n",
           cpu->reg[s->baser],  sign_ext(s->offset6, 5, 0), cpu->reg[s->baser] + sign_ext(s->offset6, 5, 0), cpu->reg[s->sr]);

}

static void instr_rti(cpu_t * cpu, instr_t * instr)
{
    printf("; Opcode ignored\n");
}

static void instr_not(cpu_t * cpu, instr_t * instr)
{
    not_t * n = (not_t *)instr;

    cpu->reg[n->dr] = ~(cpu->reg[n->sr]);

    printf("; R%d <- NOT x%04hX = x%04hX; ", n->dr, ~(cpu->reg[n->sr]), cpu->reg[n->sr]);
    set_cc(cpu, cpu->reg[n->dr], instr);

}

static void instr_ldi(cpu_t * cpu, instr_t * instr)
{
    ld_ldi_lea_t * l = (ld_ldi_lea_t *)instr;

    address_t addr = cpu->pc + sign_ext(l->pcoffset9, 8, 0);
    address_t final_addr = cpu->mem[addr];

    cpu->reg[l->dr] = cpu->mem[final_addr];

    printf("; R%d <- M[M[x%04hX]] = M[x%04hX]  = x%hX; ",
           l->dr,
           addr,
           final_addr,
           cpu->reg[l->dr]
           );

    set_cc(cpu, cpu->reg[l->dr], instr);

}

static void instr_sti(cpu_t * cpu, instr_t * instr)
{
    st_sti_t * s = (st_sti_t *)instr;

    address_t address = (cpu->pc + sign_ext(s->pcoffset9, 8, 0));
    address_t finalAddress = cpu->mem[address];

    cpu->mem[finalAddress] = cpu->reg[s->sr];
    printf("; M[M[x%04hX]]= M[x%04hX] <- x%hX\n",
           address, finalAddress, cpu->reg[s->sr]);

}

static void instr_jmp(cpu_t * cpu, instr_t * instr)
{
    jmp_ret_t * j = (jmp_ret_t *)instr;

    cpu->pc =  cpu->reg[j->baser];

    printf("; PC <- x%hX\n", cpu->reg[j->baser]);

}

static void instr_err(cpu_t * cpu, instr_t * instr)
{
    printf("; Reserved opcode; ignored.\n");
}

static void instr_lea(cpu_t * cpu, instr_t * instr)
{
    ld_ldi_lea_t * l = (ld_ldi_lea_t *)instr;

    address_t targetAddress = (cpu->pc + sign_ext(l->pcoffset9, 8, 0));

    cpu->reg[l->dr] = targetAddress;

    printf("; R%d <- x%hX; ", l->dr, targetAddress);
    set_cc(cpu, cpu->reg[l->dr], instr);

}

static char read_char(void)
{
    char buffer[3] = "";

    fgets(buffer, sizeof(buffer), stdin);
    return buffer[0];
}

static int char_part(word_t w)
{
    return sign_ext(w, 7, 0);
}

static void instr_trap(cpu_t * cpu, instr_t * instr)
{
    trap_t * t = (trap_t *)instr;

    cpu->reg[7] = (word_t)(cpu->pc);

    char ch;
    word_t left_mask = 0xff00;
    word_t right_mask = 0x00ff;

    printf("; ");

    switch (t->trapvect8)
    {
        case 0x20:
        case 0x23:

            set_cc(cpu, cpu->reg[7], instr);
            if (t->trapvect8 == 0x20)
            {
                printf("; GETC: ");
            }
            else
            {
                printf("; IN: Input a character>");
            }

            ch = read_char();

            cpu->reg[0] = (left_mask & cpu->reg[0]) | (right_mask & ch);
            printf("Read %c = %d\n", (char)ch, ch);
            break;

        case 0x21:

            ch = char_part(cpu->reg[0]);
            set_cc(cpu, -1, instr);
            printf("; OUT: %d = %c\n", ch, (char)ch);
            break;

        case 0x22:

            set_cc(cpu, cpu->reg[7], instr);
            printf("; PUTS: ");
            address_t loc = cpu->reg[0];
            ch = (char)char_part(cpu->mem[loc]);

            while (ch != 0)
            {
                printf("%c", ch);
                loc = loc + 1;
                ch = (char)char_part(cpu->mem[loc]);
            }
            printf("\n");
            break;

        case 0x25:

            cpu->cc = CC_POS;
            cpu->running = 0;
            set_cc(cpu, cpu->cc, instr);
            printf("; Halting\n");
            break;

        default:
            printf("; Bad trap vector (halting)\n");
            cpu->running = 0;
            return;
    } 
} 

static void one_instruction_cycle(cpu_t * cpu)
{
    while (cpu->running == 0)
    {
        printf("Halted\n");
        return;
    }

    address_t instr_loc = cpu->pc;

    cpu->ir = cpu->mem[instr_loc];
    cpu->pc++;

    instr_t instr;
    decode_instr(cpu->ir, &instr);

    print_addr_val_hex_instr(instr_loc, &instr);

    switch (instr.opcode)
    {
        case  0: instr_br(cpu, &instr); break;
        case  1: instr_add(cpu, &instr); break;
        case  2: instr_ld(cpu, &instr); break;
        case  3: instr_st(cpu, &instr); break;
        case  4: instr_jsr(cpu, &instr); break;
        case  5: instr_and(cpu, &instr); break;
        case  6: instr_ldr(cpu, &instr); break;
        case  7: instr_str(cpu, &instr); break;
        case  8: instr_rti(cpu, &instr); break;
        case  9: instr_not(cpu, &instr); break;
        case 10: instr_ldi(cpu, &instr); break;
        case 11: instr_sti(cpu, &instr); break;
        case 12: instr_jmp(cpu, &instr); break;
        case 13: instr_err(cpu, &instr); break;
        case 14: instr_lea(cpu, &instr); break;
        case 15: instr_trap(cpu, &instr); break;
        default:
        {
            printf("Bad opcode: %d; quitting\n", instr.opcode);
            cpu->running = 0;
            break;
        }
    } 
} 

static void many_instruction_cycles(int nbr_cycles, cpu_t * cpu)
{
    if (nbr_cycles < 1)
    {
        printf("Number of instruction cycles to do should be > 0\n");
        return;
    }
    else if (!cpu->running)
    {
        printf("Halted\n");
        return;
    }

    if (nbr_cycles > MULTI_INSTR_LIMIT)
    {
        nbr_cycles = MULTI_INSTR_LIMIT;
    }

    int cycle;
    for (cycle = 0; cpu->running && cycle < nbr_cycles; cycle++)
    {
        one_instruction_cycle(cpu);
    }
} 

static int exec_cmd(char cmd_char, char * command, cpu_t * cpu)
{

    switch (cmd_char)
    {
        case '?':
        case 'h':
        case 'H':
        {
            help_msg();
            break;
        }
        case 'd':
        case 'D':
        {
            dump_cpu(cpu);
            dump_memory(cpu, 0, 0xffff);
            break;
        }
        case 'q':
        case 'Q':
        {
            printf("Quitting\n");
            return 1;
        }
        case '\n':
        {
            one_instruction_cycle(cpu);
            break;
        }
        default:
        {
            printf("There is no %c command.\n", cmd_char);
            break;
        }
    } 

    return 0;
} 

static int read_exec_cmd(cpu_t * cpu)
{
    int nbr_cycles;
    char cmd_char;
    int done = 0;
    int numbers_read = 0;

#define CMD_BUFFER_LEN 80
    char cmd_buffer[CMD_BUFFER_LEN];
    char * read_success;

    read_success = fgets(cmd_buffer, CMD_BUFFER_LEN, stdin);

    if (read_success == NULL)
    {
        return 1;
    }
    else
    {
        numbers_read = sscanf(cmd_buffer, "%d", &nbr_cycles);

        if (numbers_read > 0)
        {
            many_instruction_cycles(nbr_cycles, cpu);
        }
        else
        {
            done = exec_cmd(cmd_buffer[0], cmd_buffer, cpu);
        }

    }

    

    return done;
} 

void
init_cpu(cpu_t * cpu)
{
    cpu->pc = 00;
    cpu->ir = 0000;
    cpu->running = 1;
    cpu->cc = CC_ZER;
    int i;

    for (i = 0; i < NREG; i++)
    {
        cpu->reg[i] = 0;
    }
}

FILE * get_datafile(int argc, char * argv[])
{

    char * default_datafile_name = "default.hex";
    char * datafile_name;
    FILE * datafile = NULL;

    datafile_name = argv[1];

    printf("Loading %s\n", datafile_name);

    datafile = fopen(datafile_name, "r");

    if (datafile == NULL)
    {
        printf("ERROR: could not open file.\n");
        exit(EXIT_FAILURE);
    }

    return datafile;
} 

void init_memory(int argc, char * argv[], cpu_t * cpu)
{
    FILE * datafile = get_datafile(argc, argv);
    unsigned int value_read = 0;
    int success = 0;
    int i;

    for (i = 0; i < MEMLEN; i++)
    {
        cpu->mem[i] = 0;
    }

    success = read_hex_number(datafile, &value_read);

    if (success == 0)
    {
        printf("Error in reading the hex number!");
        exit(0);
    }

    printf("Origin = x%04hX\n", (word_t)value_read);

    cpu->pc = value_read;

    i = cpu->pc;

    while (success)
    {
        success = read_hex_number(datafile, &value_read);

        if (!success)
        {
            break;
        }

        cpu->mem[i] = value_read;

        i++;

        if (i > 0xFFFF)
        {
            i = 0x0000;
        }
    }

    fclose(datafile);
    printf("\n");
} 

void dump_cpu(cpu_t * cpu)
{
    printf("CPU STATE:\n");
    printf("PC = x%04hX   ", cpu->pc);
    printf("IR = x%04hX   ", cpu->ir);
    printf("CC = %c   ", cc2char(cpu));
    printf("RUNNING: %d\n", cpu->running);
    dump_gprs(cpu);
    printf("\n");
}

void dump_memory(cpu_t * cpu, address_t from, address_t to)
{
    if (to == (address_t)(from - 1))
    {
        printf("MEMORY (from x%04hX):\n", from);
    }
    else
    {
        printf("MEMORY (locations x%04hX to x%04hX):\n", from, to);
    }

    address_t addr = 0;
    for (addr = from; addr != to; addr++)
    {
        instr_t instr;
        decode_instr(cpu->mem[addr], &instr);
        print_addr_val_hex_dec_instr(addr, &instr);
    }

    instr_t instr;
    decode_instr(cpu->mem[to], &instr);
    print_addr_val_hex_dec_instr(addr, &instr);
    printf("\n");
} 

void dump_gprs(cpu_t * cpu)
{
    int regn;
    word_t w;

    for (regn = 0; regn < NREG; regn++)
    {
        w = cpu->reg[regn];
        printf("R%d x%04hX %- 6d%s", regn, w, w, (regn % 4 == 3 ? "\n" : "   "));
    }
}

int main(int argc, char * argv[])
{
    cpu_t cpu_value, * cpu = &cpu_value;

    printf("CS 350 Final Project: LC-3 Simulator ");

    if (argc != 2)
    {
        usage(argv);
    }

    init_cpu(cpu);
    init_memory(argc, argv, cpu);

    dump_cpu(cpu);
    dump_memory(cpu, cpu->pc, cpu->pc - 1);

    char * prompt = "$> ";
    printf("\nBeginning execution; type h for help\n%s", prompt);

    while (read_exec_cmd(cpu) != 1)
    {
        char * prompt = "$> ";
        printf("%s", prompt);
    }

    return 0;
} 
