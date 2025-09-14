/*************************************************************************
	> File Name: terminal-cat.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 14 Sep 2025 10:41:16 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h> // for usleep()

// 动画速度（微秒），数值越小越快
#define ANIMATION_SPEED 200000 // 稍慢一点，让走动更清晰
#define BLINK_SPEED      80000 // 眨眼速度可以快一些

// ANSI 转义码，用于清空屏幕
#define CLEAR_SCREEN "\033[2J\033[H"

void print_frame(const char *frame, int speed) {
    printf(CLEAR_SCREEN);
    printf("%s", frame);
    fflush(stdout);
    usleep(speed);
}

int main() {
    // 走动帧 1 (图片上半部分)
    const char *walk_frame1 =
        " (\n"
        " )\\\n"
        " ( ))\n"
        " \\ /\\           _,,,,,\n"
        "  \\(  \\       _.-'      `-._   /\\\n"
        "   \\`''`--''``         .-`  `-.({|)_`o\n"
        "   jgs ) (_))           ((   ))  `^^-^\n"
        "      (___) (__)(__)(__)(__)\n";

    // 走动帧 2 (图片下半部分)
    const char *walk_frame2 =
        " (\n"
        " )\\\n"
        " ( ))\n"
        " \\ /\\           _,,,,,\n"
        "  \\(  \\       _.-'      `-._   /\\\n"
        "   \\`''`--''``         .-`  `-.({|)_`o\n"
        "   jgs ) (_))           ((   ))  `^^-^\n"
        "      (___) (__)(__)\n"; // 注意这里少了两个(__)模拟腿部抬起

    // 眨眼帧 (基于walk_frame1修改眼睛)
    const char *blink_frame =
        " (\n"
        " )\\\n"
        " ( ))\n"
        " \\ /\\           _,,,,,\n"
        "  \\(  \\       _.-'      `-._   /\\\n"
        "   \\`''`--''``         .-`  `-.({|)_`.\n" // 将眼睛 'o' 变为 '.'
        "   jgs ) (_))           ((   ))  `^^-^\n"
        "      (___) (__)(__)(__)(__)\n";

    int blink_counter = 0; // 用于控制眨眼频率

    while (1) {
        // 显示走动帧 1
        print_frame(walk_frame1, ANIMATION_SPEED);

        // 每隔3次走动循环，插入一次眨眼
        if (blink_counter % 3 == 0) {
            print_frame(blink_frame, BLINK_SPEED);
        }
        
        // 显示走动帧 2
        print_frame(walk_frame2, ANIMATION_SPEED);

        // 再次判断是否眨眼
        if (blink_counter % 3 == 1) { // 调整眨眼时机，让它在两帧走动之间都有可能眨眼
            print_frame(blink_frame, BLINK_SPEED);
        }

        blink_counter++;
    }

    return 0;
}
