#define Yes	1
#define No	0

typedef char bigtextchunk[4096];
typedef char textchunk[1024];
typedef char smalltextchunk[256];
typedef char OptionList[256];

typedef struct
{
	char productname[256];
	char pcfilename[256];
	char modelName[256];
	char shortNickName[256];
	char nickName[256];
	
	int hasZFolding;
	int hasSimplex;
	int hasDuplex;
	int hasSaddleStitch;

	char iconFileName[256];

	char mimetype[256];
	int penalty;
	char driverName[256];
} printerData;

typedef struct 
{
	char optName[256];
	char UIType[256];
	char defaultValue[256];
	const OptionList list[];
} PPDOption;

typedef struct
{
	char translatedOptionName[256];
	const OptionList translatedValuesList[];
} OptionStruct;

typedef struct OptionPair
{
	const PPDOption *theOption;
	const OptionStruct *optValues;
} OptionPair, *OptionPairList;

typedef struct
{
	char langFolderName[256];
	char languageVersion[256];
	char languageEncoding[256];
	
	char defaultPageSize[256];
	
	char noTranslationItem[256];

	// const PPDOption *theOption;
	// const OptionStruct *optValues;

} languageData;


