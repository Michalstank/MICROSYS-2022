/*
* UART.c
*
* Created: 050522
*  Author: Nj�l-
*/

#include "UART.h"
#include "menuGraphics.h"


// Antall "tomme" rader for kvar meny.
char emptyRowsMainMenu = 14;
char emptyRowsFanStateMenu = 13;
char emptyRowsSettingsMenu = 18;
char emptyRowsStatisticsMenu = 13;
uint8_t linesize = 18;


// Hovudmenygrafikkkomponentar
char *topMainMenuLine = {"------[MAIN]------"};
char *bottomMainMenuLine = {"------------------"};

// Viftemenygrafikkkomponentar
char *topFanStateMenuLine = {"------[FANMENY]------"};
char *bottomFanStateMenuLine = {"---------------------"};

// Instillingsmenygrafikkkomponentar
char *topSettingMenuLine = {"-------[SETTINGS]-------"};
char *bottomSettingMenuLine = {"------------------------"};
char *settingMenuOption3 = {"[I2C settings       ]"};


// Statistikkmenygrafikkkomponentar
char *topStatisticsMenuLine = {"-------[OVERVIEW]-------"};
char *bottomStatisticsMenuLine = {"------------------------"};



// Protorype for alert messages
void printAlertGraphics(void) {

}


void printGraphics(char *menuSelect) {
	/*initScreen();*/

	// ----------- Hovedmeny grafikk -----------
	
	if (strcmp(menuSelect, "mainMenu") == 0) {
		
		// Printer ut topplinja, definert over
		printf("%s\n", topMainMenuLine);
		
		// Printer ut grafikk rad for rad
		for (uint8_t row = 0; row < emptyRowsMainMenu; row++) {
			

			// Printer ut linjer med menyvalg
			if (row == rowMainMenuOptionStart) {
				printf("[1] Status\n");
				printf("[2] Settings\n");
				printf("[3] Overview\n");
				printf(" \n");
				printf("Press 'Enter' to\n");
				printf("return to main\n");
				
			}
		}
		// Botnlinja av hovudmenyen
		printf("%s \n", bottomMainMenuLine);
		
		// Printer tomme linjer for syns skyld
		for (uint8_t emptyLine = 0; emptyLine < 15; emptyLine ++){
			printf(" \n");
		}
	}



	// ----------- Viftemenygrafikk -----------
	
	if (strcmp(menuSelect, "fanStateMenu") == 0) {
		
		// Printer ut topplinja, definert over
		printf("%s \n", topFanStateMenuLine);
		
		// Printer ut grafikk rad for rad
		for (uint8_t row = 0; row < emptyRowsFanStateMenu; row++) {
	
			if (row == rowRPMValuesStart) {
								printf("[Temperature    OFF ]");
								printf("\n[RPM            OFF ]");
								printf("\n[Current          0 ]\n");
			}
		}
		// Slutten av menylinjen
		printf("%s \n", bottomFanStateMenuLine);
		
		// Printer tomme linjer for syns skyld
		for (uint8_t emptyLine = 0; emptyLine < 18; emptyLine ++){
			printf(" \n");
		}
	}



	// ----------- Settings grafikk -----------
	if (strcmp(menuSelect, "settingsMenu") == 0) {
		
		// Printer ut topplinja, definert over
		printf("%s \n", topSettingMenuLine);
		
		// Grafikk rad for rad
		for (uint8_t row = 0; row < emptyRowsSettingsMenu; row++) {

			// Innholdet til menyen
			if (row == rowSettingMenuOptionStart) {
				if( controllState == 1){
					printf("[Manual control    ON  ]");
					printf("\n[Degrees           *   ]");
					linesize = 17;
				} else{
					printf("[Manual control    OFF ]");
				}
				if( lcdState == 1){
					printf("\n[I2C screen        ON  ]");
					} else{
					printf("\n[I2C screen        OFF ]");
				}
				
				printf("\n[Servo speed       %d", rpmState);
				printf("   ]\n");
			}
		}
		// Botnlinja av viftemenyen
		printf("%s \n", bottomSettingMenuLine);
		
		for (uint8_t emptyLine = 0; emptyLine < linesize; emptyLine ++){
			printf(" \n");
		}
	}
	
	
	
	// ----------- Overview grafikk -----------
	
	if (strcmp(menuSelect, "statisticsMenu") == 0) {
		
		// Topplinja av statistikkmenyen
		printf("%s \n", topStatisticsMenuLine);
		
		// Printer ut grafikk rad for rad
		for (uint8_t row = 0; row < emptyRowsSettingsMenu; row++) {

			// Innholdet til menyen
			if (row == rowStatisticsMenuStart) {
				printf("[Current RPM:       NA ]");
				printf("\n[Current Temp:      NA ]");
				printf("\n[Number of Fans:    NA ]");
				printf("\n[New input           * ]\n");
			}
		}
		
		// Botnlinja av statistikkmenyen
		printf("%s \n", bottomStatisticsMenuLine);
		for (uint8_t emptyLine = 0; emptyLine < 17; emptyLine ++){
			printf(" \n");
		}
	}
	
	interpret = 0;
}









