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
#include <gd_wglfonct.h>

_wglContexte wglc_x, wglc_gd, wglc_gl, wglc_ps, *wglc_wgl;

f77name(wglscon)(char *contexte, F2Cl flen)
{
  char lcl_contexte[8], lcl_len_contexte;
  int len_contexte=flen;

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
  
  if (0 == strcmp(contexte, "gd"))
    {
    wglscon_gd();
    return 0;
    }
  
  /*  
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
  wglc_x.wglcolfs_fst= x_wglcolfs_fst;
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
  wglc_x.wglecr = x_wglecr;
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
  wglc_x.wglmesagr = x_wglmesagr;
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

  wglc_gd.graphics_library = 1;
  wglc_gd.wglafi = gd_wglafi;
  wglc_gd.wglali = gd_wglali;
  wglc_gd.wglbbf = gd_wglbbf;
  wglc_gd.wglbti = gd_wglbti;
  wglc_gd.wglbtn = gd_wglbtn;
  wglc_gd.wglbtpi = gd_wglbtpi;
  wglc_gd.wglcfi = gd_wglcfi;
  wglc_gd.wglchngfs = gd_wglchngfs;
  wglc_gd.wglcli = gd_wglcli;
  wglc_gd.wglclr = gd_wglclr;
  wglc_gd.wglclw = gd_wglclw;
  wglc_gd.wglcmi = gd_wglcmi;
  wglc_gd.wglcol = gd_wglcol;
  wglc_gd.wglcolf = gd_wglcolf;
  wglc_gd.wgldbf = gd_wgldbf;
  wglc_gd.wgldcm = gd_wgldcm;
  wglc_gd.wgldeflst = gd_wgldeflst;
  wglc_gd.wgldpt = gd_wgldpt;
  wglc_gd.wgldri = gd_wgldri;
  wglc_gd.wglecr = gd_wglecr;
  wglc_gd.wglfbf = gd_wglfbf;
  wglc_gd.wglfsh = gd_wglfsh;
  wglc_gd.wglfshlb = gd_wglfshlb;
  wglc_gd.wglgpl = gd_wglgpl;
  wglc_gd.wglgwz = gd_wglgwz;
  wglc_gd.wglinicmap = gd_wglinicmap;
  wglc_gd.wglinids = gd_wglinids;
  wglc_gd.wglinipat = gd_wglinipat;
  wglc_gd.wglinit = gd_wglinit;
  wglc_gd.wgliniwin = gd_wgliniwin;
  wglc_gd.wgliniwpx = gd_wgliniwpx;
  wglc_gd.wglinstcmap = gd_wglinstcmap;
  wglc_gd.wgllwi = gd_wgllwi;
  wglc_gd.wglmapc = gd_wglmapc;
  wglc_gd.wglmapcs = gd_wglmapcs;
  wglc_gd.wglmco = gd_wglmco;
  wglc_gd.wglmcos = gd_wglmcos;
  wglc_gd.wglmcw = gd_wglmcw;
  wglc_gd.wglmvi = gd_wglmvi;
  wglc_gd.wglncl = gd_wglncl;
  wglc_gd.wglnewwin = gd_wglnewwin;
  wglc_gd.wglopw = gd_wglopw;
  wglc_gd.wglpfi = gd_wglpfi;
  wglc_gd.wglpli = gd_wglpli;
  wglc_gd.wglppo = gd_wglppo;
  wglc_gd.wglpsz = gd_wglpsz;
  wglc_gd.wglpti = gd_wglpti;
  wglc_gd.wglptis = gd_wglptis;
  wglc_gd.wglrfi = gd_wglrfi;
  wglc_gd.wglrli = gd_wglrli;
  wglc_gd.wglroc = gd_wglroc;
  wglc_gd.wglrwc = gd_wglrwc;
  wglc_gd.wglsbf = gd_wglsbf;
  wglc_gd.wglsetw = gd_wglsetw;
  wglc_gd.wglsld = gd_wglsld;
  wglc_gd.wglsmk = gd_wglsmk;
  wglc_gd.wglspt = gd_wglspt;
  wglc_gd.wglswb = gd_wglswb;
  wglc_gd.wglxai = gd_wglxai;

  wglc_wgl = &wglc_gd;

}

int wglscon_gl()
{
}


int wglscon_ps()
{
}
