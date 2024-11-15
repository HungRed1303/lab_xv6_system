#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* findname(char *path)
{
  char *p;

  // Tìm ký tự đầu tiên sau dấu gạch chéo cuối cùng.
  // vd a/b/c => /( o giua b va c)
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++; //toi c

  return p;
}

void find(char *path, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  fd = open(path, 0);//Mở thư mục hoặc tập tin theo đường dẫn path và trả về mô tả tập tin fd.
  fstat(fd, &st);//Đọc thông tin trạng thái của fd và lưu vào st, cho phép xác định kiểu của nó (tập tin, thư mục, hoặc thiết bị).
  
  switch(st.type){
  case T_DEVICE://Neu la thiet bi thi bo qua
    break;
  case T_FILE: //Neu la file thi kiểm tra xem tên tập tin có khớp với filename không. Nếu có, in ra đường dẫn.
    if(!strcmp(findname(path),filename)){
        printf("%s\n",path);
    }
    break;
  case T_DIR:
    //Sao chép đường dẫn path vào buf. Biến buf sẽ chứa đường dẫn đầy đủ của các mục con trong thư mục này,
     //và sẽ được mở rộng thêm tên của từng mục khi duyệt qua.
    strcpy(buf, path); //root/
    // Đặt con trỏ p tại vị trí cuối của buf và thêm dấu / vào cuối đường dẫn.
    //  Điều này giúp chuẩn bị cho việc nối thêm tên của từng mục con trong thư mục vào buf.
    p = buf+strlen(buf);
    *p++ = '/';
    //Đọc một mục (entry) từ thư mục và lưu thông tin vào cấu trúc de (kiểu struct dirent).
    //Va kiem tra xem da doc het chua
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      //Neu rong thi bo qua va tiep tuc voi muc khac
      if(de.inum == 0)
        continue;
    //Bỏ qua các mục đặc biệt "." (thư mục hiện tại) và ".." (thư mục cha) để tránh lặp vô tận khi đệ quy.
      if (!strcmp(de.name, ".") || !strcmp(de.name, "..")){
        continue;
      }
      //Sao chép tên của mục hiện tại (de.name) vào buf tại vị trí p (ngay sau dấu /).
      memmove(p, de.name, DIRSIZ);
      //Đặt ký tự kết thúc chuỗi ('\0') sau tên của mục để tạo thành một chuỗi hoàn chỉnh, lưu vào buf. 
      //Lúc này, buf chứa đường dẫn đầy đủ đến mục hiện tại trong thư mục, ví dụ: "a/b/subfile".
      p[DIRSIZ] = 0;
      //De quy tiep
      find(buf, filename);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "usage: find [root] [filename]\n");
    exit(1);
  }
  
  find(argv[1], argv[2]);
  exit(0);
}