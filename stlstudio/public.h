#ifndef __PUBLIC_H__
#define __PUBLIC_H__

int parse_file_path(const char* fullpath, char* filename, char* filedir);

void w2c(const wchar_t * src_string, char* desc_string);
void c2w(const char *str, wchar_t *pwstr);

int DeleteDir(LPCWSTR lpszPath);

#endif  //__PUBLIC_H__