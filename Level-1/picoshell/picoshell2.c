#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

// パイプの作成
int create_pipe(int fd[2])
{
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    return (0);
}

// 子プロセスの入力リダイレクト設定
int setup_input(int prev_fd)
{
    if (prev_fd != -1)
    {
        if (dup2(prev_fd, STDIN_FILENO) == -1)
            return (-1);
        close(prev_fd);
    }
    return (0);
}

// 子プロセスの出力リダイレクト設定
int setup_output(int fd[2])
{
    close(fd[0]); // 読み込み側を閉じる
    if (dup2(fd[1], STDOUT_FILENO) == -1)
        return (-1);
    close(fd[1]);
    return (0);
}

// コマンド実行（子プロセス内）
void execute_command(char **cmd, int prev_fd, int fd[2], int has_next)
{
    if (setup_input(prev_fd) == -1)
        exit(1);
    
    if (has_next)
    {
        if (setup_output(fd) == -1)
            exit(1);
    }
    
    execvp(cmd[0], cmd);
    perror(cmd[0]);
    exit(1);
}

// 親プロセスでのファイルディスクリプタのクリーンアップ
void cleanup_parent_fds(int prev_fd, int fd[2], int has_next)
{
    if (prev_fd != -1)
        close(prev_fd);
    
    if (has_next)
        close(fd[1]); // 書き込み側を閉じる
}

// 単一コマンドの処理
int process_command(char **cmd, int prev_fd, int *next_fd, int has_next)
{
    int fd[2];
    pid_t pid;
    
    // 次のコマンドがある場合はパイプを作成
    if (has_next && create_pipe(fd) == -1)
        return (-1);
    
    // フォーク
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (-1);
    }
    
    if (pid == 0) // 子プロセス
    {
        execute_command(cmd, prev_fd, fd, has_next);
    }
    else // 親プロセス
    {
        cleanup_parent_fds(prev_fd, fd, has_next);
        if (has_next)
            *next_fd = fd[0]; // 次のコマンドの入力用
    }
    
    return (0);
}

// すべての子プロセスの終了を待つ
void wait_all_children(void)
{
    while (wait(NULL) > 0)
        ;
}

// メインのシェル処理
int picoshell(char **cmds[])
{
    int prev_fd = -1;
    int i = 0;
    
    while (cmds[i])
    {
        int has_next = (cmds[i + 1] != NULL);
        
        if (process_command(cmds[i], prev_fd, &prev_fd, has_next) == -1)
            return (1);
        
        i++;
    }
    
    wait_all_children();
    return (0);
}

int main(void)
{
    char *ls[] = {"ls", "-l", NULL};
    char *grep[] = {"grep", "c", NULL};
    char *wc[] = {"wc", "-l", NULL};
    char **cmds[] = {ls, grep, wc, NULL};

    picoshell(cmds);
    return 0;
}