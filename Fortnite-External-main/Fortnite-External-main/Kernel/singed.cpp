#include <Windows.h>
#include <string>

struct CommandResult {
  int exitCode;
  std::string output;
};

CommandResult system_no_output(std::string command) {
  // Create pipes for the child process's STDOUT and STDERR.
  HANDLE stdout_read_handle, stdout_write_handle;
  HANDLE stderr_read_handle, stderr_write_handle;
  if (!CreatePipe(&stdout_read_handle, &stdout_write_handle, NULL, 0)) {
    throw std::runtime_error("Error creating pipe for STDOUT");
  }
  if (!CreatePipe(&stderr_read_handle, &stderr_write_handle, NULL, 0)) {
    throw std::runtime_error("Error creating pipe for STDERR");
  }
  ...
}

void ExecuteCommand(const std::wstring& command, HANDLE stdout_write_handle, HANDLE stderr_write_handle) {
  // Create a new process to execute the command.
  STARTUPINFOW startup_info = {0};
  startup_info.cb = sizeof(STARTUPINFO);
  startup_info.hStdError = stderr_write_handle;
  startup_info.hStdOutput = stdout_write_handle;
  startup_info.hStdInput = NULL;
  startup_info.dwFlags |= STARTF_USESTDHANDLES;

  PROCESS_INFORMATION process_info = {0};
  std::wstring cmd = L"/C " + command;
  if (!CreateProcessW(NULL, &cmd[0], NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &startup_info, &process_info)) {
    DWORD error = GetLastError();
    throw std::runtime_error("Error creating process: " + std::to_string(error));
  }

  // Wait for the process to finish.
  WaitForSingleObject(process_info.hProcess, INFINITE);

  // Close the process and thread handles.
  CloseHandle(process_info.hProcess);
  CloseHandle(process_info.hThread);
  // Close the write ends of the pipes.
  CloseHandle(stdout_write_handle);
  CloseHandle(stderr_write_handle);

  // Read the output from the child process's pipes.
  std::string stdout_output, stderr_output;
  for (;;) {
    char buffer[1024];
    DWORD bytes_read;
    if (!ReadFile(stdout_read_handle, buffer, sizeof(buffer), &bytes_read, NULL) || bytes_read == 0) {
      break;
    }
    stdout_output.append(buffer, bytes_read);
  }
  for (;;) {
    char buffer[1024];
    DWORD bytes_read;
    if (!ReadFile(stderr_read_handle, buffer, sizeof(buffer), &bytes_read, NULL) || bytes_read == 0) {
      break;
    }
    stderr_output.append(buffer, bytes_read);
  }

  // Wait for the child process to finish and retrieve its exit code.
  WaitForSingleObject(process_info.hProcess, INFINITE);
  DWORD exit_code;
  if (!GetExitCodeProcess(process_info.hProcess, &exit_code)) {
    throw std::runtime_error("Error getting exit code");
  }

  // Close handles.
  CloseHandle(stdout_read_handle);
  CloseHandle(stderr_read_handle);
  CloseHandle(process_info.hProcess);
  CloseHandle(process_info.hThread);

