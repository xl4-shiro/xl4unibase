/*
 * Excelfore Universal Base Library
 * Copyright (C) 2019 Excelfore Corporation (https://excelfore.com)
 *
 * This file is part of Excelfore-unibase.
 *
 * Excelfore-unibase is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Excelfore-unibase is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Excelfore-unibase.  If not, see
 * <https://www.gnu.org/licenses/old-licenses/gpl-2.0.html>.
 */
/*
 * ub_strutils.c
 *
 * Copyright (C) 2019 Excelfore Corporation
 * Author: Shiro Ninomiya (shiro@excelfore.com)
 */
#include "unibase_private.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void ub_hexdump(bool console, bool debug, unsigned char *buf, int size, int addr)
{
	int i;
	int p=0;
	if(!console && !debug) return;
	while(size){
		ub_console_debug_select_print(console, debug, "%08X - ", addr);
		for(i=addr&0xf;i>0;i--) ub_console_debug_select_print(console, debug, "   ");
		if((addr&0xf)>0x8) ub_console_debug_select_print(console, debug, " ");
		for(i=addr&0xf;i<0x10;i++){
			if(i==0x8) ub_console_debug_select_print(console, debug, " ");
			ub_console_debug_select_print(console, debug, "%02x ", buf[p++]);
			if(! --size) break;
		}
		addr=(addr+0x10)&~0xf;
		ub_console_debug_select_print(console, debug, "\n");
	}
}

char *ub_bmac2smac(ub_macaddr_t bmac, char *smac)
{
	sprintf(smac, UB_PRIhexB6, UB_ARRAY_B6(bmac));
	return smac;
}

uint8_t *ub_smac2bmac(const char *smac, ub_macaddr_t bmac)
{
	int i;
	char astr[18];
	for(i=0;i<18;i++){
		if(!smac[i]) break;
		astr[i]=tolower((int)smac[i]);
	}
	astr[17]=0;
	for(i=0;i<6;i++) bmac[i]=(uint8_t)strtol(astr+i*3, NULL, 16);
	return bmac;
}

char *ub_bsid2ssid(ub_streamid_t bsid, char *ssid)
{
	sprintf(ssid, UB_PRIhexB8, UB_ARRAY_B8(bsid));
	return ssid;
}

uint8_t *ub_ssid2bsid(const char *ssid, ub_streamid_t bsid)
{
	int i;
	char astr[24];
	for(i=0;i<24;i++){
		if(!ssid[i]) break;
		astr[i]=tolower((int)ssid[i]);
	}
	astr[23]=0;
	for(i=0;i<8;i++) bsid[i]=(uint8_t)strtol(astr+i*3, NULL, 16);
	return bsid;
}

static bool check_delimi(char a, uint8_t base)
{
	if(a>='0' && a<='9') return false;
	if((base==16) &&
	   ((a>='a' && a<='f') ||
	    (a>='A' && a<='F'))) return false;
	return true;
}

int ub_str2bytearray(uint8_t* dest, const char* input, uint8_t base)
{
	const char *astr;
	int i;
	unsigned long int v;
	char vstr[3];
	int len=0;
	bool gon;
	if(!dest) return 0;
	if(!input || input[0]==0) return 0;
	for(astr=input;*astr!=0;){
		gon=false;
		for(i=0;i<3;i++){
			if(check_delimi(astr[i], base) || astr[i]==0){
				if(i==0) break;
				vstr[i]=0;
				v=strtoul(vstr, NULL, base);
				if(v>=256) break;
				dest[len++]=v;
				if(astr[i]) gon=true;
				break;
			}
			vstr[i]=astr[i];
		}
		if(!gon) break;
		astr+=i+1;
	}
	return len;
}

static const char *white_spaces=" \t\n\r";
int ub_find_nospace(const char *astr, int maxn)
{
	int c;
	for(c=0;*astr!=0;c++,astr++){
		if(c==maxn) return -1;
		if(!strchr(white_spaces, *astr)) return c;
	}
	return -1;
}

int ub_find_space(const char *astr, int maxn)
{
	char inq;
	int c,esc;
	for(c=0,esc=0,inq=0;*astr!=0;c++,astr++){
		if(c==maxn) return -1;
		if(esc){
			esc=0;
			continue;
		}else if(*astr=='\\'){
			esc=1;
			continue;
		}

		if(inq){
			if(*astr!=inq) continue;
			inq=0;
			continue;
		}
		if(*astr=='"' || *astr=='\''){
			inq=*astr;
			continue;
		}
		if(strchr(white_spaces, *astr)) return c;
	}
	return -1;
}

int ub_command_line_parser(char *line, char *argv[], int maxargc)
{
	int i=0,c;
	int p=0;
	int linelen;

	if(!line) return 0;
	linelen=strlen(line);
	while(i<maxargc){
		if(p>=linelen) break;
		if(i==0) {
			argv[i++]=line;
			continue;
		}
		c=ub_find_nospace(line+p, linelen-p);
		if(c==-1) {
			// strip trailing spaces
			*(line+p)=0;
			break;
		}
		p+=c;
		argv[i++]=line+p;
		c=ub_find_space(line+p, linelen-p);
		if(c<=0) break;
		p+=c;
		*(line+p)=0;
		p+=1;
	}
	// if "...", '...', remove top and bottom of quotation mark
	for(c=0;c<i;c++){
		p=strlen(argv[c]);
		if((argv[c][0]=='"' && argv[c][p-1]=='"') ||
		   (argv[c][0]=='\'' && argv[c][p-1]=='\'')){
			argv[c][p-1]=0;
			argv[c]+=1;
			continue;
		}
	}
	return i;
}
