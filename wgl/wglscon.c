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

#include <wgl.h>
#include <wgl_gd.h>
#include <wgl_x.h>

f77name(wglscon)(char *contexte, int len_contexte)
{
  char lcl_contexte[8], lcl_len_contexte;

  lcl_len_contexte = len_contexte > 8 ? 8 : len_contexte;
  strncpy(lcl_contexte, contexte, lcl_len_contexte);
  lcl_contexte[lcl_len_contexte-1] = '\0';
  c_wglscon(lcl_contexte);
}

c_wglscon(char *contexte)
{
  if (0 == strcmp(contexte, "x"))
    {
    c_wglscon_x();
    return 0;
    }
  
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
  
  fprintf(stderr, "Contexte non supporte : %s\n",contexte);

}

c_wglscon_x()
{
  wglc_x.graphics_library = 0;
  wglc_x.wglafi = wgl_xafi;
  wglc_x.wglali = wgl_xali;
  wglc_x.wglbbf = wgl_xbbf;
  wglc_x.wglbrb = wgl_xbrb;
  wglc_x.wglbti = wgl_xbti;
  wglc_x.wglbtn = wgl_xbtn;
  wglc_x.wglbtpi = wgl_xbtpi;
  wglc_x.wglcfi = wgl_xcfi;
  wglc_x.wglchngfs = wgl_xchngfs;
  wglc_x.wglcli = wgl_xcli;
  wglc_x.wglclr = wgl_xclr;
  wglc_x.wglclw = wgl_xclw;
  wglc_x.wglcmi = wgl_xcmi;
  wglc_x.wglcol = wgl_xcol;
  wglc_x.wglcolf = wgl_xcolf;
  wglc_x.wglcrb = wgl_xcrb;
  wglc_x.wgldbf = wgl_xdbf;
  wglc_x.wgldbg = wgl_xdbg;
  wglc_x.wgldcm = wgl_xdcm;
  wglc_x.wgldeflst = wgl_xdeflst;
  wglc_x.wgldefncw = wgl_xdefncw;
  wglc_x.wgldld = wgl_xdld;
  wglc_x.wgldpt = wgl_xdpt;
  wglc_x.wgldri = wgl_xdri;
  wglc_x.wglerb = wgl_xerb;
  wglc_x.wglfbf = wgl_xfbf;
  wglc_x.wglfsc = wgl_xfsc;
  wglc_x.wglfsh = wgl_xfsh;
  wglc_x.wglfshlb = wgl_xfshlb;
  wglc_x.wglgacw = wgl_xgacw;
  wglc_x.wglgaf = wgl_xgaf;
  wglc_x.wglgbf = wgl_xgbf;
  wglc_x.wglgci = wgl_xgci;
  wglc_x.wglgco = wgl_xgco;
  wglc_x.wglgdbg = wgl_xgdbg;
  wglc_x.wglgetcmap = wgl_xgetcmap;
  wglc_x.wglgld = wgl_xgld;
  wglc_x.wglglpti = wgl_xglpti;
  wglc_x.wglglw = wgl_xglw;
  wglc_x.wglgmk = wgl_xgmk;
  wglc_x.wglgmod = wgl_xgmod;
  wglc_x.wglgpl = wgl_xgpl;
  wglc_x.wglgpt = wgl_xgpt;
  wglc_x.wglgsi = wgl_xgsi;
  wglc_x.wglgsp = wgl_xgsp;
  wglc_x.wglgvi = wgl_xgvi;
  wglc_x.wglgvp = wgl_xgvp;
  wglc_x.wglgwz = wgl_xgwz;
  wglc_x.wglgzi = wgl_xgzi;
  wglc_x.wglgzp = wgl_xgzp;
  wglc_x.wglias = wgl_xias;
  wglc_x.wgliax = wgl_xiax;
  wglc_x.wglinicmap = wgl_xinicmap;
  wglc_x.wglinids = wgl_xinids;
  wglc_x.wglinipat = wgl_xinipat;
  wglc_x.wglinit = wgl_xinit;
  wglc_x.wgliniwin = wgl_xiniwin;
  wglc_x.wgliniwpx = wgl_xiniwpx;
  wglc_x.wglinstcmap = wgl_xinstcmap;
  wglc_x.wglinvpat = wgl_xinvpat;
  wglc_x.wglkas = wgl_xkas;
  wglc_x.wgllpt = wgl_xlpt;
  wglc_x.wgllwi = wgl_xlwi;
  wglc_x.wglmapc = wgl_xmapc;
  wglc_x.wglmapcs = wgl_xmapcs;
  wglc_x.wglmco = wgl_xmco;
  wglc_x.wglmcos = wgl_xmcos;
  wglc_x.wglmcw = wgl_xmcw;
  wglc_x.wglmesagr = wgl_xmesgagr;
  wglc_x.wglmvi = wgl_xmvi;
  wglc_x.wglncl = wgl_xncl;
  wglc_x.wglnewwin = wgl_xnewwin;
  wglc_x.wglopmw = wgl_xopmw;
  wglc_x.wglopw = wgl_xopw;
  wglc_x.wglpfi = wgl_xpfi;
  wglc_x.wglpli = wgl_xpli;
  wglc_x.wglppo = wgl_xppo;
  wglc_x.wglpsz = wgl_xpsz;
  wglc_x.wglpti = wgl_xpti;
  wglc_x.wglptis = wgl_xptis;
  wglc_x.wglrfi = wgl_xrfi;
  wglc_x.wglrli = wgl_xrli;
  wglc_x.wglroc = wgl_xroc;
  wglc_x.wglrstdpar = wgl_xrstdpar;
  wglc_x.wglrwc = wgl_xrwc;
  wglc_x.wglsavpfc = wgl_xsavpfc;
  wglc_x.wglsbf = wgl_xsbf;
  wglc_x.wglsetw = wgl_xsetw;
  wglc_x.wglsld = wgl_xsld;
  wglc_x.wglsmk = wgl_xsmk;
  wglc_x.wglspt = wgl_xspt;
  wglc_x.wglssp = wgl_xssp;
  wglc_x.wglstu = wgl_xstu;
  wglc_x.wglswb = wgl_xswb;
  wglc_x.wglxai = wgl_xxai;

  wglc_wgl = wglc_x;
}

int wglscon_gd()
{
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
  wglc_gd.wgliax = wgl_gdiax;
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
  wglc_gd.wglssp = wgl_gdssp;
  wglc_gd.wglstu = wgl_gdstu;
  wglc_gd.wglswb = wgl_gdswb;
  wglc_gd.wglxai = wgl_gdxai;

  wglc_wgl = wglc_gd;
}

int wglscon_gl()
{
}


int wglscon_ps()
{
}
