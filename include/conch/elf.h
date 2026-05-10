/* @file: elf.h
 * #desc:
 *    The definitions of standard elf types structures.
 *
 * #copy:
 *    Copyright (C) 1970 Public Free Software.
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not,
 *    see <https://www.gnu.org/licenses/>.
 */

#ifndef _CONCH_ELF_H
#define _CONCH_ELF_H

#include <conch/config.h>
#include <conch/c_stdint.h>


/* NOTE: elf types structures from glibc */

#define EI_NIDENT 16

/* elf file header */
struct elf32_ehdr {
	uint8_t e_ident[EI_NIDENT]; /* magic number */
	uint16_t e_type;            /* object file type */
	uint16_t e_machine;         /* architecture */
	uint32_t e_version;         /* object file version */
	uint32_t e_entry;           /* entry point virtual address */
	uint32_t e_phoff;           /* program header table file offset */
	uint32_t e_shoff;           /* section header table file offset */
	uint32_t e_flags;           /* processor-specific flags */
	uint16_t e_ehsize;          /* elf header size */
	uint16_t e_phentsize;       /* program header table entry size */
	uint16_t e_phnum;           /* program header table entry count */
	uint16_t e_shentsize;       /* section header table entry size */
	uint16_t e_shnum;           /* section header table entry count */
	uint16_t e_shstrndx;        /* section header string table index */
};

struct elf64_ehdr {
	uint8_t e_ident[EI_NIDENT]; /* magic number */
	uint16_t e_type;            /* object file type */
	uint16_t e_machine;         /* architecture */
	uint32_t e_version;         /* object file version */
	uint64_t e_entry;           /* entry point virtual address */
	uint64_t e_phoff;           /* program header table file offset */
	uint64_t e_shoff;           /* section header table file offset */
	uint32_t e_flags;           /* processor-specific flags */
	uint16_t e_ehsize;          /* elf header size */
	uint16_t e_phentsize;       /* program header table entry size */
	uint16_t e_phnum;           /* program header table entry count */
	uint16_t e_shentsize;       /* section header table entry size */
	uint16_t e_shnum;           /* section header table entry count */
	uint16_t e_shstrndx;        /* section header string table index */
};

/* elf*_ehdr.e_ident */
#define EI_MAG0 0     /* file identification byte 0 index */
#define ELF_MAG0 0x7f /* magic number byte 0 */
#define EI_MAG1 1     /* file identification byte 1 index */
#define ELF_MAG1 'E'  /* magic number byte 1 */
#define EI_MAG2 2     /* file identification byte 2 index */
#define ELF_MAG2 'L'  /* magic number byte 2 */
#define EI_MAG3 3     /* file identification byte 3 index */
#define ELF_MAG3 'F'  /* magic number byte 3 */

#define ELF_MAG  "\177ELF"
#define SELF_MAG 4

#define EI_CLASS       4 /* file class byte index */
#define ELF_CLASS_NONE 0 /* invalid class */
#define ELF_CLASS_32   1 /* 32-bit objects */
#define ELF_CLASS_64   2 /* 64-bit objects */
#define ELF_CLASS_NUM  3

#define EI_DATA       5 /* data encoding byte index */
#define ELF_DATA_NONE 0 /* invalid data encoding */
#define ELF_DATA_2LSB 1 /* 2's complement, little endian */
#define ELF_DATA_2MSB 2 /* 2's complement, big endian */
#define ELF_DATA_NUM  3

#define EI_VERSION  6 /* file version byte index */
		      /* value must be EV_CURRENT */

#define EI_OSABI             7   /* OS ABI identification byte index */
#define ELF_OSABI_NONE       0   /* UNIX System V ABI */
#define ELF_OSABI_SYSV       0   /* Alias */
#define ELF_OSABI_HPUX       1   /* HP-UX */
#define ELF_OSABI_NETBSD     2   /* NetBSD */
#define ELF_OSABI_GNU        3   /* Object uses GNU ELF extensions */
#define ELF_OSABI_LINUX      3   /* Compatibility alias  */
#define ELF_OSABI_SOLARIS    6   /* Sun Solaris  */
#define ELF_OSABI_AIX        7   /* IBM AIX  */
#define ELF_OSABI_IRIX       8   /* SGI Irix  */
#define ELF_OSABI_FREEBSD    9   /* FreeBSD  */
#define ELF_OSABI_TRU64      10  /* Compaq TRU64 UNIX  */
#define ELF_OSABI_MODESTO    11  /* Novell Modesto  */
#define ELF_OSABI_OPENBSD    12  /* OpenBSD */
#define ELF_OSABI_ARM_AEABI  64  /* ARM EABI */
#define ELF_OSABI_ARM        97  /* ARM */
#define ELF_OSABI_STANDALONE 255 /* Standalone (embedded) application */

#define EI_ABIVERSION 8 /* ABI version */
#define EI_PAD        9 /* byte index of padding bytes */

/* elf*_ehdr.e_type */
#define ET_NONE   0 /* no file type */
#define ET_REL    1 /* relocatable file */
#define ET_EXEC   2 /* executable file */
#define ET_DYN    3 /* shared object file */
#define ET_CORE   4 /* core file */
#define ET_NUM    5 /* number of defined types */

#define ET_LOOS   0xfe00 /* OS-specific range start */
#define ET_HIOS   0xfeff /* OS-specific range end */
#define ET_LOPROC 0xff00 /* processor-specific range start */
#define ET_HIPROC 0xffff /* processor-specific range end */

/* elf*_ehdr.e_machine */
#define EM_NONE          0   /* no machine */
#define EM_M32           1   /* AT&T WE 32100 */
#define EM_SPARC         2   /* SUN SPARC */
#define EM_386           3   /* Intel 80386 */
#define EM_68K           4   /* Motorola m68k family */
#define EM_88K           5   /* Motorola m88k family */
#define EM_IAMCU         6   /* Intel MCU */
#define EM_860           7   /* Intel 80860 */
#define EM_MIPS          8   /* MIPS R3000 big-endian */
#define EM_S370          9   /* IBM System/370 */
#define EM_MIPS_RS3_LE   10  /* MIPS R3000 little-endian */
/* reserved 11-14 */
#define EM_PARISC        15  /* HPPA */
/* reserved 16 */
#define EM_VPP500        17  /* Fujitsu VPP500 */
#define EM_SPARC32PLUS   18  /* Sun's "v8plus" */
#define EM_960           19  /* Intel 80960 */
#define EM_PPC           20  /* PowerPC */
#define EM_PPC64         21  /* PowerPC 64-bit */
#define EM_S390          22  /* IBM S390 */
#define EM_SPU           23  /* IBM SPU/SPC */
/* reserved 24-35 */
#define EM_V800          36  /* NEC V800 series */
#define EM_FR20          37  /* Fujitsu FR20 */
#define EM_RH32          38  /* TRW RH-32 */
#define EM_RCE           39  /* Motorola RCE */
#define EM_ARM           40  /* ARM */
#define EM_FAKE_ALPHA    41  /* Digital Alpha */
#define EM_SH            42  /* Hitachi SH */
#define EM_SPARCV9       43  /* SPARC v9 64-bit */
#define EM_TRICORE       44  /* Siemens Tricore */
#define EM_ARC           45  /* Argonaut RISC Core */
#define EM_H8_300        46  /* Hitachi H8/300 */
#define EM_H8_300H       47  /* Hitachi H8/300H */
#define EM_H8S           48  /* Hitachi H8S */
#define EM_H8_500        49  /* Hitachi H8/500 */
#define EM_IA_64         50  /* Intel Merced */
#define EM_MIPS_X        51  /* Stanford MIPS-X */
#define EM_COLDFIRE      52  /* Motorola Coldfire */
#define EM_68HC12        53  /* Motorola M68HC12 */
#define EM_MMA           54  /* Fujitsu MMA Multimedia Accelerator */
#define EM_PCP           55  /* Siemens PCP */
#define EM_NCPU          56  /* Sony nCPU embedded RISC */
#define EM_NDR1          57  /* Denso NDR1 microprocessor */
#define EM_STARCORE      58  /* Motorola Start*Core processor */
#define EM_ME16          59  /* Toyota ME16 processor */
#define EM_ST100         60  /* STMicroelectronic ST100 processor */
#define EM_TINYJ         61  /* Advanced Logic Corp. Tinyj emb.fam */
#define EM_X86_64        62  /* AMD x86-64 architecture */
#define EM_PDSP          63  /* Sony DSP Processor */
#define EM_PDP10         64  /* Digital PDP-10 */
#define EM_PDP11         65  /* Digital PDP-11 */
#define EM_FX66          66  /* Siemens FX66 microcontroller */
#define EM_ST9PLUS       67  /* STMicroelectronics ST9+ 8/16 mc */
#define EM_ST7           68  /* STmicroelectronics ST7 8 bit mc */
#define EM_68HC16        69  /* Motorola MC68HC16 microcontroller */
#define EM_68HC11        70  /* Motorola MC68HC11 microcontroller */
#define EM_68HC08        71  /* Motorola MC68HC08 microcontroller */
#define EM_68HC05        72  /* Motorola MC68HC05 microcontroller */
#define EM_SVX           73  /* Silicon Graphics SVx */
#define EM_ST19          74  /* STMicroelectronics ST19 8 bit mc */
#define EM_VAX           75  /* Digital VAX */
#define EM_CRIS          76  /* Axis Communications 32-bit emb.proc */
#define EM_JAVELIN       77  /* Infineon Technologies 32-bit emb.proc */
#define EM_FIREPATH      78  /* Element 14 64-bit DSP Processor */
#define EM_ZSP           79  /* LSI Logic 16-bit DSP Processor */
#define EM_MMIX          80  /* Donald Knuth's educational 64-bit proc */
#define EM_HUANY         81  /* Harvard University machine-independent
			        object files */
#define EM_PRISM         82  /* SiTera Prism */
#define EM_AVR           83  /* Atmel AVR 8-bit microcontroller */
#define EM_FR30          84  /* Fujitsu FR30 */
#define EM_D10V          85  /* Mitsubishi D10V */
#define EM_D30V          86  /* Mitsubishi D30V */
#define EM_V850          87  /* NEC v850 */
#define EM_M32R          88  /* Mitsubishi M32R */
#define EM_MN10300       89  /* Matsushita MN10300 */
#define EM_MN10200       90  /* Matsushita MN10200 */
#define EM_PJ            91  /* picoJava */
#define EM_OPENRISC      92  /* OpenRISC 32-bit embedded processor */
#define EM_ARC_COMPACT   93  /* ARC International ARCompact */
#define EM_XTENSA        94  /* Tensilica Xtensa Architecture */
#define EM_VIDEOCORE     95  /* Alphamosaic VideoCore */
#define EM_TMM_GPP       96  /* Thompson Multimedia General Purpose Proc */
#define EM_NS32K         97  /* National Semi. 32000 */
#define EM_TPC           98  /* Tenor Network TPC */
#define EM_SNP1K         99  /* Trebia SNP 1000 */
#define EM_ST200         100 /* STMicroelectronics ST200 */
#define EM_IP2K          101 /* Ubicom IP2xxx */
#define EM_MAX           102 /* MAX processor */
#define EM_CR            103 /* National Semi. CompactRISC */
#define EM_F2MC16        104 /* Fujitsu F2MC16 */
#define EM_MSP430        105 /* Texas Instruments msp430 */
#define EM_BLACKFIN      106 /* Analog Devices Blackfin DSP */
#define EM_SE_C33        107 /* Seiko Epson S1C33 family */
#define EM_SEP           108 /* Sharp embedded microprocessor */
#define EM_ARCA          109 /* Arca RISC */
#define EM_UNICORE       110 /* PKU-Unity & MPRC Peking Uni. mc series */
#define EM_EXCESS        111 /* eXcess configurable cpu */
#define EM_DXP           112 /* Icera Semi. Deep Execution Processor */
#define EM_ALTERA_NIOS2  113 /* Altera Nios II */
#define EM_CRX           114 /* National Semi. CompactRISC CRX */
#define EM_XGATE         115 /* Motorola XGATE */
#define EM_C166          116 /* Infineon C16x/XC16x */
#define EM_M16C          117 /* Renesas M16C */
#define EM_DSPIC30F      118 /* Microchip Technology dsPIC30F */
#define EM_CE            119 /* Freescale Communication Engine RISC */
#define EM_M32C          120 /* Renesas M32C */
/* reserved 121-130 */
#define EM_TSK3000       131 /* Altium TSK3000 */
#define EM_RS08          132 /* Freescale RS08 */
#define EM_SHARC         133 /* Analog Devices SHARC family */
#define EM_ECOG2         134 /* Cyan Technology eCOG2 */
#define EM_SCORE7        135 /* Sunplus S+core7 RISC */
#define EM_DSP24         136 /* New Japan Radio (NJR) 24-bit DSP */
#define EM_VIDEOCORE3    137 /* Broadcom VideoCore III */
#define EM_LATTICEMICO32 138 /* RISC for Lattice FPGA */
#define EM_SE_C17        139 /* Seiko Epson C17 */
#define EM_TI_C6000      140 /* Texas Instruments TMS320C6000 DSP */
#define EM_TI_C2000      141 /* Texas Instruments TMS320C2000 DSP */
#define EM_TI_C5500      142 /* Texas Instruments TMS320C55x DSP */
#define EM_TI_ARP32      143 /* Texas Instruments App. Specific RISC */
#define EM_TI_PRU        144 /* Texas Instruments Prog. Realtime Unit */
/* reserved 145-159 */
#define EM_MMDSP_PLUS    160 /* STMicroelectronics 64bit VLIW DSP */
#define EM_CYPRESS_M8C   161 /* Cypress M8C */
#define EM_R32C          162 /* Renesas R32C */
#define EM_TRIMEDIA      163 /* NXP Semi. TriMedia */
#define EM_QDSP6         164 /* QUALCOMM DSP6 */
#define EM_8051          165 /* Intel 8051 and variants */
#define EM_STXP7X        166 /* STMicroelectronics STxP7x */
#define EM_NDS32         167 /* Andes Tech. compact code emb. RISC */
#define EM_ECOG1X        168 /* Cyan Technology eCOG1X */
#define EM_MAXQ30        169 /* Dallas Semi. MAXQ30 mc */
#define EM_XIMO16        170 /* New Japan Radio (NJR) 16-bit DSP */
#define EM_MANIK         171 /* M2000 Reconfigurable RISC */
#define EM_CRAYNV2       172 /* Cray NV2 vector architecture */
#define EM_RX            173 /* Renesas RX */
#define EM_METAG         174 /* Imagination Tech. META */
#define EM_MCST_ELBRUS   175 /* MCST Elbrus */
#define EM_ECOG16        176 /* Cyan Technology eCOG16 */
#define EM_CR16          177 /* National Semi. CompactRISC CR16 */
#define EM_ETPU          178 /* Freescale Extended Time Processing Unit */
#define EM_SLE9X         179 /* Infineon Tech. SLE9X */
#define EM_L10M          180 /* Intel L10M */
#define EM_K10M          181 /* Intel K10M */
/* reserved 182 */
#define EM_AARCH64       183 /* ARM AARCH64 */
/* reserved 184 */
#define EM_AVR32         185 /* Amtel 32-bit microprocessor */
#define EM_STM8          186 /* STMicroelectronics STM8 */
#define EM_TILE64        187 /* Tilera TILE64 */
#define EM_TILEPRO       188 /* Tilera TILEPro */
#define EM_MICROBLAZE    189 /* Xilinx MicroBlaze */
#define EM_CUDA          190 /* NVIDIA CUDA */
#define EM_TILEGX        191 /* Tilera TILE-Gx */
#define EM_CLOUDSHIELD   192 /* CloudShield */
#define EM_COREA_1ST     193 /* KIPO-KAIST Core-A 1st gen. */
#define EM_COREA_2ND     194 /* KIPO-KAIST Core-A 2nd gen. */
#define EM_ARCV2         195 /* Synopsys ARCv2 ISA.  */
#define EM_OPEN8         196 /* Open8 RISC */
#define EM_RL78          197 /* Renesas RL78 */
#define EM_VIDEOCORE5    198 /* Broadcom VideoCore V */
#define EM_78KOR         199 /* Renesas 78KOR */
#define EM_56800EX       200 /* Freescale 56800EX DSC */
#define EM_BA1           201 /* Beyond BA1 */
#define EM_BA2           202 /* Beyond BA2 */
#define EM_XCORE         203 /* XMOS xCORE */
#define EM_MCHP_PIC      204 /* Microchip 8-bit PIC(r) */
#define EM_INTELGT       205 /* Intel Graphics Technology */
/* reserved 206-209 */
#define EM_KM32          210 /* KM211 KM32 */
#define EM_KMX32         211 /* KM211 KMX32 */
#define EM_EMX16         212 /* KM211 KMX16 */
#define EM_EMX8          213 /* KM211 KMX8 */
#define EM_KVARC         214 /* KM211 KVARC */
#define EM_CDP           215 /* Paneve CDP */
#define EM_COGE          216 /* Cognitive Smart Memory Processor */
#define EM_COOL          217 /* Bluechip CoolEngine */
#define EM_NORC          218 /* Nanoradio Optimized RISC */
#define EM_CSR_KALIMBA   219 /* CSR Kalimba */
#define EM_Z80           220 /* Zilog Z80 */
#define EM_VISIUM        221 /* Controls and Data Services VISIUMcore */
#define EM_FT32          222 /* FTDI Chip FT32 */
#define EM_MOXIE         223 /* Moxie processor */
#define EM_AMDGPU        224 /* AMD GPU */
/* reserved 225-242 */
#define EM_RISCV         243 /* RISC-V */
#define EM_BPF           247 /* Linux BPF -- in-kernel virtual machine */
#define EM_CSKY          252 /* C-SKY */
#define EM_LOONGARCH     258 /* LoongArch */
#define EM_NUM           259

#define EM_ARC_A5 EM_ARC_COMPACT /* old spellings/synonyms */

/*
 * If it is necessary to assign new unofficial EM_* values, please
 * pick large random numbers (0x8523, 0xa7f2, etc.) to minimize the
 * chances of collision with official or non-GNU unofficial values.
 */
#define EM_ALPHA 0x9026

/* elf*_ehdr.e_version */
#define EV_NONE    0 /* invalid elf version */
#define EV_CURRENT 1 /* current version */
#define EV_NUM     2

/* section header */
struct elf32_shdr {
	uint32_t sh_name;      /* section name (string tbl index) */
	uint32_t sh_type;      /* section type */
	uint32_t sh_flags;     /* section flags */
	uint32_t sh_addr;      /* section virtual addr at execution */
	uint32_t sh_offset;    /* section file offset */
	uint32_t sh_size;      /* section size in bytes */
	uint32_t sh_link;      /* link to another section */
	uint32_t sh_info;      /* additional section information */
	uint32_t sh_addralign; /* section alignment */
	uint32_t sh_entsize;   /* entry size if section holds table */
};

struct elf64_shdr {
	uint32_t sh_name;      /* section name (string tbl index) */
	uint32_t sh_type;      /* section type */
	uint64_t sh_flags;     /* section flags */
	uint64_t sh_addr;      /* section virtual addr at execution */
	uint64_t sh_offset;    /* section file offset */
	uint64_t sh_size;      /* section size in bytes */
	uint32_t sh_link;      /* link to another section */
	uint32_t sh_info;      /* additional section information */
	uint64_t sh_addralign; /* section alignment */
	uint64_t sh_entsize;   /* entry size if section holds table */
};

/* elf*_shdr.sh_name */
#define SHN_UNDEF     0      /* undefined section */
#define SHN_LORESERVE 0xff00 /* start of reserved indices */
#define SHN_LOPROC    0xff00 /* start of processor-specific */
#define SHN_BEFORE    0xff00 /* order section before all others (Solaris) */
#define SHN_AFTER     0xff01 /* order section after all others (Solaris)  */
#define SHN_HIPROC    0xff1f /* end of processor-specific */
#define SHN_LOOS      0xff20 /* start of OS-specific */
#define SHN_HIOS      0xff3f /* end of OS-specific */
#define SHN_ABS       0xfff1 /* associated symbol is absolute */
#define SHN_COMMON    0xfff2 /* associated symbol is common */
#define SHN_XINDEX    0xffff /* index is in extra table */
#define SHN_HIRESERVE 0xffff /* end of reserved indices */

/* elf*_shdr.sh_type */
#define SHT_NULL           0  /* section header table entry unused */
#define SHT_PROGBITS       1  /* program data */
#define SHT_SYMTAB         2  /* symbol table */
#define SHT_STRTAB         3  /* string table */
#define SHT_RELA           4  /* relocation entries with addends */
#define SHT_HASH           5  /* symbol hash table */
#define SHT_DYNAMIC        6  /* dynamic linking information */
#define SHT_NOTE           7  /* notes */
#define SHT_NOBITS         8  /* program space with no data (bss) */
#define SHT_REL            9  /* relocation entries, no addends */
#define SHT_SHLIB          10 /* reserved */
#define SHT_DYNSYM         11 /* dynamic linker symbol table */
#define SHT_INIT_ARRAY     14 /* array of constructors */
#define SHT_FINI_ARRAY     15 /* array of destructors */
#define SHT_PREINIT_ARRAY  16 /* array of pre-constructors */
#define SHT_GROUP          17 /* section group */
#define SHT_SYMTAB_SHNDX   18 /* extended section indices */
#define SHT_RELR           19 /* relr relative relocations */
#define SHT_NUM            20 /* number of defined types  */

#define SHT_LOOS           0x60000000 /* start OS-specific */
#define SHT_GNU_ATTRIBUTES 0x6ffffff5 /* object attributes */
#define SHT_GNU_HASH       0x6ffffff6 /* GNU-style hash table */
#define SHT_GNU_LIBLIST    0x6ffffff7 /* prelink library list */
#define SHT_CHECKSUM       0x6ffffff8 /* checksum for DSO content */
#define SHT_LOSUNW         0x6ffffffa /* Sun-specific low bound */
#define SHT_SUNW_move      0x6ffffffa
#define SHT_SUNW_COMDAT    0x6ffffffb
#define SHT_SUNW_syminfo   0x6ffffffc
#define SHT_GNU_verdef     0x6ffffffd /* version definition section*/
#define SHT_GNU_verneed    0x6ffffffe /* version needs section */
#define SHT_GNU_versym     0x6fffffff /* version symbol table */
#define SHT_HISUNW         0x6fffffff /* Sun-specific high bound */
#define SHT_HIOS           0x6fffffff /* end OS-specific type */
#define SHT_LOPROC         0x70000000 /* start of processor-specific */
#define SHT_HIPROC         0x7fffffff /* end of processor-specific */
#define SHT_LOUSER         0x80000000 /* start of application-specific */
#define SHT_HIUSER         0x8fffffff /* end of application-specific */

/* elf*_shdr.sh_flags */
#define SHF_WRITE            (1 << 0)   /* writable */
#define SHF_ALLOC            (1 << 1)   /* occupies memory during execution */
#define SHF_EXECINSTR        (1 << 2)   /* executable */
#define SHF_MERGE            (1 << 4)   /* might be merged */
#define SHF_STRINGS          (1 << 5)   /* contains nul-terminated strings */
#define SHF_INFO_LINK        (1 << 6)   /* 'sh_info' contains SHT index */
#define SHF_LINK_ORDER       (1 << 7)   /* preserve order after combining */
#define SHF_OS_NONCONFORMING (1 << 8)   /* non-standard OS specific handling
					   required */
#define SHF_GROUP            (1 << 9)   /* section is member of a group */
#define SHF_TLS              (1 << 10)  /* section hold thread-local data */
#define SHF_COMPRESSED       (1 << 11)  /* section with compressed data */
#define SHF_MASKOS           0x0ff00000 /* OS-specific */
#define SHF_MASKPROC         0xf0000000 /* processor-specific */
#define SHF_GNU_RETAIN       (1 << 21)  /* not to be GCed by linker */
#define SHF_ORDERED          (1 << 30)  /* special ordering requirement
					   (Solaris) */
#define SHF_EXCLUDE          (1U << 31) /* section is excluded unless
					   referenced or allocated (Solaris) */

/* section compression header */
struct elf32_chdr {
	uint32_t ch_type;      /* compression format */
	uint32_t ch_size;      /* uncompressed data size */
	uint32_t ch_addralign; /* uncompressed data alignment */
};

struct elf64_chdr {
	uint32_t ch_type;      /* compression format */
	uint32_t ch_reserved;
	uint64_t ch_size;      /* uncompressed data size */
	uint64_t ch_addralign; /* uncompressed data alignment */
};

/* elf*_chdr.ch_type */
#define ELF_COMPRESS_ZLIB   1          /* Zlib/DEFLATE algorithm */
#define ELF_COMPRESS_ZSTD   2          /* Zstandard algorithm */
#define ELF_COMPRESS_LOOS   0x60000000 /* start of OS-specific */
#define ELF_COMPRESS_HIOS   0x6fffffff /* end of OS-specific */
#define ELF_COMPRESS_LOPROC 0x70000000 /* start of processor-specific */
#define ELF_COMPRESS_HIPROC 0x7fffffff /* end of processor-specific */

/* section group handling */
#define GRP_COMDAT 0x1 /* mark group as COMDAT */

/* symbol table entry */
struct elf32_sym {
	uint32_t st_name;  /* symbol name (string tbl index) */
	uint32_t st_value; /* symbol value */
	uint32_t st_size;  /* symbol size */
	uint8_t st_info;   /* symbol type and binding */
	uint8_t st_other;  /* symbol visibility */
	uint16_t st_shndx; /* section index */
};

struct elf64_sym {
	uint32_t st_name;  /* symbol name (string tbl index) */
	uint8_t st_info;   /* symbol type and binding */
	uint8_t st_other;  /* symbol visibility */
	uint16_t st_shndx; /* section index */
	uint64_t st_value; /* symbol value */
	uint64_t st_size;  /* symbol size */
};

/*
 * The syminfo section if available contains additional information about
 * every dynamic symbol.
 */
struct elf32_syminfo {
	uint16_t si_boundto; /* direct bindings, symbol bound to */
	uint16_t si_flags;   /* per symbol flags */
};

struct elf64_syminfo {
	uint16_t si_boundto; /* direct bindings, symbol bound to */
	uint16_t si_flags;   /* per symbol flags */
};

/* elf*_syminfo.si_boundto */
#define SYMINFO_BT_SELF       0xffff /* symbol bound to self */
#define SYMINFO_BT_PARENT     0xfffe /* symbol bound to parent */
#define SYMINFO_BT_LOWRESERVE 0xff00 /* beginning of reserved entries */

/* elf*_syminfo.si_flags */
#define SYMINFO_FLG_DIRECT   0x0001 /* direct bound symbol */
#define SYMINFO_FLG_PASSTHRU 0x0002 /* pass-through symbol for translator */
#define SYMINFO_FLG_COPY     0x0004 /* symbol is a copy-reloc */
#define SYMINFO_FLG_LAZYLOAD 0x0008 /* symbol bound to object to be lazy
				       loaded */

/* syminfo version values */
#define SYMINFO_NONE    0
#define SYMINFO_CURRENT 1
#define SYMINFO_NUM     2

/* elf*_sym.st_info */
#define ELF32_ST_BIND(x) ((uint8_t)(x) >> 4)
#define ELF32_ST_TYPE(x) ((x) & 0xf)
#define ELF32_ST_INFO(bind, type) (((bind) << 4) + ((type) & 0xf))

#define ELF64_ST_BIND(x) ELF32_ST_BIND(x)
#define ELF64_ST_TYPE(x) ELF32_ST_TYPE(x)
#define ELF64_ST_INFO(bind, type) ELF32_ST_INFO(bind, type)

/* ST_BIND */
#define STB_LOCAL      0  /* local symbol */
#define STB_GLOBAL     1  /* global symbol */
#define STB_WEAK       2  /* weak symbol */
#define STB_NUM        3  /* number of defined types */
#define STB_LOOS       10 /* start of OS-specific */
#define STB_GNU_UNIQUE 10 /* unique symbol */
#define STB_HIOS       12 /* end of OS-specific */
#define STB_LOPROC     13 /* start of processor-specific */
#define STB_HIPROC     15 /* end of processor-specific */

/* ST_TYPE */
#define STT_NOTYPE    0  /* symbol type is unspecified */
#define STT_OBJECT    1  /* symbol is a data object */
#define STT_FUNC      2  /* symbol is a code object */
#define STT_SECTION   3  /* symbol associated with a section */
#define STT_FILE      4  /* symbol's name is file name */
#define STT_COMMON    5  /* symbol is a common data object */
#define STT_TLS       6  /* symbol is thread-local data object */
#define STT_NUM       7  /* number of defined types.  */
#define STT_LOOS      10 /* start of OS-specific */
#define STT_GNU_IFUNC 10 /* symbol is indirect code object */
#define STT_HIOS      12 /* end of OS-specific */
#define STT_LOPROC    13 /* start of processor-specific */
#define STT_HIPROC    15 /* end of processor-specific */

/*
 * Symbol table indices are found in the hash buckets and chain table
 * of a symbol hash table section.  This special index value indicates
 * the end of a chain, meaning no further symbols are found in that bucket.
 */

#define STN_UNDEF 0 /* end of a chain */

/* elf*_syminfo.st_other */
#define ELF32_ST_VISIBILITY(o) ((o) & 0x03)
#define ELF64_ST_VISIBILITY(o) ELF32_ST_VISIBILITY(o)

/* ST_VISIBILITY */
#define STV_DEFAULT   0 /* default symbol visibility rules */
#define STV_INTERNAL  1 /* processor specific hidden class */
#define STV_HIDDEN    2 /* sym unavailable in other modules */
#define STV_PROTECTED 3 /* not preemptible, not exported */

/* relocation table entry without addend (in section of type SHT_REL) */
struct elf32_rel {
	uint32_t r_offset; /* address */
	uint32_t r_info;   /* relocation type and symbol index */
};

/*
 * I have seen two different definitions of the elf64_rel and elf64_rela
 * structures, so we'll leave them out until Novell (or whoever) gets their
 * act together.
 */

/* The following, at least, is used on Sparc v9, MIPS, and Alpha */

struct elf64_rel {
	uint64_t r_offset; /* address */
	uint64_t r_info;   /* relocation type and symbol index */
};

/* relocation table entry with addend (in section of type SHT_RELA) */
struct elf32_rela {
	uint32_t r_offset; /* address */
	uint32_t r_info;   /* relocation type and symbol index */
	int32_t r_addend;  /* addend */
};

struct elf64_rela {
	uint64_t r_offset; /* address */
	uint64_t r_info;   /* relocation type and symbol index */
	int64_t r_addend;  /* addend */
};

/* elf*_rel*.r_info */
#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)
#define ELF32_R_INFO(sym, type) (((sym) << 8) + ((type) & 0xff))

#define ELF64_R_SYM(x) ((x) >> 32)
#define ELF64_R_TYPE(x) ((x) & 0xffffffff)
#define ELF64_R_INFO(sym, type) (((uint64_t)(sym) << 32) + (type))

/* program segment header */
struct elf32_phdr {
	uint32_t p_type;   /* segment type */
	uint32_t p_offset; /* segment file offset */
	uint32_t p_vaddr;  /* segment virtual address */
	uint32_t p_paddr;  /* segment physical address */
	uint32_t p_filesz; /* segment size in file */
	uint32_t p_memsz;  /* segment size in memory */
	uint32_t p_flags;  /* segment flags */
	uint32_t p_align;  /* segment alignment */
};

struct elf64_phdr {
	uint32_t p_type;   /* segment type */
	uint32_t p_flags;  /* segment flags */
	uint64_t p_offset; /* segment file offset */
	uint64_t p_vaddr;  /* segment virtual address */
	uint64_t p_paddr;  /* segment physical address */
	uint64_t p_filesz; /* segment size in file */
	uint64_t p_memsz;  /* segment size in memory */
	uint64_t p_align;  /* segment alignment */
};

/*
 * Special value for e_phnum.  This indicates that the real number of
 * program headers is too large to fit into e_phnum.  Instead the real
 * value is in the field sh_info of section 0.
 */

#define PN_XNUM 0xffff

/* elf*_phdr.p_type */
#define PT_NULL    0 /* program header table entry unused */
#define PT_LOAD    1 /* loadable program segment */
#define PT_DYNAMIC 2 /* dynamic linking information */
#define PT_INTERP  3 /* program interpreter */
#define PT_NOTE    4 /* auxiliary information */
#define PT_SHLIB   5 /* reserved */
#define PT_PHDR    6 /* entry for header table itself */
#define PT_TLS     7 /* thread-local storage segment */
#define PT_NUM     8 /* number of defined types */

#define PT_LOOS         0x60000000 /* start of OS-specific */
#define PT_GNU_EH_FRAME 0x6474e550 /* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK    0x6474e551 /* indicates stack executability */
#define PT_GNU_RELRO    0x6474e552 /* read-only after relocation */
#define PT_GNU_PROPERTY 0x6474e553 /* GNU property */
#define PT_GNU_SFRAME   0x6474e554 /* sframe segment */
#define PT_LOSUNW       0x6ffffffa 
#define PT_SUNWBSS      0x6ffffffa /* Sun Specific segment */
#define PT_SUNWSTACK    0x6ffffffb /* stack segment */
#define PT_HISUNW       0x6fffffff 
#define PT_HIOS         0x6fffffff /* end of OS-specific */
#define PT_LOPROC       0x70000000 /* start of processor-specific */
#define PT_HIPROC       0x7fffffff /* end of processor-specific */

/* elf*_phdr.p_flags */
#define PF_X        (1 << 0)   /* segment is executable */
#define PF_W        (1 << 1)   /* segment is writable */
#define PF_R        (1 << 2)   /* segment is readable */
#define PF_MASKOS   0x0ff00000 /* os-specific */
#define PF_MASKPROC 0xf0000000 /* processor-specific */

/* note section header */
struct elf32_nhdr {
	uint32_t n_namesz; /* length of the note's name */
	uint32_t n_descsz; /* length of the note's descriptor */
	uint32_t n_type;   /* type of the note */
};

struct elf64_nhdr {
	uint32_t n_namesz; /* length of the note's name */
	uint32_t n_descsz; /* length of the note's descriptor */
	uint32_t n_type;   /* type of the note */
};

/* legal values for note segment descriptor types for CORE files */
#define NT_PRSTATUS               1   /* contains copy of prstatus struct */
#define NT_PRFPREG                2   /* contains copy of fpregset struct */
#define NT_FPREGSET               2   /* contains copy of fpregset struct */
#define NT_PRPSINFO               3   /* contains copy of prpsinfo struct */
#define NT_PRXREG                 4   /* contains copy of prxregset struct */
#define NT_TASKSTRUCT             4   /* contains copy of task structure */
#define NT_PLATFORM               5   /* string from sysinfo(SI_PLATFORM) */
#define NT_AUXV                   6   /* contains copy of auxv array */
#define NT_GWINDOWS               7   /* contains copy of gwindows struct */
#define NT_ASRS                   8   /* contains copy of asrset struct */
#define NT_PSTATUS                10  /* contains copy of pstatus struct */
#define NT_PSINFO                 13  /* contains copy of psinfo struct */
#define NT_PRCRED                 14  /* contains copy of prcred struct */
#define NT_UTSNAME                15  /* contains copy of utsname struct */
#define NT_LWPSTATUS              16  /* contains copy of lwpstatus struct */
#define NT_LWPSINFO               17  /* contains copy of lwpinfo struct */
#define NT_PRFPXREG               20  /* contains copy of fprxregset struct */
#define NT_SIGINFO                0x53494749 /* contains copy of siginfo_t,
						size might increase */
#define NT_FILE                   0x46494c45 /* contains information about
						mapped files */
#define NT_PRXFPREG               0x46e62b7f /* contains copy of
						user_fxsr_struct */
#define NT_PPC_VMX                0x100 /* PowerPC Altivec/VMX registers */
#define NT_PPC_SPE                0x101 /* PowerPC SPE/EVR registers */
#define NT_PPC_VSX                0x102 /* PowerPC VSX registers */
#define NT_PPC_TAR                0x103 /* target address register */
#define NT_PPC_PPR                0x104 /* program priority register */
#define NT_PPC_DSCR               0x105 /* data stream control register */
#define NT_PPC_EBB                0x106 /* event based branch registers */
#define NT_PPC_PMU                0x107 /* performance monitor registers */
#define NT_PPC_TM_CGPR            0x108 /* TM checkpointed GPR registers */
#define NT_PPC_TM_CFPR            0x109 /* TM checkpointed FPR registers */
#define NT_PPC_TM_CVMX            0x10a /* TM checkpointed VMX registers */
#define NT_PPC_TM_CVSX            0x10b /* TM checkpointed VSX registers */
#define NT_PPC_TM_SPR             0x10c /* TM special purpose registers */
#define NT_PPC_TM_CTAR            0x10d /* TM checkpointed target address
					   register */
#define NT_PPC_TM_CPPR            0x10e /* TM checkpointed program priority
					   register */
#define NT_PPC_TM_CDSCR           0x10f /* TM checkpointed data stream control
					   register */
#define NT_PPC_PKEY               0x110 /* memory protection keys registers */
#define NT_PPC_DEXCR              0x111 /* PowerPC DEXCR registers  */
#define NT_PPC_HASHKEYR           0x112 /* PowerPC HASHKEYR register */
#define NT_386_TLS                0x200 /* i386 TLS slots (struct user_desc) */
#define NT_386_IOPERM             0x201 /* x86 io permission bitmap (1=deny) */
#define NT_X86_XSTATE             0x202 /* x86 extended state using xsave */
#define NT_X86_SHSTK              0x204 /* x86 SHSTK state */
#define NT_X86_XSAVE_LAYOUT       0x205 /* XSAVE layout description */
#define NT_S390_HIGH_GPRS         0x300 /* s390 upper register halves */
#define NT_S390_TIMER             0x301 /* s390 timer register */
#define NT_S390_TODCMP            0x302 /* s390 TOD clock comparator register */
#define NT_S390_TODPREG           0x303 /* s390 TOD programmable register */
#define NT_S390_CTRS              0x304 /* s390 control registers */
#define NT_S390_PREFIX            0x305 /* s390 prefix register */
#define NT_S390_LAST_BREAK        0x306 /* s390 breaking event address */
#define NT_S390_SYSTEM_CALL       0x307 /* s390 system call restart data */
#define NT_S390_TDB               0x308 /* s390 transaction diagnostic block */
#define NT_S390_VXRS_LOW          0x309 /* s390 vector registers 0-15 upper
					   half */
#define NT_S390_VXRS_HIGH         0x30a /* s390 vector registers 16-31 */
#define NT_S390_GS_CB             0x30b /* s390 guarded storage registers */
#define NT_S390_GS_BC             0x30c /* s390 guarded storage broadcast
					   control block */
#define NT_S390_RI_CB             0x30d /* s390 runtime instrumentation */
#define NT_S390_PV_CPU_DATA       0x30e /* s390 protvirt cpu dump data */
#define NT_ARM_VFP                0x400 /* ARM VFP/NEON registers */
#define NT_ARM_TLS                0x401 /* ARM TLS register */
#define NT_ARM_HW_BREAK           0x402 /* ARM hardware breakpoint registers */
#define NT_ARM_HW_WATCH           0x403 /* ARM hardware watchpoint registers */
#define NT_ARM_SYSTEM_CALL        0x404 /* ARM system call number */
#define NT_ARM_SVE                0x405 /* ARM Scalable Vector Extension
					   registers */
#define NT_ARM_PAC_MASK           0x406 /* ARM pointer authentication code
					   masks */
#define NT_ARM_PACA_KEYS          0x407 /* ARM pointer authentication address
					   keys */
#define NT_ARM_PACG_KEYS          0x408 /* ARM pointer authentication generic
					   key */
#define NT_ARM_TAGGED_ADDR_CTRL   0x409 /* AArch64 tagged address control */
#define NT_ARM_PAC_ENABLED_KEYS   0x40a /* AArch64 pointer authentication
					   enabled keys */
#define NT_ARM_SSVE               0x40b /* ARM Streaming SVE registers */
#define NT_ARM_ZA                 0x40c /* ARM SME ZA registers */
#define NT_ARM_ZT                 0x40d /* ARM SME ZT registers */
#define NT_ARM_FPMR               0x40e /* ARM floating point mode register */
#define NT_ARM_POE                0x40f /* ARM POE registers */
#define NT_ARM_GCS                0x410 /* ARM GCS state */
#define NT_VMCOREDD               0x700 /* Vmcore Device Dump Note */
#define NT_MIPS_DSP               0x800 /* MIPS DSP ASE registers */
#define NT_MIPS_FP_MODE           0x801 /* MIPS floating-point mode */
#define NT_MIPS_MSA               0x802 /* MIPS SIMD registers */
#define NT_RISCV_CSR              0x900 /* RISC-V control and status
					   registers */
#define NT_RISCV_VECTOR           0x901 /* RISC-V vector registers */
#define NT_RISCV_TAGGED_ADDR_CTRL 0x902 /* RISC-V tagged address control */
#define NT_LOONGARCH_CPUCFG       0xa00 /* LoongArch CPU config registers */
#define NT_LOONGARCH_CSR          0xa01 /* LoongArch control and status
					   registers */
#define NT_LOONGARCH_LSX          0xa02 /* LoongArch Loongson SIMD extension
					   registers */
#define NT_LOONGARCH_LASX         0xa03 /* LoongArch Loongson advanced SIMD
					   extension registers */
#define NT_LOONGARCH_LBT          0xa04 /* LoongArch Loongson binary
					   translation registers */
#define NT_LOONGARCH_HW_BREAK     0xa05 /* LoongArch hardware breakpoint
					   registers */
#define NT_LOONGARCH_HW_WATCH     0xa06 /* LoongArch hardware watchpoint
					   registers */

/* known names of NOTE */

/* Solaris entries in the note section have this name */
#define ELF_NOTE_SOLARIS "SUNW Solaris"

/* note entries for GNU systems have this name */
#define ELF_NOTE_GNU "GNU"

/* note entries for freedesktop.org have this name */
#define ELF_NOTE_FDO "FDO"

/* defined types of notes for Solaris */

/* value of descriptor (one word) is desired pagesize for the binary */
#define ELF_NOTE_PAGESIZE_HINT 1

/* defined note types for GNU systems */

/*
 * ABI information.  The descriptor consists of words:
 *  word 0: OS descriptor
 *  word 1: major version of the ABI
 *  word 2: minor version of the ABI
 *  word 3: subminor version of the ABI
 */
#define NT_GNU_ABI_TAG 1
#define ELF_NOTE_ABI NT_GNU_ABI_TAG /* Old name.  */

/*
 * known OSes.  These values can appear in word 0 of an
 * NT_GNU_ABI_TAG note section entry.
 */
#define ELF_NOTE_OS_LINUX    0
#define ELF_NOTE_OS_GNU      1
#define ELF_NOTE_OS_SOLARIS2 2
#define ELF_NOTE_OS_FREEBSD  3

/* Synthetic hwcap information.  The descriptor begins with two words:
 *  word 0: number of entries
 *  word 1: bitmask of enabled entries
 *  Then follow variable-length entries, one byte followed by a
 *  '\0'-terminated hwcap name string.  The byte gives the bit
 *  number to test if enabled, (1U << bit) & bitmask.
 */
#define NT_GNU_HWCAP 2

/*
 * Build ID bits as generated by ld --build-id.
 * The descriptor consists of any nonzero number of bytes.
 */
#define NT_GNU_BUILD_ID 3

/* version note generated by GNU gold containing a version string */
#define NT_GNU_GOLD_VERSION 4

/* program property */
#define NT_GNU_PROPERTY_TYPE_0 5

/* packaging metadata as defined on https://systemd.io/ELF_PACKAGE_METADATA/ */
#define NT_FDO_PACKAGING_METADATA 0xcafe1a7e

/* dlopen metadata as defined on https://systemd.io/ELF_DLOPEN_METADATA/ */
#define NT_FDO_DLOPEN_METADATA 0x407c0c0a

/* note section name of program property */
#define NOTE_GNU_PROPERTY_SECTION_NAME ".note.gnu.property"

/* values used in GNU .note.gnu.property notes (NT_GNU_PROPERTY_TYPE_0) */

/* stack size */
#define GNU_PROPERTY_STACK_SIZE 1

/* no copy relocation on protected data symbol */
#define GNU_PROPERTY_NO_COPY_ON_PROTECTED 2

/*
 * A 4-byte unsigned integer property: A bit is set if it is set in all
 * relocatable inputs.
 */
#define GNU_PROPERTY_UINT32_AND_LO 0xb0000000
#define GNU_PROPERTY_UINT32_AND_HI 0xb0007fff

/*
 * A 4-byte unsigned integer property: A bit is set if it is set in any
 * relocatable inputs.
 */
#define GNU_PROPERTY_UINT32_OR_LO 0xb0008000
#define GNU_PROPERTY_UINT32_OR_HI 0xb000ffff

/* The needed properties by the object file */
#define GNU_PROPERTY_1_NEEDED GNU_PROPERTY_UINT32_OR_LO

/*
 * Set if the object file requires canonical function pointers and
 * cannot be used with copy relocation.
 */
#define GNU_PROPERTY_1_NEEDED_INDIRECT_EXTERN_ACCESS (1U << 0)

/* processor-specific semantics, lo */
#define GNU_PROPERTY_LOPROC 0xc0000000
/* processor-specific semantics, hi */
#define GNU_PROPERTY_HIPROC 0xdfffffff
/* application-specific semantics, lo */
#define GNU_PROPERTY_LOUSER 0xe0000000
/* application-specific semantics, hi */
#define GNU_PROPERTY_HIUSER 0xffffffff

/* AArch64 specific GNU properties */
#define GNU_PROPERTY_AARCH64_FEATURE_1_AND 0xc0000000
#define GNU_PROPERTY_AARCH64_FEATURE_1_BTI (1U << 0)
#define GNU_PROPERTY_AARCH64_FEATURE_1_PAC (1U << 1)
#define GNU_PROPERTY_AARCH64_FEATURE_1_GCS (1U << 2)

/*
 * The x86 instruction sets indicated by the corresponding bits are
 * used in program.  Their support in the hardware is optional.
 */
#define GNU_PROPERTY_X86_ISA_1_USED 0xc0010002

/*
 * The x86 instruction sets indicated by the corresponding bits are
 * used in program and they must be supported by the hardware.
 */
#define GNU_PROPERTY_X86_ISA_1_NEEDED 0xc0008002

/* X86 processor-specific features used in program */
#define GNU_PROPERTY_X86_FEATURE_1_AND 0xc0000002

/*
 * GNU_PROPERTY_X86_ISA_1_BASELINE: CMOV, CX8 (cmpxchg8b), FPU (fld),
 * MMX, OSFXSR (fxsave), SCE (syscall), SSE and SSE2.
 */
#define GNU_PROPERTY_X86_ISA_1_BASELINE (1U << 0)

/*
 * GNU_PROPERTY_X86_ISA_1_V2: GNU_PROPERTY_X86_ISA_1_BASELINE,
 * CMPXCHG16B (cmpxchg16b), LAHF-SAHF (lahf), POPCNT (popcnt), SSE3,
 * SSSE3, SSE4.1 and SSE4.2.
 */
#define GNU_PROPERTY_X86_ISA_1_V2 (1U << 1)

/*
 * GNU_PROPERTY_X86_ISA_1_V3: GNU_PROPERTY_X86_ISA_1_V2, AVX, AVX2, BMI1,
 * BMI2, F16C, FMA, LZCNT, MOVBE, XSAVE.
 */
#define GNU_PROPERTY_X86_ISA_1_V3 (1U << 2)

/*
 * GNU_PROPERTY_X86_ISA_1_V4: GNU_PROPERTY_X86_ISA_1_V3, AVX512F,
 * AVX512BW, AVX512CD, AVX512DQ and AVX512VL.
 */
#define GNU_PROPERTY_X86_ISA_1_V4 (1U << 3)

/* This indicates that all executable sections are compatible with IBT */
#define GNU_PROPERTY_X86_FEATURE_1_IBT (1U << 0)

/* This indicates that all executable sections are compatible with SHSTK */
#define GNU_PROPERTY_X86_FEATURE_1_SHSTK (1U << 1)

/* dynamic section entry */
struct elf32_dyn {
	int32_t d_tag; /* dynamic entry type */
	union {
		uint32_t d_val; /* integer value */
		uint32_t d_ptr; /* address value */
	} d_un;
};

struct elf64_dyn {
	int64_t d_tag; /* dynamic entry type */
	union {
		uint64_t d_val; /* integer value */
		uint64_t d_ptr; /* address value */
	} d_un;
};

/* elf*.d_tag */
#define DT_NULL            0   /* marks end of dynamic section */
#define DT_NEEDED          1   /* name of needed library */
#define DT_PLTRELSZ        2   /* size in bytes of PLT relocs */
#define DT_PLTGOT          3   /* processor defined value */
#define DT_HASH            4   /* address of symbol hash table */
#define DT_STRTAB          5   /* address of string table */
#define DT_SYMTAB          6   /* address of symbol table */
#define DT_RELA            7   /* address of Rela relocs */
#define DT_RELASZ          8   /* total size of Rela relocs */
#define DT_RELAENT         9   /* size of one Rela reloc */
#define DT_STRSZ           10  /* size of string table */
#define DT_SYMENT          11  /* size of one symbol table entry */
#define DT_INIT            12  /* address of init function */
#define DT_FINI            13  /* address of termination function */
#define DT_SONAME          14  /* name of shared object */
#define DT_RPATH           15  /* library search path (deprecated) */
#define DT_SYMBOLIC        16  /* start symbol search here */
#define DT_REL             17  /* address of Rel relocs */
#define DT_RELSZ           18  /* total size of Rel relocs */
#define DT_RELENT          19  /* size of one Rel reloc */
#define DT_PLTREL          20  /* type of reloc in PLT */
#define DT_DEBUG           21  /* for debugging; unspecified */
#define DT_TEXTREL         22  /* reloc might modify .text */
#define DT_JMPREL          23  /* address of PLT relocs */
#define DT_BIND_NOW        24  /* process relocations of object */
#define DT_INIT_ARRAY      25  /* array with addresses of init fct */
#define DT_FINI_ARRAY      26  /* array with addresses of fini fct */
#define DT_INIT_ARRAYSZ    27  /* size in bytes of DT_INIT_ARRAY */
#define DT_FINI_ARRAYSZ    28  /* size in bytes of DT_FINI_ARRAY */
#define DT_RUNPATH         29  /* library search path */
#define DT_FLAGS           30  /* flags for the object being loaded */
#define DT_ENCODING        32  /* start of encoded range */
#define DT_PREINIT_ARRAY   32  /* array with addresses of preinit fct*/
#define DT_PREINIT_ARRAYSZ 33  /* size in bytes of DT_PREINIT_ARRAY */
#define DT_SYMTAB_SHNDX    34  /* address of SYMTAB_SHNDX section */
#define DT_RELRSZ          35  /* total size of RELR relative relocations */
#define DT_RELR            36  /* address of RELR relative relocations */
#define DT_RELRENT         37  /* size of one RELR relative relocation */
#define DT_NUM             38  /* number used */

#define DT_LOOS            0x6000000d  /* start of OS-specific */
#define DT_HIOS            0x6ffff000  /* end of OS-specific */
#define DT_LOPROC          0x70000000  /* start of processor-specific */
#define DT_HIPROC          0x7fffffff  /* end of processor-specific */
#define DT_PROCNUM         DT_MIPS_NUM /* most used by any processor */

/*
 * DT_* entries which fall between DT_VALRNGHI & DT_VALRNGLO use the
 * Dyn.d_un.d_val field of the elf*_dyn structure.
 * This follows Sun's approach.
 */

#define DT_VALRNGLO       0x6ffffd00 
#define DT_GNU_PRELINKED  0x6ffffdf5 /* prelinking timestamp */
#define DT_GNU_CONFLICTSZ 0x6ffffdf6 /* size of conflict section */
#define DT_GNU_LIBLISTSZ  0x6ffffdf7 /* size of library list */
#define DT_CHECKSUM       0x6ffffdf8 
#define DT_PLTPADSZ       0x6ffffdf9 
#define DT_MOVEENT        0x6ffffdfa 
#define DT_MOVESZ         0x6ffffdfb 
#define DT_FEATURE_1      0x6ffffdfc /* feature selection (DTF_*) */
#define DT_POSFLAG_1      0x6ffffdfd /* flags for DT_* entries, effecting
					the following DT_* entry */
#define DT_SYMINSZ        0x6ffffdfe /* size of syminfo table (in bytes) */
#define DT_SYMINENT       0x6ffffdff /* entry size of syminfo */
#define DT_VALRNGHI       0x6ffffdff 
#define DT_VALTAGIDX(tag) (DT_VALRNGHI - (tag)) /* reverse order! */
#define DT_VALNUM 12

/*
 * DT_* entries which fall between DT_ADDRRNGHI & DT_ADDRRNGLO use the
 * Dyn.d_un.d_ptr field of the elf*_dyn structure.
 *
 * If any adjustment is made to the ELF object after it has been
 * built these entries will need to be adjusted.
 */

#define DT_ADDRRNGLO       0x6ffffe00 
#define DT_GNU_HASH        0x6ffffef5 /* GNU-style hash table */
#define DT_TLSDESC_PLT     0x6ffffef6 
#define DT_TLSDESC_GOT     0x6ffffef7 
#define DT_GNU_CONFLICT    0x6ffffef8 /* start of conflict section */
#define DT_GNU_LIBLIST     0x6ffffef9 /* library list */
#define DT_CONFIG          0x6ffffefa /* configuration information */
#define DT_DEPAUDIT        0x6ffffefb /* dependency auditing */
#define DT_AUDIT           0x6ffffefc /* object auditing */
#define DT_PLTPAD          0x6ffffefd /* PLT padding */
#define DT_MOVETAB         0x6ffffefe /* move table */
#define DT_SYMINFO         0x6ffffeff /* syminfo table */
#define DT_ADDRRNGHI       0x6ffffeff 
#define DT_ADDRTAGIDX(tag) (DT_ADDRRNGHI - (tag)) /* reverse order! */
#define DT_ADDRNUM 11

/*
 * The versioning entry types.  The next are defined as part of the
 * GNU extension.
 */
#define DT_VERSYM 0x6ffffff0

#define DT_RELACOUNT 0x6ffffff9
#define DT_RELCOUNT  0x6ffffffa

/* These were chosen by Sun */
#define DT_FLAGS_1            0x6ffffffb /* state flags, see DF_1_* below */
#define DT_VERDEF             0x6ffffffc /* address of version definition
					    table */
#define DT_VERDEFNUM          0x6ffffffd /* number of version definitions */
#define DT_VERNEED            0x6ffffffe /* address of table with needed
					    versions */
#define DT_VERNEEDNUM         0x6fffffff /* number of needed versions */
#define DT_VERSIONTAGIDX(tag) (DT_VERNEEDNUM - (tag)) /* reverse order! */
#define DT_VERSIONTAGNUM 16

/*
 * Sun added these machine-independent extensions in the "processor-specific"
 * range.  Be compatible.
 */
#define DT_AUXILIARY        0x7ffffffd /* shared object to load before self */
#define DT_FILTER           0x7fffffff /* shared object to get values from */
#define DT_EXTRATAGIDX(tag) ((uint32_t)-((int32_t)(tag) << 1 >> 1) - 1)
#define DT_EXTRANUM         3

/* values of 'd_un.d_val' in the dt_flags entry */
#define DF_ORIGIN     0x00000001 /* object may use DF_ORIGIN */
#define DF_SYMBOLIC   0x00000002 /* symbol resolutions starts here */
#define DF_TEXTREL    0x00000004 /* object contains text relocations */
#define DF_BIND_NOW   0x00000008 /* no lazy binding for this object */
#define DF_STATIC_TLS 0x00000010 /* module uses the static TLS model */

/*
 * state flags selectable in the 'd_un.d_val' element of the DT_FLAGS_1
 * entry in the dynamic section.
 */
#define DF_1_NOW        0x00000001 /* set RTLD_NOW for this object */
#define DF_1_GLOBAL     0x00000002 /* set RTLD_GLOBAL for this object */
#define DF_1_GROUP      0x00000004 /* set RTLD_GROUP for this object */
#define DF_1_NODELETE   0x00000008 /* set RTLD_NODELETE for this object */
#define DF_1_LOADFLTR   0x00000010 /* trigger filtee loading at runtime */
#define DF_1_INITFIRST  0x00000020 /* set RTLD_INITFIRST for this object */
#define DF_1_NOOPEN     0x00000040 /* set RTLD_NOOPEN for this object */
#define DF_1_ORIGIN     0x00000080 /* $ORIGIN must be handled  */
#define DF_1_DIRECT     0x00000100 /* direct binding enabled */
#define DF_1_TRANS      0x00000200 
#define DF_1_INTERPOSE  0x00000400 /* object is used to interpose */
#define DF_1_NODEFLIB   0x00000800 /* ignore default lib search path */
#define DF_1_NODUMP     0x00001000 /* object can't be dldump'ed */
#define DF_1_CONFALT    0x00002000 /* configuration alternative created */
#define DF_1_ENDFILTEE  0x00004000 /* filtee terminates filters search */
#define DF_1_DISPRELDNE 0x00008000 /* disp reloc applied at build time */
#define DF_1_DISPRELPND 0x00010000 /* disp reloc applied at run-time */
#define DF_1_NODIRECT   0x00020000 /* object has no-direct binding */
#define DF_1_IGNMULDEF  0x00040000 
#define DF_1_NOKSYMS    0x00080000 
#define DF_1_NOHDR      0x00100000 
#define DF_1_EDITED     0x00200000 /* object is modified after built */
#define DF_1_NORELOC    0x00400000 
#define DF_1_SYMINTPOSE 0x00800000 /* object has individual interposers */
#define DF_1_GLOBAUDIT  0x01000000 /* global auditing required */
#define DF_1_SINGLETON  0x02000000 /* singleton symbols are used */
#define DF_1_STUB       0x04000000 
#define DF_1_PIE        0x08000000 
#define DF_1_KMOD       0x10000000 
#define DF_1_WEAKFILTER 0x20000000 
#define DF_1_NOCOMMON   0x40000000 

/* flags for the feature selection in DT_FEATURE_1 */
#define DTF_1_PARINIT 0x00000001 
#define DTF_1_CONFEXP 0x00000002 

/* flags in the DT_POSFLAG_1 entry effecting only the next DT_* entry */
#define DF_P1_LAZYLOAD  0x00000001 /* lazyload following object */
#define DF_P1_GROUPPERM 0x00000002 /* symbols from next object are not
				      generally available */

/* version definition sections */
struct elf32_verdef {
	uint16_t vd_version; /* version revision */
	uint16_t vd_flags;   /* version information */
	uint16_t vd_ndx;     /* version index */
	uint16_t vd_cnt;     /* number of associated aux entries */
	uint32_t vd_hash;    /* version name hash value */
	uint32_t vd_aux;     /* offset in bytes to verdaux array */
	uint32_t vd_next;    /* offset in bytes to next verdef entry */
};

struct elf64_verdef {
	uint16_t vd_version; /* version revision */
	uint16_t vd_flags;   /* version information */
	uint16_t vd_ndx;     /* version index */
	uint16_t vd_cnt;     /* number of associated aux entries */
	uint32_t vd_hash;    /* version name hash value */
	uint32_t vd_aux;     /* offset in bytes to verdaux array */
	uint32_t vd_next;    /* offset in bytes to next verdef entry */
};

typedef uint16_t elf32_versym;
typedef uint16_t elf64_versym;

/* legal values for vd_version (version revision) */
#define VER_DEF_NONE    0   /* no version */
#define VER_DEF_CURRENT 1   /* current version */
#define VER_DEF_NUM     2   /* given version number */

/* legal values for vd_flags (version information flags) */
#define VER_FLG_BASE 0x1 /* version definition of file itself */
#define VER_FLG_WEAK 0x2 /* weak version identifier. also
			    used by vna_flags below */

/* versym symbol index values */
#define VER_NDX_LOCAL     0      /* symbol is local */
#define VER_NDX_GLOBAL    1      /* symbol is global */
#define VER_NDX_LORESERVE 0xff00 /* beginning of reserved entries */
#define VER_NDX_ELIMINATE 0xff01 /* symbol is to be eliminated */

/* auxiliary version information */
struct elf32_verdaux {
	uint32_t vda_name; /* version or dependency names */
	uint32_t vda_next; /* offset in bytes to next verdaux entry */
};

struct elf64_verdaux {
	uint32_t vda_name; /* version or dependency names */
	uint32_t vda_next; /* offset in bytes to next verdaux entry */
};

/* version dependency section */
struct elf32_verneed {
	uint16_t vn_version; /* version of structure */
	uint16_t vn_cnt;     /* number of associated aux entries */
	uint32_t vn_file;    /* offset of filename for this dependency */
	uint32_t vn_aux;     /* offset in bytes to vernaux array */
	uint32_t vn_next;    /* offset in bytes to next verneed entry */
};

struct elf64_verneed {
	uint16_t vn_version; /* version of structure */
	uint16_t vn_cnt;     /* number of associated aux entries */
	uint32_t vn_file;    /* offset of filename for this dependency */
	uint32_t vn_aux;     /* offset in bytes to vernaux array */
	uint32_t vn_next;    /* offset in bytes to next verneed entry */
};

/* legal values for vn_version (version revision).  */
#define VER_NEED_NONE    0 /* no version */
#define VER_NEED_CURRENT 1 /* current version */
#define VER_NEED_NUM     2 /* given version number */

/* auxiliary needed version information */
struct elf32_vernaux {
	uint32_t vna_hash;  /* hash value of dependency name */
	uint16_t vna_flags; /* dependency specific information */
	uint16_t vna_other; /* unused */
	uint32_t vna_name;  /* dependency name string offset */
	uint32_t vna_next;  /* offset in bytes to next vernaux entry */
};

struct elf64_vernaux {
	uint32_t vna_hash;  /* hash value of dependency name */
	uint16_t vna_flags; /* dependency specific information */
	uint16_t vna_other; /* unused */
	uint32_t vna_name;  /* dependency name string offset */
	uint32_t vna_next;  /* offset in bytes to next vernaux entry */
};

/* auxiliary vector */

/*
 * This vector is normally only used by the program interpreter.  The
 * usual definition in an ABI supplement uses the name auxv.  The
 * vector is not usually defined in a standard <elf.h> file, but it
 * can't hurt.  We rename it to avoid conflicts.  The sizes of these
 * types are an arrangement between the exec server and the program
 * interpreter, so we don't fully specify them here.
 */

struct elf32_auxv {
	uint32_t a_type; /* entry type */
	union {
		uint32_t a_val; /* integer value */
	} a_un;
};

struct elf64_auxv {
	uint64_t a_type; /* entry type */
	union {
		uint64_t a_val; /* integer value */
	} a_un;
};

/* elf*_auxv.a_type */
#define AT_NULL              0  /* end of vector */
#define AT_IGNORE            1  /* entry should be ignored */
#define AT_EXECFD            2  /* file descriptor of program */
#define AT_PHDR              3  /* program headers for program */
#define AT_PHENT             4  /* size of program header entry */
#define AT_PHNUM             5  /* number of program headers */
#define AT_PAGESZ            6  /* system page size */
#define AT_BASE              7  /* base address of interpreter */
#define AT_FLAGS             8  /* flags */
#define AT_ENTRY             9  /* entry point of program */
#define AT_NOTELF            10 /* program is not ELF */
#define AT_UID               11 /* real uid */
#define AT_EUID              12 /* effective uid */
#define AT_GID               13 /* real gid */
#define AT_EGID              14 /* effective gid */
#define AT_CLKTCK            17 /* frequency of times() */
/* some more special a_type values describing the hardware */
#define AT_PLATFORM          15 /* string identifying platform */
#define AT_HWCAP             16 /* machine-dependent hints about
				   processor capabilities */
/*
 * This entry gives some information about the FPU initialization
 * performed by the kernel.
 */
#define AT_FPUCW             18 /* used FPU control word */
/* cache block sizes.  */
#define AT_DCACHEBSIZE       19 /* data cache block size */
#define AT_ICACHEBSIZE       20 /* instruction cache block size */
#define AT_UCACHEBSIZE       21 /* unified cache block size */
/*
 * A special ignored value for PPC, used by the kernel to control the
 * interpretation of the AUXV. must be > 16.
 */
#define AT_IGNOREPPC         22 /* entry should be ignored */
#define AT_SECURE            23 /* boolean, was exec setuid-like? */
#define AT_BASE_PLATFORM     24 /* string identifying real platforms */
#define AT_RANDOM            25 /* address of 16 random bytes */
#define AT_HWCAP2            26 /* more machine-dependent hints about
				   processor capabilities */

#define AT_RSEQ_FEATURE_SIZE 27 /* rseq supported feature size */
#define AT_RSEQ_ALIGN        28 /* rseq allocation alignment */

/* more machine-dependent hints about processor capabilities */
#define AT_HWCAP3            29 /* extension of AT_HWCAP */
#define AT_HWCAP4            30 /* extension of AT_HWCAP */
#define AT_EXECFN            31 /* filename of executable */
/*
 * pointer to the global system page used for system calls and other
 * nice things.
 */
#define AT_SYSINFO           32  
#define AT_SYSINFO_EHDR      33  
/*
 * shapes of the caches.  bits 0-3 contains associativity; bits 4-7 contains
 * log2 of line size; mask those to get cache size.
 */
#define AT_L1I_CACHESHAPE    34  
#define AT_L1D_CACHESHAPE    35  
#define AT_L2_CACHESHAPE     36  
#define AT_L3_CACHESHAPE     37  
/*
 * shapes of the caches, with more room to describe them.
 * *GEOMETRY are comprised of cache line size in bytes in the bottom 16 bits
 * and the cache associativity in the next 16 bits.
 */
#define AT_L1I_CACHESIZE     40  
#define AT_L1I_CACHEGEOMETRY 41  
#define AT_L1D_CACHESIZE     42  
#define AT_L1D_CACHEGEOMETRY 43  
#define AT_L2_CACHESIZE      44  
#define AT_L2_CACHEGEOMETRY  45  
#define AT_L3_CACHESIZE      46  
#define AT_L3_CACHEGEOMETRY  47  
#define AT_MINSIGSTKSZ       51 /* stack needed for signal delivery */

/* move records */
struct elf32_move {
	uint64_t m_value;   /* symbol value */
	uint32_t m_info;    /* size and index */
	uint32_t m_poffset; /* symbol offset */
	uint16_t m_repeat;  /* repeat count */
	uint16_t m_stride;  /* stride info */
};

struct elf64_move {
	uint64_t m_value;   /* symbol value */
	uint64_t m_info;    /* size and index */
	uint64_t m_poffset; /* symbol offset */
	uint16_t m_repeat;  /* repeat count */
	uint16_t m_stride;  /* stride info */
};

/* macro to construct move records */
#define ELF32_M_SYM(info) ((info) >> 8)
#define ELF32_M_SIZE(info) ((uint8_t)(info))
#define ELF32_M_INFO(sym, size) (((sym) << 8) + (uint8_t)(size))

#define ELF64_M_SYM(info) ELF32_M_SYM(info)
#define ELF64_M_SIZE(info) ELF32_M_SIZE(info)
#define ELF64_M_INFO(sym, size) ELF32_M_INFO(sym, size)

/* Motorola 68k specific definitions */

/* elf32_ehdr.e_flags */
#define EF_CPU32 0x00810000 

/* m68k relocation for ELF*_R_TYPE */
#define R_68K_NONE         0  /* no relocation */
#define R_68K_32           1  /* direct 32-bit */
#define R_68K_16           2  /* direct 16-bit */
#define R_68K_8            3  /* direct 8-bit */
#define R_68K_PC32         4  /* PC relative 32-bit */
#define R_68K_PC16         5  /* PC relative 16-bit */
#define R_68K_PC8          6  /* PC relative 8-bit */
#define R_68K_GOT32        7  /* 32-bit PC relative GOT entry */
#define R_68K_GOT16        8  /* 16-bit PC relative GOT entry */
#define R_68K_GOT8         9  /* 8-bit PC relative GOT entry */
#define R_68K_GOT32O       10 /* 32-bit GOT offset */
#define R_68K_GOT16O       11 /* 16-bit GOT offset */
#define R_68K_GOT8O        12 /* 8-bit GOT offset */
#define R_68K_PLT32        13 /* 32-bit PC relative PLT address */
#define R_68K_PLT16        14 /* 16-bit PC relative PLT address */
#define R_68K_PLT8         15 /* 8-bit PC relative PLT address */
#define R_68K_PLT32O       16 /* 32-bit PLT offset */
#define R_68K_PLT16O       17 /* 16-bit PLT offset */
#define R_68K_PLT8O        18 /* 8-bit PLT offset */
#define R_68K_COPY         19 /* copy symbol at runtime */
#define R_68K_GLOB_DAT     20 /* create GOT entry */
#define R_68K_JMP_SLOT     21 /* create PLT entry */
#define R_68K_RELATIVE     22 /* adjust by program base */
#define R_68K_TLS_GD32     25 /* 32-bit GOT offset for GD */
#define R_68K_TLS_GD16     26 /* 16-bit GOT offset for GD */
#define R_68K_TLS_GD8      27 /* 8-bit GOT offset for GD */
#define R_68K_TLS_LDM32    28 /* 32-bit GOT offset for LDM */
#define R_68K_TLS_LDM16    29 /* 16-bit GOT offset for LDM */
#define R_68K_TLS_LDM8     30 /* 8-bit GOT offset for LDM */
#define R_68K_TLS_LDO32    31 /* 32-bit module-relative offset */
#define R_68K_TLS_LDO16    32 /* 16-bit module-relative offset */
#define R_68K_TLS_LDO8     33 /* 8-bit module-relative offset */
#define R_68K_TLS_IE32     34 /* 32-bit GOT offset for IE */
#define R_68K_TLS_IE16     35 /* 16-bit GOT offset for IE */
#define R_68K_TLS_IE8      36 /* 8-bit GOT offset for IE */
#define R_68K_TLS_LE32     37 /* 32-bit offset relative to static TLS block */
#define R_68K_TLS_LE16     38 /* 16-bit offset relative to static TLS block */
#define R_68K_TLS_LE8      39 /* 8-bit offset relative to static TLS block */
#define R_68K_TLS_DTPMOD32 40 /* 32-bit module number */
#define R_68K_TLS_DTPREL32 41 /* 32-bit module-relative offset */
#define R_68K_TLS_TPREL32  42 /* 32-bit TP-relative offset */
#define R_68K_NUM          43

/* Intel 80386 specific definitions */

/* i386 relocation for ELF*_R_TYPE */
#define R_386_NONE          0  /* no relocation */
#define R_386_32            1  /* direct 32-bit */
#define R_386_PC32          2  /* PC relative 32-bit */
#define R_386_GOT32         3  /* 32-bit GOT entry */
#define R_386_PLT32         4  /* 32-bit PLT address */
#define R_386_COPY          5  /* copy symbol at runtime */
#define R_386_GLOB_DAT      6  /* create GOT entry */
#define R_386_JMP_SLOT      7  /* create PLT entry */
#define R_386_RELATIVE      8  /* adjust by program base */
#define R_386_GOTOFF        9  /* 32-bit offset to GOT */
#define R_386_GOTPC         10 /* 32-bit PC relative offset to GOT */
#define R_386_32PLT         11 
#define R_386_TLS_TPOFF     14 /* offset in static TLS block */
#define R_386_TLS_IE        15 /* address of GOT entry for static TLS block
				  offset */
#define R_386_TLS_GOTIE     16 /* GOT entry for static TLS block offset */
#define R_386_TLS_LE        17 /* offset relative to static TLS block */
#define R_386_TLS_GD        18 /* direct 32-bit for GNU version of general
				  dynamic thread local data */
#define R_386_TLS_LDM       19 /* direct 32-bit for GNU version of local
				  dynamic thread local data in LE code */
#define R_386_16            20
#define R_386_PC16          21
#define R_386_8             22
#define R_386_PC8           23
#define R_386_TLS_GD_32     24 /* direct 32-bit for general dynamic thread
				  local data */
#define R_386_TLS_GD_PUSH   25 /* tag for pushl in GD TLS code */
#define R_386_TLS_GD_CALL   26 /* relocation for call to __tls_get_addr() */
#define R_386_TLS_GD_POP    27 /* tag for popl in GD TLS code */
#define R_386_TLS_LDM_32    28 /* direct 32-bit for local dynamic thread local
				  data in LE code */
#define R_386_TLS_LDM_PUSH  29 /* tag for pushl in LDM TLS code */
#define R_386_TLS_LDM_CALL  30 /* relocation for call to __tls_get_addr() in
				  LDM code */
#define R_386_TLS_LDM_POP   31 /* tag for popl in LDM TLS code */
#define R_386_TLS_LDO_32    32 /* offset relative to TLS block */
#define R_386_TLS_IE_32     33 /* GOT entry for negated static TLS block
				  offset */
#define R_386_TLS_LE_32     34 /* negated offset relative to static TLS
				  block */
#define R_386_TLS_DTPMOD32  35 /* ID of module containing symbol */
#define R_386_TLS_DTPOFF32  36 /* offset in TLS block */
#define R_386_TLS_TPOFF32   37 /* negated offset in static TLS block */
#define R_386_SIZE32        38 /* 32-bit symbol size */
#define R_386_TLS_GOTDESC   39 /* GOT offset for TLS descriptor */
#define R_386_TLS_DESC_CALL 40 /* marker of call through TLS descriptor
				  for relaxation */
#define R_386_TLS_DESC      41 /* TLS descriptor containing pointer to code
				  and to argument,
				  returning the TLS offset for the symbol */
#define R_386_IRELATIVE     42 /* adjust indirectly by program base */
#define R_386_GOT32X        43 /* load from 32-bit GOT entry, relaxable */
#define R_386_NUM           44

/* SUN SPARC specific definitions */

/* elf*_sym.st_info */
#define STT_SPARC_REGISTER 13 /* global register reserved to app */

/* elf64_ehdr.e_flags */
#define EF_SPARCV9_MM     3   
#define EF_SPARCV9_TSO    0   
#define EF_SPARCV9_PSO    1   
#define EF_SPARCV9_RMO    2   
#define EF_SPARC_LEDATA   0x800000 /* little endian data */
#define EF_SPARC_EXT_MASK 0xFFFF00 
#define EF_SPARC_32PLUS   0x000100 /* generic V8+ features */
#define EF_SPARC_SUN_US1  0x000200 /* Sun UltraSPARC1 extensions */
#define EF_SPARC_HAL_R1   0x000400 /* HAL R1 extensions */
#define EF_SPARC_SUN_US3  0x000800 /* Sun UltraSPARCIII extensions */

/* SPARC relocation for ELF*_R_TYPE */
#define R_SPARC_NONE             0   /* no reloc */
#define R_SPARC_8                1   /* direct 8-bit */
#define R_SPARC_16               2   /* direct 16-bit */
#define R_SPARC_32               3   /* direct 32-bit */
#define R_SPARC_DISP8            4   /* PC relative 8-bit */
#define R_SPARC_DISP16           5   /* PC relative 16-bit */
#define R_SPARC_DISP32           6   /* PC relative 32-bit */
#define R_SPARC_WDISP30          7   /* PC relative 30-bit shifted */
#define R_SPARC_WDISP22          8   /* PC relative 22-bit shifted */
#define R_SPARC_HI22             9   /* high 22-bit */
#define R_SPARC_22               10  /* direct 22-bit */
#define R_SPARC_13               11  /* direct 13-bit */
#define R_SPARC_LO10             12  /* truncated 10-bit */
#define R_SPARC_GOT10            13  /* truncated 10-bit GOT entry */
#define R_SPARC_GOT13            14  /* 13-bit GOT entry */
#define R_SPARC_GOT22            15  /* 22-bit GOT entry shifted */
#define R_SPARC_PC10             16  /* PC relative 10-bit truncated */
#define R_SPARC_PC22             17  /* PC relative 22-bit shifted */
#define R_SPARC_WPLT30           18  /* 30-bit PC relative PLT address */
#define R_SPARC_COPY             19  /* copy symbol at runtime */
#define R_SPARC_GLOB_DAT         20  /* create GOT entry */
#define R_SPARC_JMP_SLOT         21  /* create PLT entry */
#define R_SPARC_RELATIVE         22  /* adjust by program base */
#define R_SPARC_UA32             23  /* direct 32-bit unaligned */

/* Sparc64 relocation for ELF*_R_TYPE */
#define R_SPARC_PLT32            24  /* direct 32-bit ref to PLT entry */
#define R_SPARC_HIPLT22          25  /* high 22-bit PLT entry */
#define R_SPARC_LOPLT10          26  /* truncated 10-bit PLT entry */
#define R_SPARC_PCPLT32          27  /* PC rel 32-bit ref to PLT entry */
#define R_SPARC_PCPLT22          28  /* PC rel high 22-bit PLT entry */
#define R_SPARC_PCPLT10          29  /* PC rel trunc 10-bit PLT entry */
#define R_SPARC_10               30  /* direct 10-bit */
#define R_SPARC_11               31  /* direct 11-bit */
#define R_SPARC_64               32  /* direct 64-bit */
#define R_SPARC_OLO10            33  /* 10-bit with secondary 13-bit addend */
#define R_SPARC_HH22             34  /* top 22-bits of direct 64-bit */
#define R_SPARC_HM10             35  /* high middle 10-bits of ... */
#define R_SPARC_LM22             36  /* low middle 22-bits of ... */
#define R_SPARC_PC_HH22          37  /* top 22-bits of pc rel 64-bit */
#define R_SPARC_PC_HM10          38  /* high middle 10-bit of ... */
#define R_SPARC_PC_LM22          39  /* low miggle 22-bits of ... */
#define R_SPARC_WDISP16          40  /* PC relative 16-bit shifted */
#define R_SPARC_WDISP19          41  /* PC relative 19-bit shifted */
#define R_SPARC_GLOB_JMP         42  /* was part of v9 ABI but was removed */
#define R_SPARC_7                43  /* direct 7-bit */
#define R_SPARC_5                44  /* direct 5-bit */
#define R_SPARC_6                45  /* direct 6-bit */
#define R_SPARC_DISP64           46  /* PC relative 64-bit */
#define R_SPARC_PLT64            47  /* direct 64-bit ref to PLT entry */
#define R_SPARC_HIX22            48  /* high 22-bit complemented */
#define R_SPARC_LOX10            49  /* truncated 11-bit complemented */
#define R_SPARC_H44              50  /* direct high 12 of 44 bit */
#define R_SPARC_M44              51  /* direct mid 22 of 44 bit */
#define R_SPARC_L44              52  /* direct low 10 of 44 bit */
#define R_SPARC_REGISTER         53  /* global register usage */
#define R_SPARC_UA64             54  /* direct 64-bit unaligned */
#define R_SPARC_UA16             55  /* direct 16-bit unaligned */
#define R_SPARC_TLS_GD_HI22      56  
#define R_SPARC_TLS_GD_LO10      57  
#define R_SPARC_TLS_GD_ADD       58  
#define R_SPARC_TLS_GD_CALL      59  
#define R_SPARC_TLS_LDM_HI22     60  
#define R_SPARC_TLS_LDM_LO10     61  
#define R_SPARC_TLS_LDM_ADD      62  
#define R_SPARC_TLS_LDM_CALL     63  
#define R_SPARC_TLS_LDO_HIX22    64  
#define R_SPARC_TLS_LDO_LOX10    65  
#define R_SPARC_TLS_LDO_ADD      66  
#define R_SPARC_TLS_IE_HI22      67  
#define R_SPARC_TLS_IE_LO10      68  
#define R_SPARC_TLS_IE_LD        69  
#define R_SPARC_TLS_IE_LDX       70  
#define R_SPARC_TLS_IE_ADD       71  
#define R_SPARC_TLS_LE_HIX22     72  
#define R_SPARC_TLS_LE_LOX10     73  
#define R_SPARC_TLS_DTPMOD32     74  
#define R_SPARC_TLS_DTPMOD64     75  
#define R_SPARC_TLS_DTPOFF32     76  
#define R_SPARC_TLS_DTPOFF64     77  
#define R_SPARC_TLS_TPOFF32      78  
#define R_SPARC_TLS_TPOFF64      79  
#define R_SPARC_GOTDATA_HIX22    80  
#define R_SPARC_GOTDATA_LOX10    81  
#define R_SPARC_GOTDATA_OP_HIX22 82  
#define R_SPARC_GOTDATA_OP_LOX10 83  
#define R_SPARC_GOTDATA_OP       84  
#define R_SPARC_H34              85  
#define R_SPARC_SIZE32           86  
#define R_SPARC_SIZE64           87  
#define R_SPARC_WDISP10          88  
#define R_SPARC_JMP_IREL         248 
#define R_SPARC_IRELATIVE        249 
#define R_SPARC_GNU_VTINHERIT    250 
#define R_SPARC_GNU_VTENTRY      251 
#define R_SPARC_REV32            252 
#define R_SPARC_NUM              253 

/* elf64_dyn.d_tag */
#define DT_SPARC_REGISTER 0x70000001 
#define DT_SPARC_NUM      2   

/* MIPS R3000 specific definitions */

/* elf32_ehdr.e_flags */
#define EF_MIPS_NOREORDER          1   /* a .noreorder directive was used */
#define EF_MIPS_PIC                2   /* contains PIC code */
#define EF_MIPS_CPIC               4   /* uses PIC calling sequence */
#define EF_MIPS_XGOT               8   
#define EF_MIPS_UCODE              16  
#define EF_MIPS_ABI2               32  
#define EF_MIPS_ABI_ON32           64  
#define EF_MIPS_OPTIONS_FIRST      0x00000080 /* process the .MIPS.options
						 section first by ld */
#define EF_MIPS_32BITMODE          0x00000100 /* indicates code compiled
						 for a 64-bit
					         machine in 32-bit mode
						 (regs are 32-bits wide) */
#define EF_MIPS_FP64               512        /* uses FP64 (12 callee-saved) */
#define EF_MIPS_NAN2008            1024       /* uses IEEE 754-2008 NaN
						 encoding */
#define EF_MIPS_ARCH_ASE           0x0f000000 /* architectural Extensions used
						 by this file */
#define EF_MIPS_ARCH_ASE_MDMX      0x08000000 /* use MDMX multimedia
						 extensions */
#define EF_MIPS_ARCH_ASE_M16       0x04000000 /* use MIPS-16 ISA extensions */
#define EF_MIPS_ARCH_ASE_MICROMIPS 0x02000000 /* use MICROMIPS ISA
						 extensions */
#define EF_MIPS_ARCH               0xf0000000 /* MIPS architecture level */

/* MIPS architecture level */
#define EF_MIPS_ARCH_1        0x00000000 /* -mips1 code */
#define EF_MIPS_ARCH_2        0x10000000 /* -mips2 code */
#define EF_MIPS_ARCH_3        0x20000000 /* -mips3 code */
#define EF_MIPS_ARCH_4        0x30000000 /* -mips4 code */
#define EF_MIPS_ARCH_5        0x40000000 /* -mips5 code */
#define EF_MIPS_ARCH_32       0x50000000 /* MIPS32 code */
#define EF_MIPS_ARCH_64       0x60000000 /* MIPS64 code */
#define EF_MIPS_ARCH_32R2     0x70000000 /* MIPS32r2 code */
#define EF_MIPS_ARCH_64R2     0x80000000 /* MIPS64r2 code */
#define EF_MIPS_ARCH_32R6     0x90000000 /* MIPS32r6 code */
#define EF_MIPS_ARCH_64R6     0xa0000000 /* MIPS64r6 code */
#define EF_MIPS_ABI           0x0000f000 /* the ABI of the file. also see
					    EF_MIPS_ABI2 above */
#define EF_MIPS_ABI_O32       0x00001000 /* The original o32 abi */
#define EF_MIPS_ABI_O64       0x00002000 /* O32 extended to work on 64-bit
					    architectures */
#define EF_MIPS_ABI_EABI32    0x00003000 /* EABI in 32-bit mode */
#define EF_MIPS_ABI_EABI64    0x00004000 /* EABI in 64-bit mode */
#define EF_MIPS_MACH          0x00ff0000 
#define EF_MIPS_MACH_3900     0x00810000 
#define EF_MIPS_MACH_4010     0x00820000 
#define EF_MIPS_MACH_4100     0x00830000 
#define EF_MIPS_MACH_ALLEGREX 0x00840000 
#define EF_MIPS_MACH_4650     0x00850000 
#define EF_MIPS_MACH_4120     0x00870000 
#define EF_MIPS_MACH_4111     0x00880000 
#define EF_MIPS_MACH_SB1      0x008a0000 
#define EF_MIPS_MACH_OCTEON   0x008b0000 
#define EF_MIPS_MACH_XLR      0x008c0000 
#define EF_MIPS_MACH_OCTEON2  0x008d0000 
#define EF_MIPS_MACH_OCTEON3  0x008e0000 
#define EF_MIPS_MACH_5400     0x00910000 
#define EF_MIPS_MACH_5900     0x00920000 
#define EF_MIPS_MACH_IAMR2    0x00930000 
#define EF_MIPS_MACH_5500     0x00980000 
#define EF_MIPS_MACH_9000     0x00990000 
#define EF_MIPS_MACH_LS2E     0x00a00000 
#define EF_MIPS_MACH_LS2F     0x00a10000 
#define EF_MIPS_MACH_GS464    0x00a20000 
#define EF_MIPS_MACH_GS464E   0x00a30000 
#define EF_MIPS_MACH_GS264E   0x00a40000 

/* The following are unofficial names and should not be used */
#define E_MIPS_ARCH_1  EF_MIPS_ARCH_1
#define E_MIPS_ARCH_2  EF_MIPS_ARCH_2
#define E_MIPS_ARCH_3  EF_MIPS_ARCH_3
#define E_MIPS_ARCH_4  EF_MIPS_ARCH_4
#define E_MIPS_ARCH_5  EF_MIPS_ARCH_5
#define E_MIPS_ARCH_32 EF_MIPS_ARCH_32
#define E_MIPS_ARCH_64 EF_MIPS_ARCH_64

/* special section indices */
#define SHN_MIPS_ACOMMON    0xff00 /* allocated common symbols */
#define SHN_MIPS_TEXT       0xff01 /* allocated test symbols */
#define SHN_MIPS_DATA       0xff02 /* allocated data symbols */
#define SHN_MIPS_SCOMMON    0xff03 /* small common symbols */
#define SHN_MIPS_SUNDEFINED 0xff04 /* small undefined symbols */

/* legal values for sh_type field of elf32_shdr */
#define SHT_MIPS_LIBLIST       0x70000000 /* shared objects used in link */
#define SHT_MIPS_MSYM          0x70000001 
#define SHT_MIPS_CONFLICT      0x70000002 /* conflicting symbols */
#define SHT_MIPS_GPTAB         0x70000003 /* global data area sizes */
#define SHT_MIPS_UCODE         0x70000004 /* reserved for SGI/MIPS compilers */
#define SHT_MIPS_DEBUG         0x70000005 /* MIPS ECOFF debugging info */
#define SHT_MIPS_REGINFO       0x70000006 /* register usage information */
#define SHT_MIPS_PACKAGE       0x70000007 
#define SHT_MIPS_PACKSYM       0x70000008 
#define SHT_MIPS_RELD          0x70000009 
#define SHT_MIPS_IFACE         0x7000000b 
#define SHT_MIPS_CONTENT       0x7000000c 
#define SHT_MIPS_OPTIONS       0x7000000d /* miscellaneous options */
#define SHT_MIPS_SHDR          0x70000010 
#define SHT_MIPS_FDESC         0x70000011 
#define SHT_MIPS_EXTSYM        0x70000012 
#define SHT_MIPS_DENSE         0x70000013 
#define SHT_MIPS_PDESC         0x70000014 
#define SHT_MIPS_LOCSYM        0x70000015 
#define SHT_MIPS_AUXSYM        0x70000016 
#define SHT_MIPS_OPTSYM        0x70000017 
#define SHT_MIPS_LOCSTR        0x70000018 
#define SHT_MIPS_LINE          0x70000019 
#define SHT_MIPS_RFDESC        0x7000001a 
#define SHT_MIPS_DELTASYM      0x7000001b 
#define SHT_MIPS_DELTAINST     0x7000001c 
#define SHT_MIPS_DELTACLASS    0x7000001d 
#define SHT_MIPS_DWARF         0x7000001e /* DWARF debugging information */
#define SHT_MIPS_DELTADECL     0x7000001f 
#define SHT_MIPS_SYMBOL_LIB    0x70000020 
#define SHT_MIPS_EVENTS        0x70000021 /* event section */
#define SHT_MIPS_TRANSLATE     0x70000022 
#define SHT_MIPS_PIXIE         0x70000023 
#define SHT_MIPS_XLATE         0x70000024 
#define SHT_MIPS_XLATE_DEBUG   0x70000025 
#define SHT_MIPS_WHIRL         0x70000026 
#define SHT_MIPS_EH_REGION     0x70000027 
#define SHT_MIPS_XLATE_OLD     0x70000028 
#define SHT_MIPS_PDR_EXCEPTION 0x70000029 
#define SHT_MIPS_ABIFLAGS      0x7000002a 
#define SHT_MIPS_XHASH         0x7000002b 

/* legal values for sh_flags field of elf32_shdr */
#define SHF_MIPS_GPREL   0x10000000 /* must be in global data area */
#define SHF_MIPS_MERGE   0x20000000 
#define SHF_MIPS_ADDR    0x40000000 
#define SHF_MIPS_STRINGS 0x80000000 
#define SHF_MIPS_NOSTRIP 0x08000000 
#define SHF_MIPS_LOCAL   0x04000000 
#define SHF_MIPS_NAMES   0x02000000 
#define SHF_MIPS_NODUPE  0x01000000 

/* symbol tables */

/* MIPS specific values for 'st_other' */
#define STO_MIPS_DEFAULT         0x00 
#define STO_MIPS_INTERNAL        0x01 
#define STO_MIPS_HIDDEN          0x02 
#define STO_MIPS_PROTECTED       0x03 
#define STO_MIPS_PLT             0x08 
#define STO_MIPS_SC_ALIGN_UNUSED 0xff 

/* MIPS specific values for `st_info' */
#define STB_MIPS_SPLIT_COMMON 13  

union elf32_gptab {
	struct {
		uint32_t gt_current_g_value; /* -g value used for compilation */
		uint32_t gt_unused;          /* not used */
	} gt_header; /* first entry in section */
	struct {
		uint32_t gt_g_value; /* if this value were used for -g */
		uint32_t gt_bytes;   /* this many bytes would be used */
	} gt_entry; /* subsequent entries in section */
};

/* entry found in sections of type SHT_MIPS_REGINFO */

struct elf32_reginfo {
	uint32_t ri_gprmask;    /* general registers used */
	uint32_t ri_cprmask[4]; /* coprocessor registers used */
	int32_t ri_gp_value;    /* $gp register value */
};

/* entries found in sections of type SHT_MIPS_OPTIONS */

struct elf_options {
	uint8_t kind;     /* determines interpretation of the
			     variable part of descriptor */
	uint8_t size;     /* size of descriptor, including header */
	uint16_t section; /* section header index of section affected,
			     0 for global options */
	uint32_t info;    /* kind-specific information */
};

/* elf_options.kind */
#define ODK_NULL       0 /* undefined */
#define ODK_REGINFO    1 /* register usage information */
#define ODK_EXCEPTIONS 2 /* exception processing options */
#define ODK_PAD        3 /* section padding options */
#define ODK_HWPATCH    4 /* hardware workarounds performed */
#define ODK_FILL       5 /* record the fill value used by the linker */
#define ODK_TAGS       6 /* reserve space for desktop tools to write */
#define ODK_HWAND      7 /* HW workarounds. 'AND' bits when merging */
#define ODK_HWOR       8 /* HW workarounds. 'OR' bits when merging */

/* elf_options.info for ODK_EXCEPTIONS */
#define OEX_FPU_MIN   0x1f    /* FPE's which MUST be enabled */
#define OEX_FPU_MAX   0x1f00  /* FPE's which MAY be enabled */
#define OEX_PAGE0     0x10000 /* page zero must be mapped */
#define OEX_SMM       0x20000 /* force sequential memory mode? */
#define OEX_FPDBUG    0x40000 /* force floating point debug mode? */
#define OEX_PRECISEFP OEX_FPDBUG 
#define OEX_DISMISS   0x80000 /* dismiss invalid address faults? */
#define OEX_FPU_INVAL 0x10 
#define OEX_FPU_DIV0  0x08 
#define OEX_FPU_OFLO  0x04 
#define OEX_FPU_UFLO  0x02 
#define OEX_FPU_INEX  0x01 

/* elf_options.info for ODK_HWPATCH */
#define OHW_R4KEOP    0x1 /* R4000 end-of-page patch */
#define OHW_R8KPFETCH 0x2 /* may need R8000 prefetch patch */
#define OHW_R5KEOP    0x4 /* R5000 end-of-page patch */
#define OHW_R5KCVTL   0x8 /* R5000 cvt.[ds].l bug.  clean=1 */
#define OPAD_PREFIX   0x1 
#define OPAD_POSTFIX  0x2 
#define OPAD_SYMBOL   0x4 

/* entry found in '.options' section */
struct elf_options_hw {
	uint32_t hwp_flags1; /* extra flags */
	uint32_t hwp_flags2; /* extra flags */
};

/* elf_options.info for ODK_HWAND and ODK_HWOR */
#define OHWA0_R4KEOP_CHECKED 0x00000001 
#define OHWA1_R4KEOP_CLEAN   0x00000002 

/* MIPS relocation for ELF*_R_TYPE */
#define R_MIPS_NONE                 0   /* no reloc */
#define R_MIPS_16                   1   /* direct 16-bit */
#define R_MIPS_32                   2   /* direct 32-bit */
#define R_MIPS_REL32                3   /* PC relative 32-bit */
#define R_MIPS_26                   4   /* direct 26-bit shifted */
#define R_MIPS_HI16                 5   /* high 16-bit */
#define R_MIPS_LO16                 6   /* low 16-bit */
#define R_MIPS_GPREL16              7   /* GP relative 16-bit */
#define R_MIPS_LITERAL              8   /* 16-bit literal entry */
#define R_MIPS_GOT16                9   /* 16-bit GOT entry */
#define R_MIPS_PC16                 10  /* PC relative 16-bit */
#define R_MIPS_CALL16               11  /* 16-bit GOT entry for function */
#define R_MIPS_GPREL32              12  /* GP relative 32-bit */
#define R_MIPS_SHIFT5               16  
#define R_MIPS_SHIFT6               17  
#define R_MIPS_64                   18  
#define R_MIPS_GOT_DISP             19  
#define R_MIPS_GOT_PAGE             20  
#define R_MIPS_GOT_OFST             21  
#define R_MIPS_GOT_HI16             22  
#define R_MIPS_GOT_LO16             23  
#define R_MIPS_SUB                  24  
#define R_MIPS_INSERT_A             25  
#define R_MIPS_INSERT_B             26  
#define R_MIPS_DELETE               27  
#define R_MIPS_HIGHER               28  
#define R_MIPS_HIGHEST              29  
#define R_MIPS_CALL_HI16            30  
#define R_MIPS_CALL_LO16            31  
#define R_MIPS_SCN_DISP             32  
#define R_MIPS_REL16                33  
#define R_MIPS_ADD_IMMEDIATE        34  
#define R_MIPS_PJUMP                35  
#define R_MIPS_RELGOT               36  
#define R_MIPS_JALR                 37  
#define R_MIPS_TLS_DTPMOD32         38  /* module number 32-bit */
#define R_MIPS_TLS_DTPREL32         39  /* module-relative offset 32-bit */
#define R_MIPS_TLS_DTPMOD64         40  /* module number 64-bit */
#define R_MIPS_TLS_DTPREL64         41  /* module-relative offset 64-bit */
#define R_MIPS_TLS_GD               42  /* 16-bit GOT offset for GD */
#define R_MIPS_TLS_LDM              43  /* 16-bit GOT offset for LDM */
#define R_MIPS_TLS_DTPREL_HI16      44  /* module-relative offset, high 16-bit */
#define R_MIPS_TLS_DTPREL_LO16      45  /* module-relative offset, low 16-bit */
#define R_MIPS_TLS_GOTTPREL         46  /* 16-bit GOT offset for IE */
#define R_MIPS_TLS_TPREL32          47  /* TP-relative offset, 32-bit */
#define R_MIPS_TLS_TPREL64          48  /* TP-relative offset, 64-bit */
#define R_MIPS_TLS_TPREL_HI16       49  /* TP-relative offset, high 16-bit */
#define R_MIPS_TLS_TPREL_LO16       50  /* TP-relative offset, low 16-bit */
#define R_MIPS_GLOB_DAT             51  
#define R_MIPS_PC21_S2              60  
#define R_MIPS_PC26_S2              61  
#define R_MIPS_PC18_S3              62  
#define R_MIPS_PC19_S2              63  
#define R_MIPS_PCHI16               64  
#define R_MIPS_PCLO16               65  
#define R_MIPS16_26                 100 
#define R_MIPS16_GPREL              101 
#define R_MIPS16_GOT16              102 
#define R_MIPS16_CALL16             103 
#define R_MIPS16_HI16               104 
#define R_MIPS16_LO16               105 
#define R_MIPS16_TLS_GD             106 
#define R_MIPS16_TLS_LDM            107 
#define R_MIPS16_TLS_DTPREL_HI16    108 
#define R_MIPS16_TLS_DTPREL_LO16    109 
#define R_MIPS16_TLS_GOTTPREL       110 
#define R_MIPS16_TLS_TPREL_HI16     111 
#define R_MIPS16_TLS_TPREL_LO16     112 
#define R_MIPS16_PC16_S1            113 
#define R_MIPS_COPY                 126 
#define R_MIPS_JUMP_SLOT            127 
#define R_MIPS_RELATIVE             128 
#define R_MICROMIPS_26_S1           133 
#define R_MICROMIPS_HI16            134 
#define R_MICROMIPS_LO16            135 
#define R_MICROMIPS_GPREL16         136 
#define R_MICROMIPS_LITERAL         137 
#define R_MICROMIPS_GOT16           138 
#define R_MICROMIPS_PC7_S1          139 
#define R_MICROMIPS_PC10_S1         140 
#define R_MICROMIPS_PC16_S1         141 
#define R_MICROMIPS_CALL16          142 
#define R_MICROMIPS_GOT_DISP        145 
#define R_MICROMIPS_GOT_PAGE        146 
#define R_MICROMIPS_GOT_OFST        147 
#define R_MICROMIPS_GOT_HI16        148 
#define R_MICROMIPS_GOT_LO16        149 
#define R_MICROMIPS_SUB             150 
#define R_MICROMIPS_HIGHER          151 
#define R_MICROMIPS_HIGHEST         152 
#define R_MICROMIPS_CALL_HI16       153 
#define R_MICROMIPS_CALL_LO16       154 
#define R_MICROMIPS_SCN_DISP        155 
#define R_MICROMIPS_JALR            156 
#define R_MICROMIPS_HI0_LO16        157 
#define R_MICROMIPS_TLS_GD          162 
#define R_MICROMIPS_TLS_LDM         163 
#define R_MICROMIPS_TLS_DTPREL_HI16 164 
#define R_MICROMIPS_TLS_DTPREL_LO16 165 
#define R_MICROMIPS_TLS_GOTTPREL    166 
#define R_MICROMIPS_TLS_TPREL_HI16  169 
#define R_MICROMIPS_TLS_TPREL_LO16  170 
#define R_MICROMIPS_GPREL7_S2       172 
#define R_MICROMIPS_PC23_S2         173 
#define R_MIPS_PC32                 248 
#define R_MIPS_EH                   249 
#define R_MIPS_GNU_REL16_S2         250 
#define R_MIPS_GNU_VTINHERIT        253 
#define R_MIPS_GNU_VTENTRY          254 
#define R_MIPS_NUM                  255 

/* legal values for p_type field of elf32_phdr */
#define PT_MIPS_REGINFO  0x70000000 /* register usage information */
#define PT_MIPS_RTPROC   0x70000001 /* runtime procedure table */
#define PT_MIPS_OPTIONS  0x70000002 
#define PT_MIPS_ABIFLAGS 0x70000003 /* FP mode requirement */

/* special program header types */
#define PF_MIPS_LOCAL	0x10000000

/* legal values for d_tag field of elf32_dyn */
#define DT_MIPS_RLD_VERSION           0x70000001 /* runtime linker interface version */
#define DT_MIPS_TIME_STAMP            0x70000002 /* timestamp */
#define DT_MIPS_ICHECKSUM             0x70000003 /* checksum */
#define DT_MIPS_IVERSION              0x70000004 /* version string (string tbl index) */
#define DT_MIPS_FLAGS                 0x70000005 /* flags */
#define DT_MIPS_BASE_ADDRESS          0x70000006 /* base address */
#define DT_MIPS_MSYM                  0x70000007 
#define DT_MIPS_CONFLICT              0x70000008 /* address of CONFLICT section */
#define DT_MIPS_LIBLIST               0x70000009 /* address of LIBLIST section */
#define DT_MIPS_LOCAL_GOTNO           0x7000000a /* number of local GOT entries */
#define DT_MIPS_CONFLICTNO            0x7000000b /* number of CONFLICT entries */
#define DT_MIPS_LIBLISTNO             0x70000010 /* number of LIBLIST entries */
#define DT_MIPS_SYMTABNO              0x70000011 /* number of DYNSYM entries */
#define DT_MIPS_UNREFEXTNO            0x70000012 /* first external DYNSYM */
#define DT_MIPS_GOTSYM                0x70000013 /* first GOT entry in DYNSYM */
#define DT_MIPS_HIPAGENO              0x70000014 /* number of GOT page table entries */
#define DT_MIPS_RLD_MAP               0x70000016 /* address of run time loader map */
#define DT_MIPS_DELTA_CLASS           0x70000017 /* delta C++ class definition */
#define DT_MIPS_DELTA_CLASS_NO        0x70000018 /* number of entries in DT_MIPS_DELTA_CLASS */
#define DT_MIPS_DELTA_INSTANCE        0x70000019 /* delta C++ class instances */
#define DT_MIPS_DELTA_INSTANCE_NO     0x7000001a /* number of entries in DT_MIPS_DELTA_INSTANCE */
#define DT_MIPS_DELTA_RELOC           0x7000001b /* delta relocations */
#define DT_MIPS_DELTA_RELOC_NO        0x7000001c /* number of entries in DT_MIPS_DELTA_RELOC */
#define DT_MIPS_DELTA_SYM             0x7000001d /* delta symbols that delta relocations refer to */
#define DT_MIPS_DELTA_SYM_NO          0x7000001e /* number of entries in DT_MIPS_DELTA_SYM */
#define DT_MIPS_DELTA_CLASSSYM        0x70000020 /* delta symbols that hold the class declaration */
#define DT_MIPS_DELTA_CLASSSYM_NO     0x70000021 /* number of entries in DT_MIPS_DELTA_CLASSSYM */
#define DT_MIPS_CXX_FLAGS             0x70000022 /* flags indicating for C++ flavor */
#define DT_MIPS_PIXIE_INIT            0x70000023 
#define DT_MIPS_SYMBOL_LIB            0x70000024 
#define DT_MIPS_LOCALPAGE_GOTIDX      0x70000025 
#define DT_MIPS_LOCAL_GOTIDX          0x70000026 
#define DT_MIPS_HIDDEN_GOTIDX         0x70000027 
#define DT_MIPS_PROTECTED_GOTIDX      0x70000028 
#define DT_MIPS_OPTIONS               0x70000029 /* address of .options */
#define DT_MIPS_INTERFACE             0x7000002a /* address of .interface */
#define DT_MIPS_DYNSTR_ALIGN          0x7000002b 
#define DT_MIPS_INTERFACE_SIZE        0x7000002c /* size of the .interface section */
#define DT_MIPS_RLD_TEXT_RESOLVE_ADDR 0x7000002d /* address of rld_text_rsolve function
						    stored in GOT */
#define DT_MIPS_PERF_SUFFIX           0x7000002e /* default suffix of dso to be added by
						    rld on dlopen() calls */
#define DT_MIPS_COMPACT_SIZE          0x7000002f /* (O32)size of compact rel section */
#define DT_MIPS_GP_VALUE              0x70000030 /* GP value for aux GOTs */
#define DT_MIPS_AUX_DYNAMIC           0x70000031 /* address of aux .dynamic */
/* The address of .got.plt in an executable using the new non-PIC ABI */
#define DT_MIPS_PLTGOT                0x70000032 
/*
 * The base of the PLT in an executable using the new non-PIC ABI if that
 * PLT is writable.  For a non-writable PLT, this is omitted or has a zero
 * value.
 */
#define DT_MIPS_RWPLT                 0x70000034 
/*
 * An alternative description of the classic MIPS RLD_MAP that is usable
 * in a PIE as it stores a relative offset from the address of the tag
 * rather than an absolute address.
 */
#define DT_MIPS_RLD_MAP_REL           0x70000035 
/* GNU-style hash table with xlat */
#define DT_MIPS_XHASH                 0x70000036 
#define DT_MIPS_NUM                   0x37 

/* legal values for DT_MIPS_FLAGS elf32_dyn entry */

#define RHF_NONE                   0        /* no flags */
#define RHF_QUICKSTART             (1 << 0) /* use quickstart */
#define RHF_NOTPOT                 (1 << 1) /* hash size not power of 2 */
#define RHF_NO_LIBRARY_REPLACEMENT (1 << 2) /* ignore LD_LIBRARY_PATH */
#define RHF_NO_MOVE                (1 << 3)
#define RHF_SGI_ONLY               (1 << 4)
#define RHF_GUARANTEE_INIT         (1 << 5)
#define RHF_DELTA_C_PLUS_PLUS      (1 << 6)
#define RHF_GUARANTEE_START_INIT   (1 << 7)
#define RHF_PIXIE                  (1 << 8)
#define RHF_DEFAULT_DELAY_LOAD     (1 << 9)
#define RHF_REQUICKSTART           (1 << 10)
#define RHF_REQUICKSTARTED         (1 << 11)
#define RHF_CORD                   (1 << 12)
#define RHF_NO_UNRES_UNDEF         (1 << 13)
#define RHF_RLD_ORDER_SAFE         (1 << 14)

/* entries found in sections of type SHT_MIPS_LIBLIST */

struct elf32_lib {
	uint32_t l_name;       /* name (string table index) */
	uint32_t l_time_stamp; /* timestamp */
	uint32_t l_checksum;   /* checksum */
	uint32_t l_version;    /* interface version */
	uint32_t l_flags;      /* flags */
};

struct elf64_lib {
	uint32_t l_name;       /* name (string table index) */
	uint32_t l_time_stamp; /* timestamp */
	uint32_t l_checksum;   /* checksum */
	uint32_t l_version;    /* interface version */
	uint32_t l_flags;      /* flags */
};

/* legal values for l_flags */
#define LL_NONE            0
#define LL_EXACT_MATCH     (1 << 0) /* require exact match */
#define LL_IGNORE_INT_VER (1 << 1)  /* ignore interface version */
#define LL_REQUIRE_MINOR  (1 << 2)
#define LL_EXPORTS        (1 << 3)
#define LL_DELAY_LOAD     (1 << 4)
#define LL_DELTA          (1 << 5)

/* entries found in sections of type SHT_MIPS_CONFLICT */

struct elf_mips_abi_flags_v0 {
	uint16_t version;  /* version of flags structure */
	uint8_t isa_level; /* level of the ISA: 1-5, 32, 64 */
	uint8_t isa_rev;   /* revision of ISA: 0 for MIPS V and below, 1-n otherwise */
	uint8_t gpr_size;  /* size of general purpose registers */
	uint8_t cpr1_size; /* size of co-processor 1 registers */
	uint8_t cpr2_size; /* size of co-processor 2 registers */
	uint8_t fp_abi;    /* floating-point ABI */
	uint32_t isa_ext;  /* processor-specific extension */
	uint32_t ases;     /* mask of ASEs used */
	uint32_t flags1;   /* mask of general flags */
	uint32_t flags2;
};

/* values for the register size bytes of an abi flags structure */

#define MIPS_AFL_REG_NONE 0x00 /* no registers */
#define MIPS_AFL_REG_32   0x01 /* 32-bit registers */
#define MIPS_AFL_REG_64   0x02 /* 64-bit registers */
#define MIPS_AFL_REG_128  0x03 /* 128-bit registers */

/* masks for the ases word of an ABI flags structure */

#define MIPS_AFL_ASE_DSP       0x00000001 /* DSP ASE */
#define MIPS_AFL_ASE_DSPR2     0x00000002 /* DSP R2 ASE */
#define MIPS_AFL_ASE_EVA       0x00000004 /* Enhanced VA Scheme */
#define MIPS_AFL_ASE_MCU       0x00000008 /* MCU (MicroController) ASE */
#define MIPS_AFL_ASE_MDMX      0x00000010 /* MDMX ASE */
#define MIPS_AFL_ASE_MIPS3D    0x00000020 /* MIPS-3D ASE */
#define MIPS_AFL_ASE_MT        0x00000040 /* MT ASE */
#define MIPS_AFL_ASE_SMARTMIPS 0x00000080 /* SmartMIPS ASE */
#define MIPS_AFL_ASE_VIRT      0x00000100 /* VZ ASE */
#define MIPS_AFL_ASE_MSA       0x00000200 /* MSA ASE */
#define MIPS_AFL_ASE_MIPS16    0x00000400 /* MIPS16 ASE */
#define MIPS_AFL_ASE_MICROMIPS 0x00000800 /* MICROMIPS ASE */
#define MIPS_AFL_ASE_XPA       0x00001000 /* XPA ASE */
#define MIPS_AFL_ASE_MASK      0x00001fff /* All ASEs */

/* values for the isa_ext word of an ABI flags structure */

#define MIPS_AFL_EXT_XLR         1  /* RMI Xlr instruction */
#define MIPS_AFL_EXT_OCTEON2     2  /* Cavium Networks Octeon2 */
#define MIPS_AFL_EXT_OCTEONP     3  /* Cavium Networks OcteonP */
#define MIPS_AFL_EXT_LOONGSON_3A 4  /* Loongson 3A */
#define MIPS_AFL_EXT_OCTEON      5  /* Cavium Networks Octeon */
#define MIPS_AFL_EXT_5900        6  /* MIPS R5900 instruction */
#define MIPS_AFL_EXT_4650        7  /* MIPS R4650 instruction */
#define MIPS_AFL_EXT_4010        8  /* LSI R4010 instruction */
#define MIPS_AFL_EXT_4100        9  /* NEC VR4100 instruction */
#define MIPS_AFL_EXT_3900        10 /* Toshiba R3900 instruction */
#define MIPS_AFL_EXT_10000       11 /* MIPS R10000 instruction */
#define MIPS_AFL_EXT_SB1         12 /* Broadcom SB-1 instruction */
#define MIPS_AFL_EXT_4111        13 /* NEC VR4111/VR4181 instruction */
#define MIPS_AFL_EXT_4120        14 /* NEC VR4120 instruction */
#define MIPS_AFL_EXT_5400        15 /* NEC VR5400 instruction */
#define MIPS_AFL_EXT_5500        16 /* NEC VR5500 instruction */
#define MIPS_AFL_EXT_LOONGSON_2E 17 /* ST Microelectronics Loongson 2E */
#define MIPS_AFL_EXT_LOONGSON_2F 18 /* ST Microelectronics Loongson 2F */

/* masks for the flags1 word of an ABI flags structure */
#define MIPS_AFL_FLAGS1_ODDSPREG 1 /* Uses odd single-precision registers */

/* object attribute values */
enum {
	/* not tagged or not using any ABIs affected by the differences */
	Val_GNU_MIPS_ABI_FP_ANY = 0,
	/* using hard-float -mdouble-float */
	Val_GNU_MIPS_ABI_FP_DOUBLE = 1,
	/* using hard-float -msingle-float */
	Val_GNU_MIPS_ABI_FP_SINGLE = 2,
	/* using soft-float */
	Val_GNU_MIPS_ABI_FP_SOFT = 3,
	/* using -mips32r2 -mfp64 */
	Val_GNU_MIPS_ABI_FP_OLD_64 = 4,
	/* using -mfpxx */
	Val_GNU_MIPS_ABI_FP_XX = 5,
	/* using -mips32r2 -mfp64 */
	Val_GNU_MIPS_ABI_FP_64 = 6,
	/* using -mips32r2 -mfp64 -mno-odd-spreg */
	Val_GNU_MIPS_ABI_FP_64A = 7,
	/* maximum allocated FP ABI value */
	Val_GNU_MIPS_ABI_FP_MAX = 7
};

/* HPPA specific definitions */

/* legal values for e_flags field of elf32_ehdr */
#define EF_PARISC_TRAPNIL  0x00010000 /* trap nil pointer dereference */
#define EF_PARISC_EXT      0x00020000 /* program uses arch. extensions */
#define EF_PARISC_LSB      0x00040000 /* program expects little endian */
#define EF_PARISC_WIDE     0x00080000 /* program expects wide mode */
#define EF_PARISC_NO_KABP  0x00100000 /* no kernel assisted branch prediction */
#define EF_PARISC_LAZYSWAP 0x00400000 /* allow lazy swapping */
#define EF_PARISC_ARCH     0x0000ffff /* architecture version */

/* defined values for `e_flags & EF_PARISC_ARCH' are: */
#define EFA_PARISC_1_0 0x020b /* PA-RISC 1.0 big-endian */
#define EFA_PARISC_1_1 0x0210 /* PA-RISC 1.1 big-endian */
#define EFA_PARISC_2_0 0x0214 /* PA-RISC 2.0 big-endian */

/* additional section indices */
#define SHN_PARISC_ANSI_COMMON 0xff00 /* section for tentatively declared symbols in ANSI C */
#define SHN_PARISC_HUGE_COMMON 0xff01 /* common blocks in huge model */

/* legal values for sh_type field of elf32_shdr */
#define SHT_PARISC_EXT    0x70000000 /* contains product specific ext */
#define SHT_PARISC_UNWIND 0x70000001 /* unwind information */
#define SHT_PARISC_DOC    0x70000002 /* debug info for optimized code */

/* legal values for sh_flags field of elf32_shdr */
#define SHF_PARISC_SHORT 0x20000000 /* section with short addressing */
#define SHF_PARISC_HUGE  0x40000000 /* section far from gp */
#define SHF_PARISC_SBP   0x80000000 /* static branch prediction code */

/* legal values for ST_TYPE subfield of st_info (symbol type) */
#define STT_PARISC_MILLICODE 13 /* millicode function entry point */

#define STT_HP_OPAQUE (STT_LOOS + 0x1)
#define STT_HP_STUB   (STT_LOOS + 0x2)

/* HPPA relocation  */
#define R_PARISC_NONE           0   /* no relocation */
#define R_PARISC_DIR32          1   /* direct 32-bit reference */
#define R_PARISC_DIR21L         2   /* left 21-bit of eff. address */
#define R_PARISC_DIR17R         3   /* right 17-bit of eff. address */
#define R_PARISC_DIR17F         4   /* 17-bit of eff. address */
#define R_PARISC_DIR14R         6   /* right 14-bit of eff. address */
#define R_PARISC_PCREL32        9   /* 32-bit rel. address */
#define R_PARISC_PCREL21L       10  /* left 21-bit of rel. address */
#define R_PARISC_PCREL17R       11  /* right 17-bit of rel. address */
#define R_PARISC_PCREL17F       12  /* 17-bit of rel. address */
#define R_PARISC_PCREL14R       14  /* right 14-bit of rel. address */
#define R_PARISC_DPREL21L       18  /* left 21-bit of rel. address */
#define R_PARISC_DPREL14R       22  /* right 14-bit of rel. address */
#define R_PARISC_GPREL21L       26  /* GP-relative, left 21-bit */
#define R_PARISC_GPREL14R       30  /* GP-relative, right 14-bit */
#define R_PARISC_LTOFF21L       34  /* LT-relative, left 21-bit */
#define R_PARISC_LTOFF14R       38  /* LT-relative, right 14-bit */
#define R_PARISC_SECREL32       41  /* 32-bit section rel. address */
#define R_PARISC_SEGBASE        48  /* no relocation, set segment base */
#define R_PARISC_SEGREL32       49  /* 32-bit segment rel. address */
#define R_PARISC_PLTOFF21L      50  /* PLT rel. address, left 21-bit */
#define R_PARISC_PLTOFF14R      54  /* PLT rel. address, right 14-bit */
#define R_PARISC_LTOFF_FPTR32   57  /* 32-bit LT-rel. function pointer */
#define R_PARISC_LTOFF_FPTR21L  58  /* LT-rel. fct ptr, left 21-bit */
#define R_PARISC_LTOFF_FPTR14R  62  /* LT-rel. fct ptr, right 14-bit */
#define R_PARISC_FPTR64         64  /* 64-bit function address */
#define R_PARISC_PLABEL32       65  /* 32-bit function address */
#define R_PARISC_PLABEL21L      66  /* left 21-bit of fdesc address */
#define R_PARISC_PLABEL14R      70  /* right 14-bit of fdesc address */
#define R_PARISC_PCREL64        72  /* 64-bit PC-rel. address */
#define R_PARISC_PCREL22F       74  /* 22-bit PC-rel. address */
#define R_PARISC_PCREL14WR      75  /* PC-rel. address, right 14-bit */
#define R_PARISC_PCREL14DR      76  /* PC rel. address, right 14-bit */
#define R_PARISC_PCREL16F       77  /* 16-bit PC-rel. address */
#define R_PARISC_PCREL16WF      78  /* 16-bit PC-rel. address */
#define R_PARISC_PCREL16DF      79  /* 16-bit PC-rel. address */
#define R_PARISC_DIR64          80  /* 64-bit of eff. address */
#define R_PARISC_DIR14WR        83  /* 14-bit of eff. address */
#define R_PARISC_DIR14DR        84  /* 14-bit of eff. address */
#define R_PARISC_DIR16F         85  /* 16-bit of eff. address */
#define R_PARISC_DIR16WF        86  /* 16-bit of eff. address */
#define R_PARISC_DIR16DF        87  /* 16-bit of eff. address */
#define R_PARISC_GPREL64        88  /* 64-bit of GP-rel. address */
#define R_PARISC_GPREL14WR      91  /* GP-rel. address, right 14-bit */
#define R_PARISC_GPREL14DR      92  /* GP-rel. address, right 14-bit */
#define R_PARISC_GPREL16F       93  /* 16-bit GP-rel. address */
#define R_PARISC_GPREL16WF      94  /* 16-bit GP-rel. address */
#define R_PARISC_GPREL16DF      95  /* 16-bit GP-rel. address */
#define R_PARISC_LTOFF64        96  /* 64-bit LT-rel. address */
#define R_PARISC_LTOFF14WR      99  /* LT-rel. address, right 14-bit */
#define R_PARISC_LTOFF14DR      100 /* LT-rel. address, right 14-bit */
#define R_PARISC_LTOFF16F       101 /* 16-bit LT-rel. address */
#define R_PARISC_LTOFF16WF      102 /* 16-bit LT-rel. address */
#define R_PARISC_LTOFF16DF      103 /* 16-bit LT-rel. address */
#define R_PARISC_SECREL64       104 /* 64-bit section rel. address */
#define R_PARISC_SEGREL64       112 /* 64-bit segment rel. address */
#define R_PARISC_PLTOFF14WR     115 /* PLT-rel. address, right 14-bit */
#define R_PARISC_PLTOFF14DR     116 /* PLT-rel. address, right 14-bit */
#define R_PARISC_PLTOFF16F      117 /* 16-bit LT-rel. address */
#define R_PARISC_PLTOFF16WF     118 /* 16-bit PLT-rel. address */
#define R_PARISC_PLTOFF16DF     119 /* 16-bit PLT-rel. address */
#define R_PARISC_LTOFF_FPTR64   120 /* 64-bit LT-rel. function ptr */
#define R_PARISC_LTOFF_FPTR14WR 123 /* LT-rel. fct. ptr., right 14-bit */
#define R_PARISC_LTOFF_FPTR14DR 124 /* LT-rel. fct. ptr., right 14-bit */
#define R_PARISC_LTOFF_FPTR16F  125 /* 16-bit LT-rel. function ptr */
#define R_PARISC_LTOFF_FPTR16WF 126 /* 16-bit LT-rel. function ptr */
#define R_PARISC_LTOFF_FPTR16DF 127 /* 16-bit LT-rel. function ptr */
#define R_PARISC_LORESERVE      128 
#define R_PARISC_COPY           128 /* copy relocation */
#define R_PARISC_IPLT           129 /* dynamic reloc, imported PLT */
#define R_PARISC_EPLT           130 /* dynamic reloc, exported PLT */
#define R_PARISC_TPREL32        153 /* 32-bit TP-rel. address */
#define R_PARISC_TPREL21L       154 /* TP-rel. address, left 21-bit */
#define R_PARISC_TPREL14R       158 /* TP-rel. address, right 14-bit */
#define R_PARISC_LTOFF_TP21L    162 /* LT-TP-rel. address, left 21-bit */
#define R_PARISC_LTOFF_TP14R    166 /* LT-TP-rel. address, right 14-bit */
#define R_PARISC_LTOFF_TP14F    167 /* 14-bit LT-TP-rel. address */
#define R_PARISC_TPREL64        216 /* 64-bit TP-rel. address */
#define R_PARISC_TPREL14WR      219 /* TP-rel. address, right 14-bit */
#define R_PARISC_TPREL14DR      220 /* TP-rel. address, right 14-bit */
#define R_PARISC_TPREL16F       221 /* 16-bit TP-rel. address */
#define R_PARISC_TPREL16WF      222 /* 16-bit TP-rel. address */
#define R_PARISC_TPREL16DF      223 /* 16-bit TP-rel. address */
#define R_PARISC_LTOFF_TP64     224 /* 64-bit LT-TP-rel. address */
#define R_PARISC_LTOFF_TP14WR   227 /* LT-TP-rel. address, right 14-bit */
#define R_PARISC_LTOFF_TP14DR   228 /* LT-TP-rel. address, right 14-bit */
#define R_PARISC_LTOFF_TP16F    229 /* 16-bit LT-TP-rel. address */
#define R_PARISC_LTOFF_TP16WF   230 /* 16-bit LT-TP-rel. address */
#define R_PARISC_LTOFF_TP16DF   231 /* 16-bit LT-TP-rel. address */
#define R_PARISC_GNU_VTENTRY    232 
#define R_PARISC_GNU_VTINHERIT  233 
#define R_PARISC_TLS_GD21L      234 /* GD 21-bit left */
#define R_PARISC_TLS_GD14R      235 /* GD 14-bit right */
#define R_PARISC_TLS_GDCALL     236 /* GD call to __t_g_a */
#define R_PARISC_TLS_LDM21L     237 /* LD module 21-bit left */
#define R_PARISC_TLS_LDM14R     238 /* LD module 14-bit right */
#define R_PARISC_TLS_LDMCALL    239 /* LD module call to __t_g_a */
#define R_PARISC_TLS_LDO21L     240 /* LD offset 21-bit left */
#define R_PARISC_TLS_LDO14R     241 /* LD offset 14-bit right */
#define R_PARISC_TLS_DTPMOD32   242 /* DTP module 32-bit */
#define R_PARISC_TLS_DTPMOD64   243 /* DTP module 64-bit */
#define R_PARISC_TLS_DTPOFF32   244 /* DTP offset 32-bit */
#define R_PARISC_TLS_DTPOFF64   245 /* DTP offset 32-bit */
#define R_PARISC_TLS_LE21L      R_PARISC_TPREL21L 
#define R_PARISC_TLS_LE14R      R_PARISC_TPREL14R 
#define R_PARISC_TLS_IE21L      R_PARISC_LTOFF_TP21L 
#define R_PARISC_TLS_IE14R      R_PARISC_LTOFF_TP14R 
#define R_PARISC_TLS_TPREL32    R_PARISC_TPREL32 
#define R_PARISC_TLS_TPREL64    R_PARISC_TPREL64 
#define R_PARISC_HIRESERVE      255 

/* legal values for p_type field of elf32_phdr/elf64_phdr */
#define PT_HP_TLS           (PT_LOOS + 0x0) 
#define PT_HP_CORE_NONE     (PT_LOOS + 0x1) 
#define PT_HP_CORE_VERSION  (PT_LOOS + 0x2) 
#define PT_HP_CORE_KERNEL   (PT_LOOS + 0x3) 
#define PT_HP_CORE_COMM     (PT_LOOS + 0x4) 
#define PT_HP_CORE_PROC     (PT_LOOS + 0x5) 
#define PT_HP_CORE_LOADABLE (PT_LOOS + 0x6) 
#define PT_HP_CORE_STACK    (PT_LOOS + 0x7) 
#define PT_HP_CORE_SHM      (PT_LOOS + 0x8) 
#define PT_HP_CORE_MMF      (PT_LOOS + 0x9) 
#define PT_HP_PARALLEL      (PT_LOOS + 0x10) 
#define PT_HP_FASTBIND      (PT_LOOS + 0x11) 
#define PT_HP_OPT_ANNOT     (PT_LOOS + 0x12) 
#define PT_HP_HSL_ANNOT     (PT_LOOS + 0x13) 
#define PT_HP_STACK         (PT_LOOS + 0x14) 

#define PT_PARISC_ARCHEXT 0x70000000 
#define PT_PARISC_UNWIND  0x70000001 

/* legal values for p_flags field of elf32_phdr/elf64_phdr */
#define PF_PARISC_SBP 0x08000000 

#define PF_HP_PAGE_SIZE   0x00100000 
#define PF_HP_FAR_SHARED  0x00200000 
#define PF_HP_NEAR_SHARED 0x00400000 
#define PF_HP_CODE        0x01000000 
#define PF_HP_MODIFY      0x02000000 
#define PF_HP_LAZYSWAP    0x04000000 
#define PF_HP_SBP         0x08000000 

/* Alpha specific definitions */

/* legal values for e_flags field of elf64_ehdr */
#define EF_ALPHA_32BIT    1   /* all addresses must be < 2GB */
#define EF_ALPHA_CANRELAX 2   /* relocations for relaxing exist */

/* legal values for sh_type field of elf64_shdr */

/* These two are primerily concerned with ECOFF debugging info */
#define SHT_ALPHA_DEBUG   0x70000001 
#define SHT_ALPHA_REGINFO 0x70000002 

/* legal values for sh_flags field of elf64_shdr */
#define SHF_ALPHA_GPREL 0x10000000 

/* legal values for st_other field of elf64_sym */
#define STO_ALPHA_NOPV       0x80 /* no PV required */
#define STO_ALPHA_STD_GPLOAD 0x88 /* PV only used for initial ldgp */

/* Alpha relocation for ELF*_R_TYPE */
#define R_ALPHA_NONE      0   /* no relocation */
#define R_ALPHA_REFLONG   1   /* direct 32 bit */
#define R_ALPHA_REFQUAD   2   /* direct 64 bit */
#define R_ALPHA_GPREL32   3   /* GP relative 32 bit */
#define R_ALPHA_LITERAL   4   /* GP relative 16 bit w/optimization */
#define R_ALPHA_LITUSE    5   /* optimization hint for LITERAL */
#define R_ALPHA_GPDISP    6   /* add displacement to GP */
#define R_ALPHA_BRADDR    7   /* PC+4 relative 23 bit shifted */
#define R_ALPHA_HINT      8   /* PC+4 relative 16 bit shifted */
#define R_ALPHA_SREL16    9   /* PC relative 16 bit */
#define R_ALPHA_SREL32    10  /* PC relative 32 bit */
#define R_ALPHA_SREL64    11  /* PC relative 64 bit */
#define R_ALPHA_GPRELHIGH 17  /* GP relative 32 bit, high 16 bits */
#define R_ALPHA_GPRELLOW  18  /* GP relative 32 bit, low 16 bits */
#define R_ALPHA_GPREL16   19  /* GP relative 16 bit */
#define R_ALPHA_COPY      24  /* copy symbol at runtime */
#define R_ALPHA_GLOB_DAT  25  /* create GOT entry */
#define R_ALPHA_JMP_SLOT  26  /* create PLT entry */
#define R_ALPHA_RELATIVE  27  /* adjust by program base */
#define R_ALPHA_TLS_GD_HI 28  
#define R_ALPHA_TLSGD     29  
#define R_ALPHA_TLS_LDM   30  
#define R_ALPHA_DTPMOD64  31  
#define R_ALPHA_GOTDTPREL 32  
#define R_ALPHA_DTPREL64  33  
#define R_ALPHA_DTPRELHI  34  
#define R_ALPHA_DTPRELLO  35  
#define R_ALPHA_DTPREL16  36  
#define R_ALPHA_GOTTPREL  37  
#define R_ALPHA_TPREL64   38  
#define R_ALPHA_TPRELHI   39  
#define R_ALPHA_TPRELLO   40  
#define R_ALPHA_TPREL16   41  
#define R_ALPHA_NUM       46  

/* magic values of the LITUSE relocation addend */
#define LITUSE_ALPHA_ADDR    0   
#define LITUSE_ALPHA_BASE    1   
#define LITUSE_ALPHA_BYTOFF  2   
#define LITUSE_ALPHA_JSR     3   
#define LITUSE_ALPHA_TLS_GD  4   
#define LITUSE_ALPHA_TLS_LDM 5   

/* legal values for d_tag of elf64_dyn */
#define DT_ALPHA_PLTRO (DT_LOPROC + 0)
#define DT_ALPHA_NUM 1

/* PowerPC specific declarations */

/* Values for elf32/64_ehdr.e_flags */
#define EF_PPC_EMB 0x80000000 /* PowerPC embedded flag */

/* Cygnus local bits below */
#define EF_PPC_RELOCATABLE     0x00010000 /* PowerPC -mrelocatable flag */
#define EF_PPC_RELOCATABLE_LIB 0x00008000 /* PowerPC -mrelocatable-lib flag */

/* PowerPC relocations defined by the ABIs */
#define R_PPC_NONE            0   
#define R_PPC_ADDR32          1   /* 32bit absolute address */
#define R_PPC_ADDR24          2   /* 26bit address, 2 bits ignored */
#define R_PPC_ADDR16          3   /* 16bit absolute address */
#define R_PPC_ADDR16_LO       4   /* lower 16bit of absolute address */
#define R_PPC_ADDR16_HI       5   /* high 16bit of absolute address */
#define R_PPC_ADDR16_HA       6   /* adjusted high 16bit */
#define R_PPC_ADDR14          7   /* 16bit address, 2 bits ignored */
#define R_PPC_ADDR14_BRTAKEN  8   
#define R_PPC_ADDR14_BRNTAKEN 9   
#define R_PPC_REL24           10  /* PC relative 26 bit */
#define R_PPC_REL14           11  /* PC relative 16 bit */
#define R_PPC_REL14_BRTAKEN   12  
#define R_PPC_REL14_BRNTAKEN  13  
#define R_PPC_GOT16           14  
#define R_PPC_GOT16_LO        15  
#define R_PPC_GOT16_HI        16  
#define R_PPC_GOT16_HA        17  
#define R_PPC_PLTREL24        18  
#define R_PPC_COPY            19  
#define R_PPC_GLOB_DAT        20  
#define R_PPC_JMP_SLOT        21  
#define R_PPC_RELATIVE        22  
#define R_PPC_LOCAL24PC       23  
#define R_PPC_UADDR32         24  
#define R_PPC_UADDR16         25  
#define R_PPC_REL32           26  
#define R_PPC_PLT32           27  
#define R_PPC_PLTREL32        28  
#define R_PPC_PLT16_LO        29  
#define R_PPC_PLT16_HI        30  
#define R_PPC_PLT16_HA        31  
#define R_PPC_SDAREL16        32  
#define R_PPC_SECTOFF         33  
#define R_PPC_SECTOFF_LO      34  
#define R_PPC_SECTOFF_HI      35  
#define R_PPC_SECTOFF_HA      36  

/* PowerPC relocations defined for the TLS access ABI */
#define R_PPC_TLS             67  /* none (sym+add)@tls */
#define R_PPC_DTPMOD32        68  /* word32 (sym+add)@dtpmod */
#define R_PPC_TPREL16         69  /* half16* (sym+add)@tprel */
#define R_PPC_TPREL16_LO      70  /* half16 (sym+add)@tprel@l */
#define R_PPC_TPREL16_HI      71  /* half16 (sym+add)@tprel@h */
#define R_PPC_TPREL16_HA      72  /* half16 (sym+add)@tprel@ha */
#define R_PPC_TPREL32         73  /* word32 (sym+add)@tprel */
#define R_PPC_DTPREL16        74  /* half16* (sym+add)@dtprel */
#define R_PPC_DTPREL16_LO     75  /* half16 (sym+add)@dtprel@l */
#define R_PPC_DTPREL16_HI     76  /* half16 (sym+add)@dtprel@h */
#define R_PPC_DTPREL16_HA     77  /* half16 (sym+add)@dtprel@ha */
#define R_PPC_DTPREL32        78  /* word32 (sym+add)@dtprel */
#define R_PPC_GOT_TLSGD16     79  /* half16* (sym+add)@got@tlsgd */
#define R_PPC_GOT_TLSGD16_LO  80  /* half16 (sym+add)@got@tlsgd@l */
#define R_PPC_GOT_TLSGD16_HI  81  /* half16 (sym+add)@got@tlsgd@h */
#define R_PPC_GOT_TLSGD16_HA  82  /* half16 (sym+add)@got@tlsgd@ha */
#define R_PPC_GOT_TLSLD16     83  /* half16* (sym+add)@got@tlsld */
#define R_PPC_GOT_TLSLD16_LO  84  /* half16 (sym+add)@got@tlsld@l */
#define R_PPC_GOT_TLSLD16_HI  85  /* half16 (sym+add)@got@tlsld@h */
#define R_PPC_GOT_TLSLD16_HA  86  /* half16 (sym+add)@got@tlsld@ha */
#define R_PPC_GOT_TPREL16     87  /* half16* (sym+add)@got@tprel */
#define R_PPC_GOT_TPREL16_LO  88  /* half16 (sym+add)@got@tprel@l */
#define R_PPC_GOT_TPREL16_HI  89  /* half16 (sym+add)@got@tprel@h */
#define R_PPC_GOT_TPREL16_HA  90  /* half16 (sym+add)@got@tprel@ha */
#define R_PPC_GOT_DTPREL16    91  /* half16* (sym+add)@got@dtprel */
#define R_PPC_GOT_DTPREL16_LO 92  /* half16* (sym+add)@got@dtprel@l */
#define R_PPC_GOT_DTPREL16_HI 93  /* half16* (sym+add)@got@dtprel@h */
#define R_PPC_GOT_DTPREL16_HA 94  /* half16* (sym+add)@got@dtprel@ha */
#define R_PPC_TLSGD           95  /* none (sym+add)@tlsgd */
#define R_PPC_TLSLD           96  /* none (sym+add)@tlsld */

/*
 * The remaining relocs are from the Embedded ELF ABI, and are not
 * in the SVR4 ELF ABI.
 */
#define R_PPC_EMB_NADDR32    101 
#define R_PPC_EMB_NADDR16    102 
#define R_PPC_EMB_NADDR16_LO 103 
#define R_PPC_EMB_NADDR16_HI 104 
#define R_PPC_EMB_NADDR16_HA 105 
#define R_PPC_EMB_SDAI16     106 
#define R_PPC_EMB_SDA2I16    107 
#define R_PPC_EMB_SDA2REL    108 
#define R_PPC_EMB_SDA21      109 /* 16 bit offset in SDA */
#define R_PPC_EMB_MRKREF     110 
#define R_PPC_EMB_RELSEC16   111 
#define R_PPC_EMB_RELST_LO   112 
#define R_PPC_EMB_RELST_HI   113 
#define R_PPC_EMB_RELST_HA   114 
#define R_PPC_EMB_BIT_FLD    115 
#define R_PPC_EMB_RELSDA     116 /* 16 bit relative offset in SDA */

/* Diab tool relocations */
#define R_PPC_DIAB_SDA21_LO  180 /* like EMB_SDA21, but lower 16 bit */
#define R_PPC_DIAB_SDA21_HI  181 /* like EMB_SDA21, but high 16 bit */
#define R_PPC_DIAB_SDA21_HA  182 /* like EMB_SDA21, adjusted high 16 */
#define R_PPC_DIAB_RELSDA_LO 183 /* like EMB_RELSDA, but lower 16 bit */
#define R_PPC_DIAB_RELSDA_HI 184 /* like EMB_RELSDA, but high 16 bit */
#define R_PPC_DIAB_RELSDA_HA 185 /* like EMB_RELSDA, adjusted high 16 */

/* GNU extension to support local ifunc */
#define R_PPC_IRELATIVE 248 

/* GNU relocs used in PIC code sequences */
#define R_PPC_REL16    249 /* half16 (sym+add-.) */
#define R_PPC_REL16_LO 250 /* half16 (sym+add-.)@l */
#define R_PPC_REL16_HI 251 /* half16 (sym+add-.)@h */
#define R_PPC_REL16_HA 252 /* half16 (sym+add-.)@ha */

/*
 * This is a phony reloc to handle any old fashioned TOC16 references
 * that may still be in object files
 */
#define R_PPC_TOC16 255

/* PowerPC specific values for the Dyn d_tag field */
#define DT_PPC_GOT (DT_LOPROC + 0)
#define DT_PPC_OPT (DT_LOPROC + 1)
#define DT_PPC_NUM 2

/* PowerPC specific values for the DT_PPC_OPT Dyn entry */
#define PPC_OPT_TLS 1

/* PowerPC64 relocations defined by the ABIs */
#define R_PPC64_NONE            R_PPC_NONE 
#define R_PPC64_ADDR32          R_PPC_ADDR32 /* 32bit absolute address */
#define R_PPC64_ADDR24          R_PPC_ADDR24 /* 26bit address, word aligned */
#define R_PPC64_ADDR16          R_PPC_ADDR16 /* 16bit absolute address */
#define R_PPC64_ADDR16_LO       R_PPC_ADDR16_LO /* lower 16bits of address */
#define R_PPC64_ADDR16_HI       R_PPC_ADDR16_HI /* high 16bits of address */
#define R_PPC64_ADDR16_HA       R_PPC_ADDR16_HA /* adjusted high 16bits */
#define R_PPC64_ADDR14          R_PPC_ADDR14 /* 16bit address, word aligned */
#define R_PPC64_ADDR14_BRTAKEN  R_PPC_ADDR14_BRTAKEN 
#define R_PPC64_ADDR14_BRNTAKEN R_PPC_ADDR14_BRNTAKEN 
#define R_PPC64_REL24           R_PPC_REL24 /* PC-rel. 26 bit, word aligned */
#define R_PPC64_REL14           R_PPC_REL14 /* PC relative 16 bit */
#define R_PPC64_REL14_BRTAKEN   R_PPC_REL14_BRTAKEN 
#define R_PPC64_REL14_BRNTAKEN  R_PPC_REL14_BRNTAKEN 
#define R_PPC64_GOT16           R_PPC_GOT16 
#define R_PPC64_GOT16_LO        R_PPC_GOT16_LO 
#define R_PPC64_GOT16_HI        R_PPC_GOT16_HI 
#define R_PPC64_GOT16_HA        R_PPC_GOT16_HA 
#define R_PPC64_COPY            R_PPC_COPY 
#define R_PPC64_GLOB_DAT        R_PPC_GLOB_DAT 
#define R_PPC64_JMP_SLOT        R_PPC_JMP_SLOT 
#define R_PPC64_RELATIVE        R_PPC_RELATIVE 
#define R_PPC64_UADDR32         R_PPC_UADDR32 
#define R_PPC64_UADDR16         R_PPC_UADDR16 
#define R_PPC64_REL32           R_PPC_REL32 
#define R_PPC64_PLT32           R_PPC_PLT32 
#define R_PPC64_PLTREL32        R_PPC_PLTREL32 
#define R_PPC64_PLT16_LO        R_PPC_PLT16_LO 
#define R_PPC64_PLT16_HI        R_PPC_PLT16_HI 
#define R_PPC64_PLT16_HA        R_PPC_PLT16_HA 
#define R_PPC64_SECTOFF         R_PPC_SECTOFF 
#define R_PPC64_SECTOFF_LO      R_PPC_SECTOFF_LO 
#define R_PPC64_SECTOFF_HI      R_PPC_SECTOFF_HI 
#define R_PPC64_SECTOFF_HA      R_PPC_SECTOFF_HA 
#define R_PPC64_ADDR30          37  /* word30 (S + A - P) >> 2 */
#define R_PPC64_ADDR64          38  /* doubleword64 S + A */
#define R_PPC64_ADDR16_HIGHER   39  /* half16 #higher(S + A) */
#define R_PPC64_ADDR16_HIGHERA  40  /* half16 #highera(S + A) */
#define R_PPC64_ADDR16_HIGHEST  41  /* half16 #highest(S + A) */
#define R_PPC64_ADDR16_HIGHESTA 42  /* half16 #highesta(S + A) */
#define R_PPC64_UADDR64         43  /* doubleword64 S + A */
#define R_PPC64_REL64           44  /* doubleword64 S + A - P */
#define R_PPC64_PLT64           45  /* doubleword64 L + A */
#define R_PPC64_PLTREL64        46  /* doubleword64 L + A - P */
#define R_PPC64_TOC16           47  /* half16* S + A - .TOC */
#define R_PPC64_TOC16_LO        48  /* half16 #lo(S + A - .TOC.) */
#define R_PPC64_TOC16_HI        49  /* half16 #hi(S + A - .TOC.) */
#define R_PPC64_TOC16_HA        50  /* half16 #ha(S + A - .TOC.) */
#define R_PPC64_TOC             51  /* doubleword64 .TOC */
#define R_PPC64_PLTGOT16        52  /* half16* M + A */
#define R_PPC64_PLTGOT16_LO     53  /* half16 #lo(M + A) */
#define R_PPC64_PLTGOT16_HI     54  /* half16 #hi(M + A) */
#define R_PPC64_PLTGOT16_HA     55  /* half16 #ha(M + A) */
#define R_PPC64_ADDR16_DS       56  /* half16ds* (S + A) >> 2 */
#define R_PPC64_ADDR16_LO_DS    57  /* half16ds  #lo(S + A) >> 2 */
#define R_PPC64_GOT16_DS        58  /* half16ds* (G + A) >> 2 */
#define R_PPC64_GOT16_LO_DS     59  /* half16ds  #lo(G + A) >> 2 */
#define R_PPC64_PLT16_LO_DS     60  /* half16ds  #lo(L + A) >> 2 */
#define R_PPC64_SECTOFF_DS      61  /* half16ds* (R + A) >> 2 */
#define R_PPC64_SECTOFF_LO_DS   62  /* half16ds  #lo(R + A) >> 2 */
#define R_PPC64_TOC16_DS        63  /* half16ds* (S + A - .TOC.) >> 2 */
#define R_PPC64_TOC16_LO_DS     64  /* half16ds  #lo(S + A - .TOC.) >> 2 */
#define R_PPC64_PLTGOT16_DS     65  /* half16ds* (M + A) >> 2 */
#define R_PPC64_PLTGOT16_LO_DS  66  /* half16ds  #lo(M + A) >> 2 */

/* PowerPC64 relocations defined for the TLS access ABI */
#define R_PPC64_TLS                67  /* none (sym+add)@tls */
#define R_PPC64_DTPMOD64           68  /* doubleword64 (sym+add)@dtpmod */
#define R_PPC64_TPREL16            69  /* half16* (sym+add)@tprel */
#define R_PPC64_TPREL16_LO         70  /* half16 (sym+add)@tprel@l */
#define R_PPC64_TPREL16_HI         71  /* half16 (sym+add)@tprel@h */
#define R_PPC64_TPREL16_HA         72  /* half16.(sym+add)@tprel@ha */
#define R_PPC64_TPREL64            73  /* doubleword64 (sym+add)@tprel */
#define R_PPC64_DTPREL16           74  /* half16* (sym+add)@dtprel */
#define R_PPC64_DTPREL16_LO        75  /* half16 (sym+add)@dtprel@l */
#define R_PPC64_DTPREL16_HI        76  /* half16 (sym+add)@dtprel@h */
#define R_PPC64_DTPREL16_HA        77  /* half16 (sym+add)@dtprel@ha */
#define R_PPC64_DTPREL64           78  /* doubleword64 (sym+add)@dtprel */
#define R_PPC64_GOT_TLSGD16        79  /* half16* (sym+add)@got@tlsgd */
#define R_PPC64_GOT_TLSGD16_LO     80  /* half16 (sym+add)@got@tlsgd@l */
#define R_PPC64_GOT_TLSGD16_HI     81  /* half16 (sym+add)@got@tlsgd@h */
#define R_PPC64_GOT_TLSGD16_HA     82  /* half16 (sym+add)@got@tlsgd@ha */
#define R_PPC64_GOT_TLSLD16        83  /* half16* (sym+add)@got@tlsld */
#define R_PPC64_GOT_TLSLD16_LO     84  /* half16 (sym+add)@got@tlsld@l */
#define R_PPC64_GOT_TLSLD16_HI     85  /* half16 (sym+add)@got@tlsld@h */
#define R_PPC64_GOT_TLSLD16_HA     86  /* half16 (sym+add)@got@tlsld@ha */
#define R_PPC64_GOT_TPREL16_DS     87  /* half16ds* (sym+add)@got@tprel */
#define R_PPC64_GOT_TPREL16_LO_DS  88  /* half16ds (sym+add)@got@tprel@l */
#define R_PPC64_GOT_TPREL16_HI     89  /* half16 (sym+add)@got@tprel@h */
#define R_PPC64_GOT_TPREL16_HA     90  /* half16 (sym+add)@got@tprel@ha */
#define R_PPC64_GOT_DTPREL16_DS    91  /* half16ds* (sym+add)@got@dtprel */
#define R_PPC64_GOT_DTPREL16_LO_DS 92  /* half16ds (sym+add)@got@dtprel@l */
#define R_PPC64_GOT_DTPREL16_HI    93  /* half16 (sym+add)@got@dtprel@h */
#define R_PPC64_GOT_DTPREL16_HA    94  /* half16 (sym+add)@got@dtprel@ha */
#define R_PPC64_TPREL16_DS         95  /* half16ds* (sym+add)@tprel */
#define R_PPC64_TPREL16_LO_DS      96  /* half16ds (sym+add)@tprel@l */
#define R_PPC64_TPREL16_HIGHER     97  /* half16 (sym+add)@tprel@higher */
#define R_PPC64_TPREL16_HIGHERA    98  /* half16 (sym+add)@tprel@highera */
#define R_PPC64_TPREL16_HIGHEST    99  /* half16 (sym+add)@tprel@highest */
#define R_PPC64_TPREL16_HIGHESTA   100 /* half16 (sym+add)@tprel@highesta */
#define R_PPC64_DTPREL16_DS        101 /* half16ds* (sym+add)@dtprel */
#define R_PPC64_DTPREL16_LO_DS     102 /* half16ds (sym+add)@dtprel@l */
#define R_PPC64_DTPREL16_HIGHER    103 /* half16 (sym+add)@dtprel@higher */
#define R_PPC64_DTPREL16_HIGHERA   104 /* half16 (sym+add)@dtprel@highera */
#define R_PPC64_DTPREL16_HIGHEST   105 /* half16 (sym+add)@dtprel@highest */
#define R_PPC64_DTPREL16_HIGHESTA  106 /* half16 (sym+add)@dtprel@highesta */
#define R_PPC64_TLSGD              107 /* none (sym+add)@tlsgd */
#define R_PPC64_TLSLD              108 /* none (sym+add)@tlsld */
#define R_PPC64_TOCSAVE            109 /* none */

/* Added when HA and HI relocs were changed to report overflow */
#define R_PPC64_ADDR16_HIGH    110 
#define R_PPC64_ADDR16_HIGHA   111 
#define R_PPC64_TPREL16_HIGH   112 
#define R_PPC64_TPREL16_HIGHA  113 
#define R_PPC64_DTPREL16_HIGH  114 
#define R_PPC64_DTPREL16_HIGHA 115 

/* GNU extension to support local ifunc */
#define R_PPC64_JMP_IREL  247 
#define R_PPC64_IRELATIVE 248 
#define R_PPC64_REL16     249 /* half16 (sym+add-.) */
#define R_PPC64_REL16_LO  250 /* half16 (sym+add-.)@l */
#define R_PPC64_REL16_HI  251 /* half16 (sym+add-.)@h */
#define R_PPC64_REL16_HA  252 /* half16 (sym+add-.)@ha */

/*
 * e_flags bits specifying ABI.
 * 1 for original function descriptor using ABI,
 * 2 for revised ABI without function descriptors,
 * 0 for unspecified or not using any features affected by the differences.
 */
#define EF_PPC64_ABI	3

/* PowerPC64 specific values for the Dyn d_tag field */
#define DT_PPC64_GLINK (DT_LOPROC + 0) 
#define DT_PPC64_OPD   (DT_LOPROC + 1) 
#define DT_PPC64_OPDSZ (DT_LOPROC + 2) 
#define DT_PPC64_OPT   (DT_LOPROC + 3) 
#define DT_PPC64_NUM   4   

/* PowerPC64 specific bits in the DT_PPC64_OPT Dyn entry */
#define PPC64_OPT_TLS        1   
#define PPC64_OPT_MULTI_TOC  2   
#define PPC64_OPT_LOCALENTRY 4   

/* PowerPC64 specific values for the elf64_sym st_other field */
#define STO_PPC64_LOCAL_BIT 5
#define STO_PPC64_LOCAL_MASK (7 << STO_PPC64_LOCAL_BIT)
#define PPC64_LOCAL_ENTRY_OFFSET(other) \
	(((1 << (((other) & STO_PPC64_LOCAL_MASK) \
	>> STO_PPC64_LOCAL_BIT)) >> 2) << 2)

/* ARM specific declarations */

/* Processor specific flags for the ELF header e_flags field */
#define EF_ARM_RELEXEC        0x01 
#define EF_ARM_HASENTRY       0x02 
#define EF_ARM_INTERWORK      0x04 
#define EF_ARM_APCS_26        0x08 
#define EF_ARM_APCS_FLOAT     0x10 
#define EF_ARM_PIC            0x20 
#define EF_ARM_ALIGN8         0x40  /* 8-bit structure alignment is in use */
#define EF_ARM_NEW_ABI        0x80 
#define EF_ARM_OLD_ABI        0x100 
#define EF_ARM_SOFT_FLOAT     0x200 
#define EF_ARM_VFP_FLOAT      0x400 
#define EF_ARM_MAVERICK_FLOAT 0x800 
#define EF_ARM_ABI_FLOAT_SOFT 0x200 /* NB conflicts with EF_ARM_SOFT_FLOAT */
#define EF_ARM_ABI_FLOAT_HARD 0x400 /* NB conflicts with EF_ARM_VFP_FLOAT */

/* other constants defined in the ARM ELF spec. version B-01 */
/* NB. These conflict with values defined above */
#define EF_ARM_SYMSARESORTED    0x04 
#define EF_ARM_DYNSYMSUSESEGIDX 0x08 
#define EF_ARM_MAPSYMSFIRST     0x10 
#define EF_ARM_EABIMASK         0XFF000000 

/* constants defined in AAELF */
#define EF_ARM_BE8 0x00800000 
#define EF_ARM_LE8 0x00400000 

#define EF_ARM_EABI_VERSION(flags) ((flags) & EF_ARM_EABIMASK)
#define EF_ARM_EABI_UNKNOWN        0x00000000 
#define EF_ARM_EABI_VER1           0x01000000 
#define EF_ARM_EABI_VER2           0x02000000 
#define EF_ARM_EABI_VER3           0x03000000 
#define EF_ARM_EABI_VER4           0x04000000 
#define EF_ARM_EABI_VER5           0x05000000 

/* additional symbol types for thumb */
#define STT_ARM_TFUNC STT_LOPROC /* thumb function */
#define STT_ARM_16BIT STT_HIPROC /* thumb label */

/* ARM-specific values for sh_flags */
#define SHF_ARM_ENTRYSECT 0x10000000 /* section contains an entry point */
#define SHF_ARM_COMDEF    0x80000000 /* section may be multiply defined
					in the input to a link step */

/* ARM-specific program header flags */
#define PF_ARM_SB  0x10000000 /* segment contains the location addressed by the static base. */
#define PF_ARM_PI  0x20000000 /* position-independent segment.  */
#define PF_ARM_ABS 0x40000000 /* absolute segment.  */

/* Processor specific values for the Phdr p_type field.  */
#define PT_ARM_EXIDX		(PT_LOPROC + 1)	/* ARM unwind segment.  */

/* Processor specific values for the Shdr sh_type field.  */
#define SHT_ARM_EXIDX		(SHT_LOPROC + 1) /* ARM unwind section.  */
#define SHT_ARM_PREEMPTMAP	(SHT_LOPROC + 2) /* Preemption details.  */
#define SHT_ARM_ATTRIBUTES	(SHT_LOPROC + 3) /* ARM attributes section.  */

/* AArch64 relocation for ELF*_R_TYPE */
#define R_AARCH64_NONE                         0    /* no relocation */
#define R_AARCH64_ABS64                        257  /* direct 64-bit*/
#define R_AARCH64_ABS32                        258  /* direct 32-bit */
#define R_AARCH64_ABS16                        259  /* direct 16-bit */
#define R_AARCH64_PREL64                       260  /* PC-relative 64-bit */
#define R_AARCH64_PREL32                       261  /* PC-relative 32-bit */
#define R_AARCH64_PREL16                       262  /* PC-relative 16-bit */
#define R_AARCH64_MOVW_UABS_G0                 263  /* dir. MOVZ imm. from bits 15:0 */
#define R_AARCH64_MOVW_UABS_G0_NC              264  /* likewise for MOVK; no check */
#define R_AARCH64_MOVW_UABS_G1                 265  /* dir. MOVZ imm. from bits 31:16 */
#define R_AARCH64_MOVW_UABS_G1_NC              266  /* likewise for MOVK; no check */
#define R_AARCH64_MOVW_UABS_G2                 267  /* dir. MOVZ imm. from bits 47:32 */
#define R_AARCH64_MOVW_UABS_G2_NC              268  /* likewise for MOVK; no check */
#define R_AARCH64_MOVW_UABS_G3                 269  /* dir. MOV{K,Z} imm. from 63:48 */
#define R_AARCH64_MOVW_SABS_G0                 270  /* dir. MOV{N,Z} imm. from 15:0 */
#define R_AARCH64_MOVW_SABS_G1                 271  /* dir. MOV{N,Z} imm. from 31:16 */
#define R_AARCH64_MOVW_SABS_G2                 272  /* dir. MOV{N,Z} imm. from 47:32 */
#define R_AARCH64_LD_PREL_LO19                 273  /* PC-rel. LD imm. from bits 20:2 */
#define R_AARCH64_ADR_PREL_LO21                274  /* PC-rel. ADR imm. from bits 20:0 */
#define R_AARCH64_ADR_PREL_PG_HI21             275  /* page-rel. ADRP imm. from 32:12 */
#define R_AARCH64_ADR_PREL_PG_HI21_NC          276  /* likewise; no overflow check */
#define R_AARCH64_ADD_ABS_LO12_NC              277  /* dir. ADD imm. from bits 11:0 */
#define R_AARCH64_LDST8_ABS_LO12_NC            278  /* likewise for LD/ST; no check */
#define R_AARCH64_TSTBR14                      279  /* PC-rel. TBZ/TBNZ imm. from 15:2 */
#define R_AARCH64_CONDBR19                     280  /* PC-rel. cond. br. imm. from 20:2 */
#define R_AARCH64_JUMP26                       282  /* PC-rel. B imm. from bits 27:2 */
#define R_AARCH64_CALL26                       283  /* likewise for CALL */
#define R_AARCH64_LDST16_ABS_LO12_NC           284  /* dir. ADD imm. from bits 11:1 */
#define R_AARCH64_LDST32_ABS_LO12_NC           285  /* likewise for bits 11:2 */
#define R_AARCH64_LDST64_ABS_LO12_NC           286  /* likewise for bits 11:3 */
#define R_AARCH64_MOVW_PREL_G0                 287  /* PC-rel. MOV{N,Z} imm. from 15:0 */
#define R_AARCH64_MOVW_PREL_G0_NC              288  /* likewise for MOVK; no check */
#define R_AARCH64_MOVW_PREL_G1                 289  /* PC-rel. MOV{N,Z} imm. from 31:16 */
#define R_AARCH64_MOVW_PREL_G1_NC              290  /* likewise for MOVK; no check */
#define R_AARCH64_MOVW_PREL_G2                 291  /* PC-rel. MOV{N,Z} imm. from 47:32 */
#define R_AARCH64_MOVW_PREL_G2_NC              292  /* likewise for MOVK; no check */
#define R_AARCH64_MOVW_PREL_G3                 293  /* PC-rel. MOV{N,Z} imm. from 63:48 */
#define R_AARCH64_LDST128_ABS_LO12_NC          299  /* Dir. ADD imm. from bits 11:4 */
#define R_AARCH64_MOVW_GOTOFF_G0               300  /* GOT-rel. off. MOV{N,Z} imm. 15:0 */
#define R_AARCH64_MOVW_GOTOFF_G0_NC            301  /* likewise for MOVK; no check */
#define R_AARCH64_MOVW_GOTOFF_G1               302  /* GOT-rel. o. MOV{N,Z} imm. 31:16 */
#define R_AARCH64_MOVW_GOTOFF_G1_NC            303  /* likewise for MOVK; no check */
#define R_AARCH64_MOVW_GOTOFF_G2               304  /* GOT-rel. o. MOV{N,Z} imm. 47:32 */
#define R_AARCH64_MOVW_GOTOFF_G2_NC            305  /* likewise for MOVK; no check */
#define R_AARCH64_MOVW_GOTOFF_G3               306  /* GOT-rel. o. MOV{N,Z} imm. 63:48 */
#define R_AARCH64_GOTREL64                     307  /* GOT-relative 64-bit */
#define R_AARCH64_GOTREL32                     308  /* GOT-relative 32-bit */
#define R_AARCH64_GOT_LD_PREL19                309  /* PC-rel. GOT off. load imm. 20:2 */
#define R_AARCH64_LD64_GOTOFF_LO15             310  /* GOT-rel. off. LD/ST imm. 14:3 */
#define R_AARCH64_ADR_GOT_PAGE                 311  /* P-page-rel. GOT off. ADRP 32:12 */
#define R_AARCH64_LD64_GOT_LO12_NC             312  /* dir. GOT off. LD/ST imm. 11:3 */
#define R_AARCH64_LD64_GOTPAGE_LO15            313  /* GOT-page-rel. GOT off. LD/ST 14:3 */
#define R_AARCH64_TLSGD_ADR_PREL21             512  /* PC-relative ADR imm. 20:0 */
#define R_AARCH64_TLSGD_ADR_PAGE21             513  /* page-rel. ADRP imm. 32:12 */
#define R_AARCH64_TLSGD_ADD_LO12_NC            514  /* direct ADD imm. from 11:0 */
#define R_AARCH64_TLSGD_MOVW_G1                515  /* GOT-rel. MOV{N,Z} 31:16 */
#define R_AARCH64_TLSGD_MOVW_G0_NC             516  /* GOT-rel. MOVK imm. 15:0 */
#define R_AARCH64_TLSLD_ADR_PREL21             517  /* like 512; local dynamic model */
#define R_AARCH64_TLSLD_ADR_PAGE21             518  /* like 513; local dynamic model */
#define R_AARCH64_TLSLD_ADD_LO12_NC            519  /* like 514; local dynamic model */
#define R_AARCH64_TLSLD_MOVW_G1                520  /* like 515; local dynamic model */
#define R_AARCH64_TLSLD_MOVW_G0_NC             521  /* like 516; local dynamic model */
#define R_AARCH64_TLSLD_LD_PREL19              522  /* TLS PC-rel. load imm. 20:2 */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G2         523  /* TLS DTP-rel. MOV{N,Z} 47:32 */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G1         524  /* TLS DTP-rel. MOV{N,Z} 31:16 */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC      525  /* likewise; MOVK; no check */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G0         526  /* TLS DTP-rel. MOV{N,Z} 15:0 */
#define R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC      527  /* likewise; MOVK; no check */
#define R_AARCH64_TLSLD_ADD_DTPREL_HI12        528  /* DTP-rel. ADD imm. from 23:12 */
#define R_AARCH64_TLSLD_ADD_DTPREL_LO12        529  /* DTP-rel. ADD imm. from 11:0 */
#define R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC     530  /* likewise; no ovfl. check */
#define R_AARCH64_TLSLD_LDST8_DTPREL_LO12      531  /* DTP-rel. LD/ST imm. 11:0 */
#define R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC   532  /* likewise; no check */
#define R_AARCH64_TLSLD_LDST16_DTPREL_LO12     533  /* DTP-rel. LD/ST imm. 11:1 */
#define R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC  534  /* likewise; no check */
#define R_AARCH64_TLSLD_LDST32_DTPREL_LO12     535  /* DTP-rel. LD/ST imm. 11:2 */
#define R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC  536  /* likewise; no check */
#define R_AARCH64_TLSLD_LDST64_DTPREL_LO12     537  /* DTP-rel. LD/ST imm. 11:3 */
#define R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC  538  /* likewise; no check */
#define R_AARCH64_TLSIE_MOVW_GOTTPREL_G1       539  /* GOT-rel. MOV{N,Z} 31:16 */
#define R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC    540  /* GOT-rel. MOVK 15:0 */
#define R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21    541  /* Page-rel. ADRP 32:12 */
#define R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC  542  /* Direct LD off. 11:3 */
#define R_AARCH64_TLSIE_LD_GOTTPREL_PREL19     543  /* PC-rel. load imm. 20:2 */
#define R_AARCH64_TLSLE_MOVW_TPREL_G2          544  /* TLS TP-rel. MOV{N,Z} 47:32 */
#define R_AARCH64_TLSLE_MOVW_TPREL_G1          545  /* TLS TP-rel. MOV{N,Z} 31:16 */
#define R_AARCH64_TLSLE_MOVW_TPREL_G1_NC       546  /* likewise; MOVK; no check */
#define R_AARCH64_TLSLE_MOVW_TPREL_G0          547  /* TLS TP-rel. MOV{N,Z} 15:0 */
#define R_AARCH64_TLSLE_MOVW_TPREL_G0_NC       548  /* likewise; MOVK; no check */
#define R_AARCH64_TLSLE_ADD_TPREL_HI12         549  /* TP-rel. ADD imm. 23:12 */
#define R_AARCH64_TLSLE_ADD_TPREL_LO12         550  /* TP-rel. ADD imm. 11:0 */
#define R_AARCH64_TLSLE_ADD_TPREL_LO12_NC      551  /* likewise; no ovfl. check */
#define R_AARCH64_TLSLE_LDST8_TPREL_LO12       552  /* TP-rel. LD/ST off. 11:0 */
#define R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC    553  /* likewise; no ovfl. check */
#define R_AARCH64_TLSLE_LDST16_TPREL_LO12      554  /* TP-rel. LD/ST off. 11:1 */
#define R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC   555  /* likewise; no check */
#define R_AARCH64_TLSLE_LDST32_TPREL_LO12      556  /* TP-rel. LD/ST off. 11:2 */
#define R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC   557  /* likewise; no check */
#define R_AARCH64_TLSLE_LDST64_TPREL_LO12      558  /* TP-rel. LD/ST off. 11:3 */
#define R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC   559  /* likewise; no check */
#define R_AARCH64_TLSDESC_LD_PREL19            560  /* PC-rel. load immediate 20:2 */
#define R_AARCH64_TLSDESC_ADR_PREL21           561  /* PC-rel. ADR immediate 20:0 */
#define R_AARCH64_TLSDESC_ADR_PAGE21           562  /* page-rel. ADRP imm. 32:12 */
#define R_AARCH64_TLSDESC_LD64_LO12            563  /* direct LD off. from 11:3 */
#define R_AARCH64_TLSDESC_ADD_LO12             564  /* direct ADD imm. from 11:0 */
#define R_AARCH64_TLSDESC_OFF_G1               565  /* GOT-rel. MOV{N,Z} imm. 31:16 */
#define R_AARCH64_TLSDESC_OFF_G0_NC            566  /* GOT-rel. MOVK imm. 15:0; no ck */
#define R_AARCH64_TLSDESC_LDR                  567  /* relax LDR */
#define R_AARCH64_TLSDESC_ADD                  568  /* relax ADD */
#define R_AARCH64_TLSDESC_CALL                 569  /* relax BLR */
#define R_AARCH64_TLSLE_LDST128_TPREL_LO12     570  /* TP-rel. LD/ST off. 11:4 */
#define R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC  571  /* likewise; no check */
#define R_AARCH64_TLSLD_LDST128_DTPREL_LO12    572  /* DTP-rel. LD/ST imm. 11:4 */
#define R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC 573  /* likewise; no check */
#define R_AARCH64_COPY                         1024 /* copy symbol at runtime */
#define R_AARCH64_GLOB_DAT                     1025 /* create GOT entry */
#define R_AARCH64_JUMP_SLOT                    1026 /* create PLT entry */
#define R_AARCH64_RELATIVE                     1027 /* adjust by program base */
#define R_AARCH64_TLS_DTPMOD                   1028 /* module number, 64-bit */
#define R_AARCH64_TLS_DTPREL                   1029 /* module-relative offset, 64-bit */
#define R_AARCH64_TLS_TPREL                    1030 /* TP-relative offset, 64-bit */
#define R_AARCH64_TLSDESC                      1031 /* TLS Descriptor */
#define R_AARCH64_IRELATIVE                    1032 /* STT_GNU_IFUNC relocation */

/* MTE memory tag segment type.  */
#define PT_AARCH64_MEMTAG_MTE	(PT_LOPROC + 2)

/* AArch64 specific values for the Dyn d_tag field.  */

/* elf*_dyn.d_tag */
#define DT_AARCH64_BTI_PLT	(DT_LOPROC + 1)
#define DT_AARCH64_PAC_PLT	(DT_LOPROC + 3)
#define DT_AARCH64_VARIANT_PCS	(DT_LOPROC + 5)
#define DT_AARCH64_NUM		6

/* elf*_sym.st_other */
#define STO_AARCH64_VARIANT_PCS 0x80

/* ARM specific definitions */

/* ARM relocation for ELF*_R_TYPE */
#define R_ARM_NONE              0   /* no relocation */
#define R_ARM_PC24              1   /* deprecated PC relative 26-bit branch */
#define R_ARM_ABS32             2   /* direct 32-bit  */
#define R_ARM_REL32             3   /* PC relative 32-bit */
#define R_ARM_PC13              4   
#define R_ARM_ABS16             5   /* direct 16-bit */
#define R_ARM_ABS12             6   /* direct 12-bit */
#define R_ARM_THM_ABS5          7   /* direct & 0x7C (LDR, STR) */
#define R_ARM_ABS8              8   /* direct 8-bit */
#define R_ARM_SBREL32           9   
#define R_ARM_THM_PC22          10  /* PC relative 24-bit (Thumb32 BL) */
#define R_ARM_THM_PC8           11  /* PC relative & 0x3FC (Thumb16 LDR, ADD, ADR) */
#define R_ARM_AMP_VCALL9        12  
#define R_ARM_SWI24             13  /* obsolete static relocation */
#define R_ARM_TLS_DESC          13  /* dynamic relocation */
#define R_ARM_THM_SWI8          14  /* reserved */
#define R_ARM_XPC25             15  /* reserved */
#define R_ARM_THM_XPC22         16  /* reserved */
#define R_ARM_TLS_DTPMOD32      17  /* ID of module containing symbol */
#define R_ARM_TLS_DTPOFF32      18  /* offset in TLS block */
#define R_ARM_TLS_TPOFF32       19  /* offset in static TLS block */
#define R_ARM_COPY              20  /* copy symbol at runtime */
#define R_ARM_GLOB_DAT          21  /* create GOT entry */
#define R_ARM_JUMP_SLOT         22  /* create PLT entry */
#define R_ARM_RELATIVE          23  /* adjust by program base */
#define R_ARM_GOTOFF            24  /* 32-bit offset to GOT */
#define R_ARM_GOTPC             25  /* 32-bit PC relative offset to GOT */
#define R_ARM_GOT32             26  /* 32-bit GOT entry */
#define R_ARM_PLT32             27  /* deprecated, 32-bit PLT address */
#define R_ARM_CALL              28  /* PC relative 24-bit (BL, BLX) */
#define R_ARM_JUMP24            29  /* PC relative 24-bit (B, BL<cond>) */
#define R_ARM_THM_JUMP24        30  /* PC relative 24-bit (Thumb32 B.W) */
#define R_ARM_BASE_ABS          31  /* adjust by program base */
#define R_ARM_ALU_PCREL_7_0     32  /* obsolete */
#define R_ARM_ALU_PCREL_15_8    33  /* obsolete */
#define R_ARM_ALU_PCREL_23_15   34  /* obsolete */
#define R_ARM_LDR_SBREL_11_0    35  /* deprecated, prog. base relative */
#define R_ARM_ALU_SBREL_19_12   36  /* deprecated, prog. base relative */
#define R_ARM_ALU_SBREL_27_20   37  /* deprecated, prog. base relative */
#define R_ARM_TARGET1           38  
#define R_ARM_SBREL31           39  /* program base relative */
#define R_ARM_V4BX              40  
#define R_ARM_TARGET2           41  
#define R_ARM_PREL31            42  /* 32-bit PC relative */
#define R_ARM_MOVW_ABS_NC       43  /* direct 16-bit (MOVW) */
#define R_ARM_MOVT_ABS          44  /* direct high 16-bit (MOVT) */
#define R_ARM_MOVW_PREL_NC      45  /* PC relative 16-bit (MOVW) */
#define R_ARM_MOVT_PREL         46  /* PC relative (MOVT) */
#define R_ARM_THM_MOVW_ABS_NC   47  /* direct 16-bit (Thumb32 MOVW) */
#define R_ARM_THM_MOVT_ABS      48  /* direct high 16-bit (Thumb32 MOVT) */
#define R_ARM_THM_MOVW_PREL_NC  49  /* PC relative 16-bit (Thumb32 MOVW) */
#define R_ARM_THM_MOVT_PREL     50  /* PC relative high 16-bit (Thumb32 MOVT) */
#define R_ARM_THM_JUMP19        51  /* PC relative 20-bit (Thumb32 B<cond>.W) */
#define R_ARM_THM_JUMP6         52  /* PC relative X & 0x7E (Thumb16 CBZ, CBNZ) */
#define R_ARM_THM_ALU_PREL_11_0 53  /* PC relative 12-bit (Thumb32 ADR.W) */
#define R_ARM_THM_PC12          54  /* PC relative 12-bit (Thumb32 LDR{D,SB,H,SH}) */
#define R_ARM_ABS32_NOI         55  /* direct 32-bit */
#define R_ARM_REL32_NOI         56  /* PC relative 32-bit */
#define R_ARM_ALU_PC_G0_NC      57  /* PC relative (ADD, SUB) */
#define R_ARM_ALU_PC_G0         58  /* PC relative (ADD, SUB) */
#define R_ARM_ALU_PC_G1_NC      59  /* PC relative (ADD, SUB) */
#define R_ARM_ALU_PC_G1         60  /* PC relative (ADD, SUB) */
#define R_ARM_ALU_PC_G2         61  /* PC relative (ADD, SUB) */
#define R_ARM_LDR_PC_G1         62  /* PC relative (LDR,STR,LDRB,STRB) */
#define R_ARM_LDR_PC_G2         63  /* PC relative (LDR,STR,LDRB,STRB) */
#define R_ARM_LDRS_PC_G0        64  /* PC relative (STR{D,H}, LDR{D,SB,H,SH}) */
#define R_ARM_LDRS_PC_G1        65  /* PC relative (STR{D,H}, LDR{D,SB,H,SH}) */
#define R_ARM_LDRS_PC_G2        66  /* PC relative (STR{D,H}, LDR{D,SB,H,SH}) */
#define R_ARM_LDC_PC_G0         67  /* PC relative (LDC, STC) */
#define R_ARM_LDC_PC_G1         68  /* PC relative (LDC, STC) */
#define R_ARM_LDC_PC_G2         69  /* PC relative (LDC, STC) */
#define R_ARM_ALU_SB_G0_NC      70  /* program base relative (ADD,SUB) */
#define R_ARM_ALU_SB_G0         71  /* program base relative (ADD,SUB) */
#define R_ARM_ALU_SB_G1_NC      72  /* program base relative (ADD,SUB) */
#define R_ARM_ALU_SB_G1         73  /* program base relative (ADD,SUB) */
#define R_ARM_ALU_SB_G2         74  /* program base relative (ADD,SUB) */
#define R_ARM_LDR_SB_G0         75  /* program base relative (LDR, STR, LDRB, STRB) */
#define R_ARM_LDR_SB_G1         76  /* program base relative (LDR, STR, LDRB, STRB) */
#define R_ARM_LDR_SB_G2         77  /* program base relative (LDR, STR, LDRB, STRB) */
#define R_ARM_LDRS_SB_G0        78  /* program base relative (LDR, STR, LDRB, STRB) */
#define R_ARM_LDRS_SB_G1        79  /* program base relative (LDR, STR, LDRB, STRB) */
#define R_ARM_LDRS_SB_G2        80  /* program base relative (LDR, STR, LDRB, STRB) */
#define R_ARM_LDC_SB_G0         81  /* program base relative (LDC,STC) */
#define R_ARM_LDC_SB_G1         82  /* program base relative (LDC,STC) */
#define R_ARM_LDC_SB_G2         83  /* program base relative (LDC,STC) */
#define R_ARM_MOVW_BREL_NC      84  /* program base relative 16-bit (MOVW) */
#define R_ARM_MOVT_BREL         85  /* program base relative high 16-bit (MOVT) */
#define R_ARM_MOVW_BREL         86  /* program base relative 16-bit (MOVW) */
#define R_ARM_THM_MOVW_BREL_NC  87  /* program base relative 16-bit (Thumb32 MOVW) */
#define R_ARM_THM_MOVT_BREL     88  /* program base relative high 16-bit (Thumb32 MOVT) */
#define R_ARM_THM_MOVW_BREL     89  /* program base relative 16-bit (Thumb32 MOVW) */
#define R_ARM_TLS_GOTDESC       90  
#define R_ARM_TLS_CALL          91  
#define R_ARM_TLS_DESCSEQ       92  /* TLS relaxation */
#define R_ARM_THM_TLS_CALL      93  
#define R_ARM_PLT32_ABS         94  
#define R_ARM_GOT_ABS           95  /* GOT entry */
#define R_ARM_GOT_PREL          96  /* PC relative GOT entry */
#define R_ARM_GOT_BREL12        97  /* GOT entry relative to GOT origin (LDR) */
#define R_ARM_GOTOFF12          98  /* 12-bit, GOT entry relative to GOT origin (LDR, STR) */
#define R_ARM_GOTRELAX          99  
#define R_ARM_GNU_VTENTRY       100 
#define R_ARM_GNU_VTINHERIT     101 
#define R_ARM_THM_PC11          102 /* PC relative & 0xFFE (Thumb16 B) */
#define R_ARM_THM_PC9           103 /* PC relative & 0x1FE (Thumb16 B/B<cond>) */
#define R_ARM_TLS_GD32          104 /* PC-rel 32-bit for global dynamic thread local data */
#define R_ARM_TLS_LDM32         105 /* PC-rel 32-bit for local dynamic thread local data */
#define R_ARM_TLS_LDO32         106 /* 32-bit offset relative to TLS block */
#define R_ARM_TLS_IE32          107 /* PC-rel 32-bit for GOT entry of static TLS block offset */
#define R_ARM_TLS_LE32          108 /* 32-bit offset relative to static TLS block */
#define R_ARM_TLS_LDO12         109 /* 12-bit relative to TLS block (LDR, STR) */
#define R_ARM_TLS_LE12          110 /* 12-bit relative to static TLS block (LDR, STR) */
#define R_ARM_TLS_IE12GP        111 /* 12-bit GOT entry relative to GOT origin (LDR) */
#define R_ARM_ME_TOO            128 /* obsolete */
#define R_ARM_THM_TLS_DESCSEQ   129 
#define R_ARM_THM_TLS_DESCSEQ16 129 
#define R_ARM_THM_TLS_DESCSEQ32 130 
#define R_ARM_THM_GOT_BREL12    131 /* GOT entry relative to GOT origin, 12-bit (Thumb32 LDR) */
#define R_ARM_IRELATIVE         160 
#define R_ARM_RXPC25            249 
#define R_ARM_RSBREL32          250 
#define R_ARM_THM_RPC22         251 
#define R_ARM_RREL32            252 
#define R_ARM_RABS22            253 
#define R_ARM_RPC24             254 
#define R_ARM_RBASE             255 
#define R_ARM_NUM               256 

/* C-SKY relocation for ELF*_R_TYPE */
#define R_CKCORE_NONE               0   /* no relocation */
#define R_CKCORE_ADDR32             1   /* direct 32-bit (S + A) */
#define R_CKCORE_PCRELIMM8BY4       2   /* disp ((S + A - P) >> 2) & 0xff */
#define R_CKCORE_PCRELIMM11BY2      3   /* disp ((S + A - P) >> 1) & 0x7ff */
#define R_CKCORE_PCREL32            5   /* 32-bit rel (S + A - P) */
#define R_CKCORE_PCRELJSR_IMM11BY2  6   /* disp ((S + A - P) >>1) & 0x7ff */
#define R_CKCORE_RELATIVE           9   /* 32-bit adjust program base(B + A) */
#define R_CKCORE_COPY               10  /* 32-bit adjust by program base */
#define R_CKCORE_GLOB_DAT           11  /* off between got and sym (S) */
#define R_CKCORE_JUMP_SLOT          12  /* PLT entry (S) */
#define R_CKCORE_GOTOFF             13  /* offset to GOT (S + A - GOT) */
#define R_CKCORE_GOTPC              14  /* PC offset to GOT (GOT + A - P) */
#define R_CKCORE_GOT32              15  /* 32-bit GOT entry (G) */
#define R_CKCORE_PLT32              16  /* 32-bit PLT entry (G) */
#define R_CKCORE_ADDRGOT            17  /* GOT entry in GLOB_DAT (GOT + G) */
#define R_CKCORE_ADDRPLT            18  /* PLT entry in GLOB_DAT (GOT + G) */
#define R_CKCORE_PCREL_IMM26BY2     19  /* ((S + A - P) >> 1) & 0x3ffffff */
#define R_CKCORE_PCREL_IMM16BY2     20  /* disp ((S + A - P) >> 1) & 0xffff */
#define R_CKCORE_PCREL_IMM16BY4     21  /* disp ((S + A - P) >> 2) & 0xffff */
#define R_CKCORE_PCREL_IMM10BY2     22  /* disp ((S + A - P) >> 1) & 0x3ff */
#define R_CKCORE_PCREL_IMM10BY4     23  /* disp ((S + A - P) >> 2) & 0x3ff */
#define R_CKCORE_ADDR_HI16          24  /* high & low 16-bit ADDR
					   ((S + A) >> 16) & 0xffff */
#define R_CKCORE_ADDR_LO16          25  /* (S + A) & 0xffff */
#define R_CKCORE_GOTPC_HI16         26  /* high & low 16-bit GOTPC
					   ((GOT + A - P) >> 16) & 0xffff */
#define R_CKCORE_GOTPC_LO16         27  /* (GOT + A - P) & 0xffff */
#define R_CKCORE_GOTOFF_HI16        28  /* high & low 16-bit GOTOFF
					   ((S + A - GOT) >> 16) & 0xffff */
#define R_CKCORE_GOTOFF_LO16        29  /* (S + A - GOT) & 0xffff */
#define R_CKCORE_GOT12              30  /* 12-bit disp GOT entry (G) */
#define R_CKCORE_GOT_HI16           31  /* high & low 16-bit GOT
					   (G >> 16) & 0xffff */
#define R_CKCORE_GOT_LO16           32  /* (G & 0xffff) */
#define R_CKCORE_PLT12              33  /* 12-bit disp PLT entry (G) */
#define R_CKCORE_PLT_HI16           34  /* high & low 16-bit PLT
					   (G >> 16) & 0xffff */
#define R_CKCORE_PLT_LO16           35  /* G & 0xffff */
#define R_CKCORE_ADDRGOT_HI16       36  /* high & low 16-bit ADDRGOT
					   (GOT + G * 4) & 0xffff */
#define R_CKCORE_ADDRGOT_LO16       37  /* (GOT + G * 4) & 0xffff */
#define R_CKCORE_ADDRPLT_HI16       38  /* high & low 16-bit ADDRPLT
					   ((GOT + G * 4) >> 16) & 0xFFFF */
#define R_CKCORE_ADDRPLT_LO16       39  /* (GOT+G*4) & 0xffff */
#define R_CKCORE_PCREL_JSR_IMM26BY2 40  /* disp ((S+A-P) >>1) & x3ffffff */
#define R_CKCORE_TOFFSET_LO16       41  /* (S+A-BTEXT) & 0xffff */
#define R_CKCORE_DOFFSET_LO16       42  /* (S+A-BTEXT) & 0xffff */
#define R_CKCORE_PCREL_IMM18BY2     43  /* disp ((S+A-P) >>1) & 0x3ffff */
#define R_CKCORE_DOFFSET_IMM18      44  /* disp (S+A-BDATA) & 0x3ffff */
#define R_CKCORE_DOFFSET_IMM18BY2   45  /* disp ((S+A-BDATA)>>1) & 0x3ffff */
#define R_CKCORE_DOFFSET_IMM18BY4   46  /* disp ((S+A-BDATA)>>2) & 0x3ffff */
#define R_CKCORE_GOT_IMM18BY4       48  /* disp (G >> 2) */
#define R_CKCORE_PLT_IMM18BY4       49  /* disp (G >> 2) */
#define R_CKCORE_PCREL_IMM7BY4      50  /* disp ((S+A-P) >>2) & 0x7f */
#define R_CKCORE_TLS_LE32           51  /* 32-bit offset to TLS block */
#define R_CKCORE_TLS_IE32           52  
#define R_CKCORE_TLS_GD32           53  
#define R_CKCORE_TLS_LDM32          54  
#define R_CKCORE_TLS_LDO32          55  
#define R_CKCORE_TLS_DTPMOD32       56  
#define R_CKCORE_TLS_DTPOFF32       57  
#define R_CKCORE_TLS_TPOFF32        58  

/* C-SKY elf header definition */
#define EF_CSKY_ABIMASK   0XF0000000
#define EF_CSKY_OTHER     0X0FFF0000
#define EF_CSKY_PROCESSOR 0X0000FFFF

#define EF_CSKY_ABIV1 0X10000000
#define EF_CSKY_ABIV2  0X20000000

/* C-SKY attributes section */
#define SHT_CSKY_ATTRIBUTES (SHT_LOPROC + 1)

/* IA-64 specific declarations */

/* processor specific flags for the ehdr e_flags field */
#define EF_IA_64_MASKOS 0x0000000f /* os-specific flags */
#define EF_IA_64_ABI64  0x00000010 /* 64-bit ABI */
#define EF_IA_64_ARCH   0xff000000 /* arch. version mask */

/* processor specific values for the phdr p_type field */
#define PT_IA_64_ARCHEXT     (PT_LOPROC + 0) /* arch extension bits */
#define PT_IA_64_UNWIND      (PT_LOPROC + 1) /* ia64 unwind bits */
#define PT_IA_64_HP_OPT_ANOT (PT_LOOS + 0x12)
#define PT_IA_64_HP_HSL_ANOT (PT_LOOS + 0x13)
#define PT_IA_64_HP_STACK    (PT_LOOS + 0x14)

/* processor specific flags for the phdr p_flags field */
#define PF_IA_64_NORECOV 0x80000000 /* spec insns w/o recovery */

/* processor specific values for the shdr sh_type field */
#define SHT_IA_64_EXT (SHT_LOPROC + 0) /* extension bits */
#define SHT_IA_64_UNWIND (SHT_LOPROC + 1) /* unwind bits */

/* processor specific flags for the shdr sh_flags field */
#define SHF_IA_64_SHORT   0x10000000 /* section near gp */
#define SHF_IA_64_NORECOV 0x20000000 /* spec insns w/o recovery */

/* processor specific values for the dyn d_tag field */
#define DT_IA_64_PLT_RESERVE (DT_LOPROC + 0)
#define DT_IA_64_NUM 1

/* IA-64 relocation for ELF*_R_TYPE */
#define R_IA64_NONE            0x00 /* none */
#define R_IA64_IMM14           0x21 /* symbol + addend, add imm14 */
#define R_IA64_IMM22           0x22 /* symbol + addend, add imm22 */
#define R_IA64_IMM64           0x23 /* symbol + addend, mov imm64 */
#define R_IA64_DIR32MSB        0x24 /* symbol + addend, data4 MSB */
#define R_IA64_DIR32LSB        0x25 /* symbol + addend, data4 LSB */
#define R_IA64_DIR64MSB        0x26 /* symbol + addend, data8 MSB */
#define R_IA64_DIR64LSB        0x27 /* symbol + addend, data8 LSB */
#define R_IA64_GPREL22         0x2a /* @gprel(sym + add), add imm22 */
#define R_IA64_GPREL64I        0x2b /* @gprel(sym + add), mov imm64 */
#define R_IA64_GPREL32MSB      0x2c /* @gprel(sym + add), data4 MSB */
#define R_IA64_GPREL32LSB      0x2d /* @gprel(sym + add), data4 LSB */
#define R_IA64_GPREL64MSB      0x2e /* @gprel(sym + add), data8 MSB */
#define R_IA64_GPREL64LSB      0x2f /* @gprel(sym + add), data8 LSB */
#define R_IA64_LTOFF22         0x32 /* @ltoff(sym + add), add imm22 */
#define R_IA64_LTOFF64I        0x33 /* @ltoff(sym + add), mov imm64 */
#define R_IA64_PLTOFF22        0x3a /* @pltoff(sym + add), add imm22 */
#define R_IA64_PLTOFF64I       0x3b /* @pltoff(sym + add), mov imm64 */
#define R_IA64_PLTOFF64MSB     0x3e /* @pltoff(sym + add), data8 MSB */
#define R_IA64_PLTOFF64LSB     0x3f /* @pltoff(sym + add), data8 LSB */
#define R_IA64_FPTR64I         0x43 /* @fptr(sym + add), mov imm64 */
#define R_IA64_FPTR32MSB       0x44 /* @fptr(sym + add), data4 MSB */
#define R_IA64_FPTR32LSB       0x45 /* @fptr(sym + add), data4 LSB */
#define R_IA64_FPTR64MSB       0x46 /* @fptr(sym + add), data8 MSB */
#define R_IA64_FPTR64LSB       0x47 /* @fptr(sym + add), data8 LSB */
#define R_IA64_PCREL60B        0x48 /* @pcrel(sym + add), brl */
#define R_IA64_PCREL21B        0x49 /* @pcrel(sym + add), ptb, call */
#define R_IA64_PCREL21M        0x4a /* @pcrel(sym + add), chk.s */
#define R_IA64_PCREL21F        0x4b /* @pcrel(sym + add), fchkf */
#define R_IA64_PCREL32MSB      0x4c /* @pcrel(sym + add), data4 MSB */
#define R_IA64_PCREL32LSB      0x4d /* @pcrel(sym + add), data4 LSB */
#define R_IA64_PCREL64MSB      0x4e /* @pcrel(sym + add), data8 MSB */
#define R_IA64_PCREL64LSB      0x4f /* @pcrel(sym + add), data8 LSB */
#define R_IA64_LTOFF_FPTR22    0x52 /* @ltoff(@fptr(s+a)), imm22 */
#define R_IA64_LTOFF_FPTR64I   0x53 /* @ltoff(@fptr(s+a)), imm64 */
#define R_IA64_LTOFF_FPTR32MSB 0x54 /* @ltoff(@fptr(s+a)), data4 MSB */
#define R_IA64_LTOFF_FPTR32LSB 0x55 /* @ltoff(@fptr(s+a)), data4 LSB */
#define R_IA64_LTOFF_FPTR64MSB 0x56 /* @ltoff(@fptr(s+a)), data8 MSB */
#define R_IA64_LTOFF_FPTR64LSB 0x57 /* @ltoff(@fptr(s+a)), data8 LSB */
#define R_IA64_SEGREL32MSB     0x5c /* @segrel(sym + add), data4 MSB */
#define R_IA64_SEGREL32LSB     0x5d /* @segrel(sym + add), data4 LSB */
#define R_IA64_SEGREL64MSB     0x5e /* @segrel(sym + add), data8 MSB */
#define R_IA64_SEGREL64LSB     0x5f /* @segrel(sym + add), data8 LSB */
#define R_IA64_SECREL32MSB     0x64 /* @secrel(sym + add), data4 MSB */
#define R_IA64_SECREL32LSB     0x65 /* @secrel(sym + add), data4 LSB */
#define R_IA64_SECREL64MSB     0x66 /* @secrel(sym + add), data8 MSB */
#define R_IA64_SECREL64LSB     0x67 /* @secrel(sym + add), data8 LSB */
#define R_IA64_REL32MSB        0x6c /* data 4 + REL */
#define R_IA64_REL32LSB        0x6d /* data 4 + REL */
#define R_IA64_REL64MSB        0x6e /* data 8 + REL */
#define R_IA64_REL64LSB        0x6f /* data 8 + REL */
#define R_IA64_LTV32MSB        0x74 /* symbol + addend, data4 MSB */
#define R_IA64_LTV32LSB        0x75 /* symbol + addend, data4 LSB */
#define R_IA64_LTV64MSB        0x76 /* symbol + addend, data8 MSB */
#define R_IA64_LTV64LSB        0x77 /* symbol + addend, data8 LSB */
#define R_IA64_PCREL21BI       0x79 /* @pcrel(sym + add), 21bit inst */
#define R_IA64_PCREL22         0x7a /* @pcrel(sym + add), 22bit inst */
#define R_IA64_PCREL64I        0x7b /* @pcrel(sym + add), 64bit inst */
#define R_IA64_IPLTMSB         0x80 /* dynamic reloc, imported PLT, MSB */
#define R_IA64_IPLTLSB         0x81 /* dynamic reloc, imported PLT, LSB */
#define R_IA64_COPY            0x84 /* copy relocation */
#define R_IA64_SUB             0x85 /* addend and symbol difference */
#define R_IA64_LTOFF22X        0x86 /* LTOFF22, relaxable.  */
#define R_IA64_LDXMOV          0x87 /* use of LTOFF22X.  */
#define R_IA64_TPREL14         0x91 /* @tprel(sym + add), imm14 */
#define R_IA64_TPREL22         0x92 /* @tprel(sym + add), imm22 */
#define R_IA64_TPREL64I        0x93 /* @tprel(sym + add), imm64 */
#define R_IA64_TPREL64MSB      0x96 /* @tprel(sym + add), data8 MSB */
#define R_IA64_TPREL64LSB      0x97 /* @tprel(sym + add), data8 LSB */
#define R_IA64_LTOFF_TPREL22   0x9a /* @ltoff(@tprel(s+a)), imm2 */
#define R_IA64_DTPMOD64MSB     0xa6 /* @dtpmod(sym + add), data8 MSB */
#define R_IA64_DTPMOD64LSB     0xa7 /* @dtpmod(sym + add), data8 LSB */
#define R_IA64_LTOFF_DTPMOD22  0xaa /* @ltoff(@dtpmod(sym + add)), imm22 */
#define R_IA64_DTPREL14        0xb1 /* @dtprel(sym + add), imm14 */
#define R_IA64_DTPREL22        0xb2 /* @dtprel(sym + add), imm22 */
#define R_IA64_DTPREL64I       0xb3 /* @dtprel(sym + add), imm64 */
#define R_IA64_DTPREL32MSB     0xb4 /* @dtprel(sym + add), data4 MSB */
#define R_IA64_DTPREL32LSB     0xb5 /* @dtprel(sym + add), data4 LSB */
#define R_IA64_DTPREL64MSB     0xb6 /* @dtprel(sym + add), data8 MSB */
#define R_IA64_DTPREL64LSB     0xb7 /* @dtprel(sym + add), data8 LSB */
#define R_IA64_LTOFF_DTPREL22  0xba /* @ltoff(@dtprel(s+a)), imm22 */

/* SH specific declarations */

/* Processor specific flags for the ELF header e_flags field */
#define EF_SH_MACH_MASK    0x1f 
#define EF_SH_UNKNOWN      0x0 
#define EF_SH1             0x1 
#define EF_SH2             0x2 
#define EF_SH3             0x3 
#define EF_SH_DSP          0x4 
#define EF_SH3_DSP         0x5 
#define EF_SH4AL_DSP       0x6 
#define EF_SH3E            0x8 
#define EF_SH4             0x9 
#define EF_SH2E            0xb 
#define EF_SH4A            0xc 
#define EF_SH2A            0xd 
#define EF_SH4_NOFPU       0x10 
#define EF_SH4A_NOFPU      0x11 
#define EF_SH4_NOMMU_NOFPU 0x12 
#define EF_SH2A_NOFPU      0x13 
#define EF_SH3_NOMMU       0x14 
#define EF_SH2A_SH4_NOFPU  0x15 
#define EF_SH2A_SH3_NOFPU  0x16 
#define EF_SH2A_SH4        0x17 
#define EF_SH2A_SH3E       0x18 

/* SH relocation for ELF*_R_TYPE */
#define R_SH_NONE          0   
#define R_SH_DIR32         1   
#define R_SH_REL32         2   
#define R_SH_DIR8WPN       3   
#define R_SH_IND12W        4   
#define R_SH_DIR8WPL       5   
#define R_SH_DIR8WPZ       6   
#define R_SH_DIR8BP        7   
#define R_SH_DIR8W         8   
#define R_SH_DIR8L         9   
#define R_SH_SWITCH16      25  
#define R_SH_SWITCH32      26  
#define R_SH_USES          27  
#define R_SH_COUNT         28  
#define R_SH_ALIGN         29  
#define R_SH_CODE          30  
#define R_SH_DATA          31  
#define R_SH_LABEL         32  
#define R_SH_SWITCH8       33  
#define R_SH_GNU_VTINHERIT 34  
#define R_SH_GNU_VTENTRY   35  
#define R_SH_TLS_GD_32     144 
#define R_SH_TLS_LD_32     145 
#define R_SH_TLS_LDO_32    146 
#define R_SH_TLS_IE_32     147 
#define R_SH_TLS_LE_32     148 
#define R_SH_TLS_DTPMOD32  149 
#define R_SH_TLS_DTPOFF32  150 
#define R_SH_TLS_TPOFF32   151 
#define R_SH_GOT32         160 
#define R_SH_PLT32         161 
#define R_SH_COPY          162 
#define R_SH_GLOB_DAT      163 
#define R_SH_JMP_SLOT      164 
#define R_SH_RELATIVE      165 
#define R_SH_GOTOFF        166 
#define R_SH_GOTPC         167 
#define R_SH_NUM           256 

/* S/390 specific definitions */

/* valid values for the e_flags field */
#define EF_S390_HIGH_GPRS 0x00000001 /* high GPRs kernel facility needed */

/* s390 relocation for ELF*_R_TYPE */
#define R_390_NONE        0  /* no relocation */
#define R_390_8           1  /* direct 8-bit */
#define R_390_12          2  /* direct 12-bit */
#define R_390_16          3  /* direct 16-bit */
#define R_390_32          4  /* direct 32-bit */
#define R_390_PC32        5  /* PC relative 32-bit */
#define R_390_GOT12       6  /* 12-bit GOT offset */
#define R_390_GOT32       7  /* 32-bit GOT offset */
#define R_390_PLT32       8  /* 32-bit PC relative PLT address */
#define R_390_COPY        9  /* copy symbol at runtime */
#define R_390_GLOB_DAT    10 /* create GOT entry */
#define R_390_JMP_SLOT    11 /* create PLT entry */
#define R_390_RELATIVE    12 /* adjust by program base */
#define R_390_GOTOFF32    13 /* 32-bit offset to GOT */
#define R_390_GOTPC       14 /* 32-bit PC relative offset to GOT */
#define R_390_GOT16       15 /* 16-bit GOT offset */
#define R_390_PC16        16 /* PC relative 16-bit */
#define R_390_PC16DBL     17 /* PC relative 16-bit shifted by 1 */
#define R_390_PLT16DBL    18 /* 16-bit PC rel. PLT shifted by 1 */
#define R_390_PC32DBL     19 /* PC relative 32-bit shifted by 1 */
#define R_390_PLT32DBL    20 /* 32-bit PC rel. PLT shifted by 1 */
#define R_390_GOTPCDBL    21 /* 32-bit PC rel. GOT shifted by 1 */
#define R_390_64          22 /* direct 64-bit */
#define R_390_PC64        23 /* PC relative 64-bit */
#define R_390_GOT64       24 /* 64-bit GOT offset */
#define R_390_PLT64       25 /* 64-bit PC relative PLT address */
#define R_390_GOTENT      26 /* 32-bit PC rel. to GOT entry >> 1 */
#define R_390_GOTOFF16    27 /* 16-bit offset to GOT */
#define R_390_GOTOFF64    28 /* 64-bit offset to GOT */
#define R_390_GOTPLT12    29 /* 12-bit offset to jump slot */
#define R_390_GOTPLT16    30 /* 16-bit offset to jump slot */
#define R_390_GOTPLT32    31 /* 32-bit offset to jump slot */
#define R_390_GOTPLT64    32 /* 64-bit offset to jump slot */
#define R_390_GOTPLTENT   33 /* 32-bit rel. offset to jump slot */
#define R_390_PLTOFF16    34 /* 16-bit offset from GOT to PLT */
#define R_390_PLTOFF32    35 /* 32-bit offset from GOT to PLT */
#define R_390_PLTOFF64    36 /* 16-bit offset from GOT to PLT */
#define R_390_TLS_LOAD    37 /* tag for load insn in TLS code */
#define R_390_TLS_GDCALL  38 /* tag for function call in general
				dynamic TLS code */
#define R_390_TLS_LDCALL  39 /* tag for function call in local
				dynamic TLS code */
#define R_390_TLS_GD32    40 /* direct 32-bit for general dynamic
				thread local data */
#define R_390_TLS_GD64    41 /* direct 64-bit for general dynamic
				thread local data */
#define R_390_TLS_GOTIE12 42 /* 12-bit GOT offset for static TLS
				block offset */
#define R_390_TLS_GOTIE32 43 /* 32-bit GOT offset for static TLS
				block offset */
#define R_390_TLS_GOTIE64 44 /* 64-bit GOT offset for static TLS
				block offset */
#define R_390_TLS_LDM32   45 /* direct 32-bit for local dynamic
				thread local data in LE code */
#define R_390_TLS_LDM64   46 /* direct 64-bit for local dynamic
				thread local data in LE code */
#define R_390_TLS_IE32    47 /* 32-bit address of GOT entry for
				negated static TLS block offset */
#define R_390_TLS_IE64    48 /* 64-bit address of GOT entry for
				negated static TLS block offset */
#define R_390_TLS_IEENT   49 /* 32-bit rel. offset to GOT entry
				for negated static TLS block offset */
#define R_390_TLS_LE32    50 /* 32-bit negated offset relative to
				static TLS block */
#define R_390_TLS_LE64    51 /* 64-bit negated offset relative to
				static TLS block */
#define R_390_TLS_LDO32   52 /* 32-bit offset relative to TLS block */
#define R_390_TLS_LDO64   53 /* 64-bit offset relative to TLS block */
#define R_390_TLS_DTPMOD  54 /* ID of module containing symbol */
#define R_390_TLS_DTPOFF  55 /* offset in TLS block */
#define R_390_TLS_TPOFF   56 /* negated offset in static TLS block */
#define R_390_20          57 /* direct 20-bit */
#define R_390_GOT20       58 /* 20-bit GOT offset */
#define R_390_GOTPLT20    59 /* 20-bit offset to jump slot */
#define R_390_TLS_GOTIE20 60 /* 20-bit GOT offset for static TLS block
				offset */
#define R_390_IRELATIVE   61 /* STT_GNU_IFUNC relocation */
#define R_390_NUM         62  

/* CRIS relocation for ELF*_R_TYPE */
#define R_CRIS_NONE          0   
#define R_CRIS_8             1   
#define R_CRIS_16            2   
#define R_CRIS_32            3   
#define R_CRIS_8_PCREL       4   
#define R_CRIS_16_PCREL      5   
#define R_CRIS_32_PCREL      6   
#define R_CRIS_GNU_VTINHERIT 7   
#define R_CRIS_GNU_VTENTRY   8   
#define R_CRIS_COPY          9   
#define R_CRIS_GLOB_DAT      10  
#define R_CRIS_JUMP_SLOT     11  
#define R_CRIS_RELATIVE      12  
#define R_CRIS_16_GOT        13  
#define R_CRIS_32_GOT        14  
#define R_CRIS_16_GOTPLT     15  
#define R_CRIS_32_GOTPLT     16  
#define R_CRIS_32_GOTREL     17  
#define R_CRIS_32_PLT_GOTREL 18  
#define R_CRIS_32_PLT_PCREL  19  
#define R_CRIS_NUM           20  

/* AMD x86-64 relocation for ELF*_R_TYPE */
#define R_X86_64_NONE            0  /* no relocation */
#define R_X86_64_64              1  /* direct 64-bit  */
#define R_X86_64_PC32            2  /* PC relative 32-bit signed */
#define R_X86_64_GOT32           3  /* 32-bit GOT entry */
#define R_X86_64_PLT32           4  /* 32-bit PLT address */
#define R_X86_64_COPY            5  /* copy symbol at runtime */
#define R_X86_64_GLOB_DAT        6  /* create GOT entry */
#define R_X86_64_JUMP_SLOT       7  /* create PLT entry */
#define R_X86_64_RELATIVE        8  /* adjust by program base */
#define R_X86_64_GOTPCREL        9  /* 32-bit signed PC relative offset to GOT */
#define R_X86_64_32              10 /* direct 32-bit zero extended */
#define R_X86_64_32S             11 /* direct 32-bit sign extended */
#define R_X86_64_16              12 /* direct 16-bit zero extended */
#define R_X86_64_PC16            13 /* 16-bit sign extended pc relative */
#define R_X86_64_8               14 /* direct 8-bit sign extended  */
#define R_X86_64_PC8             15 /* 8-bit sign extended pc relative */
#define R_X86_64_DTPMOD64        16 /* ID of module containing symbol */
#define R_X86_64_DTPOFF64        17 /* offset in module's TLS block */
#define R_X86_64_TPOFF64         18 /* offset in initial TLS block */
#define R_X86_64_TLSGD           19 /* 32-bit signed PC relative offset to
				       two GOT entries for GD symbol */
#define R_X86_64_TLSLD           20 /* 32-bit signed PC relative offset to
				       two GOT entries for LD symbol */
#define R_X86_64_DTPOFF32        21 /* offset in TLS block */
#define R_X86_64_GOTTPOFF        22 /* 32-bit signed PC relative offset to
				       GOT entry for IE symbol */
#define R_X86_64_TPOFF32         23 /* offset in initial TLS block */
#define R_X86_64_PC64            24 /* PC relative 64-bit */
#define R_X86_64_GOTOFF64        25 /* 64-bit offset to GOT */
#define R_X86_64_GOTPC32         26 /* 32-bit signed pc relative offset to GOT */
#define R_X86_64_GOT64           27 /* 64-bit GOT entry offset */
#define R_X86_64_GOTPCREL64      28 /* 64-bit PC relative offset to GOT entry */
#define R_X86_64_GOTPC64         29 /* 64-bit PC relative offset to GOT */
#define R_X86_64_GOTPLT64        30 /* like GOT64, says PLT entry needed */
#define R_X86_64_PLTOFF64        31 /* 64-bit GOT relative offset to PLT entry */
#define R_X86_64_SIZE32          32 /* size of symbol plus 32-bit addend */
#define R_X86_64_SIZE64          33 /* size of symbol plus 64-bit addend */
#define R_X86_64_GOTPC32_TLSDESC 34 /* GOT offset for TLS descriptor */
#define R_X86_64_TLSDESC_CALL    35 /* marker for call through TLS descriptor */
#define R_X86_64_TLSDESC         36 /* TLS descriptor */
#define R_X86_64_IRELATIVE       37 /* adjust indirectly by program base */
#define R_X86_64_RELATIVE64      38 /* 64-bit adjust by program base */
/* 39 reserved was R_X86_64_PC32_BND */
/* 40 reserved was R_X86_64_PLT32_BND */
#define R_X86_64_GOTPCRELX       41 /* load from 32-bit signed pc relative offset
				       to GOT entry without REX prefix, relaxable */
#define R_X86_64_REX_GOTPCRELX   42 /* load from 32-bit signed pc relative offset
				       to GOT entry with REX prefix, relaxable */
#define R_X86_64_NUM             43  

/* x86-64 sh_type values */
#define SHT_X86_64_UNWIND 0x70000001 /* unwind information */

/* x86-64 d_tag values */
#define DT_X86_64_PLT    (DT_LOPROC + 0)
#define DT_X86_64_PLTSZ  (DT_LOPROC + 1)
#define DT_X86_64_PLTENT (DT_LOPROC + 3)
#define DT_X86_64_NUM    4

/* AM33 relocation for ELF*_R_TYPE */
#define R_MN10300_NONE          0  /* no relocation */
#define R_MN10300_32            1  /* direct 32-bit */
#define R_MN10300_16            2  /* direct 16-bit */
#define R_MN10300_8             3  /* direct 8-bit */
#define R_MN10300_PCREL32       4  /* PC-relative 32-bit */
#define R_MN10300_PCREL16       5  /* PC-relative 16-bit signed */
#define R_MN10300_PCREL8        6  /* PC-relative 8-bit signed */
#define R_MN10300_GNU_VTINHERIT 7  /* ancient C++ vtable garbage.. */
#define R_MN10300_GNU_VTENTRY   8  /* ... collection annotation */
#define R_MN10300_24            9  /* direct 24-bit */
#define R_MN10300_GOTPC32       10 /* 32-bit PCrel offset to GOT */
#define R_MN10300_GOTPC16       11 /* 16-bit PCrel offset to GOT */
#define R_MN10300_GOTOFF32      12 /* 32-bit offset from GOT */
#define R_MN10300_GOTOFF24      13 /* 24-bit offset from GOT */
#define R_MN10300_GOTOFF16      14 /* 16-bit offset from GOT */
#define R_MN10300_PLT32         15 /* 32-bit PCrel to PLT entry */
#define R_MN10300_PLT16         16 /* 16-bit PCrel to PLT entry */
#define R_MN10300_GOT32         17 /* 32-bit offset to GOT entry */
#define R_MN10300_GOT24         18 /* 24-bit offset to GOT entry */
#define R_MN10300_GOT16         19 /* 16-bit offset to GOT entry */
#define R_MN10300_COPY          20 /* copy symbol at runtime */
#define R_MN10300_GLOB_DAT      21 /* create GOT entry */
#define R_MN10300_JMP_SLOT      22 /* create PLT entry */
#define R_MN10300_RELATIVE      23 /* adjust by program base */
#define R_MN10300_TLS_GD        24 /* 32-bit offset for global dynamic */
#define R_MN10300_TLS_LD        25 /* 32-bit offset for local dynamic */
#define R_MN10300_TLS_LDO       26 /* module-relative offset */
#define R_MN10300_TLS_GOTIE     27 /* GOT offset for static TLS block offset */
#define R_MN10300_TLS_IE        28 /* GOT address for static TLS block offset */
#define R_MN10300_TLS_LE        29 /* offset relative to static TLS block */
#define R_MN10300_TLS_DTPMOD    30 /* ID of module containing symbol */
#define R_MN10300_TLS_DTPOFF    31 /* offset in module TLS block */
#define R_MN10300_TLS_TPOFF     32 /* offset in static TLS block */
#define R_MN10300_SYM_DIFF      33 /* adjustment for next reloc as needed by
				      linker relaxation */
#define R_MN10300_ALIGN         34 /* alignment requirement for linker relaxation */
#define R_MN10300_NUM           35  

/* M32R relocation for ELF*_R_TYPE */
#define R_M32R_NONE               0   /* no relocation */
#define R_M32R_16                 1   /* direct 16-bit */
#define R_M32R_32                 2   /* direct 32-bit */
#define R_M32R_24                 3   /* direct 24-bit */
#define R_M32R_10_PCREL           4   /* PC relative 10-bit shifted */
#define R_M32R_18_PCREL           5   /* PC relative 18-bit shifted */
#define R_M32R_26_PCREL           6   /* PC relative 26-bit shifted */
#define R_M32R_HI16_ULO           7   /* high 16-bit with unsigned low */
#define R_M32R_HI16_SLO           8   /* high 16-bit with signed low */
#define R_M32R_LO16               9   /* low 16-bit */
#define R_M32R_SDA16              10  /* 16-bit offset in SDA */
#define R_M32R_GNU_VTINHERIT      11  
#define R_M32R_GNU_VTENTRY        12  
/* M32R relocation use SHT_RELA */
#define R_M32R_16_RELA            33  /* direct 16-bit */
#define R_M32R_32_RELA            34  /* direct 32-bit */
#define R_M32R_24_RELA            35  /* direct 24-bit */
#define R_M32R_10_PCREL_RELA      36  /* PC relative 10-bit shifted */
#define R_M32R_18_PCREL_RELA      37  /* PC relative 18-bit shifted */
#define R_M32R_26_PCREL_RELA      38  /* PC relative 26-bit shifted */
#define R_M32R_HI16_ULO_RELA      39  /* high 16-bit with unsigned low */
#define R_M32R_HI16_SLO_RELA      40  /* high 16-bit with signed low */
#define R_M32R_LO16_RELA          41  /* low 16-bit */
#define R_M32R_SDA16_RELA         42  /* 16-bit offset in SDA */
#define R_M32R_RELA_GNU_VTINHERIT 43  
#define R_M32R_RELA_GNU_VTENTRY   44  
#define R_M32R_REL32              45  /* PC relative 32-bit */
#define R_M32R_GOT24              48  /* 24-bit GOT entry */
#define R_M32R_26_PLTREL          49  /* 26-bit PC relative to PLT shifted */
#define R_M32R_COPY               50  /* copy symbol at runtime */
#define R_M32R_GLOB_DAT           51  /* create GOT entry */
#define R_M32R_JMP_SLOT           52  /* create PLT entry */
#define R_M32R_RELATIVE           53  /* adjust by program base */
#define R_M32R_GOTOFF             54  /* 24-bit offset to GOT */
#define R_M32R_GOTPC24            55  /* 24-bit PC relative offset to GOT */
#define R_M32R_GOT16_HI_ULO       56  /* high 16-bit GOT entry with unsigned low */
#define R_M32R_GOT16_HI_SLO       57  /* high 16-bit GOT entry with signed low */
#define R_M32R_GOT16_LO           58  /* low 16-bit GOT entry */
#define R_M32R_GOTPC_HI_ULO       59  /* high 16-bit PC relative offset to GOT
					 with unsigned low */
#define R_M32R_GOTPC_HI_SLO       60  /* high 16-bit PC relative offset to GOT
					 with signed low */
#define R_M32R_GOTPC_LO           61  /* low 16-bit PC relative offset to GOT */
#define R_M32R_GOTOFF_HI_ULO      62  /* high 16-bit offset to GOT with unsigned low */
#define R_M32R_GOTOFF_HI_SLO      63  /* high 16-bit offset to GOT  with signed low */
#define R_M32R_GOTOFF_LO          64  /* low 16-bit offset to GOT */
#define R_M32R_NUM                256

/* MicroBlaze relocation for ELF*_R_TYPE */
#define R_MICROBLAZE_NONE          0  /* no relocation */
#define R_MICROBLAZE_32            1  /* direct 32-bit */
#define R_MICROBLAZE_32_PCREL      2  /* PC relative 32-bit */
#define R_MICROBLAZE_64_PCREL      3  /* PC relative 64-bit */
#define R_MICROBLAZE_32_PCREL_LO   4  /* low 16-bits of PCREL32 */
#define R_MICROBLAZE_64            5  /* direct 64-bit */
#define R_MICROBLAZE_32_LO         6  /* low 16-bit */
#define R_MICROBLAZE_SRO32         7  /* read-only small data area */
#define R_MICROBLAZE_SRW32         8  /* read-write small data area */
#define R_MICROBLAZE_64_NONE       9  /* no relocation */
#define R_MICROBLAZE_32_SYM_OP_SYM 10 /* symbol op symbol relocation */
#define R_MICROBLAZE_GNU_VTINHERIT 11 /* GNU C++ vtable hierarchy */
#define R_MICROBLAZE_GNU_VTENTRY   12 /* GNU C++ vtable member usage */
#define R_MICROBLAZE_GOTPC_64      13 /* PC-relative GOT offset */
#define R_MICROBLAZE_GOT_64        14 /* GOT entry offset */
#define R_MICROBLAZE_PLT_64        15 /* PLT offset (PC-relative) */
#define R_MICROBLAZE_REL           16 /* adjust by program base */
#define R_MICROBLAZE_JUMP_SLOT     17 /* create PLT entry */
#define R_MICROBLAZE_GLOB_DAT      18 /* create GOT entry */
#define R_MICROBLAZE_GOTOFF_64     19 /* 64-bit offset to GOT */
#define R_MICROBLAZE_GOTOFF_32     20 /* 32-bit offset to GOT */
#define R_MICROBLAZE_COPY          21 /* runtime copy */
#define R_MICROBLAZE_TLS           22 /* TLS relocation */
#define R_MICROBLAZE_TLSGD         23 /* TLS general dynamic */
#define R_MICROBLAZE_TLSLD         24 /* TLS local dynamic */
#define R_MICROBLAZE_TLSDTPMOD32   25 /* TLS module ID */
#define R_MICROBLAZE_TLSDTPREL32   26 /* TLS offset within TLS block */
#define R_MICROBLAZE_TLSDTPREL64   27 /* TLS offset within TLS block */
#define R_MICROBLAZE_TLSGOTTPREL32 28 /* TLS offset from thread pointer */
#define R_MICROBLAZE_TLSTPREL32    29 /* TLS offset from thread pointer */

/* legal values for d_tag (dynamic entry type) */
#define DT_NIOS2_GP 0x70000002 /* address of _gp */

/* Nios II relocation for ELF*_R_TYPE */
#define R_NIOS2_NONE          0  /* no relocation */
#define R_NIOS2_S16           1  /* direct signed 16-bit */
#define R_NIOS2_U16           2  /* direct unsigned 16-bit */
#define R_NIOS2_PCREL16       3  /* PC relative 16-bit */
#define R_NIOS2_CALL26        4  /* direct call */
#define R_NIOS2_IMM5          5  /* 5-bit constant expression */
#define R_NIOS2_CACHE_OPX     6  /* 5-bit expression, shift 22 */
#define R_NIOS2_IMM6          7  /* 6-bit constant expression */
#define R_NIOS2_IMM8          8  /* 8-bit constant expression */
#define R_NIOS2_HI16          9  /* high 16-bit */
#define R_NIOS2_LO16          10 /* low 16-bit */
#define R_NIOS2_HIADJ16       11 /* high 16-bit, adjusted */
#define R_NIOS2_BFD_RELOC_32  12 /* 32-bit symbol value + addend */
#define R_NIOS2_BFD_RELOC_16  13 /* 16-bit symbol value + addend */
#define R_NIOS2_BFD_RELOC_8   14 /* 8-bit symbol value + addend */
#define R_NIOS2_GPREL         15 /* 16-bit GP pointer offset */
#define R_NIOS2_GNU_VTINHERIT 16 /* GNU C++ vtable hierarchy */
#define R_NIOS2_GNU_VTENTRY   17 /* GNU C++ vtable member usage */
#define R_NIOS2_UJMP          18 /* unconditional branch */
#define R_NIOS2_CJMP          19 /* conditional branch */
#define R_NIOS2_CALLR         20 /* indirect call through register */
#define R_NIOS2_ALIGN         21 /* alignment requirement for linker relaxation */
#define R_NIOS2_GOT16         22 /* 16-bit GOT entry */
#define R_NIOS2_CALL16        23 /* 16-bit GOT entry for function */
#define R_NIOS2_GOTOFF_LO     24 /* %lo of offset to GOT pointer */
#define R_NIOS2_GOTOFF_HA     25 /* %hiadj of offset to GOT pointer */
#define R_NIOS2_PCREL_LO      26 /* %lo of PC relative offset */
#define R_NIOS2_PCREL_HA      27 /* %hiadj of PC relative offset */
#define R_NIOS2_TLS_GD16      28 /* 16-bit GOT offset for TLS GD */
#define R_NIOS2_TLS_LDM16     29 /* 16-bit GOT offset for TLS LDM */
#define R_NIOS2_TLS_LDO16     30 /* 16-bit module relative offset */
#define R_NIOS2_TLS_IE16      31 /* 16-bit GOT offset for TLS IE */
#define R_NIOS2_TLS_LE16      32 /* 16-bit LE TP-relative offset */
#define R_NIOS2_TLS_DTPMOD    33 /* module number */
#define R_NIOS2_TLS_DTPREL    34 /* module-relative offset */
#define R_NIOS2_TLS_TPREL     35 /* TP-relative offset */
#define R_NIOS2_COPY          36 /* copy symbol at runtime */
#define R_NIOS2_GLOB_DAT      37 /* create GOT entry */
#define R_NIOS2_JUMP_SLOT     38 /* create PLT entry */
#define R_NIOS2_RELATIVE      39 /* adjust by program base */
#define R_NIOS2_GOTOFF        40 /* 16-bit offset to GOT pointer */
#define R_NIOS2_CALL26_NOAT   41 /* direct call in .noat section */
#define R_NIOS2_GOT_LO        42 /* %lo() of GOT entry */
#define R_NIOS2_GOT_HA        43 /* %hiadj() of GOT entry */
#define R_NIOS2_CALL_LO       44 /* %lo() of function GOT entry */
#define R_NIOS2_CALL_HA       45 /* %hiadj() of function GOT entry */

/* TILEPro relocation for ELF*_R_TYPE */
#define R_TILEPRO_NONE               0   /* no relocation */
#define R_TILEPRO_32                 1   /* direct 32-bit */
#define R_TILEPRO_16                 2   /* direct 16-bit */
#define R_TILEPRO_8                  3   /* direct 8-bit */
#define R_TILEPRO_32_PCREL           4   /* PC relative 32-bit */
#define R_TILEPRO_16_PCREL           5   /* PC relative 16-bit */
#define R_TILEPRO_8_PCREL            6   /* PC relative 8-bit */
#define R_TILEPRO_LO16               7   /* low 16-bit */
#define R_TILEPRO_HI16               8   /* high 16-bit */
#define R_TILEPRO_HA16               9   /* high 16-bit, adjusted */
#define R_TILEPRO_COPY               10  /* copy relocation */
#define R_TILEPRO_GLOB_DAT           11  /* create GOT entry */
#define R_TILEPRO_JMP_SLOT           12  /* create PLT entry */
#define R_TILEPRO_RELATIVE           13  /* adjust by program base */
#define R_TILEPRO_BROFF_X1           14  /* X1 pipe branch offset */
#define R_TILEPRO_JOFFLONG_X1        15  /* X1 pipe jump offset */
#define R_TILEPRO_JOFFLONG_X1_PLT    16  /* X1 pipe jump offset to PLT */
#define R_TILEPRO_IMM8_X0            17  /* X0 pipe 8-bit */
#define R_TILEPRO_IMM8_Y0            18  /* Y0 pipe 8-bit */
#define R_TILEPRO_IMM8_X1            19  /* X1 pipe 8-bit */
#define R_TILEPRO_IMM8_Y1            20  /* Y1 pipe 8-bit */
#define R_TILEPRO_MT_IMM15_X1        21  /* X1 pipe mtspr */
#define R_TILEPRO_MF_IMM15_X1        22  /* X1 pipe mfspr */
#define R_TILEPRO_IMM16_X0           23  /* X0 pipe 16-bit */
#define R_TILEPRO_IMM16_X1           24  /* X1 pipe 16-bit */
#define R_TILEPRO_IMM16_X0_LO        25  /* X0 pipe low 16-bit */
#define R_TILEPRO_IMM16_X1_LO        26  /* X1 pipe low 16-bit */
#define R_TILEPRO_IMM16_X0_HI        27  /* X0 pipe high 16-bit */
#define R_TILEPRO_IMM16_X1_HI        28  /* X1 pipe high 16-bit */
#define R_TILEPRO_IMM16_X0_HA        29  /* X0 pipe high 16-bit, adjusted */
#define R_TILEPRO_IMM16_X1_HA        30  /* X1 pipe high 16-bit, adjusted */
#define R_TILEPRO_IMM16_X0_PCREL     31  /* X0 pipe PC relative 16-bit */
#define R_TILEPRO_IMM16_X1_PCREL     32  /* X1 pipe PC relative 16-bit */
#define R_TILEPRO_IMM16_X0_LO_PCREL  33  /* X0 pipe PC relative low 16-bit */
#define R_TILEPRO_IMM16_X1_LO_PCREL  34  /* X1 pipe PC relative low 16-bit */
#define R_TILEPRO_IMM16_X0_HI_PCREL  35  /* X0 pipe PC relative high 16-bit */
#define R_TILEPRO_IMM16_X1_HI_PCREL  36  /* X1 pipe PC relative high 16-bit */
#define R_TILEPRO_IMM16_X0_HA_PCREL  37  /* X0 pipe PC relative ha() 16-bit */
#define R_TILEPRO_IMM16_X1_HA_PCREL  38  /* X1 pipe PC relative ha() 16-bit */
#define R_TILEPRO_IMM16_X0_GOT       39  /* X0 pipe 16-bit GOT offset */
#define R_TILEPRO_IMM16_X1_GOT       40  /* X1 pipe 16-bit GOT offset */
#define R_TILEPRO_IMM16_X0_GOT_LO    41  /* X0 pipe low 16-bit GOT offset */
#define R_TILEPRO_IMM16_X1_GOT_LO    42  /* X1 pipe low 16-bit GOT offset */
#define R_TILEPRO_IMM16_X0_GOT_HI    43  /* X0 pipe high 16-bit GOT offset */
#define R_TILEPRO_IMM16_X1_GOT_HI    44  /* X1 pipe high 16-bit GOT offset */
#define R_TILEPRO_IMM16_X0_GOT_HA    45  /* X0 pipe ha() 16-bit GOT offset */
#define R_TILEPRO_IMM16_X1_GOT_HA    46  /* X1 pipe ha() 16-bit GOT offset */
#define R_TILEPRO_MMSTART_X0         47  /* X0 pipe mm "start" */
#define R_TILEPRO_MMEND_X0           48  /* X0 pipe mm "end" */
#define R_TILEPRO_MMSTART_X1         49  /* X1 pipe mm "start" */
#define R_TILEPRO_MMEND_X1           50  /* X1 pipe mm "end" */
#define R_TILEPRO_SHAMT_X0           51  /* X0 pipe shift amount */
#define R_TILEPRO_SHAMT_X1           52  /* X1 pipe shift amount */
#define R_TILEPRO_SHAMT_Y0           53  /* Y0 pipe shift amount */
#define R_TILEPRO_SHAMT_Y1           54  /* Y1 pipe shift amount */
#define R_TILEPRO_DEST_IMM8_X1       55  /* X1 pipe destination 8-bit */
/* relocation 56-59 are currently not defined */
#define R_TILEPRO_TLS_GD_CALL        60  /* "jal" for TLS GD */
#define R_TILEPRO_IMM8_X0_TLS_GD_ADD 61  /* X0 pipe "addi" for TLS GD */
#define R_TILEPRO_IMM8_X1_TLS_GD_ADD 62  /* X1 pipe "addi" for TLS GD */
#define R_TILEPRO_IMM8_Y0_TLS_GD_ADD 63  /* Y0 pipe "addi" for TLS GD */
#define R_TILEPRO_IMM8_Y1_TLS_GD_ADD 64  /* Y1 pipe "addi" for TLS GD */
#define R_TILEPRO_TLS_IE_LOAD        65  /* "lw_tls" for TLS IE */
#define R_TILEPRO_IMM16_X0_TLS_GD    66  /* X0 pipe 16-bit TLS GD offset */
#define R_TILEPRO_IMM16_X1_TLS_GD    67  /* X1 pipe 16-bit TLS GD offset */
#define R_TILEPRO_IMM16_X0_TLS_GD_LO 68  /* X0 pipe low 16-bit TLS GD offset */
#define R_TILEPRO_IMM16_X1_TLS_GD_LO 69  /* X1 pipe low 16-bit TLS GD offset */
#define R_TILEPRO_IMM16_X0_TLS_GD_HI 70  /* X0 pipe high 16-bit TLS GD offset */
#define R_TILEPRO_IMM16_X1_TLS_GD_HI 71  /* X1 pipe high 16-bit TLS GD offset */
#define R_TILEPRO_IMM16_X0_TLS_GD_HA 72  /* X0 pipe ha() 16-bit TLS GD offset */
#define R_TILEPRO_IMM16_X1_TLS_GD_HA 73  /* X1 pipe ha() 16-bit TLS GD offset */
#define R_TILEPRO_IMM16_X0_TLS_IE    74  /* X0 pipe 16-bit TLS IE offset */
#define R_TILEPRO_IMM16_X1_TLS_IE    75  /* X1 pipe 16-bit TLS IE offset */
#define R_TILEPRO_IMM16_X0_TLS_IE_LO 76  /* X0 pipe low 16-bit TLS IE offset */
#define R_TILEPRO_IMM16_X1_TLS_IE_LO 77  /* X1 pipe low 16-bit TLS IE offset */
#define R_TILEPRO_IMM16_X0_TLS_IE_HI 78  /* X0 pipe high 16-bit TLS IE offset */
#define R_TILEPRO_IMM16_X1_TLS_IE_HI 79  /* X1 pipe high 16-bit TLS IE offset */
#define R_TILEPRO_IMM16_X0_TLS_IE_HA 80  /* X0 pipe ha() 16-bit TLS IE offset */
#define R_TILEPRO_IMM16_X1_TLS_IE_HA 81  /* X1 pipe ha() 16-bit TLS IE offset */
#define R_TILEPRO_TLS_DTPMOD32       82  /* ID of module containing symbol */
#define R_TILEPRO_TLS_DTPOFF32       83  /* Offset in TLS block */
#define R_TILEPRO_TLS_TPOFF32        84  /* Offset in static TLS block */
#define R_TILEPRO_IMM16_X0_TLS_LE    85  /* X0 pipe 16-bit TLS LE offset */
#define R_TILEPRO_IMM16_X1_TLS_LE    86  /* X1 pipe 16-bit TLS LE offset */
#define R_TILEPRO_IMM16_X0_TLS_LE_LO 87  /* X0 pipe low 16-bit TLS LE offset */
#define R_TILEPRO_IMM16_X1_TLS_LE_LO 88  /* X1 pipe low 16-bit TLS LE offset */
#define R_TILEPRO_IMM16_X0_TLS_LE_HI 89  /* X0 pipe high 16-bit TLS LE offset */
#define R_TILEPRO_IMM16_X1_TLS_LE_HI 90  /* X1 pipe high 16-bit TLS LE offset */
#define R_TILEPRO_IMM16_X0_TLS_LE_HA 91  /* X0 pipe ha() 16-bit TLS LE offset */
#define R_TILEPRO_IMM16_X1_TLS_LE_HA 92  /* X1 pipe ha() 16-bit TLS LE offset */
#define R_TILEPRO_GNU_VTINHERIT      128 /* GNU C++ vtable hierarchy */
#define R_TILEPRO_GNU_VTENTRY        129 /* GNU C++ vtable member usage */
#define R_TILEPRO_NUM                130 

/* TILE-Gx relocation for ELF*_R_TYPE */
#define R_TILEGX_NONE                        0   /* no reloc */
#define R_TILEGX_64                          1   /* direct 64-bit */
#define R_TILEGX_32                          2   /* direct 32-bit */
#define R_TILEGX_16                          3   /* direct 16-bit */
#define R_TILEGX_8                           4   /* direct 8-bit */
#define R_TILEGX_64_PCREL                    5   /* PC relative 64-bit */
#define R_TILEGX_32_PCREL                    6   /* PC relative 32-bit */
#define R_TILEGX_16_PCREL                    7   /* PC relative 16-bit */
#define R_TILEGX_8_PCREL                     8   /* PC relative 8-bit */
#define R_TILEGX_HW0                         9   /* hword 0 16-bit */
#define R_TILEGX_HW1                         10  /* hword 1 16-bit */
#define R_TILEGX_HW2                         11  /* hword 2 16-bit */
#define R_TILEGX_HW3                         12  /* hword 3 16-bit */
#define R_TILEGX_HW0_LAST                    13  /* last hword 0 16-bit */
#define R_TILEGX_HW1_LAST                    14  /* last hword 1 16-bit */
#define R_TILEGX_HW2_LAST                    15  /* last hword 2 16-bit */
#define R_TILEGX_COPY                        16  /* copy relocation */
#define R_TILEGX_GLOB_DAT                    17  /* create GOT entry */
#define R_TILEGX_JMP_SLOT                    18  /* create PLT entry */
#define R_TILEGX_RELATIVE                    19  /* adjust by program base */
#define R_TILEGX_BROFF_X1                    20  /* X1 pipe branch offset */
#define R_TILEGX_JUMPOFF_X1                  21  /* X1 pipe jump offset */
#define R_TILEGX_JUMPOFF_X1_PLT              22  /* X1 pipe jump offset to PLT */
#define R_TILEGX_IMM8_X0                     23  /* X0 pipe 8-bit */
#define R_TILEGX_IMM8_Y0                     24  /* Y0 pipe 8-bit */
#define R_TILEGX_IMM8_X1                     25  /* X1 pipe 8-bit */
#define R_TILEGX_IMM8_Y1                     26  /* Y1 pipe 8-bit */
#define R_TILEGX_DEST_IMM8_X1                27  /* X1 pipe destination 8-bit */
#define R_TILEGX_MT_IMM14_X1                 28  /* X1 pipe mtspr */
#define R_TILEGX_MF_IMM14_X1                 29  /* X1 pipe mfspr */
#define R_TILEGX_MMSTART_X0                  30  /* X0 pipe mm "start" */
#define R_TILEGX_MMEND_X0                    31  /* X0 pipe mm "end" */
#define R_TILEGX_SHAMT_X0                    32  /* X0 pipe shift amount */
#define R_TILEGX_SHAMT_X1                    33  /* X1 pipe shift amount */
#define R_TILEGX_SHAMT_Y0                    34  /* Y0 pipe shift amount */
#define R_TILEGX_SHAMT_Y1                    35  /* Y1 pipe shift amount */
#define R_TILEGX_IMM16_X0_HW0                36  /* X0 pipe hword 0 */
#define R_TILEGX_IMM16_X1_HW0                37  /* X1 pipe hword 0 */
#define R_TILEGX_IMM16_X0_HW1                38  /* X0 pipe hword 1 */
#define R_TILEGX_IMM16_X1_HW1                39  /* X1 pipe hword 1 */
#define R_TILEGX_IMM16_X0_HW2                40  /* X0 pipe hword 2 */
#define R_TILEGX_IMM16_X1_HW2                41  /* X1 pipe hword 2 */
#define R_TILEGX_IMM16_X0_HW3                42  /* X0 pipe hword 3 */
#define R_TILEGX_IMM16_X1_HW3                43  /* X1 pipe hword 3 */
#define R_TILEGX_IMM16_X0_HW0_LAST           44  /* X0 pipe last hword 0 */
#define R_TILEGX_IMM16_X1_HW0_LAST           45  /* X1 pipe last hword 0 */
#define R_TILEGX_IMM16_X0_HW1_LAST           46  /* X0 pipe last hword 1 */
#define R_TILEGX_IMM16_X1_HW1_LAST           47  /* X1 pipe last hword 1 */
#define R_TILEGX_IMM16_X0_HW2_LAST           48  /* X0 pipe last hword 2 */
#define R_TILEGX_IMM16_X1_HW2_LAST           49  /* X1 pipe last hword 2 */
#define R_TILEGX_IMM16_X0_HW0_PCREL          50  /* X0 pipe PC relative hword 0 */
#define R_TILEGX_IMM16_X1_HW0_PCREL          51  /* X1 pipe PC relative hword 0 */
#define R_TILEGX_IMM16_X0_HW1_PCREL          52  /* X0 pipe PC relative hword 1 */
#define R_TILEGX_IMM16_X1_HW1_PCREL          53  /* X1 pipe PC relative hword 1 */
#define R_TILEGX_IMM16_X0_HW2_PCREL          54  /* X0 pipe PC relative hword 2 */
#define R_TILEGX_IMM16_X1_HW2_PCREL          55  /* X1 pipe PC relative hword 2 */
#define R_TILEGX_IMM16_X0_HW3_PCREL          56  /* X0 pipe PC relative hword 3 */
#define R_TILEGX_IMM16_X1_HW3_PCREL          57  /* X1 pipe PC relative hword 3 */
#define R_TILEGX_IMM16_X0_HW0_LAST_PCREL     58  /* X0 pipe PC-rel last hword 0 */
#define R_TILEGX_IMM16_X1_HW0_LAST_PCREL     59  /* X1 pipe PC-rel last hword 0 */
#define R_TILEGX_IMM16_X0_HW1_LAST_PCREL     60  /* X0 pipe PC-rel last hword 1 */
#define R_TILEGX_IMM16_X1_HW1_LAST_PCREL     61  /* X1 pipe PC-rel last hword 1 */
#define R_TILEGX_IMM16_X0_HW2_LAST_PCREL     62  /* X0 pipe PC-rel last hword 2 */
#define R_TILEGX_IMM16_X1_HW2_LAST_PCREL     63  /* X1 pipe PC-rel last hword 2 */
#define R_TILEGX_IMM16_X0_HW0_GOT            64  /* X0 pipe hword 0 GOT offset */
#define R_TILEGX_IMM16_X1_HW0_GOT            65  /* X1 pipe hword 0 GOT offset */
#define R_TILEGX_IMM16_X0_HW0_PLT_PCREL      66  /* X0 pipe PC-rel PLT hword 0 */
#define R_TILEGX_IMM16_X1_HW0_PLT_PCREL      67  /* X1 pipe PC-rel PLT hword 0 */
#define R_TILEGX_IMM16_X0_HW1_PLT_PCREL      68  /* X0 pipe PC-rel PLT hword 1 */
#define R_TILEGX_IMM16_X1_HW1_PLT_PCREL      69  /* X1 pipe PC-rel PLT hword 1 */
#define R_TILEGX_IMM16_X0_HW2_PLT_PCREL      70  /* X0 pipe PC-rel PLT hword 2 */
#define R_TILEGX_IMM16_X1_HW2_PLT_PCREL      71  /* X1 pipe PC-rel PLT hword 2 */
#define R_TILEGX_IMM16_X0_HW0_LAST_GOT       72  /* X0 pipe last hword 0 GOT offset */
#define R_TILEGX_IMM16_X1_HW0_LAST_GOT       73  /* X1 pipe last hword 0 GOT offset */
#define R_TILEGX_IMM16_X0_HW1_LAST_GOT       74  /* X0 pipe last hword 1 GOT offset */
#define R_TILEGX_IMM16_X1_HW1_LAST_GOT       75  /* X1 pipe last hword 1 GOT offset */
#define R_TILEGX_IMM16_X0_HW3_PLT_PCREL      76  /* X0 pipe PC-rel PLT hword 3 */
#define R_TILEGX_IMM16_X1_HW3_PLT_PCREL      77  /* X1 pipe PC-rel PLT hword 3 */
#define R_TILEGX_IMM16_X0_HW0_TLS_GD         78  /* X0 pipe hword 0 TLS GD offset */
#define R_TILEGX_IMM16_X1_HW0_TLS_GD         79  /* X1 pipe hword 0 TLS GD offset */
#define R_TILEGX_IMM16_X0_HW0_TLS_LE         80  /* X0 pipe hword 0 TLS LE offset */
#define R_TILEGX_IMM16_X1_HW0_TLS_LE         81  /* X1 pipe hword 0 TLS LE offset */
#define R_TILEGX_IMM16_X0_HW0_LAST_TLS_LE    82  /* X0 pipe last hword 0 LE off */
#define R_TILEGX_IMM16_X1_HW0_LAST_TLS_LE    83  /* X1 pipe last hword 0 LE off */
#define R_TILEGX_IMM16_X0_HW1_LAST_TLS_LE    84  /* X0 pipe last hword 1 LE off */
#define R_TILEGX_IMM16_X1_HW1_LAST_TLS_LE    85  /* X1 pipe last hword 1 LE off */
#define R_TILEGX_IMM16_X0_HW0_LAST_TLS_GD    86  /* X0 pipe last hword 0 GD off */
#define R_TILEGX_IMM16_X1_HW0_LAST_TLS_GD    87  /* X1 pipe last hword 0 GD off */
#define R_TILEGX_IMM16_X0_HW1_LAST_TLS_GD    88  /* X0 pipe last hword 1 GD off */
#define R_TILEGX_IMM16_X1_HW1_LAST_TLS_GD    89  /* X1 pipe last hword 1 GD off */
/* Relocs 90-91 are currently not defined.  */
#define R_TILEGX_IMM16_X0_HW0_TLS_IE         92  /* X0 pipe hword 0 TLS IE offset */
#define R_TILEGX_IMM16_X1_HW0_TLS_IE         93  /* X1 pipe hword 0 TLS IE offset */
#define R_TILEGX_IMM16_X0_HW0_LAST_PLT_PCREL 94  /* X0 pipe PC-rel PLT last hword 0 */
#define R_TILEGX_IMM16_X1_HW0_LAST_PLT_PCREL 95  /* X1 pipe PC-rel PLT last hword 0 */
#define R_TILEGX_IMM16_X0_HW1_LAST_PLT_PCREL 96  /* X0 pipe PC-rel PLT last hword 1 */
#define R_TILEGX_IMM16_X1_HW1_LAST_PLT_PCREL 97  /* X1 pipe PC-rel PLT last hword 1 */
#define R_TILEGX_IMM16_X0_HW2_LAST_PLT_PCREL 98  /* X0 pipe PC-rel PLT last hword 2 */
#define R_TILEGX_IMM16_X1_HW2_LAST_PLT_PCREL 99  /* X1 pipe PC-rel PLT last hword 2 */
#define R_TILEGX_IMM16_X0_HW0_LAST_TLS_IE    100 /* X0 pipe last hword 0 IE off */
#define R_TILEGX_IMM16_X1_HW0_LAST_TLS_IE    101 /* X1 pipe last hword 0 IE off */
#define R_TILEGX_IMM16_X0_HW1_LAST_TLS_IE    102 /* X0 pipe last hword 1 IE off */
#define R_TILEGX_IMM16_X1_HW1_LAST_TLS_IE    103 /* X1 pipe last hword 1 IE off */
/* Relocs 104-105 are currently not defined.  */
#define R_TILEGX_TLS_DTPMOD64                106 /* 64-bit ID of symbol's module */
#define R_TILEGX_TLS_DTPOFF64                107 /* 64-bit offset in TLS block */
#define R_TILEGX_TLS_TPOFF64                 108 /* 64-bit offset in static TLS block */
#define R_TILEGX_TLS_DTPMOD32                109 /* 32-bit ID of symbol's module */
#define R_TILEGX_TLS_DTPOFF32                110 /* 32-bit offset in TLS block */
#define R_TILEGX_TLS_TPOFF32                 111 /* 32-bit offset in static TLS block */
#define R_TILEGX_TLS_GD_CALL                 112 /* "jal" for TLS GD */
#define R_TILEGX_IMM8_X0_TLS_GD_ADD          113 /* X0 pipe "addi" for TLS GD */
#define R_TILEGX_IMM8_X1_TLS_GD_ADD          114 /* X1 pipe "addi" for TLS GD */
#define R_TILEGX_IMM8_Y0_TLS_GD_ADD          115 /* Y0 pipe "addi" for TLS GD */
#define R_TILEGX_IMM8_Y1_TLS_GD_ADD          116 /* Y1 pipe "addi" for TLS GD */
#define R_TILEGX_TLS_IE_LOAD                 117 /* "ld_tls" for TLS IE */
#define R_TILEGX_IMM8_X0_TLS_ADD             118 /* X0 pipe "addi" for TLS GD/IE */
#define R_TILEGX_IMM8_X1_TLS_ADD             119 /* X1 pipe "addi" for TLS GD/IE */
#define R_TILEGX_IMM8_Y0_TLS_ADD             120 /* Y0 pipe "addi" for TLS GD/IE */
#define R_TILEGX_IMM8_Y1_TLS_ADD             121 /* Y1 pipe "addi" for TLS GD/IE */

#define R_TILEGX_GNU_VTINHERIT               128 /* GNU C++ vtable hierarchy */
#define R_TILEGX_GNU_VTENTRY                 129 /* GNU C++ vtable member usage */

#define R_TILEGX_NUM                         130 

/* RISC-V specific definitions */

/* elf*_ehdr.e_flags */
#define EF_RISCV_RVC              0x0001 
#define EF_RISCV_FLOAT_ABI        0x0006 
#define EF_RISCV_FLOAT_ABI_SOFT   0x0000 
#define EF_RISCV_FLOAT_ABI_SINGLE 0x0002 
#define EF_RISCV_FLOAT_ABI_DOUBLE 0x0004 
#define EF_RISCV_FLOAT_ABI_QUAD   0x0006 
#define EF_RISCV_RVE              0x0008 
#define EF_RISCV_TSO              0x0010 

/* RISC-V relocation for ELF*_R_TYPE */
#define R_RISCV_NONE              0   
#define R_RISCV_32                1   
#define R_RISCV_64                2   
#define R_RISCV_RELATIVE          3   
#define R_RISCV_COPY              4   
#define R_RISCV_JUMP_SLOT         5   
#define R_RISCV_TLS_DTPMOD32      6   
#define R_RISCV_TLS_DTPMOD64      7   
#define R_RISCV_TLS_DTPREL32      8   
#define R_RISCV_TLS_DTPREL64      9   
#define R_RISCV_TLS_TPREL32       10  
#define R_RISCV_TLS_TPREL64       11  
#define R_RISCV_TLSDESC           12  
#define R_RISCV_BRANCH            16  
#define R_RISCV_JAL               17  
#define R_RISCV_CALL              18  
#define R_RISCV_CALL_PLT          19  
#define R_RISCV_GOT_HI20          20  
#define R_RISCV_TLS_GOT_HI20      21  
#define R_RISCV_TLS_GD_HI20       22  
#define R_RISCV_PCREL_HI20        23  
#define R_RISCV_PCREL_LO12_I      24  
#define R_RISCV_PCREL_LO12_S      25  
#define R_RISCV_HI20              26  
#define R_RISCV_LO12_I            27  
#define R_RISCV_LO12_S            28  
#define R_RISCV_TPREL_HI20        29  
#define R_RISCV_TPREL_LO12_I      30  
#define R_RISCV_TPREL_LO12_S      31  
#define R_RISCV_TPREL_ADD         32  
#define R_RISCV_ADD8              33  
#define R_RISCV_ADD16             34  
#define R_RISCV_ADD32             35  
#define R_RISCV_ADD64             36  
#define R_RISCV_SUB8              37  
#define R_RISCV_SUB16             38  
#define R_RISCV_SUB32             39  
#define R_RISCV_SUB64             40  
#define R_RISCV_GOT32_PCREL       41  
#define R_RISCV_ALIGN             43  
#define R_RISCV_RVC_BRANCH        44  
#define R_RISCV_RVC_JUMP          45  
#define R_RISCV_RELAX             51  
#define R_RISCV_SUB6              52  
#define R_RISCV_SET6              53  
#define R_RISCV_SET8              54  
#define R_RISCV_SET16             55  
#define R_RISCV_SET32             56  
#define R_RISCV_32_PCREL          57  
#define R_RISCV_IRELATIVE         58  
#define R_RISCV_PLT32             59  
#define R_RISCV_SET_ULEB128       60  
#define R_RISCV_SUB_ULEB128       61  
#define R_RISCV_TLSDESC_HI20      62  
#define R_RISCV_TLSDESC_LOAD_LO12 63  
#define R_RISCV_TLSDESC_ADD_LO12  64  
#define R_RISCV_TLSDESC_CALL      65  
#define R_RISCV_NUM               66  

/* elf*_sym.st_other */
#define STO_RISCV_VARIANT_CC 0x80 /* function uses variant calling convention */

/* elf*_shdr.sh_type */
#define SHT_RISCV_ATTRIBUTES (SHT_LOPROC + 3)

/* elf*_phdr.p_type */
#define PT_RISCV_ATTRIBUTES (PT_LOPROC + 3)

/* elf*_dyn.d_tag */
#define DT_RISCV_VARIANT_CC (DT_LOPROC + 1)

/* BPF specific definitions */

/* BPF relocation for ELF*_R_TYPE */
#define R_BPF_NONE  0 /* no relocation */
#define R_BPF_64_64 1   
#define R_BPF_64_32 10  

/* Imagination Meta relocation for ELF*_R_TYPE */
#define R_METAG_HIADDR16          0   
#define R_METAG_LOADDR16          1   
#define R_METAG_ADDR32            2 /* 32-bit absolute address */
#define R_METAG_NONE              3 /* no relocation */
#define R_METAG_RELBRANCH         4   
#define R_METAG_GETSETOFF         5   
/* Backward compatibility */
#define R_METAG_REG32OP1          6   
#define R_METAG_REG32OP2          7   
#define R_METAG_REG32OP3          8   
#define R_METAG_REG16OP1          9   
#define R_METAG_REG16OP2          10  
#define R_METAG_REG16OP3          11  
#define R_METAG_REG32OP4          12  
#define R_METAG_HIOG              13  
#define R_METAG_LOOG              14  
#define R_METAG_REL8              15  
#define R_METAG_REL16             16  
/* GNU */
#define R_METAG_GNU_VTINHERIT     30  
#define R_METAG_GNU_VTENTRY       31  
/* PIC relocations */
#define R_METAG_HI16_GOTOFF       32  
#define R_METAG_LO16_GOTOFF       33  
#define R_METAG_GETSET_GOTOFF     34  
#define R_METAG_GETSET_GOT        35  
#define R_METAG_HI16_GOTPC        36  
#define R_METAG_LO16_GOTPC        37  
#define R_METAG_HI16_PLT          38  
#define R_METAG_LO16_PLT          39  
#define R_METAG_RELBRANCH_PLT     40  
#define R_METAG_GOTOFF            41  
#define R_METAG_PLT               42  
#define R_METAG_COPY              43  
#define R_METAG_JMP_SLOT          44  
#define R_METAG_RELATIVE          45  
#define R_METAG_GLOB_DAT          46  
/* TLS relocations */
#define R_METAG_TLS_GD            47  
#define R_METAG_TLS_LDM           48  
#define R_METAG_TLS_LDO_HI16      49  
#define R_METAG_TLS_LDO_LO16      50  
#define R_METAG_TLS_LDO           51  
#define R_METAG_TLS_IE            52  
#define R_METAG_TLS_IENONPIC      53  
#define R_METAG_TLS_IENONPIC_HI16 54  
#define R_METAG_TLS_IENONPIC_LO16 55  
#define R_METAG_TLS_TPOFF         56  
#define R_METAG_TLS_DTPMOD        57  
#define R_METAG_TLS_DTPOFF        58  
#define R_METAG_TLS_LE            59  
#define R_METAG_TLS_LE_HI16       60  
#define R_METAG_TLS_LE_LO16       61  

/* NDS32 specific definitions */

/* NDS32 relocation for ELF*_R_TYPE */
#define R_NDS32_NONE      0   
#define R_NDS32_32_RELA   20  
#define R_NDS32_COPY      39  
#define R_NDS32_GLOB_DAT  40  
#define R_NDS32_JMP_SLOT  41  
#define R_NDS32_RELATIVE  42  
#define R_NDS32_TLS_TPOFF 102 
#define R_NDS32_TLS_DESC  119 

/* LoongArch specific definitions */

/* elf*_ehdr.e_flags */
#define EF_LARCH_ABI_MODIFIER_MASK 0x07 
#define EF_LARCH_ABI_SOFT_FLOAT    0x01 
#define EF_LARCH_ABI_SINGLE_FLOAT  0x02 
#define EF_LARCH_ABI_DOUBLE_FLOAT  0x03 
#define EF_LARCH_OBJABI_V1         0x40 

/* LoongArch relocation for ELF*_R_TYPE */
#define R_LARCH_NONE                       0   
#define R_LARCH_32                         1   
#define R_LARCH_64                         2   
#define R_LARCH_RELATIVE                   3   
#define R_LARCH_COPY                       4   
#define R_LARCH_JUMP_SLOT                  5   
#define R_LARCH_TLS_DTPMOD32               6   
#define R_LARCH_TLS_DTPMOD64               7   
#define R_LARCH_TLS_DTPREL32               8   
#define R_LARCH_TLS_DTPREL64               9   
#define R_LARCH_TLS_TPREL32                10  
#define R_LARCH_TLS_TPREL64                11  
#define R_LARCH_IRELATIVE                  12  
#define R_LARCH_TLS_DESC32                 13  
#define R_LARCH_TLS_DESC64                 14  
/* reserved for future relocs that the dynamic linker must understand */
/* used by the static linker for relocating .text */
#define R_LARCH_MARK_LA                    20  
#define R_LARCH_MARK_PCREL                 21  
#define R_LARCH_SOP_PUSH_PCREL             22  
#define R_LARCH_SOP_PUSH_ABSOLUTE          23  
#define R_LARCH_SOP_PUSH_DUP               24  
#define R_LARCH_SOP_PUSH_GPREL             25  
#define R_LARCH_SOP_PUSH_TLS_TPREL         26  
#define R_LARCH_SOP_PUSH_TLS_GOT           27  
#define R_LARCH_SOP_PUSH_TLS_GD            28  
#define R_LARCH_SOP_PUSH_PLT_PCREL         29  
#define R_LARCH_SOP_ASSERT                 30  
#define R_LARCH_SOP_NOT                    31  
#define R_LARCH_SOP_SUB                    32  
#define R_LARCH_SOP_SL                     33  
#define R_LARCH_SOP_SR                     34  
#define R_LARCH_SOP_ADD                    35  
#define R_LARCH_SOP_AND                    36  
#define R_LARCH_SOP_IF_ELSE                37  
#define R_LARCH_SOP_POP_32_S_10_5          38  
#define R_LARCH_SOP_POP_32_U_10_12         39  
#define R_LARCH_SOP_POP_32_S_10_12         40  
#define R_LARCH_SOP_POP_32_S_10_16         41  
#define R_LARCH_SOP_POP_32_S_10_16_S2      42  
#define R_LARCH_SOP_POP_32_S_5_20          43  
#define R_LARCH_SOP_POP_32_S_0_5_10_16_S2  44  
#define R_LARCH_SOP_POP_32_S_0_10_10_16_S2 45  
#define R_LARCH_SOP_POP_32_U               46  
/* used by the static linker for relocating non .text */
#define R_LARCH_ADD8                       47  
#define R_LARCH_ADD16                      48  
#define R_LARCH_ADD24                      49  
#define R_LARCH_ADD32                      50  
#define R_LARCH_ADD64                      51  
#define R_LARCH_SUB8                       52  
#define R_LARCH_SUB16                      53  
#define R_LARCH_SUB24                      54  
#define R_LARCH_SUB32                      55  
#define R_LARCH_SUB64                      56  
#define R_LARCH_GNU_VTINHERIT              57  
#define R_LARCH_GNU_VTENTRY                58  
/* reserved 59-63 */
#define R_LARCH_B16                        64  
#define R_LARCH_B21                        65  
#define R_LARCH_B26                        66  
#define R_LARCH_ABS_HI20                   67  
#define R_LARCH_ABS_LO12                   68  
#define R_LARCH_ABS64_LO20                 69  
#define R_LARCH_ABS64_HI12                 70  
#define R_LARCH_PCALA_HI20                 71  
#define R_LARCH_PCALA_LO12                 72  
#define R_LARCH_PCALA64_LO20               73  
#define R_LARCH_PCALA64_HI12               74  
#define R_LARCH_GOT_PC_HI20                75  
#define R_LARCH_GOT_PC_LO12                76  
#define R_LARCH_GOT64_PC_LO20              77  
#define R_LARCH_GOT64_PC_HI12              78  
#define R_LARCH_GOT_HI20                   79  
#define R_LARCH_GOT_LO12                   80  
#define R_LARCH_GOT64_LO20                 81  
#define R_LARCH_GOT64_HI12                 82  
#define R_LARCH_TLS_LE_HI20                83  
#define R_LARCH_TLS_LE_LO12                84  
#define R_LARCH_TLS_LE64_LO20              85  
#define R_LARCH_TLS_LE64_HI12              86  
#define R_LARCH_TLS_IE_PC_HI20             87  
#define R_LARCH_TLS_IE_PC_LO12             88  
#define R_LARCH_TLS_IE64_PC_LO20           89  
#define R_LARCH_TLS_IE64_PC_HI12           90  
#define R_LARCH_TLS_IE_HI20                91  
#define R_LARCH_TLS_IE_LO12                92  
#define R_LARCH_TLS_IE64_LO20              93  
#define R_LARCH_TLS_IE64_HI12              94  
#define R_LARCH_TLS_LD_PC_HI20             95  
#define R_LARCH_TLS_LD_HI20                96  
#define R_LARCH_TLS_GD_PC_HI20             97  
#define R_LARCH_TLS_GD_HI20                98  
#define R_LARCH_32_PCREL                   99  
#define R_LARCH_RELAX                      100 
#define R_LARCH_DELETE                     101 
#define R_LARCH_ALIGN                      102 
#define R_LARCH_PCREL20_S2                 103 
#define R_LARCH_CFA                        104 
#define R_LARCH_ADD6                       105 
#define R_LARCH_SUB6                       106 
#define R_LARCH_ADD_ULEB128                107 
#define R_LARCH_SUB_ULEB128                108 
#define R_LARCH_64_PCREL                   109 
#define R_LARCH_CALL36                     110 
#define R_LARCH_TLS_DESC_PC_HI20           111 
#define R_LARCH_TLS_DESC_PC_LO12           112 
#define R_LARCH_TLS_DESC64_PC_LO20         113 
#define R_LARCH_TLS_DESC64_PC_HI12         114 
#define R_LARCH_TLS_DESC_HI20              115 
#define R_LARCH_TLS_DESC_LO12              116 
#define R_LARCH_TLS_DESC64_LO20            117 
#define R_LARCH_TLS_DESC64_HI12            118 
#define R_LARCH_TLS_DESC_LD                119 
#define R_LARCH_TLS_DESC_CALL              120 
#define R_LARCH_TLS_LE_HI20_R              121 
#define R_LARCH_TLS_LE_ADD_R               122 
#define R_LARCH_TLS_LE_LO12_R              123 
#define R_LARCH_TLS_LD_PCREL20_S2          124 
#define R_LARCH_TLS_GD_PCREL20_S2          125 
#define R_LARCH_TLS_DESC_PCREL20_S2        126 

/* ARC specific definitions */

/* elf*_ehdr.e_flags */
#define EF_ARC_MACH_MSK  0x000000ff 
#define EF_ARC_OSABI_MSK 0x00000f00 
#define EF_ARC_ALL_MSK   (EF_ARC_MACH_MSK | EF_ARC_OSABI_MSK)

/* elf*_shdr.sh_type */
#define SHT_ARC_ATTRIBUTES (SHT_LOPROC + 1) /* ARC attributes section */

/* ARCompact/ARCv2 relocation for ELF*_R_TYPE */
#define R_ARC_NONE           0x0 
#define R_ARC_8              0x1 
#define R_ARC_16             0x2 
#define R_ARC_24             0x3 
#define R_ARC_32             0x4 
#define R_ARC_B22_PCREL      0x6 
#define R_ARC_H30            0x7 
#define R_ARC_N8             0x8 
#define R_ARC_N16            0x9 
#define R_ARC_N24            0xa 
#define R_ARC_N32            0xb 
#define R_ARC_SDA            0xc 
#define R_ARC_SECTOFF        0xd 
#define R_ARC_S21H_PCREL     0xe 
#define R_ARC_S21W_PCREL     0xf 
#define R_ARC_S25H_PCREL     0x10 
#define R_ARC_S25W_PCREL     0x11 
#define R_ARC_SDA32          0x12 
#define R_ARC_SDA_LDST       0x13 
#define R_ARC_SDA_LDST1      0x14 
#define R_ARC_SDA_LDST2      0x15 
#define R_ARC_SDA16_LD       0x16 
#define R_ARC_SDA16_LD1      0x17 
#define R_ARC_SDA16_LD2      0x18 
#define R_ARC_S13_PCREL      0x19 
#define R_ARC_W              0x1a 
#define R_ARC_32_ME          0x1b 
#define R_ARC_N32_ME         0x1c 
#define R_ARC_SECTOFF_ME     0x1d 
#define R_ARC_SDA32_ME       0x1e 
#define R_ARC_W_ME           0x1f 
#define R_ARC_H30_ME         0x20 
#define R_ARC_SECTOFF_U8     0x21 
#define R_ARC_SECTOFF_S9     0x22 
#define R_AC_SECTOFF_U8      0x23 
#define R_AC_SECTOFF_U8_1    0x24 
#define R_AC_SECTOFF_U8_2    0x25 
#define R_AC_SECTOFF_S9      0x26 
#define R_AC_SECTOFF_S9_1    0x27 
#define R_AC_SECTOFF_S9_2    0x28 
#define R_ARC_SECTOFF_ME_1   0x29 
#define R_ARC_SECTOFF_ME_2   0x2a 
#define R_ARC_SECTOFF_1      0x2b 
#define R_ARC_SECTOFF_2      0x2c 
#define R_ARC_SDA_12         0x2d 
#define R_ARC_SDA16_ST2      0x30 
#define R_ARC_32_PCREL       0x31 
#define R_ARC_PC32           0x32 
#define R_ARC_GOTPC32        0x33 
#define R_ARC_PLT32          0x34 
#define R_ARC_COPY           0x35 
#define R_ARC_GLOB_DAT       0x36 
#define R_ARC_JMP_SLOT       0x37 
#define R_ARC_RELATIVE       0x38 
#define R_ARC_GOTOFF         0x39 
#define R_ARC_GOTPC          0x3a 
#define R_ARC_GOT32          0x3b 
#define R_ARC_S21W_PCREL_PLT 0x3c 
#define R_ARC_S25H_PCREL_PLT 0x3d 
#define R_ARC_JLI_SECTOFF    0x3f 
#define R_ARC_TLS_DTPMOD     0x42 
#define R_ARC_TLS_DTPOFF     0x43 
#define R_ARC_TLS_TPOFF      0x44 
#define R_ARC_TLS_GD_GOT     0x45 
#define R_ARC_TLS_GD_LD      0x46 
#define R_ARC_TLS_GD_CALL    0x47 
#define R_ARC_TLS_IE_GOT     0x48 
#define R_ARC_TLS_DTPOFF_S9  0x49 
#define R_ARC_TLS_LE_S9      0x4a 
#define R_ARC_TLS_LE_32      0x4b 
#define R_ARC_S25W_PCREL_PLT 0x4c 
#define R_ARC_S21H_PCREL_PLT 0x4d 
#define R_ARC_NPS_CMEM16     0x4e 

/* OpenRISC 1000 relocation for ELF*_R_TYPE */
#define R_OR1K_NONE          0   
#define R_OR1K_32            1   
#define R_OR1K_16            2   
#define R_OR1K_8             3   
#define R_OR1K_LO_16_IN_INSN 4   
#define R_OR1K_HI_16_IN_INSN 5   
#define R_OR1K_INSN_REL_26   6   
#define R_OR1K_GNU_VTENTRY   7   
#define R_OR1K_GNU_VTINHERIT 8   
#define R_OR1K_32_PCREL      9   
#define R_OR1K_16_PCREL      10  
#define R_OR1K_8_PCREL       11  
#define R_OR1K_GOTPC_HI16    12  
#define R_OR1K_GOTPC_LO16    13  
#define R_OR1K_GOT16         14  
#define R_OR1K_PLT26         15  
#define R_OR1K_GOTOFF_HI16   16  
#define R_OR1K_GOTOFF_LO16   17  
#define R_OR1K_COPY          18  
#define R_OR1K_GLOB_DAT      19  
#define R_OR1K_JMP_SLOT      20  
#define R_OR1K_RELATIVE      21  
#define R_OR1K_TLS_GD_HI16   22  
#define R_OR1K_TLS_GD_LO16   23  
#define R_OR1K_TLS_LDM_HI16  24  
#define R_OR1K_TLS_LDM_LO16  25  
#define R_OR1K_TLS_LDO_HI16  26  
#define R_OR1K_TLS_LDO_LO16  27  
#define R_OR1K_TLS_IE_HI16   28  
#define R_OR1K_TLS_IE_LO16   29  
#define R_OR1K_TLS_LE_HI16   30  
#define R_OR1K_TLS_LE_LO16   31  
#define R_OR1K_TLS_TPOFF     32  
#define R_OR1K_TLS_DTPOFF    33  
#define R_OR1K_TLS_DTPMOD    34  


#endif
