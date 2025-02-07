﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WSHard.h"
#include "WS.h"
#include "WSFileio.h"
#include "WSRender.h"
#include "cpu/necintrf.h"

#include "shared.h"

#include <sys/mman.h>
#include <errno.h>

static unsigned long result;
static char SaveName[512]; 
static char StateName[512];
static char IEepPath[512]; 

uint32_t WsSetPdata(void)
{
    ROMBanks = 4;
	RAMBanks = 1;
	RAMSize = 0x2000;
	CartKind = 0;
    if ((ROMMap[0xFF] = (uint8_t*)malloc(0x10000)) == NULL)
    {
		fprintf(stderr,"WsSetPdata\n");
        return 0;
    }
    WsReset();
    HVMode = 0;
    return 1;
}

uint32_t WsCreate(char *CartName)
{
    uint32_t Checksum, j;
    int32_t i;
    FILE* fp;
    char buf[16];
    int32_t fsize;

    for (i = 0; i < 256; i++)
    {
        ROMMap[i] = MemDummy;
        RAMMap[i] = MemDummy;
    }
    memset(IRAM, 0, sizeof(IRAM));
    memset(MemDummy, 0xA0, sizeof(MemDummy));
    memset(IO, 0, sizeof(IO));
    
    if (CartName == NULL)
    {
        return WsSetPdata();
    }
 
#ifdef ZIP_SUPPORT   
#endif
	fp = fopen(CartName, "rb");
	if (!fp)
	{
		fprintf(stderr,"ERR_FOPEN\n");
		return 1;
	}
    
    /* ws_romsize = sizeof(fp); */

    result = fseek(fp, -10, SEEK_END);
    if (fread(buf, 1, 10, fp) != 10)
    {
		fprintf(stderr,"ERR_FREAD_ROMINFO\n");
		fclose(fp);
        return 1;
    }

    switch (buf[4])
    {
    case 1:
        ROMBanks = 4;
        break;
    case 2:
        ROMBanks = 8;
        break;
    case 3:
        ROMBanks = 16;
        break;
    case 4:
        ROMBanks = 32;
        break;
    case 5:
        ROMBanks = 48;
        break;
    case 6:
        ROMBanks = 64;
        break;
    case 7:
        ROMBanks = 96;
        break;
    case 8:
        ROMBanks = 128;
        break;
    case 9:
        ROMBanks = 256;
        break;
    default:
        ROMBanks = 0;
        break;
    }
    if (ROMBanks == 0)
    {
		fprintf(stderr,"ERR_ILLEGAL_ROMSIZE\n");
        return 1;
    }
    switch (buf[5])
    {
    case 0x01:
        RAMBanks = 1;
        RAMSize = 0x2000;
        CartKind = 0;
        break;
    case 0x02:
        RAMBanks = 1;
        RAMSize = 0x8000;
        CartKind = 0;
        break;
    case 0x03:
        RAMBanks = 2;
        RAMSize = 0x20000;
        CartKind = 0;
        break;
    case 0x04:
        RAMBanks = 4;
        RAMSize = 0x40000;
        CartKind = 0;
        break;
    case 0x10:
        RAMBanks = 1;
        RAMSize = 0x80;
        CartKind = CK_EEP;
        break;
    case 0x20:
        RAMBanks = 1;
        RAMSize = 0x800;
        CartKind = CK_EEP;
        break;
    case 0x50:
        RAMBanks = 1;
        RAMSize = 0x400;
        CartKind = CK_EEP;
        break;
    default:
        RAMBanks = 1;
        RAMSize = 0x2000;
        CartKind = 0;
        break;
    }

    WsRomPatch(buf);

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    mmapROM = (uint8_t *)mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fileno(fp), 0);
    if (mmapROM == MAP_FAILED) {
        mmapROM = NULL;
        fprintf(stderr, "ERR_MMAP %d %s\n", fsize + (ROMBanks * -0x10000), strerror(errno));
    }

    Checksum = (uint32_t)((buf[9] << 8) + buf[8]);
    Checksum += (uint32_t)(buf[9] + buf[8]);
    for (i = ROMBanks - 1; i >= 0; i--)
    {
        if (mmapROM != NULL) {
            ROMMap[0x100 - ROMBanks + i] = &mmapROM[fsize + (ROMBanks - i) * -0x10000];
            for (j = 0; j < 0x10000; j++)
            {
                Checksum -= ROMMap[0x100 - ROMBanks + i][j];
            }
        } else {
            fseek(fp, (ROMBanks - i) * -0x10000, 2);
            if ((ROMMap[0x100 - ROMBanks + i] = (uint8_t*)malloc(0x10000)) != NULL)
            {
                if (fread(ROMMap[0x100 - ROMBanks + i], 1, 0x10000, fp) == 0x10000)
                {
                    for (j = 0; j < 0x10000; j++)
                    {
                        Checksum -= ROMMap[0x100 - ROMBanks + i][j];
                    }
                }
            }
            else
            {
                fprintf(stderr,"ERR_MALLOC\n");
                return 1;
            }
        }
    }
    fclose(fp);
    if (i >= 0)
    {
        return 0;
    }
    if (Checksum & 0xFFFF)
    {
		fprintf(stderr,"ERR_CHECKSUM\n");
    }
    if (RAMBanks)
    {
        for (i = 0; i < RAMBanks; i++)
        {
            if ((RAMMap[i] = (uint8_t*)malloc(0x10000)) != NULL)
            {
                memset(RAMMap[i], 0x00, 0x10000);
            }
            else
            {
				fprintf(stderr,"ERR_MALLOC 1\n");
				return 1;
            }
        }
    }
    if (RAMSize)
    {
		char* tmp =  strstr(CartName, "/");
		if (tmp == NULL)
		{
			snprintf(SaveName, sizeof(SaveName), "%s%s%s.epm%s", PATH_DIRECTORY, SAVE_DIRECTORY, CartName, EXTENSION);
		}
		else
		{
			snprintf(SaveName, sizeof(SaveName), "%s%s%s.epm%s", PATH_DIRECTORY, SAVE_DIRECTORY, strrchr(CartName, '/')+1, EXTENSION);
		}
        
        if ((fp = fopen(SaveName, "rb")) != NULL)
        {
            for (i = 0; i < RAMBanks; i++)
            {
                if (RAMSize < 0x10000)
                {
                    if (fread(RAMMap[i], 1, RAMSize, fp) != RAMSize)
                    {
						fprintf(stderr,"ERR_FREAD_SAVE\n");
						break;
                    }
                }
                else
                {
                    if (fread(RAMMap[i], 1, 0x10000, fp) != 0x10000)
                    {
						fprintf(stderr,"ERR_FREAD_SAVE 1\n");
                        break;
                    }
                }
            }
            fclose(fp);
        }
        else
        {
			fp = fopen(SaveName, "wb");
			if (fp) fclose(fp);
		}
    }
    else
    {
        SaveName[0] = 0;
    }
    WsReset();
	HVMode_Init = buf[6] & 1;
    HVMode = HVMode_Init;
    
	return 1;
}

void WsRelease(void)
{
    FILE* fp;
    uint32_t i;

    if (SaveName[0] != 0)
    {
        if ((fp = fopen(SaveName, "wb"))!= NULL)
        {
            for (i  = 0; i < RAMBanks; i++)
            {
                if (RAMSize<0x10000)
                {
                    if (fwrite(RAMMap[i], 1, RAMSize, fp) != RAMSize)
                    {
                        break;
                    }
                }
                else
                {
                    if (fwrite(RAMMap[i], 1, 0x10000, fp)!=0x10000)
                    {
                        break;
                    }
                }
                free(RAMMap[i]);
                RAMMap[i] = NULL;
            }
            fclose(fp);
        }
        SaveName[0] = '\0';
    }
    for (i = 0xFF; i; i--)
    {
        if (ROMMap[i] == MemDummy)
        {
            break;
        }
        if (mmapROM == NULL) free(ROMMap[i]);
        ROMMap[i] = MemDummy;
    }
    if (mmapROM != NULL) {
        munmap((void *)mmapROM, 0);
        mmapROM = NULL;
    }
    StateName[0] = '\0';
}

void WsLoadEeprom(void)
{
    FILE* fp;

	snprintf(IEepPath, sizeof(IEepPath), "%s%s%s.epm%s", PATH_DIRECTORY, SAVE_DIRECTORY, strrchr(gameName, '/')+1, EXTENSION);

    if ((fp = fopen(IEepPath, "rb")) != NULL)
    {
        result = fread(IEep, sizeof(uint16_t), 64, fp);
        fclose(fp);
    }
	else
	{
		uint16_t* p = IEep + 0x30;
		memset(IEep, 0xFF, 0x60);
		memset(p, 0, 0x20);
		*p++ = 0x211D;
		*p++ = 0x180B;
		*p++ = 0x1C0D;
		*p++ = 0x1D23;
		*p++ = 0x0B1E;
		*p   = 0x0016;
	}
}

void WsSaveEeprom(void)
{
    FILE* fp;

	snprintf(IEepPath, sizeof(IEepPath), "%s%s%s.epm%s", PATH_DIRECTORY, SAVE_DIRECTORY, strrchr(gameName, '/')+1, EXTENSION);

    if ((fp = fopen(IEepPath, "wb")) != NULL)
    {
        fwrite(IEep, sizeof(uint16_t), 64, fp);
		fclose(fp);
    }
}

#define MacroLoadNecRegisterFromFile(F,R) \
		result = fread(&value, sizeof(uint32_t), 1, fp); \
	    nec_set_reg(R,value); 
uint32_t WsLoadState(const char *savename, uint32_t num)
{
    FILE* fp;
    char buf[PATH_MAX];
	uint32_t value;
	uint32_t i;
	
	snprintf(buf, sizeof(buf), "%s%s%s.%u.sta%s", PATH_DIRECTORY, SAVE_DIRECTORY, strrchr(savename,'/')+1, num, EXTENSION);
	fp = fopen(buf, "rb");
    if (!fp)
    {
		printf("Cannot load save state\n");
		return 1;
	}

	HVMode = HVMode_Init;

	MacroLoadNecRegisterFromFile(fp,NEC_IP);
	MacroLoadNecRegisterFromFile(fp,NEC_AW);
	MacroLoadNecRegisterFromFile(fp,NEC_BW);
	MacroLoadNecRegisterFromFile(fp,NEC_CW);
	MacroLoadNecRegisterFromFile(fp,NEC_DW);
	MacroLoadNecRegisterFromFile(fp,NEC_CS);
	MacroLoadNecRegisterFromFile(fp,NEC_DS);
	MacroLoadNecRegisterFromFile(fp,NEC_ES);
	MacroLoadNecRegisterFromFile(fp,NEC_SS);
	MacroLoadNecRegisterFromFile(fp,NEC_IX);
	MacroLoadNecRegisterFromFile(fp,NEC_IY);
	MacroLoadNecRegisterFromFile(fp,NEC_BP);
	MacroLoadNecRegisterFromFile(fp,NEC_SP);
	MacroLoadNecRegisterFromFile(fp,NEC_FLAGS);
	MacroLoadNecRegisterFromFile(fp,NEC_VECTOR);
	MacroLoadNecRegisterFromFile(fp,NEC_PENDING);
	MacroLoadNecRegisterFromFile(fp,NEC_NMI_STATE);
	MacroLoadNecRegisterFromFile(fp,NEC_IRQ_STATE);
    result = fread(IRAM, sizeof(uint8_t), 0x10000, fp);
    result = fread(IO, sizeof(uint8_t), 0x100, fp);
    for (i = 0; i < RAMBanks; i++)
    {
        if (RAMSize < 0x10000)
        {
            result = fread(RAMMap[i], 1, RAMSize, fp);
        }
        else
        {
            result = fread(RAMMap[i], 1, 0x10000, fp);
        }
    }
	result = fread(Palette, sizeof(uint16_t), 16 * 16, fp);
    fclose(fp);
	WriteIO(0x1C, IO[0x1C]);
	WriteIO(0x1D, IO[0x1D]);
	WriteIO(0x1E, IO[0x1E]);
	WriteIO(0x1F, IO[0x1F]);
	WriteIO(0xC1, IO[0xC1]);
	WriteIO(0xC2, IO[0xC2]);
	WriteIO(0xC3, IO[0xC3]);
	WriteIO(0xC0, IO[0xC0]);
	for (i = 0x80; i <= 0x90; i++)
	{
		WriteIO(i, IO[i]);
	}
	
	WriteIO(0x15, IO[0x15]);
	
    return 0;
}

#define MacroStoreNecRegisterToFile(F,R) \
	    value = nec_get_reg(R); \
		fwrite(&value, sizeof(uint32_t), 1, fp);
		
uint32_t WsSaveState(const char *savename, uint32_t num)
{
    FILE* fp;
    char buf[PATH_MAX];
	uint32_t value;
	uint32_t i;
	
	snprintf(buf, sizeof(buf), "%s%s%s.%u.sta%s", PATH_DIRECTORY, SAVE_DIRECTORY, strrchr(savename,'/')+1, num, EXTENSION);
    if ((fp = fopen(buf, "w+")) == NULL)
    {
		printf("Failed to save\n");
		return 1;
	}
	MacroStoreNecRegisterToFile(fp,NEC_IP);
	MacroStoreNecRegisterToFile(fp,NEC_AW);
	MacroStoreNecRegisterToFile(fp,NEC_BW);
	MacroStoreNecRegisterToFile(fp,NEC_CW);
	MacroStoreNecRegisterToFile(fp,NEC_DW);
	MacroStoreNecRegisterToFile(fp,NEC_CS);
	MacroStoreNecRegisterToFile(fp,NEC_DS);
	MacroStoreNecRegisterToFile(fp,NEC_ES);
	MacroStoreNecRegisterToFile(fp,NEC_SS);
	MacroStoreNecRegisterToFile(fp,NEC_IX);
	MacroStoreNecRegisterToFile(fp,NEC_IY);
	MacroStoreNecRegisterToFile(fp,NEC_BP);
	MacroStoreNecRegisterToFile(fp,NEC_SP);
	MacroStoreNecRegisterToFile(fp,NEC_FLAGS);
	MacroStoreNecRegisterToFile(fp,NEC_VECTOR);
	MacroStoreNecRegisterToFile(fp,NEC_PENDING);
	MacroStoreNecRegisterToFile(fp,NEC_NMI_STATE);
	MacroStoreNecRegisterToFile(fp,NEC_IRQ_STATE);
    fwrite(IRAM, sizeof(uint8_t), 0x10000, fp);
    fwrite(IO, sizeof(uint8_t), 0x100, fp);
    for (i = 0; i < RAMBanks; i++)
    {
        if (RAMSize < 0x10000)
        {
            fwrite(RAMMap[i], 1, RAMSize, fp);
        }
        else
        {
            fwrite(RAMMap[i], 1, 0x10000, fp);
        }
    }
	fwrite(Palette, sizeof(uint16_t), 16 * 16, fp);
    fclose(fp);

    return 0;
}


