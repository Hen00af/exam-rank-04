#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static int create_pipe(int pipe_fd[2]);
static void cleanup_pipe(int pipe_fd[2]);
static int setup_child_read_mode(int pipe_fd[2]);
static int setup_child_write_mode(int pipe_fd[2]);
static void execute_child_process(const char *file, char *const argv[], 
                          int pipe_fd[2], char type);
static int setup_parent_read_mode(int pipe_fd[2]);
static int setup_parent_write_mode(int pipe_fd[2]);

// パイプの作成
static int create_pipe(int pipe_fd[2])
{
    if (pipe(pipe_fd) == -1)
        return (-1);
    return (0);
}

// パイプのクリーンアップ（エラー時）
static void cleanup_pipe(int pipe_fd[2])1
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
}

// 読み込みモード用の子プロセス設定（子の出力を親が読む）
static int setup_child_read_mode(int pipe_fd[2])
{
    close(pipe_fd[0]); // 子は読まない
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
        return (-1);
    close(pipe_fd[1]);
    return (0);
}

// 書き込みモード用の子プロセス設定（親の出力を子が読む）
static int setup_child_write_mode(int pipe_fd[2])
{
    close(pipe_fd[1]); // 子は書かない
    if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
        return (-1);
    close(pipe_fd[0]);
    return (0);
}

// 子プロセスでのコマンド実行
static void execute_child_process(const char *file, char *const argv[], 
                          int pipe_fd[2], char type)
{
    if (type == 'r')
    {
        if (setup_child_read_mode(pipe_fd) == -1)
            exit(EXIT_FAILURE);
    }
    else
    {
        if (setup_child_write_mode(pipe_fd) == -1)
            exit(EXIT_FAILURE);
    }
    
    execvp(file, argv);
    exit(EXIT_FAILURE);
}

// 読み込みモード用の親プロセス設定
static int setup_parent_read_mode(int pipe_fd[2])
{
    close(pipe_fd[1]);   // 親は書かない
    return (pipe_fd[0]); // 親が読む側のfdを返す
}

// 書き込みモード用の親プロセス設定
static int setup_parent_write_mode(int pipe_fd[2])
{
    close(pipe_fd[0]);   // 親は読まない
    return (pipe_fd[1]); // 親が書く側のfdを返す
}

// 親プロセスの処理
static int handle_parent_process(int pipe_fd[2], char type)
{
    if (type == 'r')
        return setup_parent_read_mode(pipe_fd);
    else
        return setup_parent_write_mode(pipe_fd);
}

// タイプの検証
static int validate_type(char type)
{
    if (type != 'r' && type != 'w')
        return (-1);
    return (0);
}

// メインのpopen実装
static int ft_popen(const char *file, char *const argv[], char type)
{
    int     pipe_fd[2];
    pid_t   pid;
    
    // タイプの検証
    if (validate_type(type) == -1)
        return (-1);
    
    // パイプの作成
    if (create_pipe(pipe_fd) == -1)
        return (-1);
    
    // フォーク
    pid = fork();
    if (pid == -1)
    {
        cleanup_pipe(pipe_fd);
        return (-1);
    }
    
    // 子プロセス
    if (pid == 0)
    {
        execute_child_process(file, argv, pipe_fd, type);
    }
    
    // 親プロセス
    return handle_parent_process(pipe_fd, type);
}

int main(void)
{
    int     fd;
    char    *line;
    
    fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
    if (fd == -1)
    {
        perror("ft_popen");
        return (EXIT_FAILURE);
    }
    
    while ((line = get_next_line(fd)))
    {
        printf("%s", line);
        free(line);
    }
    
    close(fd);
    return (EXIT_SUCCESS);
}
