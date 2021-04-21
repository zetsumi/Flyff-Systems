# Flyff et les logs !

Il est coutume et même recommandé (voir obligatoire) d'écrire des logs pendant l'exécution d'un programe.<br>
Mais comment bien les faire et les pièges éviter.<br>

## Comment Flyff log

Je vois principalement des personnes utilisé les fonctions `Error`, `WriteError`, `WriteLog` afin d'ajouter un log dans un fichier.<br>
Mais comment fonctionne ces méthodes ? <br>

Déjà nous pouvons remarquer que les méthodes `WriteError` et `WriteLog` font la même chose à l'exception de la destination.<br>
Cela se distingue par les variables `g_szERROR` et `g_szEH`.<br>
Leurs but étant de récupérer l'ensemble des paramètres représenté par `...` puis d'indiquer le type de log `LOG_DATE`.<br>
Tandis que la fonction `Error` écrie un message de type `LOG_TYPE`.<br>

***Type de LOG :***
```cpp
enum LOG_TYPE
{
	LOG_SIMPLE,		
	LOG_DATE,
};
```

***WriteLog :***
```cpp
void WriteLog( LPCTSTR lpszFormat, ... )
{
	va_list args;
	va_start(args, lpszFormat);
	FileOutV( LOG_DATE, g_szERROR, lpszFormat, args );
	va_end(args);
}
```

***WriteError :***
```cpp
void WriteError( LPCTSTR lpszFormat, ... )
{
	va_list args;
	va_start(args, lpszFormat);
	FileOutV( LOG_DATE, g_szEH, lpszFormat, args );
	va_end(args);
}
```

***Error :***
```cpp
LPCTSTR Error(LPCTSTR strFormat, ...)
{
	char szBuff[8192];
	static char szStr[8192];

	va_list args;
	va_start(args, strFormat);
	int n = _vsntprintf(szBuff, 8191, strFormat, args);
	va_end(args);

	if (n > 0)
	{
#if defined(_DEBUG)
		if (g_bShowMsgBox && MessageBox(NULL, szBuff, "...", MB_OKCANCEL) == IDCANCEL)
			g_bShowMsgBox = FALSE;
#else
		SYSTEMTIME time;
		GetLocalTime(&time);
		sprintf(szStr, "%d/%2d/%2d   %02d:%02d:%02d   %s\n",
				time.wYear, time.wMonth, time.wDay,
				time.wHour, time.wMinute, time.wSecond,
				szBuff);

		DEBUGOUT2(szStr);
#endif
	}
	else
		szStr[0] = '\0';

	return szStr;
}
```

Regardons la fonction `FileOutV`, que fait-elle ?<br>
La fonction récupère l'ensemble des paramètres `va_list args` et les écrie dans `szBuffer` en respectant
le format `lpszFormat`.<br>
```cpp
static void FileOutV(LOG_TYPE logType, LPCTSTR lpszFileName, LPCTSTR lpszFormat, va_list args)
{
    TCHAR szBuffer[4096];
    int n = _vsntprintf(szBuffer, 4095, lpszFormat, args);
    if( n > 0 )
    {
        LogFile( szBuffer, lpszFileName, logType );
    }
}
```

Enfin on finie le process par la fonction `LogFile`.<br>
Elle va ouvrir un fichier, puis écrire le contenue du log dedans et finir par fermer le fichier.<br>
Le nom du fichier est créer par la fonction `MakeFileNameDate`.<br>
```cpp
static void LogFile(LPCTSTR szString, LPCTSTR szFileName, LOG_TYPE type)
{
	FILE *fp;
	fp = fopen(MakeFileNameDate(szFileName), "a");
	if (fp == NULL)
		return;

	if (type == LOG_DATE)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);

		char szTime[32];
		sprintf(szTime,
				"%d/%2d/%2d %02d:%02d:%02d\n",
				time.wYear, time.wMonth, time.wDay,
				time.wHour, time.wMinute, time.wSecond);

		fprintf(fp, szTime);
	}

	fprintf(fp, "%s\n", szString);
	fclose(fp);
}
```

Maintenant nous connaissons la procédure qu'effectue le code pour écrie un log.<br>

## Remarque sur la méthode de log
Parlons de la fonction `LogFile`.<br>
Dans l'ensemble elle respecte la demande qui est d'écrire un message dans un fichier.<br>
Mais le problème ici est qu'elle ouvre et ferme un fichier.<br>
L'ouverture et fermeture d'un fichier créer une interuption dans le programme puisque<br>
nous demandons à l'OS d'effectuer une opération système.<br>
Si vous souhaitez écrie un grand nombre de log dans un fichier à la suite ( dans une boucle),
l'ouverture et fermeture d'un fichier successivement créer plusieurs interuptions du programme.<br>
<br>

Une solution ?<br>
1. On peut créer un fichier au debut du programme et le fermer une fois le programme finie.<br>
2. Attendre un ensemble de log avant d'écrire (attention il faut bien gérer les exception).<br>
3. Un thread consacré aux logs.<br>
<br>

Un problème de DEV !<br>
Pourquoi écrire dans un fichier quand on debug plutôt que la console ?<br>
Lors des phases de développements il est pratique d'afficher des log pendant l'exécutions.<br>
Au lieu d'éçrire dans un fichier vous pouvez utiliser la fonction `::OutputDebugString`de Windows.<br>
Cela vous permet d'écrire vos messages dans la console de VisualStudio.<br>
Par exemple :
```cpp
void OUTPUTDEBUGSTRING(LPCTSTR lpszFormat, ...)
{
#if defined(__DEBUG)
	const size_t unSize = 1024;
	char lpString[unSize] = { 0, };
	va_list	args;
	va_start(args, lpszFormat);
	int n = _vsntprintf(lpString, 1024, lpszFormat, args);
	va_end(args);
	if (n > 0)
	{
		::OutputDebugString(lpString);
		::OutputDebugString("\n");
	}
#endif //!__DEBUG
}
```
