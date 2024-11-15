#include <kernel/types.h>
#include <user/user.h>

void primes(int pleft) { // pleft: 1 so int nhung thuc chat no la 1 file descriptor cho pipe
    int prime;
    
    while (1) { // Xu ly cac so pleft tu dau vao den khi k con so nao, DK dung: read(…)==0 thi exit
        if (read(pleft, &prime, sizeof(prime)) == 0) { // read: doc 1 so tu pleft luu vao prime, read==0: neu k con so nao
            close(pleft); // Dong pipe neu khong con du lieu
            exit(0); // ket thuc tien trinh va giai phong tai nguyen (ket thuc ham, cac cau lenh ngoai while cung k chay)
        }

        printf("prime %d\n", prime); // In ra so dc doc tu lenh read(…), so nay la so ng to
        int pnext[2]; // Pipe moi de chuyen het cac so con lai (xu ly de chi dua cac so k chia het cho so ng to hien tai)
        pipe(pnext); // Tao pipe moi

        if (fork() == 0) { // Tao tien trinh con xu ly cac so con lai
            close(pnext[1]); // Dong dau ghi, vi con chi can dau doc
            close(pleft); // Dong pipe cu de tranh trung tai nguyen
            pleft = pnext[0]; // Gan pleft moi de tiep tuc vong lap cho lan lap tiep theo
        } else { // Trong tien trinh cha, loai cac so k chia het cho so nguyen to hien tai
            close(pnext[0]); // Dong dau doc vi chi ghi
            int num; // De luu so dc doc
            while (read(pleft, &num, sizeof(num)) != 0) { // Doc cac so nguyen tu pipe cu pleft cho den khi doc het
                if (num % prime != 0) { // Chi ghi cac so k chia het cho so ng to hien tai vao pipe moi
                    write(pnext[1], &num, sizeof(num));
                }
            }
            
            // Dong cac pipe sau khi xu ly xong
            close(pleft);  // Dong dau doc cua pipe hien tai
            close(pnext[1]); // Dong dau ghi cua pipe moi
            wait(0); // Doi tien trinh con hoan thanh
            exit(0); // Ket thuc tien trinh cha sau khi hoan thanh
        }
    }
}

int main(int argc, char *argv[]) {
    int p[2]; // p[0] la dau doc, con p[1] la dau ghi
    pipe(p); // Tao pipe (he dieu hanh tu thiet lap p[0] doc, p[1] ghi)

    if (fork() == 0) { // fork(): tao tien trinh, fork() == 0: neu dang o tien trinh con -> Tao tien trinh con
        close(p[1]); // Dong dau ghi vi tien trinh con chi can doc du lieu tu cha
        primes(p[0]); // p[0] doc tung so 2,3,4,…280 tu cha dua vao ham primes xu ly de loai so k ng to
                    // neu debug thi no chi primes(2), prime(3), prime(5), …
    } else { // Neu dang o tien trinh cha
        close(p[0]); // Dong dau doc vi cha chi can ghi
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(i)); // Ghi cac so tu 2 den 280 vao p[1]
        }
        close(p[1]); // Dong dau ghi sau khi gui xong tat ca cac so: 2,3,4,...280
        wait(0); // Cho tien trinh con hoan tat (ket thuc con), roi giai phong tai nguyen cua con
        exit(0); // Ket thuc cha, roi giai phong tai nguyen cua cha
    }
}