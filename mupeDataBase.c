#include <mupeDataBase.h>
#include <mupeSdCard.h>
#include <stdio.h>
#include <string.h>
#include "mupeSdCard.h"
#include <sys/stat.h>
#include "esp_log.h"

static const char *TAG = "mupeDataBase";

void mqttDataBaseStore(char *topic, char *data, size_t sl, char *paramter) {

	struct stat st = { 0 };

	char fileName[strlen(topic) + strlen(MOUNT_POINT) + 9 + strlen(paramter)];
	strcpy(fileName, MOUNT_POINT);
	strcat(fileName, "/db");
	strcat(fileName, "/");
	strcat(fileName, topic);

	char path[sizeof(fileName)];
	int ofset = 1;
	do {
		if (strchr(&fileName[ofset], '/') != NULL) {
			ofset = strchr(&fileName[ofset], '/') - fileName;
			strncpy(path, fileName, ofset);
			path[ofset] = '\0';
		} else {
			strcpy(path, fileName);
		}
		if (stat(path, &st) == -1) {
			printf("%s %i \n", path, mkdir(path, 0700));
			printf("--\n");
		}
		ofset++;
	} while (strlen(path) != strlen(fileName));
	strcat(fileName, "/");
	strcat(fileName, paramter);
	strcat(fileName, ".dat");
	FILE *f = fopen(fileName, "ab");

	ESP_LOGI(TAG, "%s %i", fileName, f==NULL);
	if (strstr(data, paramter) != NULL) {
		char *pos1 = strstr(data, paramter);
		char *pos2 = strstr(pos1, ":");
		double d = atof(&pos2[1]);
		pos1 = strstr(data, "\"ts\":");
		pos2 = strstr(pos1, ":");
		double t = atof(&pos2[1]);
		fwrite(&t, sizeof(t), 1, f);
		fwrite(&d, sizeof(d), 1, f);
		ESP_LOGI(TAG, "%s %f,%f", fileName, t, d);
	}
	fclose(f);
}

size_t mqttDataBaseGetSize(double from, double to, char *topic, char *paramter) {
	char path[strlen(MOUNT_POINT) + strlen(topic) + strlen(paramter) + 8];
	strcpy(path, MOUNT_POINT);
	strcat(path, "/db/");
	strcat(path, topic);
	strcat(path, "/");
	strcat(path, paramter);
	strcat(path, ".dat");
	FILE *fp = fopen(path, "r");
	ESP_LOGI(TAG, "---%s ", path);
	MupeDbDtata mupeDbDtata;
	fseek(fp, 0L, SEEK_END);
	size_t f_s = ftell(fp);
	f_s = f_s / 2;
	f_s = f_s - f_s % sizeof(mupeDbDtata);
	fseek(fp, f_s, SEEK_SET);
	fread(&mupeDbDtata, 1, sizeof(mupeDbDtata), fp);
	ESP_LOGI(TAG, "->--%s %f %f %f", path, mupeDbDtata.ts, mupeDbDtata.data,
			from);
	while (from != mupeDbDtata.ts) {
		if (from < mupeDbDtata.ts) {
			f_s = f_s - f_s / 2;
			f_s = f_s - f_s % sizeof(mupeDbDtata);
		} else {
			f_s = f_s + f_s / 2;
			f_s = f_s - f_s % sizeof(mupeDbDtata);
		}
		fseek(fp, f_s, SEEK_SET);
		fread(&mupeDbDtata, 1, sizeof(mupeDbDtata), fp);
		ESP_LOGI(TAG, "->-<-%s %f %f %f", path, mupeDbDtata.ts,
				mupeDbDtata.data, from);
	}

	fclose(fp);
	return 0;
}
