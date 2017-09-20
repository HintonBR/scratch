/****************************************************************************
*
* Filename:     sample.c
*
* Description:  
*
****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./include/httpmt.h"
#include "./include/inqcli.h"
#include "./include/matcli.h"
char szMatPort[6];
char szInqPort[6];
int	 iMatPort = 3008;
int  iInqPort = 3010;
char szServer[256] = "10.80.5.30";

/*
    Forward Declarations
*/

void TestBrowse(HWND hwnd, LPREQUEST);
void TestMatch(HWND hwnd, LPREQUEST);
int TestFIPS(void);
int Test9Digit(void);
int Test5Digit(void);
int TestCRC(void);
int TestDPBC(void);
int TestLabelInfo(void);
int TestLOT(void);
int TestGPA(void);
int TestGLLL(void);
int TestGSLL(void);
int TestGSCN(void);
int TestGLCN(void);
int TestStateCode(void);
int TestCountyName(void);
int TestSWVer(void);
int TestDBVer(void);
char* CheckReturnCode(long);
void ShowMsg(char* szMsg);

char szServer[256];

#define MAT_SUCCESS                     0

/****************************************************************************
*
****************************************************************************/
void main()
{
    short iTest;
	LPREQUEST lpReq = malloc(sizeof(REQUEST));


    /* get the server address */
    printf ("\nInput the Server Host Name or Address:");

    if (gets(szServer) < 0)
        return;


	/* get the port number */
	printf ("\nInput the Matcher Port Number:");
	
	if (gets(szMatPort) < 0)
	{
		return;
	}

    fflush(stdin);
#ifdef linux
	gets(buf);
#endif

	iMatPort = atoi(szMatPort);


		printf ("\nInput the Inquiry Port Number:");
	
	if (gets(szInqPort) < 0)
	{
		return;
	}

    fflush(stdin);
#ifdef linux
	gets(buf);
#endif
	
	iInqPort = atoi(szInqPort);


    while (1)
    {

        printf ("\n\n");
        printf (" 1) Match Address                 10) Get Long City/State/ZIP\n");
		printf (" 2) Get FIPS Code                 11) Get Short City/State/ZIP\n");
        printf (" 3) Get 9 Digit Zip Code          12) Get Long City Name\n");
        printf (" 4) Get 5 Digit Zip Code          13) Get Short City Name\n");
        printf (" 5) Get Carrier Route Code        14) Get State Code\n");
        printf (" 6) Get Delivery Point Bar Code   15) Get County Name\n");
        printf (" 7) Get Label Info                16) Get Software Version\n");
        printf (" 8) Get Line of Travel            17) Get Database Version\n");
		printf (" 9) Get Parsed Address            18) Browse Database	\n");
        printf ("\n19) Exit\n");
        printf ("\nInput Test Number: (^C to exit)\t");
        scanf ("%d", &iTest);

        switch (iTest)
        {
            case 1:
               TestMatch(NULL, lpReq);

                break;

            case 2:
                if (!TestFIPS())
                    ShowMsg ("TestFIPS Failed");

                break;

            case 3:
                if (!Test9Digit())
                    ShowMsg ("Test9Digit Failed");

                break;

            case 4:
                if (!Test5Digit())
                    ShowMsg ("Test5Digit Failed");

                break;

            case 5:
                if (!TestCRC())
                    ShowMsg ("TestCRC Failed");

                break;

            case 6:
                if (!TestDPBC())
                    ShowMsg ("TestDPBC Failed");

                break;

            case 7:
                if (!TestLabelInfo())
                    ShowMsg ("TestLabelInfo Failed");

                break;

            case 8:
                if (!TestLOT())
                    ShowMsg ("TestLOT Failed");

                break;

            case 9:
                if (!TestGPA())
                    ShowMsg ("TestGPA Failed");

                break;

            case 10:
                if (!TestGLLL())
                    ShowMsg ("TestGLLL Failed");

                break;

            case 11:
                if (!TestGSLL())
                    ShowMsg ("TestGSLL Failed");

                break;

            case 12:
                if (!TestGSCN())
                    ShowMsg ("TestGSCN Failed");

                break;

            case 13:
                if (!TestGLCN())
                    ShowMsg ("TestGLCN Failed");

                break;

            case 14:
                if (!TestStateCode())
                    ShowMsg ("TestStateCode Failed");

                break;

            case 15:
                if (!TestCountyName())
                    ShowMsg ("TestCountyName Failed");

                break;

            case 16:
                if (!TestSWVer())
                    ShowMsg ("TestSWVer Failed");

                break;

            case 17:
                if (!TestDBVer())
                    ShowMsg ("TestDBVer Failed");

                break;

			//case 18:
              //  if (!TestBrowse(NULL))
                //    ShowMsg ("TestBrowse Failed");

                //break;

            case 19:
                exit(0);

            default:
                printf ("\n'%d' is an invalid selection\n", iTest);
                scanf ("%d", &iTest);  // pause for any input before exiting 
                break;
        }
    }
    return;
}

/****************************************************************************
*
****************************************************************************/
void ShowMsg(char* szMsg)
{
#ifdef _WIN32
    MessageBox (NULL, szMsg, "User Message", MB_OK | MB_SETFOREGROUND);
#else
    printf("User Message: %s\n", szMsg);
#endif
}

/****************************************************************************
*
****************************************************************************/
void TestMatch(HWND ghwnd, LPREQUEST lpReq)
{
    long lRet;
    char szLine1[101];
    char szLine2[101];
    char szCityState[101];
    char szZip[6];

    MATCHINPUT stMatchInp;
    MATCHOUTPUT stMatchOut;

	strcpy(lpReq->Group1Message, "VERIFY");
	strcpy(lpReq->sAddress1,lpReq->sAddress1);
	strcpy(lpReq->sAddress2,lpReq->sAddress2);
	strcpy(lpReq->sAptBldg,lpReq->sAptBldg);
    strcpy(lpReq->sCity,lpReq->sCity);
	strcpy(lpReq->sState,lpReq->sState);
	strcpy(lpReq->sZipCode, lpReq->sZipCode); 
	
    /* initialize the MatchAddress input structure */
    memset (&stMatchInp, ' ', sizeof (stMatchInp));
    stMatchInp.cFunc = '5';         /* perform address match & zip code correction */
    stMatchInp.cStreetStrict = 'M'; /* Medium street name match strictness */
    stMatchInp.cFirmStrict = 'M';   /* Medium firm name match strictness */
    stMatchInp.cDirStrict = 'M';    /* Medium directional/suffix match strictness */
    stMatchInp.cNormAddr = 'N';     /* return normalized address elements */
    stMatchInp.cVanCityNam = 'X';   /* return a vanity city name if best match */
    stMatchInp.cCasing = ' ';       /* return mixed case results */
    strcpy (stMatchInp.szUrbName, "");  /* Urbanization name for Puerto Rico */
    stMatchInp.cEnviron = 'I';      /* Interactive executing environment */
	memset (szLine2, 0, 101);
	strcpy(szLine1, lpReq->sAddress1);
	strcpy(szLine2, "");
	if (strcmp(lpReq->sAddress2, " ")) {
		strcat(szLine2, "APT#");
		strcat(szLine2, lpReq->sAddress2);
	}
	if (strcmp(lpReq->sAptBldg, " ")) {
		strcat(szLine2, "  BLDG#");
		strcat(szLine2, lpReq->sAptBldg);
	}
	strcpy(szCityState, strcat(strcat(lpReq->sCity , " "), lpReq->sState));
	strcpy(szZip, lpReq->sZipCode);
    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {
		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			LogEvent(ghwnd, "ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return;
		}
		
        strcpy (stMatchInp.szPriAddr, szLine1); /* primary address */
        strcpy (stMatchInp.szSecAddr, szLine2); /*secondary address */
        strcpy (stMatchInp.szCitySt, szCityState);  /* city state info */
        strcpy (stMatchInp.szZIP, szZip);       /* 5 digit Zip code */
        strcpy (stMatchInp.szFirmName, "");     /* firm name */

		LogEvent(ghwnd, "Attempting address match");
        if ((lRet = MatchAddress (&stMatchInp, &stMatchOut, 0)) != MAT_SUCCESS)
        {
            LogEvent(ghwnd, "Error number '%c' calling the MatchAddress function", stMatchOut.cGenRC);
        }

    if (stMatchOut.cGenRC == ' ' || stMatchOut.cGenRC == 'M')
    {
        /* Match successful */
		strcpy(lpReq->sAddress1, stMatchOut.szSAStrNoApt);
		strcpy(lpReq->sCity, stMatchOut.szLongCityNam);
		strcpy(lpReq->sState, stMatchOut.szStateAbbr);
		strcpy(lpReq->sZipCode, stMatchOut.szFinalZIP);
		strcpy(lpReq->sAddressCheck, "Y");
		ReleaseMatClient();
        return;
    }
    else
    {
        LogEvent(ghwnd, "Match Failed, return code is '%c'", stMatchOut.cGenRC);
		ReleaseMatClient();
        return;
    }
   }

		 ReleaseMatClient();

    return;
}

/****************************************************************************
*
****************************************************************************/
void TestBrowse(HWND ghwnd, LPREQUEST lpReq)
{
    long hClient = 0;

    BROWSEINOUT stInqReqArea;
    long i = 0;
    long lRet;
	char testString[79 + 1];
	char test[10];
	memset(&stInqReqArea, ' ', sizeof(stInqReqArea));
	memset(testString, ' ', sizeof(testString));
	memset(test, ' ', sizeof(test));

	if ((lRet = ConnectInqClient(&hClient, szServer, iInqPort)) !=0)
	{
			LogEvent(ghwnd,"ConnectInqClient returned error %ld",lRet);
			strcpy(lpReq->Group1Message, "INVALID");
			sprintf(lpReq->sAddressCheck, "ConnectInqClient returned error %ld",lRet);
			ReleaseInqClient();
			SendGroup1Answer(ghwnd, lpReq);
			return;
	}

		//Setup parameters to pass to DBBrowse function call
        strcpy (stInqReqArea.szCommand, "SZ");
        stInqReqArea.lLineNo = 0;
        strcpy (stInqReqArea.szReqString, lpReq->sZipCode);
        stInqReqArea.lNLines = 25;   

        
		//Return error message if function call fails
        if ((lRet = DBBrowse(hClient, &stInqReqArea, 0)) != INQ_SUCCESS)
        {
            switch (lRet)
            {
                case INQ_ERR_TIMEOUT:
                    LogEvent(ghwnd,"Error - Timed out calling the server");
					strcpy(lpReq->Group1Message, "INVALID");
					strcpy(lpReq->sAddressCheck , "Error - Timed out calling the server");
                    break;

                case INQ_ERR_SERVER_SYSTEM:
                    LogEvent(ghwnd,"Error - System error on the server");
					strcpy(lpReq->Group1Message, "INVALID");
					strcpy(lpReq->sAddressCheck , "Error - System error on the server");
                    break;

                case INQ_ERR_CLIENT_NOT_REGISTERED:
                    LogEvent(ghwnd,"Error - Client is not registered");
					strcpy(lpReq->Group1Message, "INVALID");
					strcpy(lpReq->sAddressCheck , "Error - Client is not registered");
                    break;

                case INQ_ERR_CANT_CONNECT:
                    LogEvent(ghwnd,"Error - Unable to connect with the Inquiry Server");
					strcpy(lpReq->Group1Message, "INVALID");
					strcpy(lpReq->sAddressCheck , "Error - Unable to connect with the Inquiry Server");
                    break;

                case INQ_ERR_INVALID_COMMAND:
                    LogEvent(ghwnd,"Error - Invalid Command String");
					strcpy(lpReq->Group1Message, "INVALID");
					strcpy(lpReq->sAddressCheck , "Error - Invalid Command String");
                    break;

                case INQ_ERR_INVALID_LINENO:
                    LogEvent(ghwnd,"Error - Invalid Line Number");
					strcpy(lpReq->Group1Message, "INVALID");
					strcpy(lpReq->sAddressCheck , "Error - Invalid Line Number");
                    break;

                case INQ_ERR_INVALID_NLINES:
                    LogEvent(ghwnd,"Error - Invalid NLines parameter");
					strcpy(lpReq->Group1Message, "INVALID");
					strcpy(lpReq->sAddressCheck , "Error - Invalid NLines parameter");
                    break;

                case INQ_ERR_INVALID_CMD_POSITION:
                    LogEvent(ghwnd,"Error - Can't execute the command from the current database position");
					strcpy(lpReq->Group1Message, "INVALID");
					strcpy(lpReq->sAddressCheck , "Error - Can't execute the command from the current database position");
                    break;

                case INQ_ERR_INVALID_INPUT:
                    LogEvent(ghwnd,"Error - Invalid inquiry input parameter");
					strcpy(lpReq->Group1Message, "INVALID");
					strcpy(lpReq->sAddressCheck , "Error - Invalid inquiry input parameter");
                    break;

                default:
                    LogEvent(ghwnd,"Error #%ld calling the C1Inquiry function\n", lRet);
					strcpy(lpReq->Group1Message, "INVALID");
					sprintf(lpReq->sAddressCheck, "Error #%ld calling the C1Inquiry function\n", lRet);
                    break;
            }
			ReleaseInqClient();
			SendGroup1Answer(ghwnd, lpReq);
			return;

        }

		LogEvent(ghwnd,"We are getting ready to read the data");
		//Get data from the return structure
		strcpy(lpReq->sAddressCheck, stInqReqArea.szReturnCode);
		strcpy(lpReq->sAddressCheck, " ");
		strcpy(lpReq->sAddressCheck, stInqReqArea.szReturnMsg);
		strncpy(test, stInqReqArea.szLineBuff[0], 9);	
		
		//Test to see if anything valid is returned
        if (!strcmp(lpReq->sAddressCheck, "") && strcmp(test, "No street")) {
			
			LogEvent(ghwnd,"The data read was valid");
			//Loop through results and send them to the client using the Group1Answer function
			lpReq->nMethod = 0;
			while (strcmp(testString, stInqReqArea.szLineBuff[i])) {
			for (i = 0; i < stInqReqArea.lNLines; i++)
			{
				
				//Send results
				if (strcmp(stInqReqArea.szLineBuff[i], "")) {
					strcpy(lpReq->sAddressCheck, "VALID");
					memset(lpReq->szLineBuff[i], 0, sizeof(lpReq->szLineBuff[i]));
					strncpy(lpReq->szLineBuff[i], stInqReqArea.szLineBuff[i] + 9, 33);
					if (i == 0) strcpy(testString, stInqReqArea.szLineBuff[i]);
					SendGroup1Answer(ghwnd, lpReq);		
					lpReq->nMethod++;
				}
				else  {
					break;
				}
			}
				//Make function call to retrieve any additional results
				strcpy (stInqReqArea.szCommand, "DOWN");
				stInqReqArea.lLineNo = 25;
				strcpy (stInqReqArea.szReqString, "");
				stInqReqArea.lNLines = 25;
				if (DBBrowse(hClient, &stInqReqArea, 0) != INQ_SUCCESS) {
					break;
				}
				i =0;
				lpReq->nMethod = 0;

			}//end of while
			
			//Send end of data message
			strcpy(lpReq->Group1Message, "END");
			memset(lpReq->szLineBuff[i], 0, sizeof(lpReq->szLineBuff[i]));
			SendGroup1Answer(ghwnd, lpReq);
		}

		//Report an invalid return
		else {
			strcpy(lpReq->Group1Message, "INVALID");
			strcpy(lpReq->sAddressCheck, "Unknown Error");
			SendGroup1Answer(ghwnd, lpReq);
		}
    

		ReleaseInqClient();
		
    return;
}

/****************************************************************************
*
****************************************************************************/
int TestFIPS(void)
{
    long lRet;
    char szFIPSCode[4];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {
		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}
        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}
        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
        {
			ReleaseMatClient ();
            return 0;
		}
;

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetFIPSCountyCode (&stStdAd, szFIPSCode, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetFIPSCode function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nCounty FIPS Code = '%s'\n", szFIPSCode);
    }

		ReleaseMatClient();

    return 1;
}

/****************************************************************************
*
****************************************************************************/
int Test9Digit (void)
{
    long lRet;
    char szOutZIP[9+1];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {

		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */

        if ((lRet = Get9DigitZIP (&stStdAd, szOutZIP, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the Get9DigitZIP function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\n9 Digit ZIP Code = '%s'\n", szOutZIP);
    }

		 ReleaseMatClient();

    return 1;
}

/****************************************************************************
*
****************************************************************************/
int Test5Digit (void)
{
    long lRet;
    char szOutZIP[5+1];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif


    while (1)
    {
		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */

        if ((lRet = Get5DigitZIP (&stStdAd, szOutZIP, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the Get5DigitZIP function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\n5 Digit ZIP Code = '%s'\n", szOutZIP);
    }

	ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestCRC (void)
{
    long lRet;
    char szCRC[4+1];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    STDADDRESS stStdAd;

    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {
		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}


        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetCarrierRouteCode (&stStdAd, szCRC, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetCarrierRouteCode function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nCarrier Route Code = '%s'\n", szCRC);
    }

		ReleaseMatClient();

    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestDPBC (void)
{
    long lRet;
    char szDPBC[13];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {

		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetDPBC (&stStdAd, szDPBC, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetDeliveryPointBarCode function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nDelivery Point Bar Code = '%s'\n", szDPBC);
    }
		ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestLabelInfo (void)
{
    long lRet;
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    MAILLABEL stMailLbl;
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {

		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetLabelInfo (&stStdAd, &stMailLbl, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetLabelInfo function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nMailing Label = \n%s %s\n\n%s\n%s, %s %s\n\n%s%s%c\n", 
                stMailLbl.szCarrier, stMailLbl.szLOTCode, stMailLbl.szStdAddr,
                stMailLbl.szLngCtyNam, stMailLbl.szState, stMailLbl.szOutZIP,
                stMailLbl.szOutZIP, stMailLbl.szDPBC, stMailLbl.cChkDigit);
    }

		ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestLOT (void)
{
    long lRet;
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    LTOOUTPUT stLTOOut;
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {

		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetLineofTravel (&stStdAd, &stLTOOut, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetLineofTravel function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nLine of Travel = %s%c%s\n", stLTOOut.szSeqNum, stLTOOut.cSeqCode,
				stLTOOut.szAlt2SeqCode);
    }

			ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestGPA (void)
{
    long lRet;
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    PARSEDADDRESS stParsed;
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {
		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}


        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetParsedAddress (&stStdAd, &stParsed, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetParsedAddress function\n", lRet);
            ReleaseMatClient();
        }
        else
        {
            printf ("House Number           ->%s\n", stParsed.szHouseNum);
            printf ("Leading Directional    ->%s\n", stParsed.szLeadDir);
            printf ("Street Name            ->%s\n", stParsed.szStreetNam);
            printf ("Street Suffix          ->%s\n", stParsed.szStreetSfx);
            printf ("Trailing Directional   ->%s\n", stParsed.szTrailDir);
            printf ("Full Street Name       ->%s\n", stParsed.szFullStreet);
            printf ("Street Addr No Apt     ->%s\n", stParsed.szStdAddrNoApt);
            printf ("Apartment Type         ->%s\n", stParsed.szAptType);
            printf ("Apartment Number       ->%s\n", stParsed.szAptNum);
            printf ("Full Apartment Number  ->%s\n", stParsed.szFullApt);
            printf ("Rural Route Code       ->%s\n", stParsed.szRRCode);
            printf ("Rural Route Box Number ->%s\n", stParsed.szRRCode);
            printf ("Post Office Box Number ->%s\n", stParsed.szPOBoxNum);
        }
    }

			ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestGLLL (void)
{
    long lRet;
    char szLongCtyStZIP[43+1];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {
		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetLongLastLine (&stStdAd, szLongCtyStZIP, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetLongLastLine function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nStandard City/State/Zip = '%s'\n", szLongCtyStZIP);
    }

		 ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestGSLL (void)
{
    long lRet;
    char szShortCtyStZIP[28+1];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {

		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetShortLastLine (&stStdAd, szShortCtyStZIP, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetShortLastLine function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nStandard City/State/Zip = '%s'\n", szShortCtyStZIP);
    }

		ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestGSCN (void)
{
    long lRet;
    char szShortCityName[13+1];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {

		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetShortCityName (&stStdAd, szShortCityName, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetShortCityName function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nShort City Name = '%s'\n", szShortCityName);
    }

		ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestGLCN (void)
{
    long lRet;
    char szLongCityName[28+1];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    STDADDRESS stStdAd;

    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {

		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetLongCityName (&stStdAd, szLongCityName, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetLongCityName function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nLong City Name = '%s'\n", szLongCityName);
    }

		ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestStateCode (void)
{
    long lRet;
    char szStateCode[2+1];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    STDADDRESS stStdAd;


    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {

		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetStateCode (&stStdAd, szStateCode, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetStateCode function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nState Code = '%s'\n", szStateCode);
    }

		ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestCountyName (void)
{
    long lRet;
    char szCountyName[25+1];
    char szLine1[100+1];
    char szLine2[100+1];
    char szCityState[100+1];
    char szZip[5+1];
    STDADDRESS stStdAd;

    fflush(stdin);
#ifdef linux
    gets(buf);
#endif

    while (1)
    {

		if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

        /* get the first address line */
        printf ("\nInput Street Address Line 1 (blank to exit):");

        if (gets(szLine1) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        if (szLine1[0] == '\0')
		{
			ReleaseMatClient();
            break; /* exit the while loop */
		}

        /* get the second address line */
        printf ("Input Street Address Line 2:");

        if (gets(szLine2) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the city state line */
        printf ("Input City State:");

        if (gets(szCityState) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        /* get the 5 digit zip */
        printf ("Input 5 digit Zip code:");

        if (gets(szZip) <= 0)
		{
			ReleaseMatClient ();
            return 0;
		}

        memset (&stStdAd, ' ', sizeof (stStdAd));

        strcpy (stStdAd.szAddrLine1, szLine1); /* primary address line */
        strcpy (stStdAd.szAddrLine2, szLine2); /* secondary address line */
        strcpy (stStdAd.szCityState, szCityState); /* city state info */
        strcpy (stStdAd.szZIP, szZip);    /* 5 digit Zip code */

        if ((lRet = GetCountyName (&stStdAd, szCountyName, 0)) != C1IAPI_SUCCESS)
        {
            printf ("\nError number %ld calling the GetCountyName function\n", lRet);
            ReleaseMatClient();
        }
        else
            printf ("\nCounty Name = '%s'\n", szCountyName);
    }

		ReleaseMatClient();	
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestSWVer (void)
{
    long lRet;
    char szSWVer[7+1];

	if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
		{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
		}

    if ((lRet = GetSWVer (szSWVer, 0)) != C1IAPI_SUCCESS)
    {
        printf ("\nError number %ld calling the GetSWVer function\n", lRet);
		ReleaseMatClient ();
        return 0;
    }
    else
        printf ("\nSoftware version = '%s'\n", szSWVer);

		ReleaseMatClient();
    return 1;
}

/****************************************************************************
*
****************************************************************************/
int TestDBVer (void)
{
    long lRet;
    char szDBVer[20+1];

	if ((lRet = ConnectMatClient(szServer, iMatPort)) !=0)
	{
			printf("ConnectMatClient returned error %ld",lRet);
			ReleaseMatClient ();
			return 0;
	}

    if ((lRet = GetDBVer (szDBVer, 0)) != C1IAPI_SUCCESS)
    {
        printf ("\nError number %ld calling the GetDBVer function\n", lRet);
        ReleaseMatClient();
		return 0;
    }
    else
        printf ("\nDatabase version = '%s'\n", szDBVer);

		ReleaseMatClient();	
    return 1;
}

char *CheckReturnCode(long lRet) {
   
    char temp[5];
    /***************************************************
    ' Did the call to the C API function return an error?
    ' ***************************************************/
    switch (lRet) {
	case C1IAPI_SUCCESS:
            // Successful function call
            return "";
	case C1IAPI_CANT_LOAD_LIBRARY:
            return "Error - Can't load a Code-1 Plus DLL";
	case C1IAPI_CANT_UNLOAD_LIBRARY:
            return "Error - Can't unload a Code-1 Plus DLL";
	case C1IAPI_CANT_GET_ENVIR_VARIABLE:
            return "Error - Can't find required environment variable";
	case C1IAPI_CANT_SET_ENVIR_VARIABLE:
            return "Error - Can't set required environment variable";
	case C1IAPI_NO_APT_NUM:
            return "Apartment range not found and apt match required";
	case C1IAPI_NO_ADDRESS:
            return "Insufficient address information for match";
	case C1IAPI_NO_HOUSE_NUM:
            return "House/Box range not found on street";
	case C1IAPI_MULTI_MATCH:
            return "Multiple matches found";
	case C1IAPI_NO_STREET_NAME:
            return "Street name not found in Zip Code";
	case C1IAPI_DATABASE_ERROR:
            return "A problem with the CODE-1 Plus master file was detected";
	case C1IAPI_NO_ZIP_FOUND:
            return "Zip Code not found in master file";
	default:
            return strcat("Error - ", strcat(itoa(lRet,temp,10)," calling the C1PAPI dll"));
    }
    
}
