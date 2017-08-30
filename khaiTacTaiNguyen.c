/* Khai Tác Tài Nguyên */
/* Để kha tác 'tài nguyên' từ tệp Mac Cổ Xưa */
/* khởi động 2560.08.25 */


#include <stdio.h>
#include <stdlib.h>


typedef struct {
   char ten[32];        // tên tà nguyên (nếu có) dài nhất 31 ký tự
   short maSo;          // mã sô tài nguyên 
   unsigned int beDai;  // bề dài dữ liệu tài nguyên
   unsigned int dichDuLieu;      // dịch dữ liệu từ đầu dữ liệu tài nguyêm
   unsigned int diaChiDuLieu;    // địa chỉ dữ liệu từ đầu tệp
   short dichTen;       // dịch từ đầu danh sách tài nguyên
   unsigned char dacTinh; // đặc tíng tài nguyên
   unsigned char *duLieu;  // dữ liệu tài nguyên

} TaiNguyen;


typedef struct {
   unsigned int loai;        // loại tài nguyên, 4 ký tự như XXXX
   short dichDanhSachTaiNguyen;
   unsigned short soLuongTaiNguyen;
   TaiNguyen *mangTaiNguyen;
} OBangLoaiTaiNguyen;

typedef struct {
   unsigned int diaChiDuLieu;  // điạ chỉ đầu dữ liệu tài nguyên từ đầu tệp
   unsigned int diaChiBanDo;   // địa chỉ đầu bản đồ tài nguyên từ đầu tệp
   unsigned int beDaiDuLieu;   // bề dài dữ liệu tài nguyên
   unsigned int beDaiBanDo;    // bề dài dữ liệu bản đồ tài nguyên
   
   unsigned short dichDanhSachTaiNguyen;    // dịch đến danh sách tài nguyên
   unsigned short dichDanhSachTenTaiNguyen;  // dịch đến danh sách tên tài nguyên
   unsigned short dacTinh;      // đặc tính cho bản đồ tài nguyên
   unsigned short soLuongTaiNguyen;  // số lượng loại tài nguyên khác nhau
   OBangLoaiTaiNguyen *mangOBangLoaiTaiNguyen;  // mảng ô bảng loại tài nguyên
} BangTaiNguyen;




void docThamSo( int argc, char **argv, char *tenTep );
BangTaiNguyen docBangTaiNguyen( char *tenTep );
void chieuThonTinBangTaiNguyen( BangTaiNguyen *bangTaiNguyen );


int main( int argc, char **argv ) {

   char tenTep[256];
   
   // ==== đọc tham số
   docThamSo( argc, argv, tenTep );

   // ==== đọc bảng tài nguyên
   BangTaiNguyen bangTaiNguyen = docBangTaiNguyen( tenTep );
   
   // ---- chiếu thôn tin và tài nguyên
   chieuThonTinBangTaiNguyen( &bangTaiNguyen );

   return 1;
}


void docThamSo( int argc, char **argv, char *tenTep ) {
   
   // ---- cần tên tệp
   if( argc > 1 ) {
      unsigned short chiSo = 0;
      while( (argv[1][chiSo] != '\0') && (chiSo < 255) ) {
         tenTep[chiSo] = argv[1][chiSo];
         chiSo++;
      }

   }
   else {
      printf( "Gõ tên tệp và địa chỉ đầu và cuối\n" );
   }
}

BangTaiNguyen docBangTaiNguyen( char *tenTep ) {
   

   BangTaiNguyen bangTaiNguyen;
   FILE *tep = fopen( tenTep, "r" );
   if( tep != NULL ) {
      // ----
      bangTaiNguyen.diaChiDuLieu = fgetc( tep ) << 24 | fgetc( tep ) << 16 | fgetc( tep ) << 8 | fgetc( tep );
      bangTaiNguyen.diaChiBanDo = fgetc( tep ) << 24 | fgetc( tep ) << 16 | fgetc( tep ) << 8 | fgetc( tep );
      bangTaiNguyen.beDaiDuLieu = fgetc( tep ) << 24 | fgetc( tep ) << 16 | fgetc( tep ) << 8 | fgetc( tep );
      bangTaiNguyen.beDaiBanDo = fgetc( tep ) << 24 | fgetc( tep ) << 16 | fgetc( tep ) << 8 | fgetc( tep );
      
      // ---- đến ầu bản đồ
      fseek( tep, bangTaiNguyen.diaChiBanDo, SEEK_SET );
      
      // ---- bỏ qua sao chép thông tin ở đều tệp (16 byte),
      //      và chỗ chứa con trỏ đến bản đồ (4 byte, cho hệ điều hành Mac cổ xưa)
      //      và chổ giàng cho số tham chiếu tệp (2 byte)
      unsigned short chiSo = 0;
      while( chiSo < 22 ) {
         fgetc( tep );
         chiSo++;
      }
      
      bangTaiNguyen.dacTinh = fgetc( tep ) << 8 | fgetc( tep );
      bangTaiNguyen.dichDanhSachTaiNguyen = fgetc( tep ) << 8 | fgetc( tep );
      bangTaiNguyen.dichDanhSachTenTaiNguyen = fgetc( tep ) << 8 | fgetc( tep );
      bangTaiNguyen.soLuongTaiNguyen = (fgetc( tep ) << 8 | fgetc( tep )) + 1;
      
      unsigned long int dichTuongDoiChoBanDoTaiNguyen = ftell( tep ) - 2;

      // ==== đọc bảng cho các loại tài nguyên
      bangTaiNguyen.mangOBangLoaiTaiNguyen = malloc(sizeof( OBangLoaiTaiNguyen )*bangTaiNguyen.soLuongTaiNguyen );
      chiSo = 0;
      while( chiSo < bangTaiNguyen.soLuongTaiNguyen ) {
         bangTaiNguyen.mangOBangLoaiTaiNguyen[chiSo].loai = fgetc( tep ) << 24 | fgetc( tep ) << 16 | fgetc( tep ) << 8 | fgetc( tep );
         bangTaiNguyen.mangOBangLoaiTaiNguyen[chiSo].soLuongTaiNguyen = (fgetc( tep ) << 8 | fgetc( tep )) + 1;
         bangTaiNguyen.mangOBangLoaiTaiNguyen[chiSo].dichDanhSachTaiNguyen = fgetc( tep ) << 8 | fgetc( tep );
         chiSo++;
      }

      // ==== đọc thông tin cho các tài nguyên
      chiSo = 0;
      while( chiSo < bangTaiNguyen.soLuongTaiNguyen ) {
         OBangLoaiTaiNguyen *oBangLoaiTaiNguyen = &(bangTaiNguyen.mangOBangLoaiTaiNguyen[chiSo]);
         unsigned short soTaiNguyen = 0;
         unsigned short soLuongTaiNguyen = oBangLoaiTaiNguyen->soLuongTaiNguyen;
         
         oBangLoaiTaiNguyen->mangTaiNguyen = malloc( sizeof( TaiNguyen )*soLuongTaiNguyen );

         fseek( tep, dichTuongDoiChoBanDoTaiNguyen + oBangLoaiTaiNguyen->dichDanhSachTaiNguyen, SEEK_SET );
         while( soTaiNguyen < soLuongTaiNguyen ) {
            TaiNguyen *taiNguyen = &(oBangLoaiTaiNguyen->mangTaiNguyen[soTaiNguyen]);
            taiNguyen->maSo = (fgetc( tep ) << 8 | fgetc( tep ));
            taiNguyen->dichTen = fgetc( tep ) << 8 | fgetc( tep );

            taiNguyen->dacTinh = fgetc( tep );
            taiNguyen->dichDuLieu = fgetc( tep ) << 16 | fgetc( tep ) << 8 | fgetc( tep );
            taiNguyen->diaChiDuLieu = taiNguyen->dichDuLieu + bangTaiNguyen.diaChiDuLieu;
            
            // ---- bỏ, danh cho con trỏ đến tài nguyên (4 byte)
            fgetc( tep );
            fgetc( tep );
            fgetc( tep );
            fgetc( tep );

            soTaiNguyen++;
         }
         chiSo++;
      }

      // ==== đọc tên tài nguyên (nếu có)
      chiSo = 0;
      while( chiSo < bangTaiNguyen.soLuongTaiNguyen ) {
         OBangLoaiTaiNguyen *oBangLoaiTaiNguyen = &(bangTaiNguyen.mangOBangLoaiTaiNguyen[chiSo]);
         unsigned short soTaiNguyen = 0;
         unsigned short soLuongTaiNguyen = oBangLoaiTaiNguyen->soLuongTaiNguyen;

         while( soTaiNguyen < soLuongTaiNguyen ) {
            TaiNguyen *taiNguyen = &(oBangLoaiTaiNguyen->mangTaiNguyen[soTaiNguyen]);

            if( taiNguyen->dichTen > -1 ) {
               unsigned long int dichDanhSachTenTaiNguyenTuyetDoi = bangTaiNguyen.diaChiBanDo + bangTaiNguyen.dichDanhSachTenTaiNguyen + taiNguyen->dichTen;

               fseek( tep, dichDanhSachTenTaiNguyenTuyetDoi, SEEK_SET );

               // ---- tên không nên dài hơn 31 ký tự 
               unsigned char beDaiTen = fgetc( tep );
               if( beDaiTen > 31 )
                  beDaiTen = 31;

               unsigned char soKyTu = 0;
               while( soKyTu < beDaiTen ) {
                  taiNguyen->ten[soKyTu] = fgetc( tep );
                  soKyTu++;
               }
               taiNguyen->ten[soKyTu] = 0x00;  // kết thúc tên
            }
            else
               taiNguyen->ten[0] = 0x00;
            
            soTaiNguyen++;
         }
         chiSo++;
      }
      
      // ==== đọc dữ lìệu
      chiSo = 0;
      while( chiSo < bangTaiNguyen.soLuongTaiNguyen ) {
         OBangLoaiTaiNguyen *oBangLoaiTaiNguyen = &(bangTaiNguyen.mangOBangLoaiTaiNguyen[chiSo]);
         unsigned short soTaiNguyen = 0;
         unsigned short soLuongTaiNguyen = oBangLoaiTaiNguyen->soLuongTaiNguyen;
         
         while( soTaiNguyen < soLuongTaiNguyen ) {
            TaiNguyen *taiNguyen = &(oBangLoaiTaiNguyen->mangTaiNguyen[soTaiNguyen]);
            
               // ---- bề dài dữ liệu
            fseek( tep, taiNguyen->diaChiDuLieu, SEEK_SET );
               taiNguyen->beDai = fgetc( tep ) << 24 | fgetc( tep ) << 16 | fgetc( tep ) << 8 | fgetc( tep );
            taiNguyen->duLieu = malloc( taiNguyen->beDai );
            if( taiNguyen->duLieu != NULL ) {
               unsigned int chiSoDuLieu = 0;
               while( chiSoDuLieu < taiNguyen->beDai ) {
                  taiNguyen->duLieu[chiSoDuLieu] = fgetc( tep );
                  chiSoDuLieu++;
               }
            }
            else {
               unsigned int loai = oBangLoaiTaiNguyen->loai;
               printf( "Sai lầm: không thể giành trí nhớ cho tài nguyên %c%c%c%c\n",
                      (loai >> 24) & 0xff, (loai >> 16) & 0xff, (loai >> 8) & 0xff, loai & 0xff );
            }
            soTaiNguyen++;
         }
         chiSo++;
      }
      
      printf( "%ld\n" ,ftell( tep ) );
      fclose( tep );
      
   }
   else {
      printf( "Vấn đề mở tệp: %s\n", tenTep );
      exit(0);
   }
   
   return bangTaiNguyen;
}

void chieuThonTinBangTaiNguyen( BangTaiNguyen *bangTaiNguyen ) {
   
   printf( "Địa chỉ bản đô: %d\n", bangTaiNguyen->diaChiBanDo );
   printf( "Bề dài bản độ: %d\n", bangTaiNguyen->beDaiBanDo );
   printf( "Địa chỉ dữ liệu tài nguyên: %d\n", bangTaiNguyen->diaChiDuLieu );
   printf( "Bề dài dữ liệu: %d\n=======\n", bangTaiNguyen->beDaiDuLieu );

   printf( "Số lượng lọai tài nguyên khác nhau: %d\n", bangTaiNguyen->soLuongTaiNguyen );
   printf( "Dịch danh sách tài nguyên: %d\n", bangTaiNguyen->dichDanhSachTaiNguyen );
   printf( "Dịch danh sách tên tài nguyên: %d\n\n", bangTaiNguyen->dichDanhSachTenTaiNguyen );
   
   unsigned short chiSo = 0;
   while( chiSo < bangTaiNguyen->soLuongTaiNguyen ) {
      OBangLoaiTaiNguyen *oBangLoaiTaiNguyen = &(bangTaiNguyen->mangOBangLoaiTaiNguyen[chiSo]);
      unsigned int loaiTaiNguyen = oBangLoaiTaiNguyen->loai;
      printf( "Loại '%c%c%c%c'   số lượng: %d   dịch: %d\n",
             (loaiTaiNguyen >> 24) & 0xff,
             (loaiTaiNguyen >> 16) & 0xff,
             (loaiTaiNguyen >> 8) & 0xff,
             loaiTaiNguyen & 0xff,
             oBangLoaiTaiNguyen->soLuongTaiNguyen,
             oBangLoaiTaiNguyen->dichDanhSachTaiNguyen );
      
      unsigned short soTaiNguyen = 0;
      unsigned short soLuongTaiNguyen = oBangLoaiTaiNguyen->soLuongTaiNguyen;

      while( soTaiNguyen < soLuongTaiNguyen ) {
         TaiNguyen *taiNguyen = &(oBangLoaiTaiNguyen->mangTaiNguyen[soTaiNguyen]);
         printf( "  %d mã số: %d  bề dài: %d  %s  đặc tính: %x dịch tên: %d  dịch dữ liệu: %d\n",
                soTaiNguyen,
                taiNguyen->maSo,
                taiNguyen->beDai,
                taiNguyen->ten,
                taiNguyen->dacTinh,
                taiNguyen->dichTen,
                taiNguyen->diaChiDuLieu );
         
         printf( "    " );
         unsigned int chiSoDuLieu = 0;
         unsigned int chiSoDuLieuCuoi = 512;
         if( taiNguyen->beDai < chiSoDuLieuCuoi )
            chiSoDuLieuCuoi = taiNguyen->beDai;
            
         while( chiSoDuLieu < chiSoDuLieuCuoi ) {
            printf( "%02x ", taiNguyen->duLieu[chiSoDuLieu] );
            chiSoDuLieu++;
            if( chiSoDuLieu % 24 == 0 ) {
               printf( "\n    " );
            }
         }
         printf( "\n\n" );
         soTaiNguyen++;
      }
      chiSo++;
   }
}


#pragma mark ==== CẤU TRÚC TÀI NGUYÊN ====
#pragma mark ---- 'CODE'

typedef struct {
   short id;
   
   
} CODE;



#pragma mark ---- 'WIND'

typedef struct {
   short id;

   short top;
   short left;
   short bottom;
   short right;
   
   short definitionID;    // loại cửa số
   unsigned short visibilityStatus;
   unsigned short closeBox;
   unsigned int referenceConst;
   unsigned char titleLength;
   char title[257];      // cho tựa dài 256 ký tự và ký tựa kết thúc
   short position;
} WIND;

WIND docWIND( unsigned char *duLieu, short id ) {
   
   WIND taiNguyenWIND;
   taiNguyenWIND.id = id;
   
   taiNguyenWIND.top = *duLieu << 8 | *(duLieu++);
   taiNguyenWIND.left = *(duLieu++) << 8 | *(duLieu++);
   taiNguyenWIND.bottom = *(duLieu++) << 8 | *(duLieu++);
   taiNguyenWIND.right = *(duLieu++) << 8 | *(duLieu++);
   
   taiNguyenWIND.definitionID = *(duLieu++) << 8 | *(duLieu++);
   taiNguyenWIND.visibilityStatus = *(duLieu++) << 8 | *(duLieu++);
   taiNguyenWIND.closeBox = *(duLieu++) << 8 | *(duLieu++);
   taiNguyenWIND.referenceConst = *(duLieu++) << 8 | *(duLieu++);
   
   // ---- title
   taiNguyenWIND.titleLength = *(duLieu++);
   unsigned chiSo = 0;
   while( chiSo <= taiNguyenWIND.titleLength ) {
      taiNguyenWIND.title[chiSo] = *(duLieu++);
      chiSo++;
   }
   taiNguyenWIND.title[chiSo] = 0x00;

//   taiNguyenWIND.position = *(duLieu++) << 8 | *(duLieu++);

   return taiNguyenWIND;
}

void inThongTinTaiNguyenWIND( WIND *taiNguyenWIND ) {

   printf( "'WIND'  %s\n", taiNguyenWIND->title );
   printf( "   Rect: %d %d %d %d\n",
          taiNguyenWIND->top,
          taiNguyenWIND->left,
          taiNguyenWIND->bottom,
          taiNguyenWIND->right );

   printf( "   definitionID: %d  close box: %d  reference const: %d\n",
          taiNguyenWIND->definitionID,
          taiNguyenWIND->closeBox,
          taiNguyenWIND->referenceConst );
}
