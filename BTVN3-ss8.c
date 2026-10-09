#include <stdio.h>

#define MAX 100

struct UserAccount {
    int account_id;
    int plan_type;
    int monthly_fee;
    int remaining_days;
    int active_devices;
};

int main(void) {
    struct UserAccount users[MAX];
    struct UserAccount active[MAX];
    struct UserAccount free_accounts[MAX];
    struct UserAccount temp;

    int N;
    int i, j;
    int active_count = 0;
    int free_count = 0;
    int downgraded_count = 0;
    int device_limit;

    long long mrr_before = 0;
    long long mrr_after = 0;
    long long difference;

    /* 1. Nhap so luong tai khoan */
    printf("Nhap so luong tai khoan (1-%d): ", MAX);

    if (scanf("%d", &N) != 1 || N < 1 || N > MAX) {
        printf("Loi: N phai nam trong khoang 1-%d.\n", MAX);
        return 1;
    }

    /* 2. Nhap du lieu va tinh MRR ban dau */
    for (i = 0; i < N; i++) {
        printf("\n--- Tai khoan %d ---\n", i + 1);

        printf("Account ID: ");
        if (scanf("%d", &users[i].account_id) != 1) {
            printf("Loi du lieu account_id.\n");
            return 1;
        }

        printf("Plan type (0=Free, 1=Standard, 2=Premium): ");
        if (scanf("%d", &users[i].plan_type) != 1 ||
            users[i].plan_type < 0 ||
            users[i].plan_type > 2) {
            printf("Loi: plan_type phai la 0, 1 hoac 2.\n");
            return 1;
        }

        printf("Monthly fee (VND): ");
        if (scanf("%d", &users[i].monthly_fee) != 1) {
            printf("Loi du lieu monthly_fee.\n");
            return 1;
        }

        printf("Remaining days: ");
        if (scanf("%d", &users[i].remaining_days) != 1) {
            printf("Loi du lieu remaining_days.\n");
            return 1;
        }

        printf("Active devices: ");
        if (scanf("%d", &users[i].active_devices) != 1) {
            printf("Loi du lieu active_devices.\n");
            return 1;
        }

        /* MRR truoc kiem toan: dung phi dau vao */
        mrr_before += users[i].monthly_fee;
    }

    /* 3. Kiem toan va phan loai tai khoan */
    for (i = 0; i < N; i++) {

        /* Xu ly du lieu rac ve so thiet bi */
        if (users[i].active_devices <= 0) {
            users[i].active_devices = 1;
        }

        /* Quy tac het han */
        if (users[i].remaining_days <= 0) {
            if (users[i].plan_type != 0) {
                downgraded_count++;
            }

            users[i].plan_type = 0;
            users[i].monthly_fee = 0;

            /* Giu nguyen remaining_days de phuc vu doi soat */
        } else {
            /* Chuan hoa phi theo goi */
            if (users[i].plan_type == 0) {
                users[i].monthly_fee = 0;
                device_limit = 1;
            } else if (users[i].plan_type == 1) {
                users[i].monthly_fee = 120000;
                device_limit = 2;
            } else {
                users[i].monthly_fee = 300000;
                device_limit = 5;
            }

            /* Gioi han so thiet bi */
            if (users[i].active_devices > device_limit) {
                users[i].active_devices = device_limit;
            }
        }

        /* Phan loai vao hai mang phu */
        if (users[i].plan_type > 0) {
            active[active_count] = users[i];
            active_count++;
        } else {
            free_accounts[free_count] = users[i];
            free_count++;
        }
    }

    /* 4. Sap xep on dinh active[] theo phi giam dan
       Insertion sort: chi dich khi phi truoc < phi can chen.
       Neu bang phi, thu tu ban dau duoc giu nguyen. */
    for (i = 1; i < active_count; i++) {
        temp = active[i];
        j = i - 1;

        while (j >= 0 &&
               active[j].monthly_fee < temp.monthly_fee) {
            active[j + 1] = active[j];
            j--;
        }

        active[j + 1] = temp;
    }

    /* 5. Ghep lai mang chinh: Active truoc, Free sau */
    for (i = 0; i < active_count; i++) {
        users[i] = active[i];
    }

    for (i = 0; i < free_count; i++) {
        users[active_count + i] = free_accounts[i];
    }

    /* 6. Tinh MRR sau kiem toan */
    for (i = 0; i < N; i++) {
        mrr_after += users[i].monthly_fee;
    }

    difference = mrr_after - mrr_before;

    /* 7. In bang ket qua */
    printf("\n================ KET QUA KIEM TOAN ================\n");
    printf("%-10s %-8s %-12s %-12s %-10s\n",
           "AccountID", "Plan", "MonthlyFee", "DaysLeft", "Devices");

    printf("---------------------------------------------------\n");

    for (i = 0; i < N; i++) {
        const char *plan_name;

        if (users[i].plan_type == 0) {
            plan_name = "Free";
        } else if (users[i].plan_type == 1) {
            plan_name = "Standard";
        } else {
            plan_name = "Premium";
        }

        printf("%-10d %-8s %-12d %-12d %-10d\n",
               users[i].account_id,
               plan_name,
               users[i].monthly_fee,
               users[i].remaining_days,
               users[i].active_devices);
    }

    printf("===================================================\n");
    printf("So tai khoan:                 %d\n", N);
    printf("Tai khoan tra phi:            %d\n", active_count);
    printf("Tai khoan Free sau kiem toan: %d\n", free_count);
    printf("So tai khoan bi ha cap:       %d\n", downgraded_count);
    printf("MRR truoc kiem toan:          %lld VND\n", mrr_before);
    printf("MRR sau kiem toan:            %lld VND\n", mrr_after);
    printf("Chenh lech MRR (Sau - Truoc): %lld VND\n", difference);

    if (difference < 0) {
        printf("Danh gia: MRR giam %lld VND sau kiem toan.\n",
               -difference);
    } else if (difference > 0) {
        printf("Danh gia: MRR tang %lld VND sau chuan hoa.\n",
               difference);
    } else {
        printf("Danh gia: MRR khong thay doi.\n");
    }

    return 0;
}
