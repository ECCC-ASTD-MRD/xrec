/* RMNLIB - Library of useful routines for C and FORTRAN programming
 * Copyright (C) 1975-2001  Division de Recherche en Prevision Numerique
 *                          Environnement Canada
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation,
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* #include <wgl_gd.h> */
#include <wgl.h>
#include <x_wglfonct.h>
_wglContexte wglc_x, wglc_gd, wglc_gl, wglc_ps, *wglc_wgl;

f77name(wglscon)(char *contexte, int len_contexte)
{
  char lcl_contexte[8], lcl_len_contexte;

  lcl_len_contexte = len_contexte > 8 ? 8 : len_contexte;
  strncpy(lcl_contexte, contexte, lcl_len_contexte);
  lcl_contexte[lcl_len_contexte] = '\0';
  c_wglscon(lcl_contexte);
}

c_wglscon(char *contexte)
{
  if (0 == strcmp(contexte, "x"))
    {
    c_wglscon_x();
    return 0;
    }
  /*  
  if (0 == strcmp(contexte, "gd"))
    {
    c_wglscon_gd();
    return 0;
    }

  if (0 == strcmp(contexte, "gl"))
    {
    c_wglscon_gl();
    return 0;
    }

  if (0 == strcmp(contexte, "ps"))
    {
    c_wglscon_ps();
    return 0;
    }
  */
  fprintf(stderr, "Contexte non supporte : %s\n",contexte);

}

c_wglscon_x()
{
  wglc_x.graphics_library = 0;
  wglc_x.wglafi = x_wglafi;
  wglc_x.wglali = x_wglali;
  wglc_x.wglbbf = x_wglbbf;
  wglc_x.wglbitfinder = x_wglbitfinder;
  wglc_x.wglbrb = x_wglbrb;
  wglc_x.wglbti = x_wglbti;
  wglc_x.wglbtn = x_wglbtn;
  wglc_x.wglbtpi = x_wglbtpi;
  wglc_x.wglcfi = x_wglcfi;
  wglc_x.wglchngfs = x_wglchngfs;
  wglc_x.wglcli = x_wglcli;
  wglc_x.wglclr = x_wglclr;
  wglc_x.wglclw = x_wglclw;
  wglc_x.wglcmi = x_wglcmi;
  wglc_x.wglcol = x_wglcol;
  wglc_x.wglcolf = x_wglcolf;
  wglc_x.wglcolfs= x_wglcolfs;
  wglc_x.wglcrb = x_wglcrb;
  wglc_x.wgldbf = x_wgldbf;
  wglc_x.wgldbg = x_wgldbg;
  wglc_x.wgldcm = x_wgldcm;
  wglc_x.wgldeflst = x_wgldeflst;
  wglc_x.wgldefncw = x_wgldefncw;
  wglc_x.wgldld = x_wgldld;
  wglc_x.wgldpt = x_wgldpt;
  wglc_x.wgldri = x_wgldri;
  wglc_x.wgldsi = x_wgldsi;
  wglc_x.wglerb = x_wglerb;
  wglc_x.wglfbf = x_wglfbf;
  wglc_x.wglfsc = x_wglfsc;
  wglc_x.wglfsh = x_wglfsh;
  wglc_x.wglfshlb = x_wglfshlb;
  wglc_x.wglfsz  = x_wglfsz;
  wglc_x.wglgacw = x_wglgacw;
  wglc_x.wglgaf = x_wglgaf;
  wglc_x.wglgbf = x_wglgbf;
  wglc_x.wglgci = x_wglgci;
  wglc_x.wglgco = x_wglgco;
  wglc_x.wglgdbg = x_wglgdbg;
  wglc_x.wglgetcmap = x_wglgetcmap;
  wglc_x.wglgld = x_wglgld;
  wglc_x.wglglpti = x_wglglpti;
  wglc_x.wglglw = x_wglglw;
  wglc_x.wglgmk = x_wglgmk;
  wglc_x.wglgmod = x_wglgmod;
  wglc_x.wglgpl = x_wglgpl;
  wglc_x.wglgpt = x_wglgpt;
  wglc_x.wglgsi = x_wglgsi;
  wglc_x.wglgvi = x_wglgvi;
  wglc_x.wglgvp = x_wglgvp;
  wglc_x.wglgwz = x_wglgwz;
  wglc_x.wglgzi = x_wglgzi;
  wglc_x.wglgzp = x_wglgzp;
  wglc_x.wglias = x_wglias;
  wglc_x.wglinicmap = x_wglinicmap;
  wglc_x.wglinids = x_wglinids;
  wglc_x.wglinipat = x_wglinipat;
  wglc_x.wgliniwin = x_wgliniwin;
  wglc_x.wglinstcmap = x_wglinstcmap;
  wglc_x.wglinvpat = x_wglinvpat;
  wglc_x.wglkas = x_wglkas;
  wglc_x.wgllpt = x_wgllpt;
  wglc_x.wgllwi = x_wgllwi;
  wglc_x.wglmapc = x_wglmapc;
  wglc_x.wglmapcs = x_wglmapcs;
  wglc_x.wglmco = x_wglmco;
  wglc_x.wglmcos = x_wglmcos;
  wglc_x.wglmcw = x_wglmcw;
  wglc_x.wglmesagr = x_wglmesgagr;
  wglc_x.wglmvi = x_wglmvi;
  wglc_x.wglncl = x_wglncl;
  wglc_x.wglnewwin = x_wglnewwin;
  wglc_x.wglopmw = x_wglopmw;
  wglc_x.wglopw = &(x_wglopw);
  wglc_x.wglpfi = x_wglpfi;
  wglc_x.wglpli = x_wglpli;
  wglc_x.wglppo = x_wglppo;
  wglc_x.wglpsi = x_wglpsi;
  wglc_x.wglpsz = x_wglpsz;
  wglc_x.wglpti = x_wglpti;
  wglc_x.wglptis = x_wglptis;
  wglc_x.wglptxs = x_wglptxs;
  wglc_x.wglrfi = x_wglrfi;
  wglc_x.wglrli = x_wglrli;
  wglc_x.wglroc = x_wglroc;
  wglc_x.wglrstdpar = x_wglrstdpar;
  wglc_x.wglrwc = x_wglrwc;
  wglc_x.wglsavpfc = x_wglsavpfc;
  wglc_x.wglsbf = x_wglsbf;
  wglc_x.wglsetw = x_wglsetw;
  wglc_x.wglsetwcmap = x_wglsetwcmap;
  wglc_x.wglsld = x_wglsld;
  wglc_x.wglsmk = x_wglsmk;
  wglc_x.wglspt = x_wglspt;
  wglc_x.wglstu = x_wglstu;
  wglc_x.wglswb = x_wglswb;
  wglc_x.wglwtwrsz = x_wglwtwrsz;
  wglc_x.wglwsi = x_wglwsi;
  wglc_x.wglhsi = x_wglhsi;
  wglc_x.wglasi = x_wglasi;
  wglc_x.wgldsi = x_wgldsi;

  wglc_wgl = &wglc_x;
}

int wglscon_gd()
{
/* 
  wglc_gd.graphics_library = 1;
  wglc_gd.wglafi = wgl_gdafi;
  wglc_gd.wglali = wgl_gdali;
  wglc_gd.wglbbf = wgl_gdbbf;
  wglc_gd.wglbrb = wgl_gdbrb;
  wglc_gd.wglbti = wgl_gdbti;
  wglc_gd.wglbtn = wgl_gdbtn;
  wglc_gd.wglbtpi = wgl_gdbtpi;
  wglc_gd.wglcfi = wgl_gdcfi;
  wglc_gd.wglchngfs = wgl_gdchngfs;
  wglc_gd.wglcli = wgl_gdcli;
  wglc_gd.wglclr = wgl_gdclr;
  wglc_gd.wglclw = wgl_gdclw;
  wglc_gd.wglcmi = wgl_gdcmi;
  wglc_gd.wglcol = wgl_gdcol;
  wglc_gd.wglcolf = wgl_gdcolf;
  wglc_gd.wglcrb = wgl_gdcrb;
  wglc_gd.wgldbf = wgl_gddbf;
  wglc_gd.wgldbg = wgl_gddbg;
  wglc_gd.wgldcm = wgl_gddcm;
  wglc_gd.wgldeflst = wgl_gddeflst;
  wglc_gd.wgldefncw = wgl_gddefncw;
  wglc_gd.wgldld = wgl_gddld;
  wglc_gd.wgldpt = wgl_gddpt;
  wglc_gd.wgldri = wgl_gddri;
  wglc_gd.wglerb = wgl_gderb;
  wglc_gd.wglfbf = wgl_gdfbf;
  wglc_gd.wglfsc = wgl_gdfsc;
  wglc_gd.wglfsh = wgl_gdfsh;
  wglc_gd.wglfshlb = wgl_gdfshlb;
  wglc_gd.wglgacw = wgl_gdgacw;
  wglc_gd.wglgaf = wgl_gdgaf;
  wglc_gd.wglgbf = wgl_gdgbf;
  wglc_gd.wglgci = wgl_gdgci;
  wglc_gd.wglgco = wgl_gdgco;
  wglc_gd.wglgdbg = wgl_gdgdbg;
  wglc_gd.wglgetcmap = wgl_gdgetcmap;
  wglc_gd.wglgld = wgl_gdgld;
  wglc_gd.wglglpti = wgl_gdglpti;
  wglc_gd.wglglw = wgl_gdglw;
  wglc_gd.wglgmk = wgl_gdgmk;
  wglc_gd.wglgmod = wgl_gdgmod;
  wglc_gd.wglgpl = wgl_gdgpl;
  wglc_gd.wglgpt = wgl_gdgpt;
  wglc_gd.wglgsi = wgl_gdgsi;
  wglc_gd.wglgsp = wgl_gdgsp;
  wglc_gd.wglgvi = wgl_gdgvi;
  wglc_gd.wglgvp = wgl_gdgvp;
  wglc_gd.wglgwz = wgl_gdgwz;
  wglc_gd.wglgzi = wgl_gdgzi;
  wglc_gd.wglgzp = wgl_gdgzp;
  wglc_gd.wglias = wgl_gdias;
  wglc_gd.wglinicmap = wgl_gdinicmap;
  wglc_gd.wglinids = wgl_gdinids;
  wglc_gd.wglinipat = wgl_gdinipat;
  wglc_gd.wglinit = wgl_gdinit;
  wglc_gd.wgliniwin = wgl_gdiniwin;
  wglc_gd.wgliniwpx = wgl_gdiniwpx;
  wglc_gd.wglinstcmap = wgl_gdinstcmap;
  wglc_gd.wglinvpat = wgl_gdinvpat;
  wglc_gd.wglkas = wgl_gdkas;
  wglc_gd.wgllpt = wgl_gdlpt;
  wglc_gd.wgllwi = wgl_gdlwi;
  wglc_gd.wglmapc = wgl_gdmapc;
  wglc_gd.wglmapcs = wgl_gdmapcs;
  wglc_gd.wglmco = wgl_gdmco;
  wglc_gd.wglmcos = wgl_gdmcos;
  wglc_gd.wglmcw = wgl_gdmcw;
  wglc_gd.wglmesagr = wgl_gdmesgagr;
  wglc_gd.wglmvi = wgl_gdmvi;
  wglc_gd.wglncl = wgl_gdncl;
  wglc_gd.wglnewwin = wgl_gdnewwin;
  wglc_gd.wglopmw = wgl_gdopmw;
  wglc_gd.wglopw = wgl_gdopw;
  wglc_gd.wglpfi = wgl_gdpfi;
  wglc_gd.wglpli = wgl_gdpli;
  wglc_gd.wglppo = wgl_gdppo;
  wglc_gd.wglpsz = wgl_gdpsz;
  wglc_gd.wglpti = wgl_gdpti;
  wglc_gd.wglptis = wgl_gdptis;
  wglc_gd.wglrfi = wgl_gdrfi;
  wglc_gd.wglrli = wgl_gdrli;
  wglc_gd.wglroc = wgl_gdroc;
  wglc_gd.wglrstdpar = wgl_gdrstdpar;
  wglc_gd.wglrwc = wgl_gdrwc;
  wglc_gd.wglsavpfc = wgl_gdsavpfc;
  wglc_gd.wglsbf = wgl_gdsbf;
  wglc_gd.wglsetw = wgl_gdsetw;
  wglc_gd.wglsld = wgl_gdsld;
  wglc_gd.wglsmk = wgl_gdsmk;
  wglc_gd.wglspt = wgl_gdspt;
  wglc_gd.wglstu = wgl_gdstu;
  wglc_gd.wglswb = wgl_gdswb;
  wglc_gd.wglxai = wgl_gdxai;

  wglc_wgl = wglc_gd;
   **/
}

int wglscon_gl()
{
}


int wglscon_ps()
{
}
