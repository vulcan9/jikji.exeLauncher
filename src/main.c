#include <stdio.h> 
#include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일
#include <string.h>
#include <io.h> // _access
#include <windows.h> // MessageBox
#include <shellapi.h> // ShellExecute

int chdir(const char* dirname);
char* getcwd(char* buffer, int maxlen);

// 에러 메시지 처리
void ErrorHandling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// 메세지 박스
void alert(char* message) {
    printf("%s\n", message);

    WCHAR wcs[1023] = { L'\0' };
    MultiByteToWideChar(CP_ACP, 0, message, strlen(message), wcs, _countof(wcs));
    MessageBox(NULL, wcs, L"알림 (launcher)", MB_OK);
}


//----------------------------
// 실행 파일 위치 찾기
//----------------------------

char* findFile(char* folderPath, const char* FILE_EXE_PATH) {

    //char* fullPath = malloc(sizeof(char) * 1024);
    static char fullPath[1023] = { 0, };

    char currentFolder[1023] = { 0, };
    int isFinded = -1;

    do {
        strcpy_s(currentFolder, sizeof(currentFolder) / sizeof(char), folderPath);
        printf("파일 검색: %s\n", currentFolder);

        // 초기화
        strcpy_s(fullPath, sizeof(fullPath) / sizeof(char), "");

        strcat_s(fullPath, sizeof(fullPath) / sizeof(char), currentFolder);
        strcat_s(fullPath, sizeof(fullPath) / sizeof(char), "\\");
        strcat_s(fullPath, sizeof(fullPath) / sizeof(char), FILE_EXE_PATH);
        printf("2.full Path : %s\n", fullPath);

        // 경로에 파일이 있는지 검사 (정상 시 0, 에러 시 -1)
        int exist = _access(fullPath, 0);
        if (exist != -1) {
            printf("파일이 존재 : %s\n", fullPath);
            isFinded = 0;
        }
        else {
            printf("파일이 존재하지 않습니다!\n\n");

            // 상위 폴더 검색
            chdir("..");
            getcwd(folderPath, BUFSIZ);

            // 이전 경로와 같은 경우 (root임)
            int isRoot = strcmp(folderPath, currentFolder);
            printf("isRoot : %d\n", isRoot);

            // 종료
            if (isRoot == 0) return "";
        }
        printf("---------------------------------------\n\n");

    } while (isFinded == -1);

    return fullPath;
}

//----------------------------
// 실행 파일 실행
//----------------------------

void execute(char* fullPath, char* cwd) {

    if (strcmp(fullPath, "") == 0) {
        alert("실행 파일을 찾을 수 없습니다.");
        return;
    }

    /*
    // 메세지 박스
    char msg[1023] = { "실행 파일을 실행 할수 있습니다." };
    strcat_s(msg, sizeof(msg) / sizeof(char), "\n실행 파일 경로 : ");
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
    //TCHAR commandline[] = TEXT("D:\\Project\\한글 폴더명 테스트\\@_run\\@_runner.exe");

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
    // 사용 안함
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
    // cmd 창을 숨김
    //si.wShowWindow = SW_HIDE;

    int success = CreateProcess(NULL, commandline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    if (success == 0) {
        alert("대상 파일을 실행하지 못하였습니다.");
        return;
    }

    // child process 종료시까지 기다림
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    */
}

////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////

// 콘솔 프로그램
int main(void) {

    // 콘솔 숨기기 (but 깜빢임)
    //HWND hWnd = GetForegroundWindow();
    //ShowWindow(hWnd, SW_HIDE);

    // 로컬 파일에서 경로 읽기
    //const char* FILE_EXE_PATH = "\\한글 폴더명 테스트\\@_asset\\runner.exe";
    const char* FILE_EXE_PATH = "\\@_asset\\runner.exe";

    // 탐색을 시작할 폴더
    char cwd[BUFSIZ] = { 0, };
    getcwd(cwd, BUFSIZ);
    printf("1.Current Directory : %s\n", cwd);

    // 실행 파일 위치 찾기
    char* fullPath = findFile(cwd, FILE_EXE_PATH);

    // 실행 파일 실행
    execute(fullPath, cwd);

    return 0;
}

// WIN 프로그램 (창 숨기기)
// 프로그램 속성 > Linker > System > Subsystem : Console로 되어 있으면 Window로 바꿔줘야함

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
    return main();
}


