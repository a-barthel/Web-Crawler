
#include "SimpleHTMLParser.h"
#include "openhttp.h"
#include <string.h>
#include <stdlib.h>
char* desKey = (char*)calloc(500, sizeof(char));
int i = 0;
SimpleHTMLParser::SimpleHTMLParser()
{
}

bool
SimpleHTMLParser::match(char **b, const char *m) {
	int len = strlen(m);
	if ( !strncasecmp(*b, m, len)) {
		(*b)+=len;
		return true;
	}
	return false;
}

bool
SimpleHTMLParser::parse(char * buffer, int n)
{
	enum { START, TAG, SCRIPT, ANCHOR, HREF,
	       COMMENT, FRAME, SRC, JS, COOKIE, 
	       DEF, AT, SERIALIZE, META } state;

	state = START;
	
	char * bufferEnd = buffer + n;
	char * b = buffer;
	bool lastCharSpace = false;
	while (b < bufferEnd) {
		//printf("<%c,%d,%d>", *b, *b,state);
		switch (state) {
		case START: {
			if (match(&b,"<SCRIPT")) {
				state = SCRIPT;
			}
			else if (match(&b,"<!--") || match(&b, "/*")) {
				state = COMMENT;
			}
			else if (match(&b,"<A ")) {
				state = ANCHOR;
			}
			else if (match(&b,"<FRAME ")) {
				state = FRAME;
			}
			else if (match(&b, "<meta content=\"")) {
				state = META;
			}
			else if	(match(&b,"<")) {
				state = TAG;
			}
			else if (match(&b,"{")) {
				state = JS;
			}
			else if (match(&b,"Set-Cookie:")) {
				state = COOKIE;
			}
			else if (match(&b, "(")) {
				state = DEF;
			}
			else if (match(&b, "@")) {
				state = AT;
			}
			else if (match(&b, ".blackbar")) {
				state = START;
			}
			else if (match(&b, ".nav")) {
				state = START;
			}
			else if (match(&b, ".first")) {
				state = START;
			}
			else if (match(&b, "$_")) {
				state = SERIALIZE;
			}
			else if (match(&b, "url;")) { 
				state = START;
			}
			else if (match(&b, ".header_wrap")) {
				state = START;
			}
			else {
				char c = *b;
				//Substitute one or more blank chars with a single space
				if (c=='\n'||c=='\r'||c=='\t'||c==' ') {
					if (!lastCharSpace) {
						onContentFound(' ');
					}
					lastCharSpace = true;
				}
				else {
					onContentFound(c);
					lastCharSpace = false;
				}
				
				b++;
			}
			break;
		}
		case ANCHOR: {
			if (match(&b,"href=\"")) {
				state = HREF;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case HREF: {
			if (match(&b,"\"")) {
				// Found ending "
				state = ANCHOR;
				urlAnchor[urlAnchorLength] = '\0';
				onAnchorFound(urlAnchor);
				//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-1) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case FRAME: {
			if (match(&b,"src=\"")) {
				state = SRC;
				urlAnchorLength=0;
				//printf("href=");
			}
			else if (match(&b,">")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
				
		}
		case SRC: {
			if (match(&b,"\"")) {
				// Found ending "
				state = FRAME;
				urlAnchor[urlAnchorLength] = '\0';
				onAnchorFound(urlAnchor);
				//printf("\n");
			}
			else {
				if ( urlAnchorLength < MaxURLLength-1) {
					urlAnchor[urlAnchorLength] = *b;
					urlAnchorLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
		}
		case SCRIPT: {
			if (match(&b,"/SCRIPT>")) {
				// End script
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case COMMENT: {
			if (match(&b,"-->") || match(&b, "*/")) {
				// End comments
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case TAG: {
			if (match(&b, ">")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case JS: {
			if (match(&b, "}")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case COOKIE: {
			if (match(&b, "path=/")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case DEF: {
			if (match(&b, ")")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case AT: {
			if (match(&b, " ")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case SERIALIZE: {
			if (match(&b, " ")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		case META: {
			if (match(&b, "name=\"description\"/>")) {
				int j = 0;
				for(; j < i;j++) {
					if(desKey[j] == '\"') break;
					onContentFound(desKey[j]);
				}
				free(desKey);
				desKey = (char*)calloc(500, sizeof(char));
				i = 0;
				state = START;
			}
			else if (match(&b, "name=\"keywords\"/>")) {
				int j = 0;
				for(; j < i;j++) {
					if(desKey[j] == '\"') break;
					onContentFound(desKey[j]);
				}
				free(desKey);
				desKey = (char*)calloc(500, sizeof(char));
				i = 0;
				state = START;
			}
			else if (match(&b, "/>")) {
				free(desKey);
				desKey = (char*)calloc(500, sizeof(char));
				i = 0;
				state = START;
			}
			else {
				if (i <= 500) desKey[i++] = *b;
				b++;
			}
			break;
		}
		default:;
		}
		
	}
}

void
SimpleHTMLParser::onContentFound(char c) {
}

void
SimpleHTMLParser::onAnchorFound(char * url) {
}
