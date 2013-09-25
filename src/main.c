#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "datatypes.h"

#include <sys/types.h>
#include <sys/stat.h>

#define NumPrinters 5

#include "PPDStrings.h"

const char *manufacturer = "Index Braille AB";

#ifdef DEBUG
#define CupsDriverName "IndexPrinterDriverDbg"
#else
#define CupsDriverName "IndexPrinterDriver"
#endif

// L'gg till text/plain, m-fl

const printerData printers[NumPrinters] = {									//   ZF  DP1  DP2  DP4
{ "Basic-S",	"BasicS.ppd",	"Basic-S",		"Basic-S",		"Basic-S",		 No, Yes,  No,  No,	"BasicS.icns",		"application/octet-stream", 0, CupsDriverName },
{ "Basic-D",	"BasicD.ppd",	"Basic-D",		"Basic-D",		"Basic-D",		Yes, Yes, Yes,  No,	"BasicD.icns",		"application/octet-stream", 0, CupsDriverName },
{ "4Waves PRO", "4WavPRO.ppd",	"4Waves",		"4Waves",		"4Waves PRO",	Yes, Yes, Yes,  No,	"4WavesPro.icns",	"application/octet-stream", 0, CupsDriverName },
{ "Everest",	"Everest.ppd",	"Everest",		"Everest",		"Everest",		 No, Yes, Yes,  No,	"Everest.icns",		"application/octet-stream", 0, CupsDriverName },
{ "4x4 PRO",	"4x4PRO.ppd",	"4x4",			"4x4",			"4x4 PRO",		 No, Yes, Yes, Yes,	"4x4Pro.icns",		"application/octet-stream", 0, CupsDriverName }
};

#define NumLanguages	2

const PPDOption duplexOpt = 
{ "IndexDuplex", "PickOne", "None", { "None", "DuplexNoTumble", "FourStitchBinding", "" } };

const OptionStruct En_Duplex =
	{ "Duplex", { "Off", "Left Edge Binding", "Four Stitch Binding (Saddle)", "" } };
const OptionStruct Sv_Duplex =
	{ "Dubbelsidig utskrift", { "Av", "Vänsterkant", "Sadelhäftning", "" } };


const PPDOption zfoldOpt = 
{ "ZFolding", "Boolean", "False", { "True", "False", "" } };

const OptionStruct En_ZFolding =
	{ "Z Folding", { "True", "False", "" } };
const OptionStruct Sv_ZFolding =
	{ "Z-vikning", { "Sant", "Falskt", "" } };

const PPDOption bMarginOpt = 
{ "BindingMargin", "PickOne", "Zero", { "Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "" } };

const OptionStruct En_bMargin =
	{ "Binding Margin", { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "" } };
const OptionStruct Sv_bMargin =
	{ "Vänster indrag", { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "" } };

#define NumOptions	3

const OptionPair PPDOpts[NumLanguages][NumOptions] = {
	{
		{ &duplexOpt, &En_Duplex },
		{ &zfoldOpt, &En_ZFolding },
		{ &bMarginOpt, &En_bMargin }
	},
	{
		{ &duplexOpt, &Sv_Duplex },
		{ &zfoldOpt, &Sv_ZFolding },
		{ &bMarginOpt, &Sv_bMargin }
	}
};

#define DuplexOption	0
#define ZFoldOption		1
#define BindMarginOption		2	// Not used. Calculated from page margins

const languageData languages[NumLanguages] = {
{ "en.lproj", "English", "ISOLatin1", "Letter", "No Translation"},
{ "sv.lproj", "Swedish", "ISOLatin1", "A4",	"Ingen översättning"}
};

size_t WriteFormatString(FILE *f, const char *format, ...);
size_t WriteString(FILE *f, const char *s);

#if 0
static void WriteOption(FILE *f, OptionPair pair, const char *avoidOption)
{
	char *s;
	char *s2;
	int i = 0;
	
	const PPDOption *theOption = pair.theOption;
	const OptionStruct *optValues = pair.optValues;

	WriteFormatString(f, "*%%******************************************************************************\n");
	WriteFormatString(f, "*%%	%s\n",  theOption->optName);
	WriteFormatString(f, "*%%******************************************************************************\n");
	
	WriteFormatString(f, "*OpenUI *%s/%s: %s\n", theOption->optName, optValues->translatedOptionName, theOption->UIType);
	
	WriteFormatString(f, "*Default%s: %s\n", theOption->optName, theOption->defaultValue);

	s = (char *) optValues->translatedValuesList[i];
	s2 = (char *) theOption->list[i];
	while (strlen(s) > 0)
	{
		if (!((avoidOption != NULL) && (strcmp(s2, avoidOption) == 0)))
		{
			WriteFormatString(f, "*%s %s/%s:\t\t\t\"\"\n", theOption->optName, s2, s);
		}
		i++;
		s = (char *) optValues->translatedValuesList[i];
		s2 = (char *) theOption->list[i];
	}
	
	WriteFormatString(f, "*CloseUI: *%s\n\n", pair.theOption->optName);
}
#endif

size_t WriteString(FILE *f, const char *s)
{
	return fwrite(s, strlen(s), 1, f);
}

size_t WriteFormatString(FILE *f, const char *format, ...)
{
	char buffer[1024];
	
	va_list ap;
	va_start (ap, format);
	vsprintf (buffer, format, ap);
	va_end (ap);

	return fwrite(buffer, strlen(buffer), 1, f);
}

static int MakeDir(const char *subdir)
{
	mode_t mode = S_IRWXU + S_IRGRP + S_IXGRP + S_IROTH + S_IXOTH;
	return mkdir(subdir, mode);
}

static int zipfile(const char *filename)
{
	char cmdline[1024];
	
	sprintf(cmdline, "/bin/gzip -f ");
	strcat(cmdline, filename);
	return system(cmdline);
}

char *GetCurrentTime(char *timestring, size_t bufsize)
{
	struct tm *curtm;
	time_t curtime;
	
	/* Set the printing time. */
	curtime = time(NULL);
	curtm   = localtime(&curtime);
	strftime(timestring, bufsize, "%c", curtm);
	return timestring;
}

void GenerateFiles()
{
	int i, j;
	
	for (i=0; i<NumPrinters; i++)
	{
		for (j=0; j<NumLanguages; j++)
		{
			MakeDir(languages[j].langFolderName);
			char filename[256];
			char dateString[256];
			strcpy(filename, "./");
#if 0
			strcpy(filename, "../../Output/");
#ifdef DEBUG
			strcat(filename, "Debug/");
#else
			strcat(filename, "Release/");
#endif
#endif
			strcat(filename, languages[j].langFolderName);
			strcat(filename, "/");
			strcat(filename, printers[i].pcfilename); // printers[i].productname);
			// strcat(filename, ".ppd");
			
			FILE *f = fopen(filename, "wb");
			if (f == NULL)
			{
				printf("Error %d opening file %s\n", errno, filename);
				break;
			}
			char timestring[256];
			GetCurrentTime(timestring, sizeof(timestring));
			sprintf(dateString, "PPDGenerator %s", timestring);
			WriteFormatString(f, chunks[PPDStart], dateString);
			
			WriteFormatString(f,  "*LanguageVersion:	%s\n", languages[j].languageVersion);
			WriteFormatString(f,  "*LanguageEncoding:	%s\n", languages[j].languageEncoding);

			WriteFormatString(f,  "*PCFileName:		\"%s\"\n\n", printers[i].pcfilename);
			WriteFormatString(f,  "*Product:		\"(%s)\"\n", printers[i].productname);
			WriteFormatString(f,  "*Manufacturer:		\"%s\"\n",   manufacturer);
			WriteFormatString(f,  "*ModelName:		\"%s\"\n",   printers[i].modelName);
			WriteFormatString(f,  "*ShortNickName:		\"%s\"\n",   printers[i].shortNickName);
			WriteFormatString(f,  "*NickName:		\"%s\"\n\n",  printers[i].nickName);

			WriteFormatString(f,  "*1284DeviceID:		\"MFG:%s; MDL:%s;\"\n",   manufacturer, printers[i].modelName);
			WriteFormatString(f,  "*APPrinterIconPath:	\"/Library/Printers/Index Braille/Printer Icons/%s\"\n\n", printers[i].iconFileName);

			// WriteFormatString(f,  "*APDialogExtension: 	\"/Library/Printers/Index Braille/PDEs/BrailleTranslations.plugin\"\n");

			WriteFormatString(f,  "*cupsVersion:		1.1\n");
			// WriteFormatString(f,  "*cupsFilter:		\"%s %d %s\"\n\n",  printers[i].mimetype, printers[i].penalty, printers[i].driverName);
			//WriteFormatString(f,  "*cupsFilter:		\"%s %d %s\"\n\n",  printers[i].mimetype, printers[i].penalty, printers[i].driverName);
			//WriteFormatString(f,  "*cupsFilter:		\"%s %d %s\"\n\n",  "application/postscript", printers[i].penalty, printers[i].driverName);
			WriteFormatString(f,  "*cupsFilter:		\"%s %d %s\"\n\n",  "text/plain", printers[i].penalty, "-"); //printers[i].driverName);
	
			WriteFormatString(f, chunks[PPDBasicCap]);
			WriteFormatString(f, chunks[PPDGeneral]);
			WriteFormatString(f, chunks[PPDPageSize], languages[j].defaultPageSize);
			WriteFormatString(f, chunks[PPDPageRegion], languages[j].defaultPageSize);
			WriteFormatString(f, chunks[PPDImageableArea], languages[j].defaultPageSize);
			WriteFormatString(f, chunks[PPDPaperDimension], languages[j].defaultPageSize);
			
			if (printers[i].hasZFolding)
				WriteFormatString(f, chunks[PPDConstraints]);
			
#ifdef USE_CONSTRAINTS
			if (printers[i].hasDuplex)
				WriteOption(f, PPDOpts[j][DuplexOption], printers[i].hasSaddleStitch ? NULL : "FourStitchBinding");
			if (printers[i].hasZFolding)
				WriteOption(f, PPDOpts[j][ZFoldOption], NULL);
#endif
			// WriteOption(f, PPDOpts[j][BindMarginOption], NULL);

#ifdef VERSION1
			WriteFormatString(f, chunks[PPDLanguageFile1], languages[j].noTranslationItem);
#else
			WriteFormatString(f, chunks[PPDLanguageFile1]);
#endif
			// WriteFormatString(f, chunks[PPDLanguageFile2]);
			// WriteFormatString(f, chunks[PPDLanguageFile3]);
			// WriteFormatString(f, chunks[PPDLanguageFile4]);
			fclose(f);
			
			zipfile(filename);
		}
	}
}

int main (int argc, const char * argv[])
{
	GenerateFiles();
    return 0;
}
