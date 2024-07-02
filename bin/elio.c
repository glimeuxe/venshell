#include "../base.h"
#include "../errors.h"

void parseprintref(const char* ref);

int main(int argc, char* argv[])
{
	if (argc < 2) {print_expected_argc("1+"); return 1;}

	int total_length = 0;
	for (int i = 1; i < argc; i++) total_length += strlen(argv[i]) + 1;

	char* search_content = (char *) malloc(total_length);
	if (!search_content) {print_malloc_failure(); return 1;}

	strcpy(search_content, argv[1]);
	for (int i = 2; i < argc; i++) {
		strcat(search_content, " ");
		strcat(search_content, argv[i]);
	}

	char curl_cmd[512];
	snprintf(curl_cmd, sizeof(curl_cmd), "curl -s -X POST https://libraryofbabel.app/do-search -H \"Content-Type: application/json\" -d '{\"content\": \"%s\", \"mode\": \"chars\"}'", search_content);

	// Run curl.
	FILE* curl_output = popen(curl_cmd, "r");
	if (!curl_output) {print_popen_failure(curl_cmd); return 1;}

	char response[1024];
	if (fgets(response, sizeof(response), curl_output) == NULL) {print_fgets_failure("curl output"); pclose(curl_output); return 1;}

	char* ref_start = strstr(response, "\"ref\":\"");
	if (ref_start == NULL) {print_strstr_failure("\"ref\":\"", response); return 1;}
	ref_start += strlen("\"ref\":\"");
	char* ref_end = strchr(ref_start, '\"');
	if (ref_end != NULL) {*ref_end = '\0'; parseprintref(ref_start);}
	else {print_parse_failure("reference", "response"); return 1;}

	return 0;
}

void parseprintref(const char *ref)
{
	char room[65];
	char wall[2];
	char shelf[2];
	char book[3];
	char page[4];
	if (sscanf(ref, "@%64[^.].%1[^.].%1[^.].%2[^.].%3s", room, wall, shelf, book, page) != 5) fprintf(stderr, "vensh: Failed to parse reference\n");
	room[sizeof(room) - 1] = '\0';
	wall[sizeof(wall) - 1] = '\0';
	shelf[sizeof(shelf) - 1] = '\0';
	book[sizeof(book) - 1] = '\0';
	page[sizeof(page) - 1] = '\0';
	printf("Instance found in Library of Babel:\n");
	printf("* Room #%s\n", room);
	printf("* Wall #%s\n", wall);
	printf("* Shelf #%s\n", shelf);
	printf("* Book #%s\n", book);
	printf("* Page #%s\n", page);
	printf("PDF: https://libraryofbabel.app/pdf/%s.pdf#page=%s\n", ref, page);
}