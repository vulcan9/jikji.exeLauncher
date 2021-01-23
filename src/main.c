#include <stdio.h> 
#include <stdlib.h>    // malloc, free �Լ��� ����� ��� ����
#include <string.h>
#include <io.h> // _access
#include <windows.h> // MessageBox
#include <shellapi.h> // ShellExecute

int chdir(const char* dirname);
char* getcwd(char* buffer, int maxlen);

// ���� �޽��� ó��
void ErrorHandling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// �޼��� �ڽ�
void alert(char* message) {
    printf("%s\n", message);

    WCHAR wcs[1024] = { L'\0' };
    //MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, tstr1, strlen(str) + 1);
    MultiByteToWideChar(CP_ACP, 0, message, strlen(message) + 1, wcs, strlen(message) + 1);
    MessageBox(NULL, wcs, L"�˸� (launcher)", MB_OK);
}


//----------------------------
// ���� ���� ��ġ ã��
//----------------------------

char* findFile(char* folderPath, const char* FILE_EXE_PATH) {

    static char fullPath[1024] = { 0, };

    char currentFolder[1024] = { 0, };
    int isFinded = -1;

    do {
        strcpy_s(currentFolder, sizeof(currentFolder) / sizeof(char), folderPath);
        printf("���� �˻�: %s\n", currentFolder);

        // �ʱ�ȭ
        strcpy_s(fullPath, sizeof(fullPath) / sizeof(char), "");

        strcat_s(fullPath, sizeof(fullPath) / sizeof(char), currentFolder);
        strcat_s(fullPath, sizeof(fullPath) / sizeof(char), "\\");
        strcat_s(fullPath, sizeof(fullPath) / sizeof(char), FILE_EXE_PATH);
        //printf("full Path : %s\n", fullPath);

        // ��ο� ������ �ִ��� �˻� (���� �� 0, ���� �� -1)
        int exist = _access(fullPath, 0);
        if (exist != -1) {
            printf("������ ���� : %s\n", fullPath);
            isFinded = 0;
        }
        else {
            //printf("������ �������� �ʽ��ϴ�!\n\n");

            // ���� ���� �˻�
            chdir("..");
            getcwd(folderPath, BUFSIZ);

            // ���� ��ο� ���� ��� (root��)
            int isRoot = strcmp(folderPath, currentFolder);
            //printf("isRoot : %d\n", isRoot);

            // ����
            if (isRoot == 0) return "";
        }
        //printf("---------------------------------------\n\n");

    } while (isFinded == -1);

    return fullPath;

    //char* fullPath = malloc(sizeof(char) * 1024);
    //char* result = _strdup(fullPath);
    //return result;
}

//----------------------------
// ���� ���� ����
//----------------------------

void execute(char* fullPath, char* cwd, char* filename) {

    if (strcmp(fullPath, "") == 0) {
        alert("���� ������ ã�� �� �����ϴ�.");
        return;
    }

    /*
    // �޼��� �ڽ�
    char msg[1024] = { "���� ������ ���� �Ҽ� �ֽ��ϴ�." };
    strcat_s(msg, sizeof(msg) / sizeof(char), "\n���� ���� ��� : ");
    strcat_s(msg, sizeof(msg) / sizeof(char), fullPath);
    alert(msg);
    */

    char cmd[1024] = { 0, };
    //strcpy_s(cmd, sizeof(cmd) / sizeof(char), fullPath);
    strcpy_s(cmd, sizeof(cmd) / sizeof(char), "\"");
    strcat_s(cmd, sizeof(cmd) / sizeof(char), fullPath);
    strcat_s(cmd, sizeof(cmd) / sizeof(char), "\"");
    //alert(cmd);

    TCHAR commandline[1024] = { 0, };
    MultiByteToWideChar(CP_ACP, 0, cmd, strlen(cmd) + 1, commandline, strlen(cmd) + 1);
    //TCHAR commandline[] = TEXT("D:\\Project\\�ѱ� ������ �׽�Ʈ\\@_run\\@_runner.exe");

    /*
    WCHAR fullPath_w[1024] = { L'\0' };
    MultiByteToWideChar(CP_ACP, 0, fullPath, strlen(fullPath), fullPath_w, _countof(fullPath_w));
    ShellExecuteW(
      HWND    hwnd,
      LPCWSTR lpOperation, // edit, explore, find, open, print, runas, NULL
      LPCWSTR lpFile,
      LPCWSTR lpParameters,
      LPCWSTR lpDirectory,
      INT     nShowCmd
    );
    */
    char arg[1024] = { 0, };
    strcpy_s(arg, sizeof(arg) / sizeof(char), "\"");
    strcat_s(arg, sizeof(arg) / sizeof(char), cwd);
    strcat_s(arg, sizeof(arg) / sizeof(char), "\" \"");
    strcat_s(arg, sizeof(arg) / sizeof(char), filename);
    strcat_s(arg, sizeof(arg) / sizeof(char), "\"");
    //alert(arg);

    TCHAR param[1024] = { 0, };
    MultiByteToWideChar(CP_ACP, 0, arg, strlen(arg) + 1, param, strlen(arg) + 1);

    ShellExecute(NULL, L"open", commandline, param, NULL, SW_HIDE);

    //---------------------------
    // ��� ����
    //---------------------------
    // CreateProcess : https://robodream.tistory.com/188

    /*
    CreateProcess(
        NULL,           // No module name (use command line)
        argv[1],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure
    )
    */

    /*
    PROCESS_INFORMATION pi;
    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);
    // cmd â�� ����
    //si.wShowWindow = SW_HIDE;

    int success = CreateProcess(NULL, commandline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    if (success == 0) {
        alert("��� ������ �������� ���Ͽ����ϴ�.");
        return;
    }

    // child process ����ñ��� ��ٸ�
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    */
}
char* fnGetFileName() {
    // https://doitnow-man.tistory.com/211
    //WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0, 0);

    TCHAR FullPath[260]; 
    TCHAR FileName[260];
    
    //���� ��θ� ���ɴϴ�. 
    GetModuleFileName(NULL, FullPath, 260);
    char fullPath[1024] = { 0, };

    //WideCharToMultiByte(CP_ACP, 0, tstr1, _tcslen(tstr1) + 1, str1, _tcslen(tstr1) * 2 + 1, 0, 0);
    WideCharToMultiByte(CP_ACP, 0, FullPath, wcslen(FullPath) + 1, fullPath, wcslen(FullPath)*2 + 1, NULL, NULL);
    printf("# FullPath : %ls %d %d\n", FullPath, _countof(FullPath), wcslen(FullPath));
    printf("# fullPath : %s %d %d\n", fullPath, _countof(fullPath), strlen(fullPath));

    //���ϸ��� ���ɴϴ�.(Ȯ���� ���� ex - filename.exe) 
    GetFileTitle(FullPath, FileName, 260);
    char filename[_MAX_FNAME] = { 0, };
    WideCharToMultiByte(CP_ACP, 0, FileName, wcslen(FileName) + 1, filename, wcslen(FileName) * 2 + 1, NULL, NULL);
    printf("# FileName : %ls %d %d\n", FileName, _countof(FileName), wcslen(FileName));
    printf("# filename : %s %d %d\n", filename, _countof(filename), strlen(filename));
    
    //char* s1 = malloc(sizeof(char) * 30);

    //Ȯ���ڸ� ������ ���ϸ� ����Դϴ�.
    static char baseName[_MAX_FNAME] = { 0, };
    _splitpath_s(fullPath, NULL, 0, NULL, 0, baseName, strlen(fullPath), NULL, 0);
    //printf("# baseName : %s\n", baseName);
    

    printf("# fullPath : %s %d %d\n", fullPath, _countof(fullPath), strlen(fullPath));
    printf("# baseName : %s %d %d\n", baseName, _countof(baseName), strlen(baseName));
    //alert(baseName);

    return baseName;

    /*
    // strtok_s �����ϸ� ���� ���ڿ��� ����ǹǷ� �����ؼ� ���
    char copyFilename[_MAX_FNAME];
    strcpy_s(copyFilename, sizeof(copyFilename) / sizeof(char), filename);

    char* context = NULL;
    char* token = strtok_s(copyFilename, ".", &context);
    if (token != NULL) strcat_s(baseName, sizeof(baseName) / sizeof(char), token);

    while (token != NULL){
        token = strtok_s(NULL, ".", &context);
        if (token != NULL && context!= NULL && strcmp(context, "") != 0) {
            strcat_s(baseName, sizeof(baseName) / sizeof(char), ".");
            strcat_s(baseName, sizeof(baseName) / sizeof(char), token);
        }
    }
    printf("baseName : %s \n", baseName);
    */

    //char* result = (char*) malloc(sizeof(char) * _MAX_FNAME);
    //if(result != NULL) strcpy_s(result, sizeof(result) / sizeof(char), baseName);
    //char* result = _strdup(baseName);
    //return result;
}

////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////

// �ܼ� ���α׷�
int main(void) {
    // â ũ��
    //system("mode con cols=80 lines=25");


    // ���� ���Ͽ��� ��� �б�
    //const char* FILE_EXE_PATH = "\\�ѱ� ������ �׽�Ʈ\\@_asset\\runner.exe";
    const char* FILE_EXE_PATH = "\\@_asset\\runner.bat";

    // Ž���� ������ ����
    char cwd[BUFSIZ] = { 0, };
    getcwd(cwd, BUFSIZ);
    printf("# ���� ��ġ : %s\n\n", cwd);

    // ���� ���� ��ġ ã��
    char* fullPath = findFile(cwd, FILE_EXE_PATH);

    // �ڽ��� ���� �̸��� �Ѱ��ش�
    char* filename = fnGetFileName();

    // ���� ���� ����
    execute(fullPath, cwd, filename);

    //free(filename);
    return 0;
}

// WIN ���α׷� (â �����)
// ���α׷� �Ӽ� > Linker > System > Subsystem : Console�� �Ǿ� ������ Window�� �ٲ������

//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR* lpszCmdParam, int nCmdShow) {
//int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    return main();
}


