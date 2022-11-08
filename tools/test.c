
int Function1(int x, int *y) {
        *y = x * x;
        *y *= x;
        return *y;
}

int main(int argc, char* argv[]) {
        char str[] = "abcde";
        int x = 0;
        int y = 1;
        if (x != y) {
                return 1;
        }
        return 0;
}
