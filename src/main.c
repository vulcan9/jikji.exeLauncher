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

    WCHAR wcs[1023] = { L'\0' };
    MultiByteToWideChar(CP_ACP, 0, message, strlen(message), wcs, _countof(wcs));
    MessageBox(NULL, wcs, L"�˸� (launcher)", MB_OK);
}


//----------------------------
// ���� ���� ��ġ ã��
//----------------------------

char* findFile(char* folderPath, const char* FILE_EXE_PATH) {

    //char* fullPath = malloc(sizeof(char) * 1024);
    static char fullPath[1023] = { 0, };

    char currentFolder[1023] = { 0, };
    int isFinded = -1;

    do {
        strcpy_s(currentFolder, sizeof(currentFolder) / sizeof(char), folderPath);
        printf("���� �˻�: %s\n", currentFolder);

        // �ʱ�ȭ
        strcpy_s(fullPath, sizeof(fullPath) / sizeof(char), "");

        strcat_s(fullPath, sizeof(fullPath) / sizeof(char), currentFolder);
        strcat_s(fullPath, sizeof(fullPath) / sizeof(char), "\\");
        strcat_s(fullPath, sizeof(fullPath) / sizeof(char), FILE_EXE_PATH);
        printf("2.full Path : %s\n", fullPath);

        // ��ο� ������ �ִ��� �˻� (���� �� 0, ���� �� -1)
        int exist = _access(fullPath, 0);
        if (exist != -1) {
            printf("������ ���� : %s\n", fullPath);
            isFinded = 0;
        }
        else {
            printf("������ �������� �ʽ��ϴ�!\n\n");

            // ���� ���� �˻�
            chdir("..");
            getcwd(folderPath, BUFSIZ);

            // ���� ��ο� ���� ��� (root��)
            int isRoot = strcmp(folderPath, currentFolder);
            printf("isRoot : %d\n", isRoot);

            // ����
            if (isRoot == 0) return "";
        }
        printf("---------------------------------------\n\n");

    } while (isFinded == -1);

    return fullPath;
}

//----------------------------
// ���� ���� ����
//----------------------------

void execute(char* fullPath, char* cwd) {

    if (strcmp(fullPath, "") == 0) {
        alert("���� ������ ã�� �� �����ϴ�.");
        return;
    }

    /*
    // �޼��� �ڽ�
    char msg[1023] = { "���� ������ ���� �Ҽ� �ֽ��ϴ�." };
    strcat_s(msg, sizeof(msg) / sizeof(char), "\n���� ���� ��� : ");
    strcat_s(msg, sizeof(msg) / sizeof(char), fullPath);
    alert(msg);
    */

    char cmd[1023] = { 0, };
    //strcpy_s(cmd, sizeof(cmd) / sizeof(char), fullPath);
    strcpy_s(cmd, sizeof(cmd) / sizeof(char), "\"");
    strcat_s(cmd, sizeof(cmd) / sizeof(char), fullPath);
    strcat_s(cmd, sizeof(cmd) / sizeof(char), "\"");
    //alert(cmd);

    TCHAR commandline[1023] = { 0, };
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, cmd, strlen(cmd), commandline, _countof(commandline));
    //TCHAR commandline[] = TEXT("D:\\Project\\�ѱ� ������ �׽�Ʈ\\@_run\\@_runner.exe");

    /*
    WCHAR fullPath_w[1023] = { L'\0' };
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
    char arg[1023] = { 0, };
    strcpy_s(arg, sizeof(arg) / sizeof(char), "\"");
    strcat_s(arg, sizeof(arg) / sizeof(char), cwd);
    strcat_s(arg, sizeof(arg) / sizeof(char), "\"");
    //alert(arg);

    TCHAR param[1023] = { 0, };
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, arg, strlen(cmd), param, _countof(param));

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

////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////

// �ܼ� ���α׷�
int main(void) {

    // �ܼ� ����� (but ������)
    //HWND hWnd = GetForegroundWindow();
    //ShowWindow(hWnd, SW_HIDE);

    // ���� ���Ͽ��� ��� �б�
    //const char* FILE_EXE_PATH = "\\�ѱ� ������ �׽�Ʈ\\@_asset\\runner.exe";
    const char* FILE_EXE_PATH = "\\@_asset\\runner.exe";

    // Ž���� ������ ����
    char cwd[BUFSIZ] = { 0, };
    getcwd(cwd, BUFSIZ);
    printf("1.Current Directory : %s\n", cwd);

    // ���� ���� ��ġ ã��
    char* fullPath = findFile(cwd, FILE_EXE_PATH);

    // ���� ���� ����
    execute(fullPath, cwd);

    return 0;
}

// WIN ���α׷� (â �����)
// ���α׷� �Ӽ� > Linker > System > Subsystem : Console�� �Ǿ� ������ Window�� �ٲ������

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
    return main();
}


