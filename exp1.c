#include <stdio.h>
#include <unistd.h>
#include <math.h>

int main() {
    int fd1[2], fd2[2];
    float a, b, c;
    float b_square, four_ac, result;

    pipe(fd1);   // Parent to Child
    pipe(fd2);   // Child to Parent

    if (fork() == 0) {
        // Child Process
        close(fd1[1]);  // Close write end of fd1
        close(fd2[0]);  // Close read end of fd2

        read(fd1[0], &a, sizeof(a));
        read(fd1[0], &c, sizeof(c));

        four_ac = 4 * a * c;

        write(fd2[1], &four_ac, sizeof(four_ac));

        close(fd1[0]);
        close(fd2[1]);
    }
    else {
        // Parent Process
        close(fd1[0]);  // Close read end of fd1
        close(fd2[1]);  // Close write end of fd2

        printf("Enter values of a, b, c: ");
        scanf("%f %f %f", &a, &b, &c);

        b_square = b * b;

        write(fd1[1], &a, sizeof(a));
        write(fd1[1], &c, sizeof(c));

        read(fd2[0], &four_ac, sizeof(four_ac));

        result = sqrt(b_square - four_ac);

        printf("Result = %f\n", result);

        close(fd1[1]);
        close(fd2[0]);
    }

    return 0;
}
